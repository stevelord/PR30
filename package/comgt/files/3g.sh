set_3g_led() {
	# set on WRT54G3G only
	[ -d /proc/diag ] || return 0
	grep WRT54G3G /proc/diag/model >/dev/null || return 0
	echo "$1" > /proc/diag/led/3g_green
	echo "$2" > /proc/diag/led/3g_blue
	echo "$3" > /proc/diag/led/3g_blink
}

scan_3g() {
	local device

	scan_ppp "$@"
	config_get device "$1" device

	# try to figure out the device if it's invalid
	[ -n "$device" -a -e "$device" ] || {
		for device in /dev/ttyUSB0 /dev/ttyUSB1 /dev/ttyUSB2 /dev/tts/2 /dev/usb/tts/0 /dev/noz0; do
			[ -e "$device" ] && {
				config_set "$1" device "$device"
				break
			}
		done
	}

	# enable 3G with the 3G button by default
	config_get button "$1" button
	[ -z "$button" ] && {
		config_set "$1" button 1
	}
}

stop_interface_3g() {
	set_3g_led 0 0 0
	killall gcom >/dev/null 2>/dev/null
}

setup_interface_3g() {
	local iface="$1"
	local config="$2"
	local chat="/etc/chatscripts/3g.chat"
	local pppoption=""
	local cnt="1"
	
	config_get device "$config" device
	config_get maxwait "$config" maxwait
	maxwait=${maxwait:-20}
	while [ ! -e "$device" -a $maxwait -gt 0 ];do # wait for driver loading to catch up
		maxwait=$(($maxwait - 1))
		sleep 1
	done

	for module in slhc ppp_generic ppp_async; do
		/sbin/insmod $module 2>&- >&-
	done

	config_get prefer "$config"  prefer
	config_get autoapn "$config" autoapn
	config_get apn "$config" apn
	config_get dialnum "$config" dialnum
	config_get service "$config" service
	config_get pincode "$config" pincode
	config_get mtu "$config" mtu
	config_get auth "$config" auth
	config_get network_auto "$config" network_auto
	config_get plmn "$config" plmn 
	config_get roaming "$config" roaming

	rcmdc -c "pin.status"
        PIN=`ushm show | grep PIN_status | cut -d= -f2`
        if [ "$PIN" = "0" ]; then
	    echo "pin ok"
        else
            echo "PIN_status!=0, please unlock pin"
	    return 1
        fi

	# mode=0, 1, 2,
	br_mode=`rcmdc -c 'network.mode' | cut -d' ' -f 4`	
	case "$br_mode" in
	     "0,") pre_mode="umts"
		;;
	     "1,") pre_mode="gprs"
		;;
	     "2,") pre_mode="both"
		;;
	esac
	#echo "pre_mode=$pre_mode, prefer=$prefer"
	if [ "$prefer" != "$pre_mode" ]; then
	case "$prefer" in
	    "umts")
	    rcmdc -c 'network.mode_set i/0 i/0 i/0'
	     ;;
	    "gprs")
	    rcmdc -c 'network.mode_set i/1 i/0 i/0'
	     ;;
	    "both")
	    rcmdc -c 'network.mode_set i/2 i/0 i/0'
	     ;;
	esac
	sleep 40
	fi
	rcmdc -c 'network.status'

	# set network_auto	
	select_mode=`rcmdc -c 'network.selection_mode' | cut -d' ' -f4`
	case "$select_mode" in
	   "auto")
	    if [ "$network_auto" = "manual" ]; then
	      echo "network select $plmn"
	      rcmdc -c 'network.selection i/1 s/$plmn'
	    fi
	    ;;
	   "manual")
	    if [ "$network_auto" = "auto" ]; then
	      echo "network select auto"
	      rcmdc -c 'network.selection i/0 s/00000'
	    else
	      echo "network select $plmn"
	      rcmdc -c 'network.selection i/1 s/$plmn'
	    fi
	    ;;
	esac

	net_status=`rcmdc -c 'network.status' | cut -d' ' -f3`
	cnt="1"
	while [ "$net_status" != "2," -a "$cnt" -le "5" ]
	do
	  net_status=`rcmdc -c 'network.status' | cut -d' ' -f3`
	  cnt=$(($cnt+1))
	done

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
            apn=`GetAPN | cut -d\; -f1 | cut -d\, -f3`
	  fi
        fi

	case "$auth" in
	  pap) pppoption="refuse-chap";;
	  chap) pppoption="refuse-pap";;
	  *) pppoption="";;
	esac

	set_3g_led 1 0 1

	# figure out hardware specific commands for the card
	case "$service" in
		cdma|evdo) chat="/etc/chatscripts/evdo.chat";;
	*)
	#	cardinfo=$(gcom -d "$device" -s /etc/gcom/getcardinfo.gcom)
	#	if echo "$cardinfo" | grep Novatel; then
	#		case "$service" in
	#			umts_only) CODE=2;;
	#			gprs_only) CODE=1;;
	#			*) CODE=0;;
	#		esac
	#		mode="AT\$NWRAT=${CODE},2"
	#	elif echo "$cardinfo" | grep Option; then
	#		case "$service" in
	#			umts_only) CODE=1;;
	#			gprs_only) CODE=0;;
	#			*) CODE=3;;
	#		esac
	#		mode="AT_OPSYS=${CODE}"
	#	fi
		# Don't assume Option to be default as it breaks with Huawei Cards/Sticks
		
	#	PINCODE="$pincode" gcom -d "$device" -s /etc/gcom/setpin.gcom || {
	#		echo "$config(3g): Failed to set the PIN code."
	#		set_3g_led 0 0 0
	#		return 1
	#	}
	#	test -z "$mode" || {
	#		MODE="$mode" gcom -d "$device" -s /etc/gcom/setmode.gcom
	#	}
	;;
	esac

	# set_3g_led 1 0 0

	config_set "$config" "connect" "${apn:+USE_APN=$apn } ${dialnum:+DIAL_NUM=$dialnum }/usr/sbin/chat -t5 -v -E -f $chat"
	start_pppd "$config" \
		noaccomp \
		nopcomp \
		novj \
		nobsdcomp \
		noauth \
		lock \
		crtscts \
		${pppoption} \
		${mtu:+mtu $mtu mru $mtu} \
		115200 "$device"
}
