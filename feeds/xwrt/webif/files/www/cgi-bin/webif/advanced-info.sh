#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
header "Advanced" "Info" "<alt=\"@TR<<System Information>>\"/>@TR<<System Information>>"

config_cb() {
	local cfg_type="$1"
	local cfg_name="$2"

	case "$cfg_type" in
		interface)
			if [ "$cfg_name" != "loopback" ]; then
				append networks "$cfg_name" "$N"
			fi
		;;
		dhcp)
			append dhcp_cfgs "$cfg_name" "$N"
		;;
	esac
}


config_get_bool show_banner general show_banner 0
[ 1 -eq "$show_banner" ] && {
	echo "<pre>"
	cat /etc/banner 2>/dev/null
	echo "</pre><br />"
}


#capacity_num=$(sqlite3 /testdb "select capacity from battery order by time desc limit 1")
    

#if [ $operator == "" ]; then
#    operator="Information not provided"
#fi

#if[ $apn == "" ]; then
#    apn="Information not provided"
#fi


uci_load network

config_get prefer wan prefer 
#	echo "<div class=warning>$prefer</div>"

config_get autoapn wan autoapn
config_get apn_manuel wan apn
config_get username wan username
config_get password wan password
   
if [ "$prefer" = "both" ]; then
    connection_type="@TR<<Automatic>>"
    
elif [ "$prefer" = "umts" ]; then
    connection_type="@TR<<UMTS only>>"
    
elif [ "$prefer" = "gprs" ]; then
    connection_type="@TR<<GPRS only>>"
    
else
    connection_type="N/A"
fi



uci_load hostapd

config_get wifi_channel general channel
config_get ssid general ssid
config_get wifi_security auth auth_type
config_get_bool ignore_ssid_broadcast general ignore_broadcast_ssid 0
    
if [ $ignore_ssid_broadcast -eq 0 ]; then   
    wifi_broadcast="@TR<<On>>"
else
    wifi_broadcast="@TR<<Off>>"
fi

battery_capacity=$(BatteryGet | sed '1d' | sed 's/Battery Capacity://g') 
if [ $battery_capacity -eq 6 ] || [ $battery_capacity -eq 5 ]; then
    
    battery_status="@TR<<Charging Mode>>"
    capacity="string|<img name=\"battery_img\" src=\"/images/charging.jpg\" alt=\"\" />"
else
    
    battery_status="@TR<<Battery Mode>>"
    
    if [ $battery_capacity -eq 4 ]; then
	capacity="string|<img name=\"battery_img\" src=\"/images/battery_4.jpg\" alt=\"\" />"

    elif [ $battery_capacity -eq 3 ]; then
	capacity="string|<img name=\"battery_img\" src=\"/images/battery_3.jpg\" alt=\"\" />"

    elif [ $battery_capacity -eq 2 ]; then
	capacity="string|<img name=\"battery_img\" src=\"/images/battery_2.jpg\" alt=\"\" />"

    elif [ $battery_capacity -eq 1 ]; then
	capacity="string|<img name=\"battery_img\" src=\"/images/battery_1.jpg\" alt=\"\" />"

    elif [ $battery_capacity -eq 0 ]; then
	capacity="string|<img name=\"battery_img\" src=\"/images/battery_0.jpg\" alt=\"\" />"

    else
	capacity="string|N/A"
    fi
	
fi


#               string|<div style=\"text-align: left\">$charg_text</div>
form_system="start_form|@TR<<System Status>>
               field|@TR<<Battery Status>>
               string|$battery_status
               field|@TR<<Battery Capacity>>
               $capacity
               end_form"


local_ip_lan=$(/sbin/ifconfig br-lan | grep 'inet addr' | sed 's/^.*addr://g' | sed 's/Bcast.*$//g')

#local_mask=$(/sbin/ifconfig br-lan | grep 'Mask' | sed 's/^.*Mask://g')

uci_load dhcp
#uci_load udhcpd
dhcp_cfgs=$(echo "$dhcp_cfgs" |uniq)
for config in $dhcp_cfgs; do
    config_get interface $config interface
    
    if [ "$interface" = "lan" ]; then 
	config_get start $config start
	config_get limit $config limit
	config_get wfip  $config wfip
	config_get wfmask  $config wfmask
    fi
    
    if [ "$start" != "" ] || [ "$limit" != "" ]; then
	# who can tell us which ip to show (wifi or br-lan) ?
	dhcp_start=$(echo $wfip | awk -v VAR=${start} '{FS="."; print $1"."$2"."$3"."VAR}')
	end=$((start+limit-1))
	dhcp_end=$(echo $wfip | awk -v VAR=${end} '{FS="."; print $1"."$2"."$3"."VAR}')

    else
	dhcp_start="N/A"
	dhcp_end="N/A"
    fi
    
done

form_local="start_form|@TR<<Local Network>>
        field|@TR<<Router IP Address (USB)>>
        string|$local_ip_lan
        field|@TR<<Router IP Address (WLAN)>> 
        string|$wfip
        field|@TR<<Netmask (WLAN)>>
        string|$wfmask
        field|@TR<<DHCP Start IP Address>>
        string|$dhcp_start
        field|@TR<<DHCP End IP Address>>
        string|$dhcp_end
        end_form"

#tech_num=$(rcmdc -c 'network.status' | sed 's/^.*RAT2 //g' | sed 's/, ST.*$//g')
tech=$(GetMemory -i | grep 'Tech_name' | sed 's/Tech_name=//g')
#roam_num=$(rcmdc -c 'network.status' | sed 's/^.*Roaming Status //g' | sed 's/, PLMN.*$//g')
roam_num=$(GetMemory -i | grep 'roaming' | sed 's/roaming=//g')
#PIN_status_num=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g')
#plmn=$(rcmdc -c 'network.status' | sed 's/^.*PLMN //g' | sed 's/, RAT1.*$//g')
#mcc=$(echo $plmn | cut -c 1,2,3)
#mnc=$(echo $plmn | sed 's/^...//g')
mcc=$(GetMemory -i | grep 'nwk_mcc' | sed 's/nwk_mcc=//g')
mnc=$(GetMemory -i | grep 'nwk_mnc' | sed 's/nwk_mnc=//g')
operator=$(GetMemory -i | grep 'OP_Name' | cut -d= -f2)                        
if [ $operator = "" ]; then                                                     
        operator="N/A"                                                          
fi

if [ $autoapn -eq 0 ]; then
    apn=$apn_manuel
elif [ "$mcc" != "000" ] && [ "$mnc" != "00" ]; then
#    apn=$(grep $mcc,$mnc, /usr/lib/webif/apnlst.txt | cut -d ',' -f 4)
    apn=$(GetAPN | cut -d '"' -f 4)
    username=$(GetAPN | cut -d\; -f 2)
    password=$(GetAPN | cut -d\; -f 3)
else
	apn="N/A"
    username="N/A"
    password="N/A"
fi

#connection technology 
#if [ $tech_num -eq 1 ]; then    
#    tech="2G (GSM)"
#elif [ $tech_num -eq 2 ]; then    
#    tech="2.5G (GPRS)"
#elif [ $tech_num -eq 4 ]; then    
#    tech="3G (WCDMA)"
#elif [ $tech_num -eq 5 ] || [ $tech_num -eq 6 ]; then    
#    tech="3.5G (HSPA)"
#else
#    tech="N/A"
#fi
if [ "$tech" = "" ]; then
    tech="N/A"
fi


#PIN status
#if [ $PIN_status_num -eq 0 ]; then    
#    PIN_status="PIN Uninitialized"
#elif [ $PIN_status_num -eq 1 ]; then
#    PIN_status="PIN Disabled"
#elif [ $PIN_status_num -eq 2 ]; then    
#    PIN_status="PIN Enabled"
#elif [ $PIN_status_num -eq 3 ]; then    
#    PIN_status="PIN Blocked"
#elif [ $PIN_status_num -eq 4 ]; then    
#    PIN_status="PIN Permanently Blocked"
#else
#    PIN_status="N/A"
#fi

if [ $PIN_status_num -eq 0 ]; then    
#    PIN_status="PIN Ready"
    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
    if [ $PIN_status_recheck -eq 1 ]; then
	PIN_status="@TR<<PIN Disabled>>"
    elif [ $PIN_status_recheck -eq 2 ]; then    
	PIN_status="@TR<<PIN Enabled>>"
    fi
elif [ $PIN_status_num -eq 1 ]; then
    PIN_status="@TR<<PIN Enabled requires verification>>"
elif [ $PIN_status_num -eq 2 ]; then    
    PIN_status="@TR<<PIN Blocked>>"
elif [ $PIN_status_num -eq 3 ]; then    
    PIN_status="@TR<<PIN Invalid>>"
elif [ $PIN_status_num -eq -1 ]; then    
    PIN_status="@TR<<Read SIM Failed>>"
else
    PIN_status="N/A"
fi

# roaming
if [ $roam_num -eq 0 ]; then    
    roaming="@TR<<Home>>"
elif [ $roam_num -eq 1 ]; then
    roaming="@TR<<Roaming>>"
else
    roaming="N/A"
fi

form_register_network="start_form|@TR<<Register Network>>
        field|@TR<<SIM Status>>
        string|$PIN_status
	field|@TR<<Network Name>>
        string|@TR<<$operator>>
        field|@TR<<Network Technology>>
        string|@TR<<$tech>>
        field|@TR<<Roaming>>
        string|$roaming
        end_form"

network_ip=$(cat /tmp/state/network | grep 'wan.ipaddr' | sed 's/^.*ipaddr=//g')
if [ "$network_ip" = "" ]; then
    network_ip="N/A"
fi
network_mask=$(cat /tmp/state/network | grep 'wan.netmask' | sed 's/^.*netmask=//g')
if [ "$network_mask" = "" ]; then
    network_mask="N/A"
fi
network_gateway=$(cat /tmp/state/network | grep 'wan.gateway' | sed 's/^.*gateway=//g')
if [ "$network_gateway" = "" ]; then
    network_gateway="N/A"
fi
network_dns1=$(cat /tmp/resolv.conf.auto | sed '2,$d' | sed 's/nameserver//g')
if [ "$network_dns1" = "" ]; then
    network_dns1="N/A"
fi
network_dns2=$(cat /tmp/resolv.conf.auto | sed '1d' | sed 's/nameserver//g')
if [ "$network_dns2" = "" ]; then
    network_dns2="N/A"
fi
form_connection="start_form|@TR<<Internet Connection>>
	field|@TR<<Connection Type>>
        string|$connection_type
        field|@TR<<APN>>
        string|$apn
        field|@TR<<Username>>
        string|$username
        field|@TR<<Password>>
        string|$password
        field|@TR<<Internet IP Address>>
        string|$network_ip
        field|@TR<<Subnet Mask>>
        string|$network_mask
        field|@TR<<Default Gateway>>
        string|$network_gateway
        field|@TR<<DNS 1>>
        string|$network_dns1
        field|@TR<<DNS 2>>
        string|$network_dns2
        end_form"



if [ $wifi_channel -eq 0 ]; then
    wifi_channel="@TR<<Auto>>"
fi

if [ "$wifi_security" = "open_no" ]; then
    wifi_security_display="@TR<<Disabled>>"
elif [ "$wifi_security" = "wep" ]; then
    wifi_security_display="WEP"
elif [ "$wifi_security" = "wpa" ]; then
    wifi_security_display="WPA (@TR<<Personal>>)"
elif [ "$wifi_security" = "wpa2" ]; then
    wifi_security_display="WPA2 (@TR<<Personal>>)"
elif [ "$wifi_security" = "wpa2" ]; then
    wifi_security_display="WPA+WPA2 (@TR<<Personal>>)"
else
    wifi_security_display="N/A"
fi


form_wifi="start_form|@TR<<WLAN Network>>
        field|@TR<<SSID>>
        string|$ssid
	field|@TR<<Standard Channel>>
        string|$wifi_channel
        field|@TR<<Security>>
        string|$wifi_security_display
        field|@TR<<SSID Broadcast>>
        string|$wifi_broadcast
        end_form"

router_fw=$(cat /etc/BR_version)
if [ "$router_fw" = "" ]; then
    router_fw="N/A"
fi
#modem_fw=$(rcmdc -c 'bl.version' | sed 's/Firmware Version://g')
modem_fw=$(GetMemory -i | grep 'fw_version' | sed 's/fw_version=//g')
if [ "$modem_fw" = "" ]; then
    modem_fw="N/A"
fi
form_firmware="start_form|@TR<<Firmware Information>>
               field|@TR<<Router Firmware Version>>
               string|$router_fw
               field|@TR<<Modem Firmware Version>>
               string|$modem_fw
#               field|@TR<<APN Version>>        
               end_form"




display_form << EOF 
$form_system
$form_local
$form_register_network
$form_connection
$form_wifi
$form_firmware
EOF


}

footer

?>
<!--
##WEBIF:name:Advanced:301:Info
-->
