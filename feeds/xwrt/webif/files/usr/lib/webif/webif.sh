######################################################
# Webif base
#
# Description:
#        Holds primary functions. Header, footer, etc..
#
# Author(s) [in order of work date]:
#        A variety of people. Several X-Wrt developers.
#
# Major revisions:
#
# Configuration files referenced:
#

libdir=/usr/lib/webif
wwwdir=/www
cgidir=/www/cgi-bin/webif
rootdir=/cgi-bin/webif
. /usr/lib/webif/functions.sh
. /lib/config/uci.sh

uci_load "webif"
_device="$CONFIG_general_device_name"

categories() {
	grep '##WEBIF' $cgidir/.categories $cgidir/*.sh 2>/dev/null | \
		awk -v "selected=$1" \
			-v "USER=$REMOTE_USER" \
			-v "rootdir=$rootdir" \
	                -f /usr/lib/webif/categories.awk
#			-f /usr/lib/webif/common.awk \
#			-f /usr/lib/webif/categories.awk -
}

subcategories() {
	grep -H "##WEBIF:name:$1:" $cgidir/*.sh 2>/dev/null | \
		sed -e 's,^.*/\([a-zA-Z0-9\.\-]*\):\(.*\)$,\2:\1,' | \
		sort -n | \
		awk -v "selected=$2" \
			-v "rootdir=$rootdir" \
			-v "USER=$REMOTE_USER" \
	                -f /usr/lib/webif/subcategories.awk 
#			-f /usr/lib/webif/common.awk \
#			-f /usr/lib/webif/subcategories.awk -
}

ShowWIPWarning() {
	echo "<div class=\"warning\">@TR<<big_warning#WARNING>>: @TR<<page_incomplete#This page is incomplete and may not work correctly, or at all.>></div>"
}

ShowUntestedWarning() {
	echo "<div class=\"warning\">@TR<<big_warning#WARNING>>: @TR<<page_untested#This page is untested and may or may not work correctly.>></div>"
}

ShowNotUpdatedWarning() {
       echo "<div class=\"warning\">@TR<<big_warning#WARNING>>: @TR<<page_untested_kamikaze#This page has not been updated or checked for correct functionality under Kamikaze.>></div>"
}

update_changes() {
	CHANGES=$(($( (cat /tmp/.webif/config-* ; ls /tmp/.webif/file-*) 2>&- | wc -l)))
	EDITED_FILES=$(find "/tmp/.webif/edited-files" -type f 2>&- | wc -l)
	CHANGES=$(($CHANGES + $EDITED_FILES))
	# calculate and add number of pending uci changes
	for uci_tmp_file in $(ls /tmp/.uci/* 2>&- | grep -v "\\.lock\$" 2>&-); do
		CHANGES_CUR=$(cat "$uci_tmp_file" | grep "\\w" | wc -l)
		CHANGES=$(($CHANGES + $CHANGES_CUR))
	done
}

pcnt=0
nothave=0
_savebutton_bk=""

has_pkgs() {
	retval=0;
	for pkg in "$@"; do
		pcnt=$((pcnt + 1))
		empty $(opkg list_installed | grep "^$pkg ") && {
			echo -n "<p>@TR<<features_require_package#Features on this page require the package>>: \"<b>$pkg</b>\". &nbsp;<a href=\"/cgi-bin/webif/system-ipkg.sh?action=install&amp;pkg=$pkg&amp;prev=$SCRIPT_NAME\">@TR<<features_install#install now>></a>.</p>"
			retval=1;
			nothave=$((nothave + 1))
		}
	done
	[ -z "$_savebutton_bk" ] && _savebutton_bk=$_savebutton
	if [ "$pcnt" = "$nothave" ]; then
		_savebutton=""
	else
		_savebutton=$_savebutton_bk
	fi
	return $retval;
}

mini_header() {

cat <<EOF
Content-Type: text/html; charset=UTF-8
Pragma: no-cache

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<?xml version="1.0" encoding="UTF-8"?>
<head>
	<link rel="stylesheet" type="text/css" href="/themes/active/webif.css" />        
	<title></title>
	<style type="text/css">
		html, body { background-color: transparent; }
	</style>
</head>
EOF
}

header() {
	empty "$ERROR" && {
		_saved_title="${SAVED:+: @TR<<Settings saved>>}"
	} || {
		FORM_submit="";
		ERROR="<h3 class=\"warning\">$ERROR</h3>"
		_saved_title=": @TR<<Settings not saved>>"
	}

	_category="$1"
	_use_progressbar="$CONFIG_general_use_progressbar"
	_head="${3:+<h2>$3$_saved_title</h2>}"
	_form="${5:+<form enctype=\"multipart/form-data\" name=\"form_$1_$2\" action=\"$5\" method=\"post\"> <input type=\"hidden\" name=\"submit\" value=\"1\" />}"
	_savebutton="${5:+<div class=\"page-save\"><input id=\"savebutton\" type=\"submit\" name=\"action\" value=\"@TR<<Save Changes>>\" /></div>}"
	_categories=$(categories $1)
	_subcategories=${2:+$(subcategories "$1" "$2")}
	_pagename="${2:+@TR<<$2>> - }"

	if ! equal $6 "" && ! equal $6 "0" ; then _pageload="<SCRIPT type='text/javascript'>start=0; end=$6</SCRIPT><SCRIPT src='/js/pageload.js' type='text/javascript'></SCRIPT><DIV id='loadmain'><SCRIPT type='text/javascript'>document.getElementById(\"loadmain\").style.display = \"none\";</SCRIPT>"; _JSload="<SCRIPT type='text/javascript'>load()</SCRIPT>"; fi

	empty "$REMOTE_USER" && neq "${SCRIPT_NAME#/cgi-bin/}" "webif.sh" && grep 'root:!' /etc/passwd >&- 2>&- && {
		_nopasswd=1
		_form=""
		_savebutton=""
	}
[ -f /www/themes/active/webif_header_info ] && . /www/themes/active/webif_header_info


# Signal strength picture

#header_signal_strength_num=$(sqlite3 /testdb "select signal from threeG_connect order by time desc limit 1")
#header_signal_strength=$(echo $header_signal_strength_num | awk '{printf "%i", $header_signal_strength_num}')

header_signal_strength=$(rcmdc -c 'network.signal_level' | sed 's/Signal Level: //g')

#header_signal_strength=3

if [ $header_signal_strength -eq 5 ]; then
    header_signal_src="/images/5bars_64.png"

elif [ $header_signal_strength -eq 4 ]; then
    header_signal_src="/images/4bars_64.png"

elif [ $header_signal_strength -eq 3 ]; then
    header_signal_src="/images/3bars_64.png"

elif [ $header_signal_strength -eq 2 ]; then
    header_signal_src="/images/2bars_64.png"

elif [ $header_signal_strength -eq 1 ]; then
    header_signal_src="/images/1bars_64.png"

else
    header_signal_src="/images/0bars_64.png"
fi

cat <<EOF
Content-Type: text/html; charset=UTF-8
Pragma: no-cache

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<!--title>$_pagename$_firmware_name @TR<<Administrative Console>></title-->
<title>@TR<<PR30 Series Administrative Console>></title>
	<link rel="stylesheet" type="text/css" href="/themes/active/waitbox.css" media="screen" />
	<link rel="stylesheet" type="text/css" href="/themes/active/webif.css" />
	$webif_header_title
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="expires" content="-1" />
	<script type="text/javascript" src="/js/styleswitcher.js"></script>
$header_inject_head</head>
<body $4>$header_inject_body

<div id="container">
<div id="header">
	<h1>@TR<<PR30 Series Administration Console>></h1>

        <span id="header-signal">
           <img name="signal_button" src="$header_signal_src" alt="" onclick="window.location.reload()" />	
        </span>
        
</div>

<div id="mainmenu">
$_categories
</div>

<div id="submenu">
$_subcategories
</div>

EOF
#$colorswitcher
if equal $_use_progressbar "1" ; then echo $_pageload
else echo "<script type='text/javascript'>function load() { }</script>"
fi

cat <<EOF
$_form

<div id="content">
	$_head
	$ERROR
EOF

	empty "$REMOTE_USER" && neq "${SCRIPT_NAME#/cgi-bin/}" "webif.sh" && {
		! empty "$FORM_passwd1$FORM_passwd2" && {
			equal "$FORM_passwd1" "$FORM_passwd2" && {
				echo '<pre>'
				(
					echo "$FORM_passwd1"
					sleep 1
					echo "$FORM_passwd2"
				) | passwd root 2>&1 && apply_passwd
				echo '</pre><meta http-equiv="refresh" content="4; URL=/cgi-bin/webif/system-info.sh">'
				footer
				exit
			} || {
				echo "<h3 class=\"warning\">@TR<<Password_mismatch#The entered passwords do not match!>></h3>"
			}
		}
		equal "$_nopasswd" 1 && {
			cat <<EOF
<br />
<br />
<br />
<h3>@TR<<Warning>>: @TR<<Password_warning|You haven't set a password for the Web interface and SSH access.<br />Please enter one now (the user name in your browser will be 'root').>></h3>
<br />
<br />
EOF
			empty "$NOINPUT" && cat <<EOF
<form enctype="multipart/form-data" action="$SCRIPT_NAME" method="POST">
<table>
	<tr>
		<td>@TR<<New Password>>:</td>
		<td><input type="password" name="passwd1" /></td>
	</tr>
	<tr>
		<td>@TR<<Confirm Password>>:</td>
		<td><input type="password" name="passwd2" /></td>
	</tr>
	<tr>
		<td colspan="2"><input type="submit" name="action" value="@TR<<Set>>" /></td>
	</tr>
</table>
</form>
EOF
			footer
			exit
		} || {
			apply_passwd
		}
	}
#	if [ "$REMOTE_USER" != "root" -a "$REMOTE_USER" != "admin" ]; then
#		config_load webif_access_control
#		permission=1;
		#if [ "$1" != "Graphs" ]; then
#			webifform=`grep "##WEBIF:name:${1}:[0-9][0-9][0-9]:${2}" /www/cgi-bin/webif/*.sh |cut -d':' -f5`
		#	config_get_bool permission "$REMOTE_USER" "${1}_${webifform}" 0
		#else
		#	config_get_bool permission "$REMOTE_USER" "Graphs" 0
		#fi
#		if [ "$permission" = "0" ]; then
#			echo "Permission Denied"
#			footer
#			exit
#		fi
#	fi
}

#######################################################
# footer
#
footer() {
	update_changes
	_changes=${CHANGES#0}
	_changes=${_changes:+(${_changes})}
	_endform=${_savebutton:+</form>}

cat <<EOF
</div>
<br />
<div id="footer">
<fieldset id="save">
	<legend><strong>@TR<<Proceed Changes>></strong></legend>
	$_savebutton
EOF
	equal "$_use_progressbar" "1" && {
	echo '<script type="text/javascript" src="/js/waitbox.js"></script>'
	}
cat <<EOF
	<ul class="apply">
		<li><a href="config.sh?mode=save&amp;cat=$_category&amp;prev=$SCRIPT_NAME" rel="lightbox" >@TR<<Apply Changes>> &laquo;</a></li>
		<li><a href="config.sh?mode=clear&amp;cat=$_category&amp;prev=$SCRIPT_NAME">@TR<<Clear Changes>> &laquo;</a></li>
		<li><a href="config.sh?mode=review&amp;cat=$_category&amp;prev=$SCRIPT_NAME">@TR<<Review Changes>> $_changes &laquo;</a></li>
	</ul>
</fieldset>
$_endform
<hr />
</div>
</div> <!-- End #container -->
</body>
</html>
EOF
}

#######################################################
# wizard footer
#
wizard_footer() {
    _prevbutton="<input id=\"prevbutton\" type=\"button\" name=\"action\" value=\"@TR<<Previous>>\" onclick=\"prev()\" />"
    _nextbutton="<input id=\"nextbutton\" type=\"button\" name=\"action\" value=\"@TR<<Next>>\" onclick=\"next()\" />"
    _hiddensave="<input id=\"hiddensave\" type=\"submit\" name=\"hiddensave\" value=\"@TR<<Save>>\" style=\"display:none\"/>"  
    _cancelbutton="<input id=\"cancelbutton\" type=\"button\" name=\"cancelbutton\" value=\"@TR<<Cancel>>\" onclick=\"cancel()\" />" 
    _finishbutton="<input id=\"finishbutton\" type=\"button\" name=\"finishbutton\" value=\"@TR<<Finish>>\"  onclick=\"finish()\" />"
    
    update_changes
    _changes=${CHANGES#0}
    _changes=${_changes:+(${_changes})}
 

   
    cat <<EOF
<script type="text/javascript">
<!--

   function sleep(ms){
       var dt = new Date();
       dt.setTime(dt.getTime() + ms);
       while (new Date().getTime() < dt.getTime());
   }
   
   function next(){
      
  
           //set_value('$FORM_submit', 1);
           //document.form_Wizard_Language.submit();
           //document.getElementById("form_Wizard_Language").submit();
           
           //document.form_Wizard_Language.hiddensave.click();
           //document.location= "/cgi-bin/webif/wizard-HSPA.sh";
           //document.form_Wizard_Language.hiddensave.click();
           //sleep(1500);
           //window.location.reload();       
           document.location= "/cgi-bin/webif/wizard-wifi.sh";
        
   }
   function prev(){

           document.form_Wizard_WLAN.hiddensave.click();
           window.location= "/cgi-bin/webif/wizard-language.sh";
           //document.getElementById("form_Wizard_HSPA").submit();
   }

   function cancel(){
       window.location= "config.sh?mode=clear&amp;cat=$_category&amp;prev=$SCRIPT_NAME";
   }

   function finish(){
       document.form_Wizard_WLAN.hiddensave.click();
//       $finish_js
      // sleep(1000);
      // window.location.reload(); 
      // window.location= "config.sh?mode=save&amp;cat=$_category&amp;prev=/cgi-bin/webif/wizard-finish.sh";
   }

-->
</script>

<div id="wizard_buttons" style="text-align:right">
EOF

case $wizard_page in
    "1")
	cat <<EOF
$_hiddensave
$_nextbutton
$_cancelbutton
EOF
	;;
    "2")

	cat <<EOF
$_hiddensave
$_prevbutton
$_cancelbutton
$_finishbutton
EOF
	;;
esac

cat <<EOF
</div>
</form>
</div> <!-- End #container -->
</body>
</html>
EOF
}

#######################################################
apply_passwd() {
	case ${SERVER_SOFTWARE%% *} in
		mini_httpd/*)
			grep '^root:' /etc/passwd | cut -d: -f1,2 > $cgidir/.htpasswd
			killall -HUP mini_httpd
			;;
	esac
}

display_form() {
	if empty "$1"; then
		awk -F'|' -f /usr/lib/webif/common.awk -f /usr/lib/webif/form.awk
	else
		echo "$1" | awk -F'|' -f /usr/lib/webif/common.awk -f /usr/lib/webif/form.awk
	fi
}

