#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh


uci_load hostapd
uci_load wfpower
#FIXME: uci_load bug
#uci_load will pass the same config twice when there is a section to be added by using uci_add before a uci_commit happens
#we will use uniq so we don't try to parse the same config section twice.
#vface=$(echo "$vface" |uniq)

#vcfg_number=$(echo "$DEVICES $N $vface" |wc -l)
#let "vcfg_number+=1"

hostapd=$(echo "hostapd" |uniq)


#####################################################################
# This is looped for every physical wireless card (wifi-device)
#

if empty "$FORM_submit"; then

    config_get FORM_offmode default offmode
    config_get FORM_idletime default idletime
    config_get FORM_channel general channel
    config_get_bool FORM_radio_enable general radio_enable 1
    config_get FORM_ssid general ssid
    config_get_bool  FORM_broadcast general ignore_broadcast_ssid 0
    
    config_get FORM_encryption auth auth_type

    case "$FORM_encryption" in
	open_no)
	    FORM_key=""
	    ;;
	wep) 
	    config_get FORM_wep_key wep wep_default_key
#	    FORM_wep_key="$FORM_key"
	    FORM_key=""
	    config_get FORM_key1 wep wep_key0
	    config_get FORM_key2 wep wep_key1
	    config_get FORM_key3 wep wep_key2
	    config_get FORM_key4 wep wep_key3
	    config_get FORM_wep_passphrase wep wep_passphrase
	    ;;
	wpa|wpa2|wpa_wpa2) 
	    config_get FORM_wpa_encryption wpa wpa_pairwise
	    config_get FORM_wpa_usekey wpa wpa_usekey
	    
	    if [ "$FORM_wpa_usekey" = "psk"]; then
		config_get FORM_key wpa wpa_psk
	    else
		config_get FORM_key wpa wpa_passphrase
	    fi	    
	    ;;
    esac

else

    eval FORM_offmode="\$FORM_radio_offmode"
    eval FORM_idletime="\$FORM_select_idletime"
    eval FORM_channel="\$FORM_select_channel"
    eval FORM_radio_enable="\$FORM_radio_radio_enable"
    eval FORM_ssid="\$FORM_text_ssid"
    eval FORM_broadcast="\$FORM_radio_broadcast_ssid"

    eval FORM_encryption="\$FORM_select_encryption"

#    validate <<EOF
#EOF
    case "$FORM_encryption" in
	open_no)
	    FORM_key=""
	    ;;
	wep) 
	    eval FORM_wep_key="\$FORM_radio_wep_key"
	    eval FORM_wep_passphrase="\$FORM_wep_passphrase"
	    FORM_key=""
	    ;;
	wpa|wpa2|wpa_wpa2) 
	    eval FORM_wpa_encryption="\$FORM_select_wpa_encryption"
#	    eval FORM_wpa_usekey="\$FORM_wpa_usekey"   
	    eval FORM_key="\$FORM_wpa_psk"
	    ;;
    esac
    			
    eval FORM_key1="\$FORM_key1"
    eval FORM_key2="\$FORM_key2"
    eval FORM_key3="\$FORM_key3"
    eval FORM_key4="\$FORM_key4"
    
    empty "$FORM_generate_wep_128" && empty "$FORM_generate_wep_40" &&
    {

	case "$FORM_encryption" in
	    wpa|wpa2|wpa_wpa2) 
		append validate_forms "wpapsk|FORM_wpa_psk|@TR<<Pre-Shared Key>>|required min=8 max=64|$FORM_key" "$N"
		;;
	    
	    wep)
		append validate_forms "int|FORM_radio_wep_key|@TR<<Selected WEP Key>>|required min=1 max=4|$FORM_wep_key" "$N"
		append validate_forms "wep|FORM_key1|@TR<<WEP Key>> 1|required|$FORM_key1" "$N"
		append validate_forms "wep|FORM_key2|@TR<<WEP Key>> 2||$FORM_key2" "$N"
		append validate_forms "wep|FORM_key3|@TR<<WEP Key>> 3||$FORM_key3" "$N"
		append validate_forms "wep|FORM_key4|@TR<<WEP Key>> 4||$FORM_key4" "$N"
		;;
	esac
	SAVED=1
	validate <<EOF
string|FORM_text_ssid|@TR<<SSID>>|required min=1 max=14|$FORM_ssid
$validate_forms
EOF


	if [ "$?" = "0" ]; then
	    uci_set "wfpower" "default" "offmode" "$FORM_offmode"
	    uci_set "wfpower" "default" "idletime" "$FORM_idletime"

	    uci_set "hostapd" "general" "channel" "$FORM_channel"
	    uci_set "hostapd" "general" "radio_enable" "$FORM_radio_enable"
	    uci_set "hostapd" "general" "ssid" "$FORM_ssid"
	    uci_set "hostapd" "general" "ignore_broadcast_ssid" "$FORM_broadcast"

	    uci_set "hostapd" "auth" "auth_type" "$FORM_encryption"
	  
	    case "$FORM_encryption" in
		wep) 
		    uci_set "hostapd" "wep" "wep_default_key" "$FORM_wep_key"
		    uci_set "hostapd" "wep" "wep_passphrase" "$FORM_wep_passphrase"
		    uci_set "hostapd" "wep" "wep_key0" "$FORM_key1"
		    uci_set "hostapd" "wep" "wep_key1" "$FORM_key2"
		    uci_set "hostapd" "wep" "wep_key2" "$FORM_key3"
		    uci_set "hostapd" "wep" "wep_key3" "$FORM_key4"
		    uci_set "hostapd" "wps" "wps_state" "0"
		    ;;
	
		wpa|wpa2|wpa_wpa2) 
		    if [ ${#FORM_key} -eq 64]; then
			uci_set "hostapd" "wpa" "wpa_psk" "$FORM_key"
			uci_set "hostapd" "wpa" "wpa_usekey" "psk"
		    else		   
			uci_set "hostapd" "wpa" "wpa_passphrase" "$FORM_key"
			uci_set "hostapd" "wpa" "wpa_usekey" "passphrase"
		    fi
		    uci_set "hostapd" "wpa" "wpa_pairwise" "$FORM_wpa_encryption"
		    uci_set "hostapd" "wps" "wps_state" "2"
		    ;;
		open_no)
		    uci_set "hostapd" "wps" "wps_state" "2"
		    ;;
	    esac

	fi
    }
fi

append forms "start_form|@TR<<Basic Configuration>>" "$N"

#		        radio|radio_offmode|$FORM_offmode|auto|@TR<<Auto-off>>
#			radio|radio_offmode|$FORM_offmode|manual|@TR<<Manual-off>>
power_saving="field|@TR<<Operation Mode>>
              select|radio_offmode|$FORM_offmode
                        option|auto|@TR<<Auto-off>>
                        option|manual|@TR<<Manual-off>>
              field|@TR<<Maximum Idle Time>>|field_idletime|hidden
			select|select_idletime|$FORM_idletime|600
                        option|180|3
                        option|300|5
                        option|600|10
                        option|900|15
                        option|1800|30
                        option|3600|60
              string|@TR<<Minutes>>
"
append forms "$power_saving" "$N"


mode_radio_enable="field|@TR<<Radio>>
			radio|radio_radio_enable|$FORM_radio_enable|1|@TR<<On>>
			radio|radio_radio_enable|$FORM_radio_enable|0|@TR<<Off>>"
append forms "$mode_radio_enable" "$N"

CHANNELS="field|@TR<<Channel>>|channelform
			select|select_channel|$FORM_channel
                        option|0|@TR<<Auto>>
                        option|1|1
                        option|2|2
                        option|3|3
                        option|4|4
                        option|5|5
                        option|6|6
                        option|7|7
                        option|8|8
                        option|9|9
                        option|10|10
                        option|11|11"


append forms "$CHANNELS" "$N"

ssid="field|@TR<<SSID>>|ssid_form
      text|text_ssid|$FORM_ssid"
append forms "$ssid" "$N"
hidden="field|@TR<<SSID Broadcast>>|broadcast_form
	radio|radio_broadcast_ssid|$FORM_broadcast|0|@TR<<On>>
	radio|radio_broadcast_ssid|$FORM_broadcast|1|@TR<<Off>>"
append forms "$hidden" "$N"
append forms "end_form" "$N"



    

append forms "start_form|@TR<<WLAN Security Setting>>" "$N"



			###################################################################
			# Generate 4 40-bit WEP keys or 1 128-bit WEP Key
eval FORM_wep_passphrase="\$FORM_wep_passphrase"
[ "$FORM_wep_passphrase" = "" ] && FORM_wep_passphrase="$(dd if=/dev/urandom count=200 bs=1 2>/dev/null|tr "\n" " "|sed 's/[^a-zA-Z0-9]//g')"
eval FORM_generate_wep_128="\$FORM_generate_wep_128"
eval FORM_generate_wep_40="\$FORM_generate_wep_40"
! empty "$FORM_generate_wep_128" &&
{
    FORM_wep_key="1"
    FORM_key1=""
    FORM_key2=""
    FORM_key3=""
    FORM_key4=""
				# generate a single 128(104)bit key
    if empty "$FORM_wep_passphrase"; then
	echo "<div class=warning>$EMPTY_passphrase_error</div>"
    else
	textkeys=$(wepkeygen -s "$FORM_wep_passphrase"  |
	    awk 'BEGIN { count=0 };
						{ total[count]=$1, count+=1; }
						END { print total[0] ":" total[1] ":" total[2] ":" total[3]}')
	FORM_key1=$(echo "$textkeys" | cut -d ':' -f 0-13 | sed s/':'//g)
	FORM_key2=""
	FORM_key3=""
	FORM_key4=""
	FORM_encryption="wep"
    fi
}

! empty "$FORM_generate_wep_40" &&
{
    FORM_wep_key="1"
    FORM_key1=""
    FORM_key2=""
    FORM_key3=""
    FORM_key4=""
				# generate a single 128(104)bit key
    if empty "$FORM_wep_passphrase"; then
	echo "<div class=warning>$EMPTY_passphrase_error</div>"
    else
	textkeys=$(wepkeygen "$FORM_wep_passphrase" | sed s/':'//g)
	keycount=1
	for curkey in $textkeys; do
	    case $keycount in
		1) FORM_key1=$curkey;;
		2) FORM_key2=$curkey;;
		3) FORM_key3=$curkey;;
		4) FORM_key4=$curkey
		    break;;
	    esac
	    let "keycount+=1"
	done
	FORM_encryption="wep"
    fi
    
}


encryption_forms="field|@TR<<Encryption Type>>
		  select|select_encryption|$FORM_encryption
		  option|open_no|@TR<<Disabled>>
		  option|wep|WEP
		  option|wpa|WPA (@TR<<Personal>>)
		  option|wpa2|WPA2 (@TR<<Personal>>)
		  option|wpa_wpa2|WPA+WPA2 (@TR<<Personal>>)"
		 
append forms "$encryption_forms" "$N"
			

wep="field|@TR<<Passphrase>>|wep_keyphrase|hidden
				text|wep_passphrase|$FORM_wep_passphrase
				string|<br />
				field|&nbsp;|wep_generate_keys|hidden
				submit|generate_wep_40|@TR<<Generate 40/64-bit Keys>>
				submit|generate_wep_128|@TR<<Generate 104/128-bit Key>>
				string|<br />
				field|@TR<<WEP Key>> 1|wep_key_1|hidden
				radio|radio_wep_key|$FORM_wep_key|1
				text|key1|$FORM_key1|<br />
				field|@TR<<WEP Key>> 2|wep_key_2|hidden
				radio|radio_wep_key|$FORM_wep_key|2
				text|key2|$FORM_key2|<br />
				field|@TR<<WEP Key>> 3|wep_key_3|hidden
				radio|radio_wep_key|$FORM_wep_key|3
				text|key3|$FORM_key3|<br />
				field|@TR<<WEP Key>> 4|wep_key_4|hidden
				radio|radio_wep_key|$FORM_wep_key|4
				text|key4|$FORM_key4|<br />
                                helpitem|WEP
                                helptext|WEP is a basic encryption method, which is not as secure as the other two methods, WPA and WPA2. If you want to use a Passphrase, then enter it in the Passphrase field and click the Generate button of the WEP encryption level you choose, 40/64-bit or 104/128-bit. If you want to  enter the WEP key manually, then enter it in the WEP Key 1-4 field(s). To indicate which WEP key to use, select the appropriate radio button."
append forms "$wep" "$N"

wpa="field|@TR<<WPA Encryption>>|field_wpa_encryption|hidden
                  select|select_wpa_encryption|$FORM_wpa_encryption
	          option|TKIP|TKIP
		  option|CCMP|AES
		  option|TKIP CCMP|TKIP+AES
                  field|WPA @TR<<Pre-shared key>>|wpapsk|hidden
		  password|wpa_psk|$FORM_key
		  helpitem|WPA
                  helptext|WPA offers two encryption methods, TKIP and AES, with dynamic encryption keys. Enter the Passphrase (8 to 63 ASCII characters) or 64 hexadecimal digits.
                  helpitem|WPA2
                  helptext|WPA2 is a stronger encryption method than WPA. This method offers two encryption methods, AES and WPA-TKIP or WPA2-AES, with dynamic encryption keys. Select the type of encryption method you want  to use, AES or TKIP or TKIP+AES. Enter the Passphrase (8 to 63 ASCII characters) or 64 hexadecimal digits."

append forms "$wpa" "$N"
			
		
append forms "end_form" "$N"

			###################################################################
			# set JavaScript
#   set_visible('field_idletime', isset('radio_offmode', 'auto'));
javascript_forms="
                  var x;
                  x = isset('radio_offmode','auto');
	          set_visible('field_idletime', x);
		  v = isset('select_encryption','wep');
		  set_visible('wep_key_1', v);
		  set_visible('wep_key_2', v);
	          set_visible('wep_key_3', v);
		  set_visible('wep_key_4', v);
	          set_visible('wep_generate_keys', v);
		  set_visible('wep_keyphrase', v);
	          set_visible('wep_keys', v);

				//
				// force encryption listbox to no selection if user tries
				// to set WPA (PSK) with Ad-hoc mode.
				//
	

				v = (isset('select_encryption','wpa') || isset('select_encryption','wpa2') || isset('select_encryption','wpa_wpa2'));
				set_visible('wpapsk', v);
                                set_visible('field_wpa_encryption', v);"
				
			


			append js "$javascript_forms" "$N"

#			fi
#		done
#	validate_wireless $iftype
#	break
#	done

header "Advanced" "WLAN" "@TR<<WLAN Configuration>>" 'onload="modechange()"' "$SCRIPT_NAME"
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
$forms
EOF

footer ?>
<!--
##WEBIF:name:Advanced:304:WLAN
-->
