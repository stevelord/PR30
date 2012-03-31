#!/bin/sh

kill_ap () {
  # free memory space
  /etc/init.d/br_wifi stop > /dev/null
  /etc/init.d/samba stop >/dev/null
  ifdown wan
  kill -s USR1 `cat /tmp/lock/brlcd.lock`
#  kill -s USR1 `cat /tmp/root/userdata/brlcd/brlcd.lock`
  killall bandluxed
  killall BatteryMonitor
  killall crond
  killall udhcpd
  killall klogd
  killall syslogd
#  killall dropbear
#  killall dnsmasq
#  umount /userdata
  umount /data
  umount /mnt
}

kill_ap
