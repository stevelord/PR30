#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
###################################################################
# DHCP configuration
#
# Description:
#	DHCP configuration.
#
# Author(s) [in order of work date]:
#	Travis Kemen	<thepeople@users.berlios.de>
#	Adam Hill	<adam@voip-x.co.uk>
# Major revisions:
#	Allow DHCP options to be specified ( Adam H )
#
# UCI variables referenced:
#
# Configuration files referenced:
#   dhcp, network
#

header "Advanced" "DHCP" "@TR<<DHCP Configuration>>" 'onload="modechange()"' "$SCRIPT_NAME"

###################################################################
# Parse Settings, this function is called when doing a config_load
#config_cb() {
#	local cfg_type="$1"
#	local cfg_name="$2"

#	case "$cfg_type" in
#		interface)
#			if [ "$cfg_name" != "loopback" ]; then
#				append networks "$cfg_name" "$N"
#			fi
#		;;
#		dhcp)
#			append dhcp_cfgs "$cfg_name" "$N"
#		;;
#		dnsmasq)
#			append dnsmasq_cfgs "$cfg_name" "$N"
#		;;
#		host)
#			append host_cfgs "$cfg_name" "$N"
#		;;
#	esac
#}

#uci_load network
uci_load dhcp
# create dnsmasq's section when missing
#[ -z "$dnsmasq_cfgs" ] && {
#	uci_add dhcp dnsmasq
#	unset dhcp_cfgs dnsmasq_cfgs
#	uci_load dhcp
#}

#vcfg_number=$(echo "$dhcp_cfgs" "$dnsmasq_cfgs" |wc -l)
#let "vcfg_number+=1"


#dhcp_cfgs=$(echo "$dhcp_cfgs" |uniq)
config="lan"
#for config in $dhcp_cfgs; do
#	count=1
#	config_get interface $config interface
#	if [ "$interface" = "lan" ]; then
if [ "$FORM_submit" = "" ]; then
           config_get start $config start
	   config_get limit $config limit
	   config_get leasetime $config wfleasetime
	   config_get_bool wfenable $config wfenable 1

else
           eval start="\$FORM_start_$config"
	   eval limit="\$FORM_limit_$config"
	   eval leasetime="\$FORM_leasetime_$config"
	   eval wfenable="\$FORM_wfenable_$config"

	    
	   append validate_forms "int|FORM_start_$config|@TR<<DHCP Start IP>>|min=1 max=254|$start" "$N"
	   append validate_forms "int|FORM_limit_$config|@TR<<DHCP Limit>>|min=0 max=255|$limit" "$N"
	   append validate_forms "int|FORM_leasetime_$config|@TR<<DHCP Lease Time>>|min=1 max=2147483647|$leasetime" "$N"

#	    SAVED=1

	   ip_total=$(($start+$limit))
	   if [ $ip_total -gt 255 ]; then
	       append error "<div class=warning> @TR<<IP range cannot be over 255>></div>" "$N"
	       check_ip=-1
	   else
	       check_ip=0
	   fi

	   validate <<EOF
$validate_forms
EOF
	   [ "$?" = "0" ]  && [ $check_ip -eq 0 ] && {	
	       
	       uci_set "dhcp" "$config" "start" "$start"
	       uci_set "dhcp" "$config" "limit" "$limit"
	       uci_set "dhcp" "$config" "wfleasetime" "$leasetime"
	       uci_set "dhcp" "$config" "wfenable" "$wfenable"			
		
	   }
fi
	
form_dhcp="start_form|@TR<<DHCP Server>>
		field|@TR<<DHCP>>
		radio|wfenable_$config|$wfenable|1|@TR<<On>>
		radio|wfenable_$config|$wfenable|0|@TR<<Off>>
		field|@TR<<Start>>
		text|start_$config|$start
		field|@TR<<Limit>>
		text|limit_$config|$limit
		field|@TR<<Lease Time (in minutes)>>
		text|leasetime_$config|$leasetime
		end_form"

append forms "$form_dhcp" "$N"
#    done
    
#fi
		
		

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
string|$error
$validate_error
$forms
EOF

cat <<EOF
<hr class="separator" />
<h5><strong>@TR<<network_hosts_Active_Leases#Active DHCP Leases>></strong></h5>
<table style="width: 100%; margin-left: 2.5em; text-align: left; font-size: 0.8em;" border="0" cellpadding="3" cellspacing="2" summary="Settings">
<tr>
	<th>@TR<<network_hosts_MAC#MAC Address>></th>
	<th>@TR<<network_hosts_IP#IP Address>></th>
	<th>@TR<<network_hosts_Name#Name>></th>
	<th>@TR<<network_hosts_Expires#Expires in>></th>
</tr>
EOF

#cat /tmp/udhcpdWf.leases 2>/dev/null | awk -v date="$(date +%s)" '

dumpleases -f /tmp/udhcpdWf.leases | sed -e '1,1d' | awk '
BEGIN {
	odd=1
	counter = 0
}
$1 > 0 {
	counter++
	if (odd == 1)
	{
		print "	<tr>"
		odd--
	} else {
		print "	<tr class=\"odd\">"
		odd++
	}
	print "		<td>" $1 "</td>"
	print "		<td>" $2 "</td>"
	print "		<td>" $3 "</td>"
	print "		<td>"
	
        if ($4 == "expired")
        {
               print "@TR<<expired>>"
	       print "		</td>"
	       print "	</tr>"
        } else {
               print $4
	       print "		</td>"
	       print "	</tr>"
        }
}
END {
	if (counter == 0) {
		print "	<tr>"
		print "		<td colspan=\"4\">@TR<<network_hosts_No_leases#There are no known DHCP leases.>></td>"
		print "	</tr>"
	}
	print "</table>"
}'

footer ?>
<!--
##WEBIF:name:Advanced:305:DHCP
-->
