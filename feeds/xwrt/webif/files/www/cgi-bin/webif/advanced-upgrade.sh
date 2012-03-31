#!/usr/bin/webif-page "-U /tmp -u 16384"
<?
. /usr/lib/webif/webif.sh

board_type=$(cat /proc/cpuinfo 2>/dev/null | sed 2,20d | cut -c16-)
machinfo=$(uname -a 2>/dev/null)

header "Advanced" "Upgrade" "@TR<<Firmware Upgrade>>" '' "$SCRIPT_NAME"

get_image() { # <source> [ <command> ]
    local from="$1"
    local cmd

    case "$from" in
	http://*|ftp://*) cmd="wget -O- -q";;
	*) cmd="cat";;
    esac
    eval "$cmd $from"
}

cat <<EOF
<script type="text/javascript" src="/webif.js"></script>
<script type="text/javascript">
<!--
function modechange()
{
	var v;
	$js

	hide('save');
	//show('save');
}
-->
</script>
EOF

if empty "$FORM_submit"; then


    display_form <<EOF
onchange|modechange
start_form
field|@TR<<Do not save current configuration>>
checkbox|nokeepconfig|$FORM_nokeepconfig|1
field| @TR<<Firmware Image>>
upload|upgrade_kernel
submit|upgrade| @TR<<Upgrade>> |
end_form

EOF

#elif [ "$FORM_upgrade_kernel" = "" ] || [ "$FORM_upgrade_roofs" = "" ]; then

#    eval FORM_upgrade_kernel="\$FORM_upgrade_kernel"
#    eval FORM_upgrade_roofs="\$FORM_upgrade_roofs"
    
#    display_form <<EOF
#onchange|modechange
#string|Please choose a firmware file.
#string|</ br>
#string|$FORM_upgrade_kernel
#string|$FORM_upgrade_roofs
#start_form
#field|@TR<<Do not save current configuration>>
#checkbox|nokeepconfig|$FORM_nokeepconfig|1
#field| @TR<<Kernel Image>>
#upload|upgrade_kernel
#field| @TR<<Rootfs Image>>
#upload|upgrade_rootfs
#submit|upgrade| @TR<<Upgrade>> |
#end_form
#EOF

else

#    eval FORM_upgrade_kernel="\$FORM_upgrade_kernel"
#    eval FORM_upgrade_roofs="\$FORM_upgrade_roofs"

    echo "<br />@TR<<Upgrading firmware>><br />"
#    echo "<br />Stop all services... <br />"

#    sh /www/cgi-bin/webif/kill_ap.sh > /dev/null 2>&1 
#    echo "<br />Download firmware, please wait ...<br />"
#    if [ -f /tmp/fw.bin ]; then
#	echo "<br />Remove /tmp/fw.bin<br />"
#	/bin/rm /tmp/fw.bin > /dev/null 2>&1
#    fi

#     /etc/init.d/br_wifi stop > /dev/null
#     /etc/init.d/samba stop > /dev/null
#     /usr/bin/killall -9 MonitorWan
#     /bin/kill -s USR1 `cat /tmp/lock/brlcd.lock`
#     /usr/bin/killall bandluxed
#     /usr/bin/killall BatteryMonitor
#     /usr/bin/killall crond
#     /usr/bin/killall udhcpd
#     /usr/bin/killall klogd
#     /usr/bin/killall syslogd
#     /bin/umount /data
#     /bin/umount /mnt 

	chmod 755 $FORM_upgrade_kernel
	mv $FORM_upgrade_kernel /tmp/fw.bin

	if [ "$FORM_nokeepconfig" = "1" ]; then
		echo "@TR<<Don't backup user configuration>>"
		mkdir -p /userdata/recover
		mkdir -p /userdata/recover/config
		cp -rf /data/default/config /userdata/recover
		if [ -e /data/default/passwd ]; then
			cp -f  /data/default/passwd /userdata/recover
		elif [ -e /userdata/default/passwd ]; then
			cp -f  /userdata/default/passwd /userdata/recover
		else
			echo "@TR<<Fail to backup user configuration>>"
		fi
		sync
	else
        	echo "@TR<<backup user configuration>>"
		mkdir -p /userdata/recover
		mkdir -p /userdata/recover/config
		cp -r /etc/config /userdata/recover
		cp -f /etc/passwd /userdata/recover
		sync
#		/bin/umount /userdata
	fi
	ls /sbin

	/tmp/fw.bin
	if [ -e /tmp/setup.sh ]; then
		/tmp/setup.sh /tmp/fw.bin
	fi
#     eval $FORM_upgrade_kernel

#    get_image $FORM_upgrade_kernel > /tmp/fw.bin
#    chmod 755 /tmp/fw.bin > /dev/null 2>&1

#    /tmp/fw.bin &
#    if [ "$FORM_nokeepconfig" = "1" ]; then
#        echo "Don't backup user configuration"
#    else
#	echo "backup user configuration"
#	cp -r /etc/config /userdata
#	sync
#	umount /userdata 
#    fi

#    echo "<br />Start upgrade kernel firmware, please wait...<br />"
#    /tmp/fw.bin &
#    echo "<br />Finish kernel upgrade<br />"
#    
#    echo "<br />Start upgrade rootfs firmware, please wait...<br />"
#    /bin/rm /tmp/fw.bin > /dev/null 2>&1
#
#    get_image $FORM_upgrade_rootfs > /tmp/fw.bin
#    chmod 755 /tmp/fw.bin
#    /tmp/fw.bin 
#
#    echo "<br />Finish rootfs upgrade<br />"
#    echo "<br />Please reboot Pocket Router,thanks<br />"
#    /sbin/reboot -f
#
#    if [ $result -eq 0 ]; then
#	echo ""
#	echo "@TR<<Firmware update is done. reboot PR39 by yourself>>"
#	echo "@TR<< The machine will reboot automatically in 5 seconds>>."
#	sleep 5s && reboot
#   else
#	echo ""
#	echo "@TR<<Firmware update failed.>>"
#    fi
fi



footer 
?>
<!--
##WEBIF:name:Advanced:310:Upgrade
-->
