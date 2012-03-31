BEGIN {
	FS=":"
	print "<h3><strong>@TR<<Subcategories>>:</strong></h3>"
	print "<ul>"
#	config_load("webif_access_control")
}
{
#	if ($3 != "Graphs") {
#		var = config_get_bool(USER, $3"_"$4, "0")
#	}
#	else {
#		var = config_get_bool(USER, "Graphs", "0")
#	}
#	if ((var == "1") || (USER == "root") || (USER == "admin")) {
#    if ((USER == "root") || (USER == "admin")) {
    if ($5 ~ "^" selected "$") 
	print "	<li class=\"selected\"><a href=\"" rootdir "/" $6 "\">@TR<<" $5 ">></a></li>"
    else 
	print "	<li><a href=\"" rootdir "/" $6 "\">@TR<<" $5 ">></a></li>"
#	}
}
END {
	print "</ul>"
}

