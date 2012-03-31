#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=bandluxed
PKG_VERSION:=1.9.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
#echo $(PKG_BUILD_DIR)

include $(INCLUDE_DIR)/package.mk

define Package/bandluxed
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=BandLuxe Control Tool
endef

define Package/bandluxed/description
 bandluxed is a scripting language interpreter useful for establishing 
 communications on serial lines and through mini-card modems as well as 3G 
 and LTE datacards.
endef

TARGET_CFLAGS:=$(TARGET_CPPFLAGS)

define Build/Configure
	$(call Build/Configure/Default, \
		--disable-xmlserver \
		--enable-rcmdserver \
		--enable-querydiag \
		--disable-sms \
		--enable-netserver \
		--enable-atemu \
		--enable-debug --enable-sharememory \
		--enable-atduo --enable-pr30 \
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)\
		-I$(BUILD_DIR)/../linux-imx25_imx25pdk/linux-2.6.31/include"

endef

define Package/bandluxed/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/diagd/bandluxed $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rcmdc/rcmdc $(1)/usr/bin/
endef

$(eval $(call BuildPackage,bandluxed))
