#
# Copyright (C) 2008-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=imx25
BOARDNAME:=Freescale I.MX25PDK/PR30
SUBTARGETS:=imx25pdk pr30
##CFLAGS:=-Os -pipe
FEATURES:=broken

LINUX_VERSION:=2.6.31

##DEFAULT_PACKAGES+=kmod-leds-gpio

define Target/Description
	Build firmware images for Freescale I.MX25PDK/PR30 boards.
endef


include $(INCLUDE_DIR)/target.mk


#GENERIC_PLATFORM_DIR := $(TOPDIR)/target/linux/$(BOARD)
#GENERIC_PATCH_DIR := $(GENERIC_PLATFORM_DIR)/patches-generic-2.6.31
#GENERIC_FILES_DIR := $(GENERIC_PLATFORM_DIR)/files-generic-2.6.31


$(eval $(call BuildTarget))
