#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
###################################################################
# HSPA configuration page
#
# Description:
#	Configures HSPA/3.5G settings.
#
# Author(s) [in order of work date]:
#      
# Major revisions:
#
# UCI variables referenced:
#  
# Configuration files referenced:
#  
#


config_cb() {
	local cfg_type="$1"
	local cfg_name_dhcp="$2"

	case "$cfg_type" in
	    interface)
		append network "$cfg_name_dhcp" "$N"
		;;
	   
	esac
}


uci_load network
network=$(echo "$network" |uniq)

#WWAN_COUNTRY_LIST=$(
#		awk '	BEGIN{FS=":"}
#			$1 ~ /[ \t]*#/ {next}
#			{print "option|" $1 "|@TR<<" $2 ">>"}' < /usr/lib/webif/apn.csv
#	)
#	JS_APN=$(
#		awk '	BEGIN{FS=":"}
#			$1 ~ /[ \t]*#/ {next}
#			{print "	apnDB." $1 " = new Object;"
#			 print "	apnDB." $1 ".name = \"" $3 "\";"
#			 print "	apnDB." $1 ".user = \"" $4 "\";"
#			 print "	apnDB." $1 ".pass = \"" $5 "\";\n"}' < /usr/lib/webif/apn.csv
#	)

WWAN_PROFILE_LIST=$( cat /usr/lib/webif/apnlst.txt | sed '1,1d'|
    awk '	BEGIN{FS=","}
			$1 ~ /[ \t]*#/ {next}
			{print "option|_" $1 "_" $2 "_|@TR<<" $7 ">>"}'
    )
JS_APN=$( cat /usr/lib/webif/apnlst.txt | sed '1,1d'|
    awk '	BEGIN{FS=","}
		$1 ~ /[ \t]*#/ {next}
			{print "        apnDB._" $1 "_" $2 "_ = new Object;"
		         print "	apnDB._" $1 "_" $2 "_.name = \"" $4 "\";"
			 print "	apnDB._" $1 "_" $2 "_.user = \"" $5 "\";"
			 print "	apnDB._" $1 "_" $2 "_.pass = \"" $6 "\";" 
                         print "        apnDB._" $1 "_" $2 "_.dial = \"" $3 "\";\n"}' 
#< /usr/lib/webif/apnlst.txt | sed '1,1d'
    )

append JS_APN_DB "$JS_APN" "$N"


for interface in $network; do

    if [ "$interface" = "wan" ]; then

	#mcc=$(rcmdc -c 'sim.info' | sed 's/^.*MCC //g' | sed 's/, MNC.*$//g')
	#mnc=$(rcmdc -c 'sim.info' | sed 's/^.*MNC//g' | sed 's/, PLMN.*$//g')  

	if empty "$FORM_submit"; then
	    config_get FORM_prefer $interface prefer
	    config_get FORM_authtype $interface auth
	    config_get FORM_roaming $interface roaming
	    config_get FORM_ppp_redial $interface ppp_redial
#	    config_get FORM_keepalive $interface keepalive
	    config_get FORM_demand $interface demand
	    config_get_bool FORM_autoapn $interface autoapn 1
	    config_get FORM_profile $interface profile
	    config_get FORM_apn $interface apn
	    config_get FORM_username $interface username
	    config_get FORM_passwd $interface password
	    config_get FORM_dial $interface dialnum
	    config_get FORM_network_auto $interface network_auto
	    config_get FORM_mccmnc $interface plmn

#	    FORM_network_auto=$(rcmdc -c 'network.selection_mode' | sed 's/^.*selection mode: //g')

	else
	    eval FORM_prefer="\$FORM_prefer"
	    eval FORM_authtype="\$FORM_radio_authtype"
	    eval FORM_roaming="\$FORM_radio_roaming"
	    eval FORM_ppp_redial="\$FORM_ppp_redial"
	    eval FORM_demand="\$FORM_demand"
#	    eval FORM_keepalive="\$FORM_keepalive"
	    eval FORM_autoapn="\$FORM_apn_auto"
	    eval FORM_profile="\$FORM_profile"
	    eval FORM_apn="\$FORM_apn"
	    eval FORM_username="\$FORM_username"
	    eval FORM_passwd="\$FORM_passwd"		
	    eval FORM_dial="\$FORM_dial"
	    eval FORM_network_auto="\$FORM_radio_network_auto"
	    eval FORM_plmn="\$FORM_radio_network_selection"
	    validate <<EOF
EOF
	    [ "$?" = "0" ] && {
		
		uci_set "network" "$interface" "prefer" "$FORM_prefer"
		uci_set "network" "$interface" "auth" "$FORM_authtype"
		uci_set "network" "$interface" "roaming" "$FORM_roaming"
		if [ "$FORM_autoapn" = "1" ]; then
		    uci_set "network" "$interface" "autoapn" "$FORM_autoapn"
		else
		    uci_set "network" "$interface" "autoapn" "0"
		    uci_set "network" "$interface" "profile" "$FORM_profile"
		    uci_set "network" "$interface" "apn" "$FORM_apn" 
		    uci_set "network" "$interface" "username" "$FORM_username"
		    uci_set "network" "$interface" "password" "$FORM_passwd"
		    uci_set "network" "$interface" "dialnum" "$FORM_dial"
		fi

#		if [ "$FORM_ppp_redial" = "persist" ]; then
#		    uci_set "network" "$interface" "keepalive" "$FORM_keepalive"
#		    uci_remove "network" "$interface" "demand"
#		else
#		    uci_remove "network" "$interface" "keepalive"
#		    uci_set "network" "$interface" "demand" "$FORM_demand"
#		fi
		if [ "$FORM_ppp_redial" = "demand" ]; then
		    uci_set "network" "$interface" "demand" "$FORM_demand"
		fi
		uci_set "network" "$interface" "ppp_redial" "$FORM_ppp_redial"
		uci_set "network" "$interface" "network_auto" "$FORM_network_auto"
		if [ "$FORM_network_auto" = "manual" ]; then
		    uci_set "network" "$interface" "plmn" "$FORM_plmn"
		fi
#		current_selection_mode=$(rcmdc -c 'network.selection_mode' | sed 's/^.*selection mode: //g')
#		if [ "$current_selection_mode" = "manual" ] && [ "$FORM_network_auto" = "auto" ]; then
#		    select_result=$(rcmdc -c 'network.selection i/0 s/00000' | sed 's/^.*selection //g')
#		    if [ "$select_result" = "Okay" ]; then
#			append validate_error "Network Selection Mode Change Succeeded!" "$N" 
#		    else
#			append validate_error "Network Selection Mode Change Failed!" "$N" 
#		    fi
#		fi
		
	    }
	fi


if ! empty "$FORM_btn_updateAPN" ; then
		cd /userdata
		getAPN=$(wget http://www.bandrich.com/cmupdate/PR_apnlst.txt -O apnlst.txt)
		if [ "$?" = "0" ] ; then
			getOP=$(wget http://www.bandrich.com/cmupdate/PR_Oplist.txt -O Oplist.txt)
			if [ "$?" = "0" ] ; then						
				cp -f /userdata/apnlst.txt /usr/lib/webif
				cp -f /userdata/Oplist.txt /usr/lib/webif		
				touch /userdata/updAPNsucc
			else				
				touch /userdata/updAPNfail
			fi
		else
		  touch /userdata/updAPNfail										
		fi
		
		rm -f /userdata/apnlst.txt
		rm -f /userdata/Oplist.txt				
fi

#	field|@TR<<Redial Timeout>>|persist_redialperiod|hidden
#	text|keepalive|$FORM_keepalive
#	helpitem|Redial Timeout
#	helptext|Helptext Redial Timeout#The number of seconds to wait after receiving no response from the provider before trying to reconnect
	

	if [ -e /userdata/updAPNsucc ] ; then
		show_msg="helpitem|Update APN status
							helptext|Success
							"
		rm /userdata/updAPNsucc;
	elif [ -e /userdata/updAPNfail ] ; then
		show_msg="helpitem|Update APN status
							helptext|Fail
							"
		rm /userdata/updAPNfail;
	else
		show_msg=""
	fi	
	
	network_options="start_form|@TR<<Configuration>>
        field|@TR<<Authentication Type>>|field_authtype
	radio|radio_authtype|$FORM_authtype|pap|@TR<<PAP>>
	radio|radio_authtype|$FORM_authtype|chap|@TR<<CHAP>>
        field|@TR<<Roaming Connection>>|field_roaming
	radio|radio_roaming|$FORM_roaming|true|@TR<<Enabled>>
	radio|radio_roaming|$FORM_roaming|false|@TR<<Disabled>>
        field|@TR<<Redial Policy>>|redial
	select|ppp_redial|$FORM_ppp_redial
	option|demand|@TR<<Connect on Demand>>
	option|persist|@TR<<Keep Alive>>
	field|@TR<<Maximum Idle Time (in minutes)>>|demand_idletime|hidden
        select|demand|$FORM_demand|180
        option|180|3
        option|300|5
        option|600|10
        option|900|15
        option|1800|30
        option|3600|60
	helpitem|Maximum Idle Time
	helptext|Helptext Idle Time#The number of minutes without internet traffic that the router should wait before disconnecting from the Internet (Connect on Demand only)

        field|@TR<<Auto APN>>|field_auto
        onclick|APN_disabled
        checkbox|apn_auto|$FORM_autoapn|1
        

	submit|btn_updateAPN|@TR<<Update APN>>
	$show_msg
	     
	field|@TR<<Profile>>|field_profile|hidden
	onchange|setAPN
        select|profile|$FORM_profile
	$WWAN_PROFILE_LIST
	field|@TR<<APN>>|field_apn|hidden
	text|apn|$FORM_apn
	field|@TR<<Username>>|field_username|hidden
	text|username|$FORM_username
	field|@TR<<Password>>|field_passwd|hidden
	password|passwd|$FORM_passwd
        field|@TR<<Dial Number>>|field_dial|hidden
	text|dial|$FORM_dial
	onchange|modechange
	end_form"


#	append forms "$network_options" "$N"

	network_selection_begin="start_form|@TR<<Network Selection>>
	field|@TR<<Preference>>|field_prefer
	select|prefer|$FORM_prefer
	option|both|@TR<<Automatic>>
	option|umts|@TR<<UMTS only>>
	option|gprs|@TR<<GPRS only>> 
        field|@TR<<Select Network>>|field_select_network
	radio|radio_network_auto|$FORM_network_auto|auto|@TR<<Auto>>
	radio|radio_network_auto|$FORM_network_auto|manual|@TR<<Manual>>  
        field|@TR<<Availabe Networks>>|field_scan_network
        submit|but_rescan|@TR<<Scan Network>>"
 
	###################################################################
	# set JavaScript
	javascript_forms="	
	set_visible('demand_idletime', isset('ppp_redial', 'demand'));
        var x = !(checked('apn_auto_1'));
        set_visible('field_profile', x);
        set_visible('field_apn', x);
        set_visible('field_username', x);
        set_visible('field_passwd', x);
        set_visible('field_dial', x);"

	append js "$javascript_forms" "$N"

	wwan_js="
        document.getElementById(\"apn\").value = apnDB[element.value].name;
	document.getElementById(\"username\").value = apnDB[element.value].user;
	document.getElementById(\"passwd\").value = apnDB[element.value].pass;
        document.getElementById(\"dial\").value = apnDB[element.value].dial;"	

    fi
done


header "Advanced" "HSPA Config" "@TR<<HSPA Configuration>>" ' onload="loadpage()" ' "$SCRIPT_NAME"
#####################################################################
# modechange script
#
cat <<EOF
<script type="text/javascript" src="/webif.js"></script>
<script type="text/javascript">
<!--
function setAPN(element) 
{
        var apnDB = new Object();
        $JS_APN_DB
        $wwan_js
}

function modechange()
{
	var v;
	$js

	hide('save');
	show('save');

}


function loadpage()
{
	var v;
	$js

	hide('save');
	show('save');

}


function APN_disabled() 
{
       var v = !(checked('apn_auto_1'));
       
       set_visible('field_profile', v);
       set_visible('field_apn', v);
       set_visible('field_username', v);
       set_visible('field_passwd', v);
       set_visible('field_dial', v);
       
}

-->
</script>

EOF

display_form <<EOF
onchange|modechange
$validate_error
$network_options
$network_selection_begin
EOF

# outputs variable to a column

#available_network=$(rcmdc -c 'network.available' | awk '{system( "show_row $5 $6"}')
if ! empty "$FORM_but_rescan" && [ "$FORM_radio_network_auto" = "manual" ]; then
    
#    run_command=$(/etc/init.d/br_wan stop)
    run_command=$(kill -USR1 `pidof MonitorWan` && sleep 15)

    available_network="<table style=\"width: 100%; margin-left: 2.5em; text-align: left; font-size: 0.8em;\" border=\"0\" cellpadding=\"4\" cellspacing=\"3\">
<tbody><tr><th></th><th>@TR<<PLMN>></th><th>@TR<<Operator>></th><th>@TR<<Type>></th></tr>" 
    network_list=$(rcmdc -c 'network.available' | 
	awk 'BEGIN{odd=1;counter=0;}
             function print_row(){
                              mcc=substr($5,1,3);
                              mnc=substr($5,4);
                              counter++;
                              if (odd == 1){
                              print "<tr class=\"odd\"><td><input type=\"radio\" name=\"radio_network_selection\" value=\""$5"\"></td><td>"$5"</td><td>";
                              odd--;
                              } else {
                              print "<tr><td><input type=\"radio\" name=\"radio_network_selection\" value=\""$5"\"></td><td>"$5"</td><td>";
                              odd++;
                              }
                              system("grep "mcc","mnc", /usr/lib/webif/Oplist.txt | cut -d ',' -f 4") | getline name;
                              print "</td><td>"$6"</td></tr>";    
                             } {print_row();}')
    append available_network "$network_list" "$N"
    append available_network "</tbody></table>" "$N"

    #run_command=$(/etc/init.d/br_wan start)
    run_command=$(kill -USR1 `pidof MonitorWan`)

#available_network=$(rcmdc -c 'network.available' | awk '{print "<tr><td>"$5"</td>""<td>test</td>""<td>"$6"</td></tr>"}')
#$(rcmdc -c 'network.selection i/1 s/46697')

else
    available_network="@TR<<Information Not Available>>"
fi

#if ! empty "$FORM_but_select_network" && [ "$FORM_radio_network_auto" = "manual" ]; then
#    run_command=$(ifdown wan | sleep 5)

#    select_result=$(rcmdc -c "network.selection i/1 s/$FORM_radio_network_selection" | sed 's/^.*selection //g')
#    if [ "$select_result" = "Okay" ]; then
#	echo "<br /><div class=warning>Manually Network Selection Succeeded!</div>"
#    else
#	echo "<br /><div class=warning>Manually Network Selection Failed!</div>"
#    fi

#    run_command=$(ifup wan)
#fi
cat <<EOF
<br />
$available_network
EOF
#submit|but_select_network|@TR<< Select >>

display_form <<EOF
end_form
EOF

footer ?>
<!--
##WEBIF:name:Advanced:302:HSPA Config
-->
