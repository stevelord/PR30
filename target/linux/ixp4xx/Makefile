#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=armeb
BOARD:=ixp4xx
BOARDNAME:=Intel IXP4xx
FEATURES:=squashfs
SUBTARGETS=generic harddisk

LINUX_VERSION:=2.6.32.9

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += ixp4xx-microcode fconfig

$(eval $(call BuildTarget))
