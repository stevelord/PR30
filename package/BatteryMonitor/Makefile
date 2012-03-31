include $(TOPDIR)/rules.mk

PKG_NAME:=BatteryMonitor
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

PKG_BUILD_DEPENDS:=

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/BatteryMonitor/Default
  	SECTION:=utils
  	CATEGORY:=Utilities
endef

define Package/BatteryMonitor
$(call Package/BatteryMonitor/Default)
	TITLE:=BatteryMonitor
	MENU=1
endef

define Package/BatteryMonitor/description
This package is for dialup
endef

MKARGS := $(TARGET_CONFIGURE_OPTS) \
	CC="$(TARGET_CROSS)gcc" \
	CXX="$(TARGET_CROSS)g++" \
	CFLAGS="$(TARGET_CFLAGS) $(FPIC)" \
	CPPFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(TOOLCHAIN_DIR)/usr/include" \
	LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -L$(TOOLCHAIN_DIR)/usr/lib" \
	LIBS=""

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) PKG_CONFIG_PATH=$(STAGING_DIR)/usr/lib $(MKARGS)
endef


define Package/BatteryMonitor/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/BatteryMonitor $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/GetAPN $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/BatteryGet $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/GetMemory $(1)/usr/bin
endef

$(eval $(call BuildPackage,BatteryMonitor))
