#!/bin/sh

TTYACM=ttyACM2
TTYGS=ttyGS0

REMOVE=remove
ADD=add

if [ $1 == "ttyACM0" ]; then 
	if [ $2 == $ADD ]; then
		if [ ! -e /tmp/icedload ]; then
			gcom -d /dev/ttyACM0 -s /etc/gcom/icera_setmode.gcom
		fi
	fi
fi

if [ $1 == $TTYACM ]; then 
	if [ $2 == $ADD ]; then
		echo "device is $1 action is $2"
                # Enable auto suspend for 3G module
		# echo auto > /sys/bus/usb/devices/1-1/power/level
		if [ $4 == "br" ]; then
		  # ifup wan		
		  killall bandluxed
		  bandluxed &
		  sleep 3
		  MonitorWan -b
		fi
	else
		echo "device is $1 action is $2"	
		killall MonitorWan
		kill -9 `pidof MonitorWan`
		killall udhcpc
		killall bandluxed
		/bin/kill `/bin/pidof pppd`
	fi
else
	echo "other device"
fi

