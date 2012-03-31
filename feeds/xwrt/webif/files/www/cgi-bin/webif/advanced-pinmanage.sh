#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh
###################################################################
# PIN Management page
#
# Description:
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


header "Advanced" "PIN Management" "@TR<<PIN Management>>" ' onload="modechange()" ' "$SCRIPT_NAME"

pin_change_disable_js="
        disable('old_pincode');
        disable('new_pincode');
        disable('new_pincode_confirm');
        disable('change_pin');"
pin_manage_disable_js="	
        disable('radio_pin_enable');
        disable('pincode_usage');
        disable('pin_usage_change');"
pin_verify_disable_js="
        disable('pincode_verify');
        disable('pin_verify');"

pin_change_enable_js="
        enable('old_pincode');
        enable('new_pincode');
        enable('new_pincode_confirm');
        enable('change_pin');"
pin_manage_enable_js="	
        enable('radio_pin_enable');
        enable('pincode_usage');
        enable('pin_usage_change');"
pin_verify_enable_js="
        enable('pincode_verify');
        enable('pin_verify');"


#reload_js="window.location.reload();"

#if [ "$FORM__pin_verify" = "" ] || [ "$FORM_pin_usage_change" = "" ] || [ "$FORM_change_pin" = "" ]; then 
#PIN_verify_stat=$(rcmdc -c 'pin.status' | sed 's/^.*, Verified //g');

    
#PIN_status_num=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g');
PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
#get_PIN_status="
if [ $PIN_status_num -eq 0 ]; then    
    
    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
    
    if [ $PIN_status_recheck -eq 1 ]; then
	PIN_status="@TR<<PIN Disabled>>"
	FORM_radio_pin=0;
	append js "$pin_change_disable_js" "$N";
	append js "$pin_manage_enable_js" "$N";
	append js "$pin_verify_disable_js" "$N";

    elif [ $PIN_status_recheck -eq 2 ]; then    
	PIN_status="@TR<<PIN Enabled>>"
	FORM_radio_pin=1;
	append js "$pin_verify_disable_js" "$N";
	append js "$pin_change_enable_js" "$N";
	append js "$pin_manage_enable_js" "$N";
    fi

    
elif [ $PIN_status_num -eq 1 ]; then
    
    PIN_status="@TR<<PIN Enable>>";
    FORM_radio_pin=1;
#    PIN_verify_stat=$(GetMemory -i | grep 'PIN_Verified' | sed 's/PIN_Verified=//g');
#    if [ $PIN_verify_stat -eq 5 ]; then    
#	append js "$pin_verify_disable_js" "$N";
#    else
    append js "$pin_manage_disable_js" "$N";
    append js "$pin_change_disable_js" "$N";
    append js "$pin_verify_enable_js" "$N";
#    fi
    
	
elif [ $PIN_status_num -eq 2 ]; then
    
    PIN_status="@TR<<PIN Blocked>>";
    append js "$pin_verify_disable_js" "$N";
    append js "$pin_manage_disable_js" "$N";
    append js "$pin_change_disable_js" "$N";

elif [ $PIN_status_num -eq 3 ]; then
    
    PIN_status="@TR<<PIN Invalid>>";
    append js "$pin_manage_disable_js" "$N";
    append js "$pin_change_disable_js" "$N";
    append js "$pin_verify_disable_js" "$N";

elif [ $PIN_status_num -eq -1 ]; then
	
    PIN_status="@TR<<Read SIM Failed>>";
    append js "$pin_manage_disable_js" "$N";
    append js "$pin_change_disable_js" "$N";
    append js "$pin_verify_disable_js" "$N";

else

    PIN_status="N/A";
fi


#$get_PIN_status    
#else

#    eval FORM_pin_verify="\$FORM_pin_verify"
#    eval FORM_pin_usage_change="\$FORM_pin_usage_change"
#    eval FORM_change_pin="\$FORM_change_pin"

#    validate <<EOF
#EOF
#int|FORM_pincode|@TR<<PIN Verify>>|min=4 max=8|$FORM_pincode
#int|FORM_pincode_usage|@TR<<New PIN Checking>>|min=4 max=8|$FORM_pincode_usage
#int|FORM_old_pincode|@TR<<New PIN Checking>>|min=4 max=8|$FORM_old_pincode
#int|FORM_new_pincode|@TR<<New PIN Checking>>|min=4 max=8|$FORM_new_pincode
#EOF

    EMPTY_passphrase_error="@TR<<Please enter the PIN code>>"

    ! empty "$FORM_pin_verify" &&
    {
	eval FORM_pincode_verify="\$FORM_pincode_verify"
#	SAVED=1
	validate <<EOF
int|FORM_pincode_verify|@TR<<PIN Verify>>|required min=4 max=8|$FORM_pincode_verify
EOF
	
#	[ "$?" = "0" ] && {
	if empty "$FORM_pincode_verify"; then
	    echo "<div class=warning>$EMPTY_passphrase_error</div>"
	else
	    result=$(rcmdc -c "pin.verify s/$FORM_pincode_verify")
	    change_result=$(echo $result | sed 's/^.*PIN Verify //g' | sed 's/, Retries.*$//g')
	    retries=$(echo $result | sed 's/^.*Retries //g' | sed 's/, Verified.*$//g')
	    if [ $change_result -eq 0 ]; then
		echo "<div class=warning>@TR<<PIN Verification Succeeded>>!</div>"
		PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		if [ $PIN_status_num -eq 0 ]; then    
		    
		    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
		    
		    if [ $PIN_status_recheck -eq 1 ]; then
			PIN_status="@TR<<PIN Disabled>>"
			FORM_radio_pin=0;
			append js "$pin_change_disable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_recheck -eq 2 ]; then    
			PIN_status="@TR<<PIN Enabled>>"
			FORM_radio_pin=1;
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_change_enable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
		    fi
		    
    
		elif [ $PIN_status_num -eq 1 ]; then
		    
		    PIN_status="@TR<<PIN Enable>>";
		    FORM_radio_pin=1;
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_enable_js" "$N";
    
	
		elif [ $PIN_status_num -eq 2 ]; then
		    
		    PIN_status="@TR<<PIN Blocked>>";
		    append js "$pin_verify_disable_js" "$N";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq 3 ]; then
		    
		    PIN_status="@TR<<PIN Invalid>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq -1 ]; then
		    
		    PIN_status="@TR<<Read SIM Failed>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		else
		    
		    PIN_status="N/A";
		fi
	    else
		echo "<div class=warning>@TR<<PIN Verification Failed>>! $retries @TR<<more tries before the SIM locked>></div>"
		PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
#get_PIN_status="
		if [ $PIN_status_num -eq 0 ]; then    
		    
		    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
		    
		    if [ $PIN_status_recheck -eq 1 ]; then
			PIN_status="@TR<<PIN Disabled>>"
			FORM_radio_pin=0;
			append js "$pin_change_disable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_recheck -eq 2 ]; then    
			PIN_status="@TR<<PIN Enabled>>"
			FORM_radio_pin=1;
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_change_enable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
		    fi
		    
		    
		elif [ $PIN_status_num -eq 1 ]; then
		    
		    PIN_status="@TR<<PIN Enable>>";
		    FORM_radio_pin=1;
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_enable_js" "$N";
    
	
		elif [ $PIN_status_num -eq 2 ]; then
		    
		    PIN_status="@TR<<PIN Blocked>>";
		    append js "$pin_verify_disable_js" "$N";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq 3 ]; then
		    
		    PIN_status="@TR<<PIN Invalid>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq -1 ]; then
		    
		    PIN_status="@TR<<Read SIM Failed>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		else

		    PIN_status="N/A";
		fi
	    fi

	fi
    }


    ! empty "$FORM_pin_usage_change" &&
    {
	eval FORM_radio_pin="\$FORM_radio_pin_enable"
	eval FORM_pincode_usage="\$FORM_pincode_usage"
#	SAVED=1
	validate <<EOF
int|FORM_pincode_usage|@TR<<PIN code>>|required min=4 max=8|$FORM_pincode_usage
EOF
#	[ "$?" = 0 ] && {
	if empty "$FORM_pincode_usage" ; then
		echo "<div class=warning>$EMPTY_passphrase_error</div>"
	else
#	    echo "<div class=warning>$FORM_radio_pin_enable</div>"
	    if [ $FORM_radio_pin -eq 1 ]; then
	
		result=$(rcmdc -c "pin.enable s/$FORM_pincode_usage")
		change_result=$(echo $result | sed 's/^.*PIN Enable //g' | sed 's/, Cmd.*$//g')
		retries=$(echo $result | sed 's/^.*Retries //g' )
		
		if [ $change_result -eq 0 ]; then
		    verify_step=$(rcmdc -c "pin.verify s/$FORM_pincode_usage")
		    echo "<div class=warning>@TR<<PIN Enabled Succeeded>>!</div>"
#		    append js "$reload_js" "$N";
		    PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		    if [ $PIN_status_num -eq 0 ]; then    
			
			PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
			
			if [ $PIN_status_recheck -eq 1 ]; then
			    PIN_status="PIN Disabled"
			    FORM_radio_pin=0;
			    append js "$pin_change_disable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			    append js "$pin_verify_disable_js" "$N";
			    
			elif [ $PIN_status_recheck -eq 2 ]; then    
			    PIN_status="@TR<<PIN Enabled>>"
			    FORM_radio_pin=1;
			    append js "$pin_verify_disable_js" "$N";
			    append js "$pin_change_enable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			fi
			
			
		    elif [ $PIN_status_num -eq 1 ]; then
			
			PIN_status="@TR<<PIN Enable>>";
			FORM_radio_pin=1;
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_enable_js" "$N";
			
		    elif [ $PIN_status_num -eq 2 ]; then
			
			PIN_status="@TR<<PIN Blocked>>";
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq 3 ]; then
			
			PIN_status="@TR<<PIN Invalid>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq -1 ]; then
			
			PIN_status="@TR<<Read SIM Failed>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    else
			
			PIN_status="N/A";
		    fi
		else
		    echo "<div class=warning>@TR<<PIN Enabled Failed>>! $retries @TR<<more tries before the SIM locked>></div>"
		    PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		    if [ $PIN_status_num -eq 0 ]; then    
			
			PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
    
			if [ $PIN_status_recheck -eq 1 ]; then
			    PIN_status="@TR<<PIN Disabled>>"
			    FORM_radio_pin=0;
			    append js "$pin_change_disable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			    append js "$pin_verify_disable_js" "$N";
			    
			elif [ $PIN_status_recheck -eq 2 ]; then    
			    PIN_status="@TR<<PIN Enabled>>"
			    FORM_radio_pin=1;
			    append js "$pin_verify_disable_js" "$N";
			    append js "$pin_change_enable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			fi
			
    
		    elif [ $PIN_status_num -eq 1 ]; then
			
			PIN_status="@TR<<PIN Enable>>";
			FORM_radio_pin=1;
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_enable_js" "$N";
    
	
		    elif [ $PIN_status_num -eq 2 ]; then
			
			PIN_status="@TR<<PIN Blocked>>";
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq 3 ]; then
    
			PIN_status="@TR<<PIN Invalid>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq -1 ]; then
			
			PIN_status="@TR<<Read SIM Failed>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    else
			
			PIN_status="N/A";
		    fi
		fi
	    else
		result=$(rcmdc -c "pin.disable s/$FORM_pincode_usage")
		change_result=$(echo $result | sed 's/^.*PIN Disable //g' | sed 's/, Cmd.*$//g')
		retries=$(echo $result | sed 's/^.*Retries //g' )

		if [ $change_result -eq 0 ]; then
		    echo "<div class=warning>@TR<<PIN Disabled Succeeded>>!</div>"
		    PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		    if [ $PIN_status_num -eq 0 ]; then    
			
			PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
			
			if [ $PIN_status_recheck -eq 1 ]; then
			    PIN_status="@TR<<PIN Disabled>>"
			    FORM_radio_pin=0;
			    append js "$pin_change_disable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			    append js "$pin_verify_disable_js" "$N";
			    
			elif [ $PIN_status_recheck -eq 2 ]; then    
			    PIN_status="@TR<<PIN Enabled>>"
			    FORM_radio_pin=1;
			    append js "$pin_verify_disable_js" "$N";
			    append js "$pin_change_enable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			fi
			
			
		    elif [ $PIN_status_num -eq 1 ]; then
			
			PIN_status="@TR<<PIN Enable>>";
			FORM_radio_pin=1;
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_enable_js" "$N";
			
		    elif [ $PIN_status_num -eq 2 ]; then
			
			PIN_status="@TR<<PIN Blocked>>";
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq 3 ]; then
    
			PIN_status="@TR<<PIN Invalid>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq -1 ]; then
			
			PIN_status="@TR<<Read SIM Failed>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    else
			
			PIN_status="N/A";
		    fi
		    
		else
		    echo "<div class=warning>@TR<<PIN Disabled Failed>>! $retries @TR<<more tries before the SIM locked>></div>"
		    PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		    if [ $PIN_status_num -eq 0 ]; then    
			
			PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
			
			if [ $PIN_status_recheck -eq 1 ]; then
			    PIN_status="@TR<<PIN Disabled>>"
			    FORM_radio_pin=0;
			    append js "$pin_change_disable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			    append js "$pin_verify_disable_js" "$N";
			    
			elif [ $PIN_status_recheck -eq 2 ]; then    
			    PIN_status="@TR<<PIN Enabled>>"
			    FORM_radio_pin=1;
			    append js "$pin_verify_disable_js" "$N";
			    append js "$pin_change_enable_js" "$N";
			    append js "$pin_manage_enable_js" "$N";
			fi

    
		    elif [ $PIN_status_num -eq 1 ]; then
			
			PIN_status="@TR<<PIN Enable>>";
			FORM_radio_pin=1;
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_enable_js" "$N";
			
		    elif [ $PIN_status_num -eq 2 ]; then
			
			PIN_status="@TR<<PIN Blocked>>";
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq 3 ]; then
			
			PIN_status="@TR<<PIN Invalid>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_num -eq -1 ]; then
			
			PIN_status="@TR<<Read SIM Failed>>";
			append js "$pin_manage_disable_js" "$N";
			append js "$pin_change_disable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    else

			PIN_status="N/A";
		    fi
		fi
	    fi	 
	fi
#	}
    }


    ! empty "$FORM_change_pin" &&
    {
	eval FORM_old_pincode="\$FORM_old_pincode"
	eval FORM_new_pincode="\$FORM_new_pincode"
	eval FORM_new_pincode_confirm="\$FORM_new_pincode_confirm"
#	SAVED=1
	validate <<EOF
int|FORM_old_pincode|@TR<<Old PIN Code>>|required min=4 max=8|$FORM_old_pincode
int|FORM_new_pincode|@TR<<New PIN Code>>|required min=4 max=8|$FORM_new_pincode
int|FORM_new_pincode_confirm|@TR<<New PIN Confirm>>|required min=4 max=8|$FORM_new_pincode_confirm
EOF
#	[ "$?" = "0" ] && {
	if [ "$FORM_old_pincode" = "" ] || ["$FORM_new_pincode" = "" ] || ["$FORM_new_pincode_confirm" = "" ]; then
	    echo "<div class=warning>$EMPTY_passphrase_error</div>"
	    
	elif [ "$FORM_new_pincode" != "$FORM_new_pincode_confirm" ]; then
	    echo "<div class=warning>@TR<<The new PIN codes are not matched>></div>"
		
	else
#	    append validate_form "int|FORM_new_pincode|@TR<<New PIN Checking>>|min=4 max=8|$FORM_new_pincode" "$N"
	    result=$(rcmdc -c "pin.change s/$FORM_old_pincode s/$FORM_new_pincode")
	    change_result=$(echo $result | sed 's/^.*PIN Change //g' | sed 's/, Cmd.*$//g')
	    retries=$(echo $result | sed 's/^.*Retries //g' )

	    if [ $change_result -eq 0 ]; then
		verify_step=$(rcmdc -c "pin.verify s/$FORM_old_pincode")
		echo "<div class=warning>@TR<<PIN Change Succeeded>>!</div>"
		PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		if [ $PIN_status_num -eq 0 ]; then    
		    
		    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
		    
		    if [ $PIN_status_recheck -eq 1 ]; then
			PIN_status="@TR<<PIN Disabled>>"
			FORM_radio_pin=0;
			append js "$pin_change_disable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
			append js "$pin_verify_disable_js" "$N";
			
		    elif [ $PIN_status_recheck -eq 2 ]; then    
			PIN_status="@TR<<PIN Enabled>>"
			FORM_radio_pin=1;
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_change_enable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
		    fi

    
		elif [ $PIN_status_num -eq 1 ]; then
		    
		    PIN_status="@TR<<PIN Enable>>";
		    FORM_radio_pin=1;
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_enable_js" "$N";
	
		elif [ $PIN_status_num -eq 2 ]; then
		    
		    PIN_status="@TR<<PIN Blocked>>";
		    append js "$pin_verify_disable_js" "$N";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq 3 ]; then
		    
		    PIN_status="@TR<<PIN Invalid>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq -1 ]; then
	
		    PIN_status="@TR<<Read SIM Failed>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		else
		    
		    PIN_status="N/A";
		fi
	    else
		echo "<div class=warning>@TR<<PIN Change Failed>>! $retries @TR<<more tries before the SIM locked>></div>"
		PIN_status_num=$(GetMemory -i | grep 'PIN_status' | sed 's/PIN_status=//g');
		if [ $PIN_status_num -eq 0 ]; then    
		    
		    PIN_status_recheck=$(rcmdc -c 'pin.status' | sed 's/^.*PIN Status //g' | sed 's/, Retries.*$//g')
		    
		    if [ $PIN_status_recheck -eq 1 ]; then
			PIN_status="@TR<<PIN Disabled>>"
			FORM_radio_pin=0;
			append js "$pin_change_disable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
			append js "$pin_verify_disable_js" "$N";

		    elif [ $PIN_status_recheck -eq 2 ]; then    
			PIN_status="@TR<<PIN Enabled>>"
			FORM_radio_pin=1;
			append js "$pin_verify_disable_js" "$N";
			append js "$pin_change_enable_js" "$N";
			append js "$pin_manage_enable_js" "$N";
		    fi
		    
		    
		elif [ $PIN_status_num -eq 1 ]; then
    
		    PIN_status="@TR<<PIN Enable>>";
		    FORM_radio_pin=1;
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_enable_js" "$N";
		    
		elif [ $PIN_status_num -eq 2 ]; then
		    
		    PIN_status="@TR<<PIN Blocked>>";
		    append js "$pin_verify_disable_js" "$N";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq 3 ]; then
		    
		    PIN_status="@TR<<PIN Invalid>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
		    
		elif [ $PIN_status_num -eq -1 ]; then
		    
		    PIN_status="@TR<<Read SIM Failed>>";
		    append js "$pin_manage_disable_js" "$N";
		    append js "$pin_change_disable_js" "$N";
		    append js "$pin_verify_disable_js" "$N";
    
		else

		    PIN_status="N/A";
		fi
	    fi
	fi
#	}
    }
#fi

form_pin_verify="start_form|@TR<<PIN Verification>>|form_pin_verify
field|@TR<<SIM Status>>
string|$PIN_status
field|@TR<<PIN Code>>
password|pincode_verify
submit|pin_verify|@TR<<Verify>>
end_form"

append forms "$form_pin_verify" "N"

form_pin_manage="start_form|@TR<<PIN Management>>|form_pin_manage|disabled
field|@TR<<PIN Protection>>
radio|radio_pin_enable|$FORM_radio_pin|1|@TR<<Enabled>>
radio|radio_pin_enable|$FORM_radio_pin|0|@TR<<Disabled>>
onchange|enablePW
field|@TR<<PIN Code>>
password|pincode_usage
submit|pin_usage_change|@TR<<Apply>>
end_form"

append forms "$form_pin_manage" "$N"

form_pin_change="start_form|@TR<<PIN Modification>>|form_pin_change|disabled
field|@TR<<Old PIN Code>>
password|old_pincode
helpitem|PIN Code
helptext|PIN code for the SIM card, should be 4 to 8 digits
field|@TR<<New PIN Code>>
password|new_pincode
field|@TR<<New PIN Confirm>>|field_pincode_confirm
password|new_pincode_confirm
submit|change_pin|@TR<<Change>>
end_form"

append forms "$form_pin_change" "$N"

#form_simlock="start_form|@TR<<SIM Lock>>|form_simlock_title|hidden
#field|@TR<<SIM Lock Status>>|field_simlock_status|hidden
#string|SIM Lock Enabled
#field|@TR<<Unlocking Code>>|field_simlock_code|hidden
#password|unlockcode
#submit|but_sim_unlock|@TR<<Unlock>>
#onclick|sim_unlock()
#end_form"



#####################################################################
# functions script
#
cat <<EOF
<script type="text/javascript" src="/webif.js"></script>
<script type="text/javascript">
<!--

function pin_verify(){
  
    var el = document.getElementById('form_pin_verify');
       if (el.disabled != true){
           el.disabled = "true";
           set_visible('field_simlock_status', true);
           set_visible('field_simlock_code', true);
       }
       else{
           el.style.display = "none";
           set_visible('field_simlock_status', false);
           set_visible('field_simlock_code', false);
       }

}

function modechange()
{
	var v;
	$js

	hide('save');
	//show('save');
}
 
/*
function displaysimlock()
{

       var el = document.getElementById('form_simlock_title');
       if (el.style.display != "block"){
           el.style.display = "block";
           set_visible('field_simlock_status', true);
           set_visible('field_simlock_code', true);
       }
       else{
           el.style.display = "none";
           set_visible('field_simlock_status', false);
           set_visible('field_simlock_code', false);
       }
}
*/
-->
</script>

EOF

display_form <<EOF
onchange|modechange
$validate_error
$forms
EOF
#$form_simlock
footer ?>
<!--
##WEBIF:name:Advanced:306:PIN Management
-->
