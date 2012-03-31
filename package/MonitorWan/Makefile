include $(TOPDIR)/rules.mk

PKG_NAME:=MonitorWan
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_INSTALL:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/MonitorWan
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Monitor Wan
  DEPENDS:=+uci
endef

define Package/MonitorWan/description
 The MonitorWan  monitor wan status
endef

TARGET_CFLAGS:=$(TARGET_CPPFLAGS)
CFLAGS:=$(TARGET_CFLAGS)
CFLAGS += -I$(BUILD_DIR)/../linux-imx25_imx25pdk/linux-2.6.31/include

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		$(CFLAGS)
endef

define Package/MonitorWan/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/MonitorWan $(1)/usr/bin/
endef

$(eval $(call BuildPackage,MonitorWan))
