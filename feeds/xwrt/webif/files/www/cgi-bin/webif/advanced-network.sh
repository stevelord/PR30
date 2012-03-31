#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
###################################################################
# WAN and LAN configuration page
#
# Description:
#	Configures basic LAN interface settings.
#
# Author(s) [in order of work date]:
#
# Major revisions:
#
# UCI variables referenced:
#   todo
# Configuration files referenced:
#   /etc/config/network
#


config_cb() {
	local cfg_type="$1"
	local cfg_name="$2"
	
	case "$cfg_type" in
#	    interface)
#		if [ "$cfg_name" != "loopback" ]; then
#		    append networks "$cfg_name" "$N"
#		fi
#		;;
	    dhcp)
		append dhcp_cfgs "$cfg_name" "$N"
		;;

	esac
}



uci_load dhcp
dhcp_cfgs=$(echo "$dhcp_cfgs" |uniq)
uci_load network

#for config in $dhcp_cfgs; do
#    config_get inter $config interface
#   if [ "$inter" = "lan" ]; then
	if empty "$FORM_submit"; then
#		config_get FORM_wfip $config wfip
#		config_get FORM_netmask $config wfmask
	        config_get FORM_wfip lan wfip
		config_get FORM_netmask lan wfmask
		config_get FORM_ipaddr lan ipaddr

	else
		eval FORM_wfip="\$FORM_text_wfip"
		eval FORM_netmask="\$FORM_text_netmask"
		eval FORM_ipaddr="\$FORM_lan_ipaddr"

		wifi_ip_head=$(echo $FORM_wfip | cut -d'.' -f 1,2,3)
		usb_ip_head=$(echo $FORM_ipaddr | cut -d'.' -f 1,2,3)
		if [ "$wifi_ip_head" = "$usb_ip_head" ]; then
		    #echo "<div class=warning> Please specify different IP address for USB and WLAN connection</div>"
		    append error "<div class=warning> Please specify different IP address for USB and WLAN connection</div>" "$N"
		    check_ip=-1
		else
		    check_ip=0
		fi
		validate <<EOF
ip|FORM_text_wfip|@TR<<WLAN IP Address>>||$FORM_wfip
netmask|FORM_text_netmask|@TR<<WLAN Netmask>>||$FORM_netmask
ip|FORM_lan_ipaddr|@TR<<USB IP Address>>||$FORM_ipaddr
EOF
	
		[ "$?" = "0" ] && [ $check_ip -eq 0 ] && {
       			
		    uci_set "dhcp" "lan" "wfip" "$FORM_wfip"
		    uci_set "dhcp" "lan" "wfmask" "$FORM_netmask"
		    uci_set "network" "lan" "ipaddr" "$FORM_ipaddr"
		}
	fi
	
	wifi_network_options="start_form|@TR<<WLAN Network Configuration>>
	field|@TR<<Router IP Address Via WLAN>>|field_wfip
	text|text_wfip|$FORM_wfip
	field|@TR<<Netmask>>|field_netmask
	text|text_netmask|$FORM_netmask
	end_form"

	lan_network_options="start_form|@TR<<USB Network Configuration>>
	field|@TR<<Router IP Address Via USB>>|field_lan_ipaddr
	text|lan_ipaddr|$FORM_ipaddr
	end_form"

	append forms "$wifi_network_options" "$N"

#    fi
#done
#uci_load network
#network=$(echo "$network" |uniq)


#for interface in $network; do
#    if [ "$interface" = "lan" ]; then
#	if empty "$FORM_submit"; then
#	        config_get FORM_ipaddr lan ipaddr
#		config_get FORM_netmask $config wfmask

#	else
#		eval FORM_ipaddr="\$FORM_lan_ipaddr"
#		eval FORM_netmask="\$FORM_${config}_netmask"

#	if [ "$FORM_wfip" = "$FORM_ipaddr" ]; then
#		    echo "<div class=warning> Please specify different IP address for USB and WLAN connection</div>"
#	fi
#		append validate_forms "ip|FORM_lan_ipaddr|@TR<<USB IP Address>>||$FORM_ipaddr" "$N"			

#		[ "$?" = "0" ] && {     			
#		    uci_set "network" "lan" "ipaddr" "$FORM_ipaddr"
#		}
#	fi
	


#	append forms "$lan_network_options" "$N"

#    fi
#done


uci_load firewall
#firewall=$(echo "$firewall" |uniq)

if empty "$FORM_submit"; then

    config_get_bool FORM_enable defaults enable 1

else
    eval FORM_enable="\$FORM_radio_firewall"
    
    [ "$?" = "0" ] && {
	uci_set "firewall" "defaults" "enable" "$FORM_enable"
    }
fi

form_firewall="start_form|@TR<<Basic Firewall>>
	field|@TR<<Firewall Settings>>
        string|<br/>
        radio|radio_firewall|$FORM_enable|1|@TR<<Enabled>>
        radio|radio_firewall|$FORM_enable|0|@TR<<Disabled>>
        string|<br/><br/>
end_form"


header "Advanced" "Network" "@TR<<Network Configuration>>" ' onload="modechange()" ' "$SCRIPT_NAME"
#####################################################################
# modechange script
#
cat <<EOF
<script type="text/javascript" src="/webif.js"></script>
<script type="text/javascript">
<!--

function modechange()
{
	var v;
	$js

	hide('save');
	show('save');
}
-->
</script>

EOF

display_form <<EOF
onchange|modechange
$validate_error
string|$error
$lan_network_options
$forms
$form_firewall
EOF

footer ?>
<!--
##WEBIF:name:Advanced:303:Network
-->
