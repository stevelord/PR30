br_mount_point() {
	local temp
        temp="$(/bin/grep $1 /proc/mounts | /usr/bin/awk -F" " '{ print $2 }')"
        echo $temp
}
                        
DATA_PATH=`br_mount_point /dev/mtdblock1`
USERDATA_PATH=`br_mount_point /dev/mtdblock5`

                
mkdir $USERDATA_PATH/default_bk        
#echo "Check default folder..."
if [ -d "$USERDATA_PATH/default/config" ]; then
    if [ -e "$USERDATA_PATH/default/passwd" ]; then
#	echo "default folder in userdata is exist"
#	echo "case1"
	cp -r $USERDATA_PATH/default/config $USERDATA_PATH/default_bk/config
	cp -f $USERDATA_PATH/default/passwd $USERDATA_PATH/default_bk/passwd
	echo "0"
        exit
    fi
fi

if [ -d "$DATA_PATH/default/config" ]; then
    if [ -e "$DATA_PATH/default/passwd" ]; then
#        echo "default folder in data is exist"
#	echo "case2"
        cp -r $DATA_PATH/default/config $USERDATA_PATH/default_bk/config
        cp -f $DATA_PATH/default/passwd $USERDATA_PATH/default_bk/passwd
	echo "0"
        exit
    fi
fi

#------------------------------------------------
if [ -d "$DATA_PATH/default/config" ]; then
    if [ -e "$USERDATA_PATH/default/passwd" ]; then
#	echo "default folder in userdata is exist"
#	echo "case3"
	cp -r $DATA_PATH/default/config $USERDATA_PATH/default_bk/config
	cp -f $USERDATA_PATH/default/passwd $USERDATA_PATH/default_bk/passwd
	echo "0"
        exit
    fi
fi

if [ -d "$USERDATA_PATH/default/config" ]; then
    if [ -e "$DATA_PATH/default/passwd" ]; then
#        echo "default folder in data is exist"
#	echo "case4"
        cp -r $USERDATA_PATH/default/config $USERDATA_PATH/default_bk/config
        cp -f $DATA_PATH/default/passwd $USERDATA_PATH/default_bk/passwd
	echo "0"
        exit
    fi
fi
                        						
#echo "default folder is not exist"
echo "-1"                        						
                        						
