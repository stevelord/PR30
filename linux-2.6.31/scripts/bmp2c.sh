#!/bin/sh

bin2c=scripts/br_bin2c
bootup_logo="bootup_logo.c"
bootup_logo_list="bootup_logo_list.txt"

if [ $# -lt 1 ]; then
	echo "Usage: bmp2c.sh <bmp file> <output file>"
	echo "For example: bmp2c.sh logo_sun_rise_0015.bmp bootup_logo.c"
	exit 1
fi

if [ $# -eq 2 ]; then
	bootup_logo=$2
fi

if ! [ -f $1 ]; then 
	echo "\"$1\" does not exist, please enter a valid bmp file";
	exit 1
fi

rm -rf $bootup_logo

bmp_files=$1

r=0

#echo "#include <common.h>" >> $bootup_logo
echo "" >> $bootup_logo


for in_file in $bmp_files
do
	r=$(($r + 1))

	array_name="bmp"$r
	temp_file=$array_name".txt"

	./$bin2c $array_name $in_file $temp_file
	cat $temp_file >> $bootup_logo
	rm $temp_file

	echo $array_name"," >> $bootup_logo_list
done


echo "" >> $bootup_logo
echo "char *bootup_logo_list[$r]=" >> $bootup_logo
echo "{" >> $bootup_logo
cat $bootup_logo_list >> $bootup_logo
echo "};" >> $bootup_logo
rm $bootup_logo_list


echo "" >> $bootup_logo
echo "int get_bootup_logo_num( void )" >> $bootup_logo
echo "{" >> $bootup_logo
echo "	return (sizeof(bootup_logo_list) / sizeof(bootup_logo_list[0]));" >> $bootup_logo
echo "}" >> $bootup_logo

echo "" >> $bootup_logo
echo "char *get_bootup_logo_ptr( int logo_index )" >> $bootup_logo
echo "{" >> $bootup_logo
echo "	if ( logo_index < get_bootup_logo_num( ) ) return bootup_logo_list[logo_index];" >>  $bootup_logo
echo "	return 0;" >>  $bootup_logo
echo "}" >> $bootup_logo


