#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
###################################################################
# Firewall configuration
#
# Description:
#	Firewall configuration.
#
# Author(s) [in order of work date]:
#	Original webif authors.
#	Travis Kemen	<kemen04@gmail.com>
# Major revisions:
#
# UCI variables referenced:
#
# Configuration files referenced:
#	firewall
#



uci_load samba

if empty "$FORM_submit"; then
    
    config_get FORM_enable samba enable

else
    eval FORM_enable="\$FORM_radio_sd_share"

    [ "$?" = "0" ] && {
	uci_set "samba" "samba" "enable" "$FORM_enable"
    }
fi


		
form_sdcard="start_form|@TR<<SD Card Settings>>
	field|@TR<<Network Sharing>>
        string|<br/>
        radio|radio_sd_share|$FORM_enable|1|@TR<<Enabled>>
        radio|radio_sd_share|$FORM_enable|0|@TR<<Disabled>>
        string|<br/><br/>
end_form"

#append forms "$form" "$N"




header_inject_head="<script type=\"text/javascript\" src=\"/webif.js\"></script>"

header "Advanced" "SD Card" "@TR<<SD Memory Card>>" 'onload="modechange()"' "$SCRIPT_NAME"
#####################################################################
# modechange script
#
cat <<EOF
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
#$validate_error
$form_sdcard
EOF

footer ?>
<!--
##WEBIF:name:Advanced:307:SD Card
-->
