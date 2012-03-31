#!/bin/sh
# Description : Exit DOZ mode procedure
# 
# 


echo 1 > /sys/class/gpio/gpio27/value


bandluxed &
sleep 3
MonitorWan -b
