#!/bin/sh
. /etc/functions.sh

config_load hostapd
#USERDATA_PATH=`br_mount_point /dev/mtdblock5`
#HOSTAPD_CONFIG_PATH=${USERDATA_PATH}/ap_driver
#HOSTAPD_FILE=${HOSTAPD_CONFIG_PATH}/hostapd.conf
HOSTAPD_FILE=/tmp/hostapd.conf

rm_hostapd_conf () {
  if [ -f $HOSTAPD_FILE ]; then
    rm -f $HOSTAPD_FILE
  fi
}

hostap_msg(){
    eval echo "\$1 \$2=\${CONFIG_${1}_${2}}"
}

hostap_add_comment (){
    echo "########################### " >> $HOSTAPD_FILE
    echo "# auto generation by script" >> $HOSTAPD_FILE
    echo "########################### " >> $HOSTAPD_FILE
}

hostap_add_line (){
    local NAME="$1"
    local VALUE="$2"
    echo "$NAME=$VALUE" >> $HOSTAPD_FILE
}

hostap_add_wep_line () {
    local NAME="$1"
    local VALUE="$2"
    local len=`echo -n $VALUE | wc -c`
    if [ "$len" = "5" ]; then
      echo "$NAME=\"$VALUE\"" >> $HOSTAPD_FILE
    elif [ "$len" = "13" ]; then
      echo "$NAME=\"$VALUE\"" >> $HOSTAPD_FILE
    else
      echo "$NAME=$VALUE" >> $HOSTAPD_FILE
    fi
}

# wifi radio 0:disable 1:enable
config_get radio_enable general radio_enable
config_get interface general interface
config_get driver general driver
config_get logger_syslog general logger_syslog
config_get logger_syslog_level general logger_syslog_level
config_get logger_stdout general logger_stdout
config_get logger_stdout_level general logger_stdout_level
config_get dump_file general dump_file
config_get ctrl_interface general ctrl_interface
config_get ctrl_interface_group general ctrl_interface_group
config_get ssid general ssid
config_get hw_mode general hw_mode
config_get channel general channel
config_get beacon_int general beacon_int
config_get dtim_period general dtim_period
config_get max_num_sta general max_num_sta

#config_get supported_rates general supported_rates
#echo $CONFIG_general_supported_rates

config_get preamble general preamble
config_get ignore_broadcast_ssid general ignore_broadcast_ssid


# wmm
config_get wme_enabled general  wme_enabled

# mac filter
config_get macaddr_acl macfilter macaddr_acl

# secruity
config_get auth_type auth auth_type
config_get auth_algs wep auth_algs
config_get wep_default_key wep wep_default_key
config_get wep_key0 wep wep_key0
config_get wep_key1 wep wep_key1
config_get wep_key2 wep wep_key2
config_get wep_key3 wep wep_key3
config_get wep_rekey_period wep wep_rekey_period

# wps
config_get wps_state wps wps_state
config_get eap_server wps eap_server

# wpa
config_get wpa_key_mgmt wpa wpa_key_mgmt
#config_get wpa_pairwise wpa wpa_pairwise
config_get wpa_group_rekey wpa wpa_group_rekey
config_get wpa_gmk_rekey wpa wpa_gmk_rekey
config_get wpa_ptk_rekey wpa wpa_ptk_rekey
config_get wpa_psk wpa wpa_psk
config_get wpa_passphrase wpa wpa_passphrase
config_get wpa_usekey wpa wpa_usekey

# radius
config_get own_ip_addr radius own_ip_addr

# neighbor
config_get ap_table_max_size neighbor ap_table_max_size
config_get ap_table_expiration_time neighbor ap_table_expiration_time

#case $radio_enable in
#    0) echo "Disable wifi";;
#    *) echo "Enable wifi";;
#esac

# general
produce_hostapd_conf () {
    rm_hostapd_conf
    hostap_add_comment
    hostap_add_line interface $interface
    hostap_add_line driver $driver
    hostap_add_line logger_syslog $logger_syslog
    hostap_add_line logger_syslog_level $logger_syslog_level
    hostap_add_line logger_stdout $logger_stdout
    hostap_add_line logger_stdout_level $logger_stdout_level
    hostap_add_line dump_file $dump_file
    hostap_add_line ctrl_interface $ctrl_interface
    hostap_add_line ctrl_interface_group $ctrl_interface_group
    hostap_add_line ssid $ssid
    hostap_add_line hw_mode $hw_mode
    hostap_add_line channel $channel
    hostap_add_line beacon_int $beacon_int
    hostap_add_line dtim_period $dtim_period
    hostap_add_line max_num_sta $max_num_sta
    hostap_add_line supported_rates "$CONFIG_general_supported_rates"
    hostap_add_line preamble $preamble
    hostap_add_line ignore_broadcast_ssid $ignore_broadcast_ssid

# wmm
    hostap_add_line wme_enabled $wme_enabled

# mac filter
    hostap_add_line macaddr_acl $macaddr_acl

# security 
    # Cindy pass wep_default_key from 1 to 4
    local wep_key=$(($wep_default_key-1))
    case $auth_type in
	open_no)
	    hostap_add_line auth_algs $auth_algs
	    ;;
	#open_wep)
	wep)
	    hostap_add_line auth_algs $auth_algs
	    #hostap_add_line wep_default_key $wep_default_key
	    hostap_add_line wep_default_key $wep_key
	    hostap_add_wep_line wep_key0 $wep_key0
	    hostap_add_wep_line wep_key1 $wep_key1
	    hostap_add_wep_line wep_key2 $wep_key2
	    hostap_add_wep_line wep_key3 $wep_key3
	    hostap_add_line wep_rekey_period $wep_rekey_period
	    ;;
	share_wep)
	    hostap_add_line auth_algs $auth_algs
	    #hostap_add_line wep_default_key $wep_default_key
	    hostap_add_line wep_default_key $wep_key
	    hostap_add_wep_line wep_key0 $wep_key0
	    hostap_add_wep_line wep_key1 $wep_key1
	    hostap_add_wep_line wep_key2 $wep_key2
	    hostap_add_wep_line wep_key3 $wep_key3
	    hostap_add_line wep_rekey_period $wep_rekey_period
	    ;;
	wpa|wpa2|wpa_wpa2)
	    case $auth_type in
	       wpa)
	        hostap_add_line wpa "1";;
	       wpa2)
	        hostap_add_line wpa "2";;
	       wpa_wpa2)
	        hostap_add_line wpa "3";;
	    esac
	    hostap_add_line wpa_key_mgmt $wpa_key_mgmt
	    #hostap_add_line wpa_pairwise $wpa_pairwise
	    hostap_add_line wpa_pairwise "$CONFIG_wpa_wpa_pairwise"
	    hostap_add_line wpa_group_rekey $wpa_group_rekey
	    hostap_add_line wpa_gmk_rekey $wpa_gmk_rekey
	    hostap_add_line wpa_ptk_rekey $wpa_ptk_rekey
	    case $wpa_usekey in
	       psk) 
	        hostap_add_line wpa_psk $wpa_psk;;
	       passphrase)
	        hostap_add_line wpa_passphrase $wpa_passphrase;;
	    esac
	    ;;
	*)
	    echo "unknown auth_type $auth_type";;
    esac

# wps
    hostap_add_line wps_state $wps_state
    hostap_add_line eap_server $eap_server

# radius
    hostap_add_line own_ip_addr $own_ip_addr

# neighbor
    hostap_add_line ap_table_max_size  $ap_table_max_size
    hostap_add_line ap_table_expiration_time $ap_table_expiration_time
}

set_hostapd_shm () {
    if [ $# == 1 ]; then
      ushm active $1
    else
      ushm active $radio_enable
    fi
    ushm ssid $ssid
    ushm channel $channel
}
