#!/usr/bin/webif-page "-U /tmp -u 4096"
<?

#COPY_FILES="/etc/firewall.config
#/etc/firewall.user
#/etc/httpd.conf
#/etc/hosts
#/etc/ethers"
COPY_FILES="/etc/httpd.conf
/etc/hosts
/etc/ethers"

COPY_DIRS="/etc/config
/etc/crontabs
/etc/dropbear"
#/etc/openvpn
#/etc/ssl

. /usr/lib/webif/webif.sh
#uci_load "system"

header "Advanced" "Backup &amp; Restore" "@TR<<Backup and Restore>>" ''

DOWNLOAD()
{
cat <<EOF
&nbsp;&nbsp;&nbsp;@TR<<confman_noauto_click#If downloading does not start automatically, click here in 30 seconds>> ... <a href="/backup/$1">$1</a><br /><br />
<script language="JavaScript" type="text/javascript">
setTimeout('top.location.href=\"/backup/$1\"',"500")
//setTimeout('top.location.href=\"/favicon.tgz\"',"300")
</script>
EOF
}
if ! equal $FORM_download "" ; then
	

		if equal $FORM_name "" ; then
			FORM_name=config
		fi
		
#		tmp=/tmp/config.$$
#		tgz=/www/config.tgz
    tmp=/tmp/config.temp
    tgz=/www/backup/$FORM_name.tgz
    rm -rf $tmp 2>/dev/null
    mkdir -p $tmp 2>/dev/null
#    echo "$FORM_name" > $tmp/config.name
    
    for file in $COPY_FILES; do
	[ -e $file ] && [ ! -h $file ] && {
	    d=`dirname $file`; [ -d $tmp$d ] || mkdir -p $tmp$d
	    cp -af $file $tmp$file 2>/dev/null
	}
    done
    for dir in $COPY_DIRS; do
	[ -e $dir ] && {
	    mkdir -p $tmp$dir
	    cp -afr $dir/* $tmp$dir/ 2>/dev/null
	}
    done
    [ -n "$tmp" ] && rm $tmp/etc/banner
    (cd $tmp; tar czf $tgz *)
    rm -rf $tmp 2>/dev/null
    DOWNLOAD $FORM_name.tgz
    sleep 30 ; rm $tgz
    


elif ! equal $FORM_instconfig "" ; then

    dir=$FORM_dir
    display_form <<EOF
start_form|@TR<<Restore Configuration>>
EOF
#    if [ -n "$dir" ] && [ -d "$dir" ] && [ -e "$dir/config.name" ] && [ -e "$dir/config.boardtype" ]; then
    if [ -n "$dir" ] && [ -d "$dir" ]; then
	echo "<tr><td colspan=\"2\">@TR<<confman_restoring_conf#Restoring configuration.>><br /><pre>"
	cd $dir
	for file in $(find etc); do
	    if [ -d $file ]; then
		[ -d /$file ] || mkdir /$file
	    else
		[ -e /$file ] && rm /$file
		cp -af $file /$file
		echo "@TR<<confman_restoring_file#restoring>> $file"
	    fi
			done
	
	echo "<br />@TR<<Rebooting now>>...<meta http-equiv=\"refresh\" content=\"4;url=reboot.sh?reboot=1\">"
	echo "</pre></td></tr>"
    else
	echo "<p>@TR<<confman_bad_dir#bad dir>>: $dir</p>"
    fi
    
    display_form <<EOF
end_form
EOF


elif ! equal $FORM_chkconfig "" ; then

    if [ -n "$FORM_configfile" ] && [ -e "$FORM_configfile" ]; then

	    rm -rf /tmp/config.* 2>/dev/null
#	    tmp=/tmp/config.$$
	    tmp=/tmp/config.temp
	    mkdir $tmp
	    (cd $tmp; tar xzf $FORM_configfile)
	    rm $FORM_configfile
	    
	    if [ -d "$tmp/etc" ]; then
	    	
	echo "<form method=\"get\" name=\"install\" action=\"$SCRIPT_NAME\">"
	display_form <<EOF
start_form|@TR<<Restore Configuration>>
string|$FORM_configfile
EOF
#	    if [ ! -e "$tmp/config.name" ]; then
#		echo "<tr><td colspan=\"2\">@TR<<confman_invalid_file#Invalid file>>: config.tgz!</td></tr>"
#	    else
#		nm=$(cat $tmp/config.name)
		
		echo "<tr><td colspan=\"2\">@TR<<confman_good_conf#The configuration file checked>>!<br /><br /></td></tr>"
#		display_form <<EOF
#field|@TR<<Config Name>>
#string|$nm
#EOF
		echo "</td></tr>"
#	    fi
	
	cat <<EOF
<tr><td>&nbsp;</td></tr>
<tr><td><input type='hidden' name='dir' value="$tmp" />
<input type="submit" class="flatbtn" name="instconfig" value="@TR<<Restore>>" /></td></tr>
<tr><td>&nbsp;</td></tr>
EOF

	display_form <<EOF
end_form
EOF
	echo "</form>"
	  footer
    exit
    fi
	fi

elif ! equal $FORM_reset "" ; then
    #cp -r /userdata/default/config /etc/config
    result=$(. /etc/br_default.sh)
    if [ $result -eq 0 ]; then
	echo "<div class=warning>@TR<<Reset back to factory default succeeded>>!</div>" 
	echo "<div class=warning>@TR<<The system is restarting in 5 seconds>>.</div>"
	sleep 5 && echo "<br />@TR<<Rebooting now>>...<meta http-equiv=\"refresh\" content=\"4;url=reboot.sh?reboot=1\">"
    else
	echo "<div class=warning>@TR<<Reset back to factory default failed>>!</div>"	
    fi

fi


cat <<EOF
<form method="post" name="download" action="$SCRIPT_NAME" enctype="multipart/form-data">
<br/>
EOF

display_form <<EOF
start_form|@TR<<Backup Configuration>>
EOF

cat <<EOF
<tr><td width="70%">@TR<<Name this configuration>>:&nbsp;&nbsp;&nbsp;<input name="name" value="${FORM_name}" /></td>
<td><input class="flatbtn" type="submit" name="download" value="@TR<<Backup>>" /></td>
</tr>
EOF

display_form <<EOF
end_form
start_form|@TR<<Restore Configuration>>
EOF

cat<<EOF
<tr>
<td width="70%">@TR<<Saved config.tgz file>>:&nbsp;&nbsp;&nbsp;<input type="file" class="flatbtn" name="configfile" value="Browes..." /></td>
<td><input class="flatbtn" type="submit" name="chkconfig" value="@TR<<Restore>>" /></td>
</tr>
EOF

display_form <<EOF
end_form
start_form|@TR<<Reset Function>>
string|<br />
submit|reset| @TR<<Reset To Default>> 
end_form
string|</form>
EOF

footer
?>
<!--
##WEBIF:name:Advanced:311:Backup &amp; Restore
-->
