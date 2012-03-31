BEGIN {
	n = 0
	sel = 0
	FS=":"
#	config_load("webif_access_control")
}
($3 == "category") && (categories !~ /:$4:/) {
	categories = categories ":" $4 ":";
 	n++
	if ($4 ~ "^" selected "$") sel = n
	c[n] = $4
	if ($4 == "-") f[$4] = rootdir "/" indexpage "?cat=" $4
}
($3 == "name") {
	gsub(/^.*\//, "", $1);
#	if ($4 != "Graphs") {
#		var = config_get_bool(USER, $4"_"$5, "0")
#	}
#	else {
#		var = config_get_bool(USER, "Graphs", "0")
#	}
#	if ((var == "1") || (USER == "root") || (USER == "admin")) {
#	if ((USER == "root") || (USER == "admin")) {
		if ((p[$4] == 0) || (p[$4] > int($5))) {
			p[$4] = int($5) + 1
			f[$4] = rootdir "/" $1
		}
#	}
}
END {
	print "<ul>"

	for (i = 1; i <= n; i++) {
		if (f[c[i]] != "") {
			if (sel == i) print "	<li class=\"selected\"><a href=\"" f[c[i]] "\">@TR<<" c[i] ">></a></li>"
			else {
				if (c[i] == "-") {
					print "	<li class=\"separator\"> </li>"
				}
				else {
					print "	<li><a href=\"" f[c[i]] "\">@TR<<" c[i] ">></a></li>";
				}
			}
		}
	}

	print "</ul>"
}
