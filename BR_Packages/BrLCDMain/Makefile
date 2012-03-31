all: brlcd

TRUNK_PATH = ../../

HOSTAPD_INC_DIR = $(TRUNK_PATH)linux-2.6.31/ti/hostapd_inc


#CC 	= gcc
CROSS_COMPILE = $(TRUNK_PATH)staging_dir/toolchain-arm_v5te_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/usr/bin/
CC = arm-openwrt-linux-uclibcgnueabi-gcc

LIB_PATH1 =-L$(TRUNK_PATH)staging_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/usr/lib
LIB_PATH1 += -L$(TRUNK_PATH)build_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/uci-0.7.5
LIB_PATH2 =$(TRUNK_PATH)staging_dir/toolchain-arm_v5te_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/lib
LIB_PATH2 += -L$(TRUNK_PATH)staging_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/usr/lib/libiconv/lib

INCLUDE	+= -I $(TRUNK_PATH)build_dir/linux-imx25_imx25pdk/linux-2.6.31/include
INCLUDE	+= -I $(TRUNK_PATH)staging_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/usr/lib/libiconv/include
INCLUDE	+= -I $(TRUNK_PATH)staging_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/usr/include/freetype2
INCLUDE	+= -I $(TRUNK_PATH)staging_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/usr/include

INCLUDE += -I$(HOSTAPD_INC_DIR)
INCLUDE += -I$(TRUNK_PATH)build_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/uci-0.7.5







CFLAGS	= -g -Wall -ansi

OBJS += \
OLED_DRAW_API.o \
status_bar.o \
brlcdmain.o \
$(HOSTAPD_INC_DIR)/wpa_ctrl.o \
$(HOSTAPD_INC_DIR)/os_unix.o



brlcd:  $(OBJS)
	$(CROSS_COMPILE)$(CC) -o brlcd $(OBJS) -L$(LIB_PATH2) $(LIB_PATH1) \
        `freetype-config --cflags` `freetype-config --libs` -liconv -lpthread -luci

	cp en.txt $(TRUNK_PATH)BR_Partition/userdata/brlcd

OLED_DRAW_API.o: OLED_DRAW_API.c
	$(CROSS_COMPILE)$(CC) $(INCLUDE) -c OLED_DRAW_API.c

status_bar.o: status_bar.c
	$(CROSS_COMPILE)$(CC) $(INCLUDE) -c status_bar.c




brlcdmain.o: brlcdmain.c
	$(CROSS_COMPILE)$(CC) $(INCLUDE) -c brlcdmain.c
clean:
	rm -rf brlcd
	rm -rf *.o
	rm -rf *.c~
