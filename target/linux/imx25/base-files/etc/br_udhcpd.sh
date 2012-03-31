#!/bin/sh
. /etc/functions.sh
 
UDHCPD_BR_CONF="/tmp/udhcpdBr.conf"
UDHCPD_WF_CONF="/tmp/udhcpdWf.conf"
UDHCPD_BR_PID="/var/run/udhcpdBr.pid"
UDHCPD_WF_PID="/var/run/udhcpdWf.pid"
UDHCPD_BR_LEASES="/tmp/udhcpdBr.leases"
UDHCPD_WF_LEASES="/tmp/udhcpdWf.leases"

rm_udhcpd_br_conf () {
  if [ -e $UDHCPD_BR_CONF ]; then
    rm -rf $UDHCPD_BR_CONF
  fi
}

rm_udhcpd_wf_conf () {
  if [ -e $UDHCPD_WF_CONF ]; then
    rm -rf $UDHCPD_WF_CONF
  fi
}

config_load network
#config_load udhcpd

config_get lan_ip lan ipaddr
config_get lan_mask lan netmask

# get from udhcpd
#config_get br_enable wlan  enable
#config_get br_wfip   wlan  wfip
#config_get br_start wlan start
#config_get br_limit wlan limit
#config_get br_leasetime wlan leasetime

# get from dhcp
config_load dhcp
config_get br_enable  lan brenable
config_get br_wfenable lan wfenable
config_get br_wfip   lan wfip
config_get br_wfmask lan wfmask
config_get br_start  lan start
config_get br_limit  lan limit
# this unit is minute
config_get br_leasetimeA lan wfleasetime 

br_leasetime=$(($br_leasetimeA*60))

udhcpd_add_line () {
	local file="$1"
	local name="$2"
	local value1="$3"
	local value2="$4"
	echo "$name    $value1   $value2" >> $file
}

udhcpd_add_br () {
	udhcpd_add_line $UDHCPD_BR_CONF "$1" "$2" "$3"
}

udhcpd_add_wf () {
	udhcpd_add_line $UDHCPD_WF_CONF "$1" "$2" "$3"
}


#root@OpenWrt:/etc/init.d# ipcalc.sh "192.168.21.1" "255.255.0.0" 100 150  
#IP=192.168.21.1
#NETMASK=255.255.0.0
#BROADCAST=192.168.255.255
#NETWORK=192.168.0.0
#PREFIX=16
#START=192.168.0.100
#END=192.168.0.250

udhcpd_produce_conf_br () {
        rm_udhcpd_br_conf 
        local zz="1" 
	eval "$(ipcalc.sh $lan_ip $lan_mask $br_start $br_limit)"
        local Sa=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[1]}'`
        local Sb=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[2]}'`
        local Sc=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[3]}'`
        local Sd=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[4]}'`
        Sd=$(($Sd+$zz))
        if [ $Sd -gt 254 ];then
          Sd=252
        fi
        local Ip="$Sa.$Sb.$Sc.$Sd"
	udhcpd_add_br start "$Ip"
	udhcpd_add_br end   "$Ip"
	udhcpd_add_br interface "br-lan"
	udhcpd_add_br max_leases "1"
	udhcpd_add_br lease_file "$UDHCPD_BR_LEASES"
        udhcpd_add_br pidfile    "$UDHCPD_BR_PID"
	udhcpd_add_br option  subnet  "$NETMASK"
	udhcpd_add_br option  router  "$lan_ip"
        udhcpd_add_br option  dns     "$lan_ip"	
        udhcpd_add_br option  lease   "$br_leasetime"
}

udhcpd_produce_conf_wf () {
    rm_udhcpd_wf_conf
    #local zz="1" 
    #local a=$(($br_start+$zz))
    local b=$(($br_limit-1))	
    #local Sa=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[1]}'`
    #local Sb=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[2]}'`
    #local Sc=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[3]}'`
    #local Sd=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[4]}'`
    #Sc=$(($Sc+$zz))
    #if [ $Sc -gt 254 ];then
    #  Sc=252
    #fi
    #local Ip="$Sa.$Sb.$Sc.$Sd"
    eval "$(ipcalc.sh $br_wfip $br_wfmask $br_start $b)"
	udhcpd_add_wf start "$START"
	udhcpd_add_wf end   "$END"
	udhcpd_add_wf interface "tiwlan0"
	udhcpd_add_wf max_leases "$br_limit"
	udhcpd_add_wf lease_file "$UDHCPD_WF_LEASES"
        udhcpd_add_wf pidfile    "$UDHCPD_WF_PID"
	udhcpd_add_wf option  subnet  "$NETMASK"
	udhcpd_add_wf option  router  "$br_wfip"
        udhcpd_add_wf option  dns     "$br_wfip"	
        udhcpd_add_wf option  lease   "$br_leasetime"
}

udhcpd_br_start () {
	rm -rf "$UDHCPD_BR_LEASES"
        touch "$UDHCPD_BR_LEASES"
	udhcpd_produce_conf_br
	if [ "$br_enable" = 1 ]; then
          sleep 2
	  udhcpd "$UDHCPD_BR_CONF"
	fi
}

udhcpd_br_stop () {
	PID=`cat $UDHCPD_BR_PID`
	kill -15 "$PID"
	rm -rf "$UDHCPD_BR_LEASES"
}

# for web
udhcpd_wf_start () {
	rm -rf "$UDHCPD_WF_LEASES"
        touch "$UDHCPD_WF_LEASES"
	ifconfig tiwlan0 $br_wfip netmask $br_wfmask
	udhcpd_produce_conf_wf
	if [ "$br_wfenable" = 1 ]; then
	  udhcpd "$UDHCPD_WF_CONF"
        fi
}

udhcpd_wf_stop () {
	PID=`cat $UDHCPD_WF_PID`
	kill -15 "$PID"
	rm -rf "$UDHCPD_WF_LEASES"
}

udhcpd_wf_restart() {
	Q=`ifconfig tiwlan0`
	UP=$?
	if [ $UP ]; then
	      udhcpd_wf_stop
	      udhcpd_wf_start
	fi
}

#USERDATA_PATH=`br_mount_point /dev/mtdblock5`
#AP_RUN_CONF="${USERDATA_PATH}/ap_driver/ap_run"
#AP_RM_CONF="${USERDATA_PATH}/ap_driver/ap_rm"
AP_RUN_CONF="/tmp/ap_run"
AP_RM_CONF="/tmp/ap_rm"

rm_ap_run_conf () {
  if [ -e $AP_RUN_CONF ]; then
	rm -rf $AP_RUN_CONF
  fi
}

rm_ap_rm_conf () {
  if [ -e $AP_RM_CONF ]; then
	rm -rf $AP_RM_CONF
  fi
}

ap_run_add_line () {
	echo "$1" >> "$AP_RUN_CONF" 
}

ap_rm_add_line () {
	echo "$1" >> "$AP_RM_CONF" 
}

produce_ap_run () {
  rm_ap_run_conf
  #local Sa=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[1]}'`
  #local Sb=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[2]}'`
  #local Sc=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[3]}'`
  #local Sd=`echo "$lan_ip" | awk '{c=split($1,s,"."); print s[4]}'`
  #local zz="1"
  #Sc=$(($Sc+$zz))
  #if [ $Sc -gt 254 ];then
  #    $Sc=253
  #fi
  #local Ip="$Sa.$Sb.$Sc.$Sd"
  local FW="firmware_ap.bin"
  local INI="tiwlan.ini"
  local NVS="nvs_map.bin"
  ap_run_add_line "#!/bin/sh"
#  ap_run_add_line "if [ -d /userdata/ap_driver ]; then"
#  ap_run_add_line " PP=/userdata/ap_driver"
#  ap_run_add_line " QQ=/data"
#  ap_run_add_line "else"
#  ap_run_add_line " PP=/tmp/root/userdata/ap_driver"
#  ap_run_add_line " QQ=/tmp/root/data"
#  ap_run_add_line "fi"
  ap_run_add_line "CH_CONF=\$(uci get hostapd.general.channel)"
  ap_run_add_line "if [ \"\$CH_CONF\" = \"0\" ]; then"
  ap_run_add_line "  UPTIME=\`cat /proc/uptime\`"
  ap_run_add_line "  RAND=\`echo \$UPTIME | cut -d' ' -f1 | cut -d'.' -f2 | sed 's/^0//'\`"
  ap_run_add_line "  CH_RAND=\$((\$RAND%11+1))"
#  ap_run_add_line "  sed -i '/channel/ c channel='\$CH_RAND \$PP/hostapd.conf"
  ap_run_add_line "  sed -i '/channel/ c channel='\$CH_RAND \/tmp/hostapd.conf"
  ap_run_add_line "fi"
  ap_run_add_line "rm /tmp/tiwlan*"
#  ap_run_add_line "cd \$PP"
  ap_run_add_line "insmod sdio"
  ap_run_add_line "insmod tiap_drv"
  #ap_run_add_line "\$PP/tiwlan_loader -f \$PP/firmware_ap.bin -i \$PP/tiwlan.ini"
  #ap_run_add_line "./tiwlan_loader -f firmware_ap.bin"
#  ap_run_add_line "\$PP/tiwlan_loader -f \$PP/$FW -i \$PP/$INI -e \$QQ/$NVS"
  ap_run_add_line "/usr/bin/tiwlan_loader -f /usr/bin/$FW -i /usr/bin/$INI -e /data/$NVS"
  ap_run_add_line "ifconfig tiwlan0  $br_wfip  netmask $lan_mask up"
  ap_run_add_line "sleep 1"
  ap_run_add_line "/usr/bin/hostapd /tmp/hostapd.conf -B"
  ap_run_add_line "if [ "$br_wfenable" = 1 ]; then"
  ap_run_add_line "  udhcpd $UDHCPD_WF_CONF"
  ap_run_add_line "fi"
  ap_run_add_line "#echo 1 > /proc/sys/net/ipv4/ip_forward"
  ap_run_add_line "ushm active 1"
  chmod a+x $AP_RUN_CONF
}

produce_ap_rm () {
  rm_ap_rm_conf
  ap_rm_add_line "UDHCPD_WF_PID=\`cat $UDHCPD_WF_PID\`"
  ap_rm_add_line "kill -15 \$UDHCPD_WF_PID"
  ap_rm_add_line "killall -15 hostapd"
  ap_rm_add_line "ifconfig tiwlan0 down"
  ap_rm_add_line "sleep 1"
  ap_rm_add_line "rmmod tiap_drv"
  ap_rm_add_line "rmmod sdio"
  ap_rm_add_line "ushm active 0"
  chmod a+x $AP_RM_CONF
}

produce_ap_conf () {
  udhcpd_produce_conf_wf
  produce_ap_run
  produce_ap_rm
}

# for web
hostapd_start () {
  PP="/userdata/ap_driver"
  if [ -d /userdata/ap_driver ]; then
    PP="/userdata/ap_driver"
  else
    PP="/tmp/root/userdata/ap_driver"
  fi 
  $PP/hostapd $PP/hostapd.conf
}

hostapd_stop () {
  killall -15 hostapd 
}

hostapd_restart () {
  hostapd_stop
  hostapd_start
}

