#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fowsr
PKG_VERSION:=1.0
PKG_RELEASE:=20100501

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_RELEASE).tar.gz
PKG_SOURCE_URL:=http://fowsr.googlecode.com/files
PKG_MD5SUM:=aab66ea115fb304e0f5a6171dd6df2ba

include $(INCLUDE_DIR)/package.mk

define Package/fowsr
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+libusb
  TITLE:=Fine Offset Wireless Weather Station Reader
  URL:=http://fowsr.googlecode.com/
endef

define Package/fowsr/description
  fowsr is a USB Data Collector application for WH1080 compatible
  weather stations.
  Output formats includes Wunderground.com and pywws.
  See http://www.foshk.com for details and download of the
  EasyWeather application.
  All weather stations that can use the EasyWeather application
  will work with this software.
endef

CONFIGURE_ARGS += \
	--with-linux-headers="$(LINUX_DIR)" \

define Package/fowsr/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fowsr $(1)/usr/bin/
endef

$(eval $(call BuildPackage,fowsr))
