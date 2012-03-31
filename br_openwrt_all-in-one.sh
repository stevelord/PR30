#!/bin/bash
# Description : Generate PR39 pre-flash image images
# Author: BandRich

TEMP_FOLDER=bin/imx25/temp
ATK_FOLDER=bin/imx25/temp/atk
MTD_FOLDER=bin/imx25/temp/mtd
WEB_FOLDER=bin/imx25/temp/web
RAW_FOLDER=bin/imx25/temp/raw

DATA_FOLDER=$RAW_FOLDER/data
DATA_IMAGE=$ATK_FOLDER/data.jffs2
DATA_IMAGE_MTD=$MTD_FOLDER/data.bin

F_DATA_FOLDER=$RAW_FOLDER/f_data
F_DATA_IMAGE=$ATK_FOLDER/f_data.jffs2
F_DATA_IMAGE_MTD=$MTD_FOLDER/f_data.bin

USERDATA_FOLDER=$RAW_FOLDER/userdata
USERDATA_IMAGE=$ATK_FOLDER/userdata.jffs2
USERDATA_IMAGE_MTD=$MTD_FOLDER/userdata.bin

KERNEL_IMAGE=$ATK_FOLDER/uImage
KERNEL_IMAGE_TEMP=$MTD_FOLDER/uImage.temp
KERNEL_IMAGE_MTD=$MTD_FOLDER/uImage.bin

ROOTFS_IMAGE=$ATK_FOLDER/root.squashfs
ROOTFS_IMAGE_TEMP=$MTD_FOLDER/rootfs.temp
ROOTFS_IMAGE_MTD=$MTD_FOLDER/rootfs.bin

KERNEL=build_dir/linux-imx25_imx25pdk/uImage
ROOTFS=build_dir/linux-imx25_imx25pdk/root.squashfs

ROOTFS_JFFS2=build_dir/linux-imx25_imx25pdk/root.jffs2-128k
ROOTFS_JFFS2_IMAGE=$ATK_FOLDER/root.jffs2-128k
ROOTFS_JFFS2_IMAGE_TEMP=$MTD_FOLDER/rootfs_jffs2.temp
ROOTFS_JFFS2_IMAGE_MTD=$MTD_FOLDER/rootfs_jffs2.bin

OPENWRT_ALL_IMAGE=$TEMP_FOLDER/openwrt_rootfs.bin

OPENWRT_WEB_V3=$WEB_FOLDER/openwrt-web-upgrade-v3.bin
OPENWRT_WEB=$WEB_FOLDER/openwrt-web-upgrade.bin
KERNEL_WEB=$WEB_FOLDER/kernel-web-upgrade.bin
ROOTFS_WEB=$WEB_FOLDER/rootfs-web-upgrade.bin
ROOTFS_JFFS2_WEB=$WEB_FOLDER/rootfs-jffs2-web-upgrade.bin

generate_partition_image ()
{
	echo "[BANDRICH] Create $2"
	cp $1 $2
	output_image_size=$((($3)*1024*1024/512))
	input_image_size=$(stat -c%s "$2")
	input_image_size=$((($input_image_size)/512))

	if [ $input_image_size -eq 0 ]; then 
		echo "warning: $1 is too small"
		input_image_size=1
	fi

	blocks=$((($output_image_size)-($input_image_size)))
	dd if=/dev/zero of=$2 seek=$input_image_size bs=512 count=$blocks	
}

generate_openwrt_bin ()
{
	# $1 openwrt firmware file                                                                            
	# $2 header file                                                                                  
	# $3 output filename                                                                              
	echo "Packing $1 as a self-extract-upgrade binary file..."
	if [ -f $2 ]; then
	    cat $2 > $3 && tar czf - $1 >> $3
	    echo "File $1 has packed as $3 ..."
	    chmod a+x $3
	else
	    echo "File $2 not exist...."
	fi
}

generate_openwrt_bin_for_web ()
{
	FW=$OPENWRT_IMAGE
	HD=$OPENWRT_BIN_HDR
	OUTFILE=$OPENWRT_BIN_NAME
	generate_openwrt_bin $FW $HD $OUTFILE 
}

mtd_size(){
	file=linux-2.6.31/arch/arm/mach-mx25/mx25_3stack.c
	size=`cat $file | grep "#define MTD_SIZE_$1" | awk -F" " '{ print $3 }'`
	echo "$size"	
}

# Copy all data to temp folder #

echo "[BANDRICH]Copy BR_Partition to temp folder"
[ -d $TEMP_FOLDER ] && rm -rf $TEMP_FOLDER
mkdir $TEMP_FOLDER
mkdir $ATK_FOLDER
mkdir $MTD_FOLDER
mkdir $WEB_FOLDER
mkdir $RAW_FOLDER





cp ./target/linux/imx25/base-files/etc/passwd BR_Partition/userdata/default/
cp -r BR_Partition/* $RAW_FOLDER 
find $RAW_FOLDER -type d -iname '.svn' -exec rm -rf {} \;
cp $KERNEL $ATK_FOLDER


cp $ROOTFS_JFFS2 $ATK_FOLDER
chmod 744 $ROOTFS_JFFS2

# Generate Partitions 

MTD_SIZE_DATA=`mtd_size DATA`
MTD_SIZE_KERNEL=`mtd_size KERNEL`
MTD_SIZE_ROOTFS=`mtd_size ROOTFS`
MTD_SIZE_USERDATA=`mtd_size USERDATA`

echo "DATA     PARTITION SIZE is $MTD_SIZE_DATA MB"
echo "KERNEL   PARTITION SIZE is $MTD_SIZE_KERNEL MB"
echo "ROOTFS   PARTITION SIZE is $MTD_SIZE_ROOTFS MB"
echo "USERDATA PARTITION SIZE is $MTD_SIZE_USERDATA MB"




mkfs.jffs2 -s 2048 -d $DATA_FOLDER -n -e 0x20000 -o $DATA_IMAGE
mkfs.jffs2 -s 2048 -d $F_DATA_FOLDER -n -e 0x20000 -o $F_DATA_IMAGE
mkfs.jffs2 -s 2048 -d $USERDATA_FOLDER -n -e 0x20000 -o $USERDATA_IMAGE 

dd if=$DATA_IMAGE bs=1024k conv=sync > $DATA_IMAGE_MTD
dd if=$F_DATA_IMAGE bs=1024k conv=sync > $F_DATA_IMAGE_MTD

dd if=$KERNEL_IMAGE bs=1024k conv=sync > $KERNEL_IMAGE_TEMP
generate_partition_image $KERNEL_IMAGE_TEMP $KERNEL_IMAGE_MTD $MTD_SIZE_KERNEL




dd if=$ROOTFS_JFFS2_IMAGE bs=1024k conv=sync > $ROOTFS_JFFS2_IMAGE_TEMP
generate_partition_image $ROOTFS_JFFS2_IMAGE_TEMP $ROOTFS_JFFS2_IMAGE_MTD $MTD_SIZE_ROOTFS

generate_partition_image $USERDATA_IMAGE $USERDATA_IMAGE_MTD $MTD_SIZE_USERDATA







if [ 1 -eq 1 ]; then
	cat upgrade-bin-header3 > $OPENWRT_WEB_V3
	tar czf - $KERNEL_IMAGE_TEMP $ROOTFS_JFFS2_IMAGE_TEMP >> $OPENWRT_WEB_V3
	chmod a+x $OPENWRT_WEB_V3
fi

cat upgrade-bin-header2 > $OPENWRT_WEB
tar czf - $KERNEL_IMAGE_TEMP >> $OPENWRT_WEB

echo -ne "\015\012rootfs-tag\015\012" >> $OPENWRT_WEB

tar czf - $ROOTFS_JFFS2_IMAGE_TEMP >> $OPENWRT_WEB
chmod a+x $OPENWRT_WEB


