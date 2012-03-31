#!/usr/bin/webif-page
<?
. /usr/lib/webif/webif.sh


uci_load webif

header "Wizard" "Language" "@TR<<Language Settings>>" 'onload="modechange()"' "$SCRIPT_NAME"

# initialize the form
if empty "$FORM_submit"; then
	# initialize all defaults

FORM_language="${CONFIG_general_lang:-en}"	


/usr/lib/webif/webif-mklanglist.sh
LANGUAGES=$(cat "/etc/languages.lst")

display_form <<EOF
start_form
#string|<div class="settings">
#string|<div class="settings-content">
#string|<table width="100%" summary="Settings">
field|@TR<<Language>>
select|language|$FORM_language
$LANGUAGES
string|<br /> <br />
#string|$flag_UK
#string|$flag_TW
#string|</table></div>
#string|<div class="clearfix">&nbsp;</div></div>
submit|upgrade|@TR<<OK>>
end_form
EOF


wizard_page=1

else

# save forms
    SAVED=1	
    #webif settings
    uci_set "webif" "general" "lang" "$FORM_language"
    config_get lang general lang
    
    if [ -e /www/themes/active/ar/common.txt ]; then
	cp /www/themes/active/ar/common.txt /usr/lib/webif/lang/ar	
    fi
    rm /www/themes/active/webif.css  #if language is arabic use right to left  
    if [ "$FORM_language" == "ar" ]; then                               
    ln -s /www/themes/active/webif_ar.css /www/themes/active/webif.css  
    else                                                                
    ln -s /www/themes/active/webif_lang.css /www/themes/active/webif.css
    fi  
    if [ "$lang" == "$FORM_language" ]; then

#	/usr/lib/webif/webif-mklanglist.sh
	LANGUAGES=$(cat "/etc/languages.lst")

	display_form <<EOF
	start_form
	field|@TR<<Language>>
	select|language|$FORM_language
	$LANGUAGES
	string|<br /> <br />
	submit|upgrade|@TR<<OK>>
	end_form
EOF
	wizard_page=1
    	fi

sh /usr/lib/webif/apply.sh 2>&1
fi

#####################################################################
# Initialize LANGUAGES form
# create list if it doesn't exist ..
/usr/lib/webif/webif-mklanglist.sh
LANGUAGES=$(cat "/etc/languages.lst")

cat <<EOF
<script type="text/javascript" src="/webif.js"></script>
<script type="text/javascript">
<!--
function switch_lang(lang)
{
    //var index = 0;
    //var i;
    alert(lang);
    //alert(document.getElementById('language').options[document.getElementById('language').selectedIndex].value);
     alert(document.getElementById('language').options[1].value);
    //alert(document.getElementById('language').selectedIndex);
    for(i = 0; i < 2; i++)
    {
       alert(i);
       var v = document.getElementById('langauge').options[i].value;
       alert(v);
       /*if( v == lang)
       {
            index = i;
            break;
       }*/
        
    }
    //document.getElementById('language').options;
    //document.getElementById('language').selectedIndex = index;
    //document.language.selectedIndex = lang;
}
-->
</script>


EOF

flag_UK="<img class=\"flags\" src=\"/images/national_flags/flags_of_United-Kingdom.gif\" name=\"UKFlag\" />"
#onclick=\"switch_lang('en')\" />"
# onclick=\"switch_lang(\"en\")\"
#flag_TW="<img class=\"flags\" src=\"/images/national_flags/flags_of_Taiwan.gif\" name=\"TWFlag\" onclick=\"switch_lang('zh')\" />"
#onclick=\"switch_lang(\"zh\")\"
#flag_US="<img class=\"flags\" src=\"/images/national_flags/flags_of_United-States.gif\" name=\"USFlag\"/>"

#flag_AL="<img class=\"flags\" src=\"/images/national_flags/flags_of_Algeria.gif\" name=\"ALFlag\" />"

#flag_FR="<img class=\"flags\" src=\"/images/national_flags/flags_of_France.gif\" name=\"FRFlag\" />"

#flag_GM="<img class=\"flags\" src=\"/images/national_flags/flags_of_Germany.gif\" name=\"GMFlag\" />"

#flag_IT="<img class=\"flags\" src=\"/images/national_flags/flags_of_Italy.gif\" name=\"ITFlag\" />"

#flag_NE="<img class=\"flags\" src=\"/images/national_flags/flags_of_Netherlands.gif\" name=\"NEFlag\" />"

#flag_PO="<img class=\"flags\" src=\"/images/national_flags/flags_of_Portugal.gif\" name=\"POFlag\" />"

#flag_SP="<img class=\"flags\" src=\"/images/national_flags/flags_of_Spain.gif\" name=\"SPFlag\" />"

#flag_RU="<img class=\"flags\" src=\"/images/national_flags/flags_of_Russia.gif\" name=\"RUFlag\" />"

#######################################################
# Show form
#display_form <<EOF
#start_form
#string|<div class="settings">
#string|<div class="settings-content">
#string|<table width="100%" summary="Settings">
#field|@TR<<Language>>
#select|language|$FORM_language
#$LANGUAGES
#string|<br /> <br />
#string|$flag_UK
#string|$flag_TW
#string|</table></div>
#string|<div class="clearfix">&nbsp;</div></div>
#submit|upgrade|@TR<<OK>>
#end_form
#EOF


#wizard_page=1
wizard_footer

?>

<!--
##WEBIF:name:Wizard:001:Language
-->
