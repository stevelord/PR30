#!/bin/sh
# Description : Enter DOZ mode procedure
#
# 


/etc/init.d/br_wifi stop


killall MonitorWan
ifdown wan


rcmdc -c 'bl.sleep i/0'
sleep 1
killall bandluxed


rm /var/lock/LCK..ttyACM*


echo 0 > /sys/class/gpio/gpio27/value


echo mem > /sys/power/state

