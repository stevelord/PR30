#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fswebcam
PKG_VERSION:=20100405
PKG_RELEASE:=1

PKG_INSTALL=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.firestorm.cx/fswebcam/files/
PKG_MD5SUM:=1ca864c8cef096fb4ace71cf14085424

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DEPENDS:=libgd

define Package/fswebcam
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=fswebcam
  URL:=http://www.firestorm.cx/fswebcam/
  DEPENDS:=+libgd
endef

define Package/fswebcam/description
  fswebcam is a neat and simple webcam app. It captures images from a V4L1/V4L2 compatible
  device or file, averages them to reduce noise and draws a caption using the GD Graphics
  Library which also handles compressing the image to PNG or JPEG. The resulting image
  is saved to a file or sent to stdio where it can be piped to something like ncftpput or scp.
endef

EXTRA_CFLAGS+= $(TARGET_CPPFLAGS)
EXTRA_LDFLAGS+= $(TARGET_LDFLAGS) -Wl,-rpath-link,$(STAGING_DIR)/usr/lib

define Package/fswebcam/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/fswebcam $(1)/usr/bin/
endef

$(eval $(call BuildPackage,fswebcam))
