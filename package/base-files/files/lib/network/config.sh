#!/bin/sh
# Copyright (C) 2006 OpenWrt.org

# DEBUG="echo"

find_config() {
	local iftype device iface ifaces ifn
	for ifn in $interfaces; do
		config_get iftype "$ifn" type
		config_get iface "$ifn" ifname
		case "$iftype" in
			bridge) config_get ifaces "$ifn" ifnames;;
		esac
		config_get device "$ifn" device
		for ifc in $device $iface $ifaces; do
			[ ."$ifc" = ."$1" ] && {
				echo "$ifn"
				return 0
			}
		done
	done

	return 1;
}

scan_interfaces() {
	local cfgfile="${1:-network}"
	interfaces=
	config_cb() {
		case "$1" in
			interface)
				config_set "$2" auto 1
			;;
		esac
		local iftype ifname device proto
		config_get iftype "$CONFIG_SECTION" TYPE
		case "$iftype" in
			interface)
				append interfaces "$CONFIG_SECTION"
				config_get proto "$CONFIG_SECTION" proto
				config_get iftype "$CONFIG_SECTION" type
				config_get ifname "$CONFIG_SECTION" ifname
				config_get device "$CONFIG_SECTION" device "$ifname"
				config_set "$CONFIG_SECTION" device "$device"
				case "$iftype" in
					bridge)
						config_set "$CONFIG_SECTION" ifnames "$device"
						config_set "$CONFIG_SECTION" ifname br-"$CONFIG_SECTION"
					;;
				esac
				( type "scan_$proto" ) >/dev/null 2>/dev/null && eval "scan_$proto '$CONFIG_SECTION'"
			;;
		esac
	}
	config_load "${cfgfile}"
}

add_vlan() {
	local vif="${1%\.*}"

	[ "$1" = "$vif" ] || ifconfig "$1" >/dev/null 2>/dev/null || {
		ifconfig "$vif" up 2>/dev/null >/dev/null || add_vlan "$vif"
		$DEBUG vconfig add "$vif" "${1##*\.}"
		return 0
	}
	return 1
}

# sort the device list, drop duplicates
sort_list() {
	local arg="$*"
	(
		for item in $arg; do
			echo "$item"
		done
	) | sort -u
}

# Create the interface, if necessary.
# Return status 0 indicates that the setup_interface() call should continue
# Return status 1 means that everything is set up already.

prepare_interface() {
	local iface="$1"
	local config="$2"
	local vifmac="$3"

	# if we're called for the bridge interface itself, don't bother trying
	# to create any interfaces here. The scripts have already done that, otherwise
	# the bridge interface wouldn't exist.
	[ "br-$config" = "$iface" -o -e "$iface" ] && return 0;

	ifconfig "$iface" 2>/dev/null >/dev/null && {
		local proto
		config_get proto "$config" proto

		# make sure the interface is removed from any existing bridge and deconfigured,
		# (deconfigured only if the interface is not set to proto=none)
		unbridge "$iface"
		[ "$proto" = none ] || ifconfig "$iface" 0.0.0.0

		# Change interface MAC address if requested
		[ -n "$vifmac" ] && {
			ifconfig "$iface" down
			ifconfig "$iface" hw ether "$vifmac" up
		}
	}

	# Setup VLAN interfaces
	add_vlan "$iface" && return 1
	ifconfig "$iface" 2>/dev/null >/dev/null || return 0

	# Setup bridging
	local iftype
	config_get iftype "$config" type
	case "$iftype" in
		bridge)
			[ -x /usr/sbin/brctl ] && {
				ifconfig "br-$config" 2>/dev/null >/dev/null && {
					local newdevs devices
					config_get devices "$config" device
					for dev in $(sort_list "$devices" "$iface"); do
						append newdevs "$dev"
					done
					uci_set_state network "$config" device "$newdevs"
					#$DEBUG ifconfig "$iface" down
					#$DEBUG ifconfig "$iface" hw ether 00:26:FA:00:00:02
					$DEBUG ifconfig "$iface" 0.0.0.0
					$DEBUG brctl addif "br-$config" "$iface"
					# Bridge existed already. No further processing necesary
				} || {
					local stp
					config_get_bool stp "$config" stp 0
					$DEBUG brctl addbr "br-$config"
					$DEBUG brctl setfd "br-$config" 0
					$DEBUG ifconfig "br-$config" up
					#$DEBUG ifconfig "$iface" down
					#$DEBUG ifconfig "$iface" hw ether 00:26:FA:00:00:02
					$DEBUG ifconfig "$iface" 0.0.0.0
					$DEBUG brctl addif "br-$config" "$iface"
					$DEBUG brctl stp "br-$config" $stp
					# Creating the bridge here will have triggered a hotplug event, which will
					# result in another setup_interface() call, so we simply stop processing
					# the current event at this point.
				}
				ifconfig "$iface" up 2>/dev/null >/dev/null
				return 1
			}
		;;
	esac
	return 0
}

set_interface_ifname() {
	local config="$1"
	local ifname="$2"

	local device
	config_get device "$1" device
	uci_set_state network "$config" ifname "$ifname"
	uci_set_state network "$config" device "$device"
}

setup_interface_none() {
	env -i ACTION="ifup" INTERFACE="$2" DEVICE="$1" PROTO=none /sbin/hotplug-call "iface" &
}

setup_interface_static() {
	local iface="$1"
	local config="$2"

	local ipaddr netmask ip6addr
	config_get ipaddr "$config" ipaddr
	config_get netmask "$config" netmask
	config_get ip6addr "$config" ip6addr
	[ -z "$ipaddr" -o -z "$netmask" ] && [ -z "$ip6addr" ] && return 1

	local gateway ip6gw dns bcast
	config_get gateway "$config" gateway
	config_get ip6gw "$config" ip6gw
	config_get dns "$config" dns
	config_get bcast "$config" broadcast

	[ -z "$ipaddr" ] || $DEBUG ifconfig "$iface" "$ipaddr" netmask "$netmask" broadcast "${bcast:-+}"
	[ -z "$ip6addr" ] || $DEBUG ifconfig "$iface" add "$ip6addr"
	[ -z "$gateway" ] || $DEBUG route add default gw "$gateway" dev "$iface"
	[ -z "$ip6gw" ] || $DEBUG route -A inet6 add default gw "$ip6gw" dev "$iface"
	[ -z "$dns" ] || {
		for ns in $dns; do
			grep "$ns" /tmp/resolv.conf.auto 2>/dev/null >/dev/null || {
				echo "nameserver $ns" >> /tmp/resolv.conf.auto
			}
		done
	}

	config_get type "$config" TYPE
	[ "$type" = "alias" ] && return 0

	env -i ACTION="ifup" INTERFACE="$config" DEVICE="$iface" PROTO=static /sbin/hotplug-call "iface" &
}

setup_interface_alias() {
	local config="$1"
	local parent="$2"
	local iface="$3"

	local cfg
	config_get cfg "$config" interface
	[ "$parent" == "$cfg" ] || return 0

	# alias counter
	local ctr
	config_get ctr "$parent" alias_count 0
	ctr="$(($ctr + 1))"
	config_set "$parent" alias_count "$ctr"

	# alias list
	local list
	config_get list "$parent" aliases
	append list "$config"
	config_set "$parent" aliases "$list"

	iface="$iface:$ctr"
	set_interface_ifname "$config" "$iface"

	local proto
	config_get proto "$config" proto "static"
	case "${proto}" in
		static)
			setup_interface_static "$iface" "$config"
		;;
		*)
			echo "Unsupported type '$proto' for alias config '$config'"
			return 1
		;;
	esac
}

setup_interface() {
	local iface_main="$1"
	local config="$2"
	local proto="$3"
	local vifmac="$4"
	local ip6addr_main=

	[ -n "$config" ] || {
		config=$(find_config "$iface_main")
		[ "$?" = 0 ] || return 1
	}

	prepare_interface "$iface_main" "$config" "$vifmac" || return 0

	[ "$iface_main" = "br-$config" ] && {
		# need to bring up the bridge and wait a second for
		# it to switch to the 'forwarding' state, otherwise
		# it will lose its routes...
		ifconfig "$iface_main" up
		sleep 1
	}

	# Interface settings
	grep "$iface_main:" /proc/net/dev > /dev/null && {
		local mtu macaddr
		config_get mtu "$config" mtu
		config_get macaddr "$config" macaddr
		[ -n "$macaddr" ] && $DEBUG ifconfig "$iface_main" down
		$DEBUG ifconfig "$iface_main" ${macaddr:+hw ether "$macaddr"} ${mtu:+mtu $mtu} up
	}
	set_interface_ifname "$config" "$iface_main"

	pidfile="/var/run/$iface_main.pid"
	[ -n "$proto" ] || config_get proto "$config" proto
	case "$proto" in
		static)
			config_get ip6addr_main "$config" ip6addr
			setup_interface_static "$iface_main" "$config"
		;;
		dhcp)
			# prevent udhcpc from starting more than once
			lock "/var/lock/dhcp-$iface_main"
			local pid="$(cat "$pidfile" 2>/dev/null)"
			if [ -d "/proc/$pid" ] && grep udhcpc "/proc/${pid}/cmdline" >/dev/null 2>/dev/null; then
				lock -u "/var/lock/dhcp-$iface_main"
			else
				local ipaddr netmask hostname proto1 clientid
				local prefer autoapn apn dialnum pincode auth
				local network_auto plmn roaming username password
				config_get ipaddr "$config" ipaddr
				config_get netmask "$config" netmask
				config_get hostname "$config" hostname
				config_get proto1 "$config" proto
				config_get clientid "$config" clientid
				
				# 20100809
				config_get prefer "$config"  prefer
				config_get autoapn "$config" autoapn
				config_get apn "$config" apn
				config_get dialnum "$config" dialnum
				config_get pincode "$config" pincode
				config_get auth "$config" auth
				config_get network_auto "$config" network_auto
				config_get plmn "$config" plmn 
				config_get roaming "$config" roaming
				config_get username "$config" username
				config_get password "$config" password
				
				rcmdc -c "pin.status"
				PIN=`ushm show | grep PIN_status | cut -d= -f2`
				if [ "$PIN" = "0" ]; then
				    echo "pin ok"
				else
				    echo "PIN_status!=0, please unlock pin"
				    return 1
				fi

				case "$prefer" in
				"umts")
				    rcmdc -c "network.mode_set i/0 i/0 i/0"
				    ;;
				"gprs")
				    rcmdc -c "network.mode_set i/1 i/0 i/0"
				    ;;
				"both")
				    rcmdc -c "network.mode_set i/2 i/0 i/0"
				    ;;
				esac

                        	# set network_auto	
				select_mode=`rcmdc -c 'network.selection_mode' | cut -d' ' -f4`
				network_auto=`grep "network_auto" /etc/config/network  | cut -d' ' -f 3 | cut -d\' -f2`
				case "$select_mode" in
				    "auto")
					if [ "$network_auto" = "manual" ]; then
					    echo "network select $plmn"
					    rcmdc -c "network.selection i/1 s/$plmn"
					fi
					;;
				    "manual")
				        if [ "$network_auto" = "auto" ]; then
				            echo "network select auto"
				            rcmdc -c "network.selection i/0 s/00000"
				        else
				            echo "network select $plmn"
				            rcmdc -c "network.selection i/1 s/$plmn"
				        fi
					;;
				esac

				# wait network status
				net_status=`GetMemory -c | grep network_status | cut -d= -f2`
				if [ "$net_status" != "2" ]; then
					return 1
				fi

				rcmdc -c "sim.info"
				RO=`ushm show | grep roaming | cut -d= -f2`
				if [ "$roaming" = "false" ]; then
				    if [ "$RO" = "1" ]; then
					echo "ro=1 and user roaming disable"
					return 0
				    fi
				fi
				
				if [ "$autoapn" = "1" ]; then
                                # get from sharememory
				    Q=`GetAPN`
				    if [ $Q ]; then
					apn=`GetAPN | cut -d\; -f1 | cut -d\, -f3 | cut -d\" -f2`
					username=`GetAPN | cut -d\; -f2`
					password=`GetAPN | cut -d\; -f3`
				    fi
				fi
				#COMGTAPN=$apn gcom -d /dev/ttyACM0 -s /etc/gcom/setapn.gcom
				rcmdc -c "wan.apn s/$apn"
				if [ "$auth" = 'pap' ]; then
				  pauth=1
				else
				  pauth=2
				fi
				# echo rcmdc -c "wan.auth i/$pauth s/\"${username}\" s/\"${password}\""
				rcmdc -c "wan.auth i/$pauth s/\"${username}\" s/\"${password}\""

				rcmdc -c 'wan.connect'
				# end 20100809

				[ -z "$ipaddr" ] || \
					$DEBUG ifconfig "$iface_main" "$ipaddr" ${netmask:+netmask "$netmask"}

				# don't stay running in background if dhcp is not the main proto on the interface (e.g. when using pptp)
				local dhcpopts
				[ ."$proto1" != ."$proto" ] && dhcpopts="-n -q"
				$DEBUG eval udhcpc -t 0 -i "$iface_main" ${ipaddr:+-r $ipaddr} ${hostname:+-H $hostname} ${clientid:+-c $clientid} -b -p "$pidfile" ${dhcpopts:- -O rootpath -R &}
				lock -u "/var/lock/dhcp-$iface_main"
			fi
		;;
		none)
			setup_interface_none "$iface_main" "$config"
		;;
		*)
			if ( eval "type setup_interface_$proto" ) >/dev/null 2>/dev/null; then
				eval "setup_interface_$proto '$iface_main' '$config' '$proto'"
			else
				echo "Interface type $proto not supported."
				return 1
			fi
		;;
	esac
	[ "$proto" = none ] || {
		for ifn in `ifconfig | grep "^$iface_main:" | awk '{print $1}'`; do
			ifconfig "$ifn" down
		done
	}

	local aliases
	config_set "$config" aliases ""
	config_set "$config" alias_count 0
	config_foreach setup_interface_alias alias "$config" "$iface_main"
	config_get aliases "$config" aliases
	[ -z "$aliases" ] || uci_set_state network "$config" aliases "$aliases"

	# put the ip6addr back to the beginning to become the main ip again
	[ -z "$ip6addr_main" ] || {
		$DEBUG ifconfig "$iface_main" del "$ip6addr_main"
		$DEBUG ifconfig "$iface_main" add "$ip6addr_main"
	}
}

stop_interface_dhcp() {
	local config="$1"
	uci -P /var/state revert "network.$config"
}

unbridge() {
	local dev="$1"
	local brdev

	[ -x /usr/sbin/brctl ] || return 0
	brctl show 2>/dev/null | grep "$dev" >/dev/null && {
		# interface is still part of a bridge, correct that

		for brdev in $(brctl show | awk '$2 ~ /^[0-9].*\./ { print $1 }'); do
			brctl delif "$brdev" "$dev" 2>/dev/null >/dev/null
		done
	}
}
