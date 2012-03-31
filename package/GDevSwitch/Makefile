include $(TOPDIR)/rules.mk

PKG_NAME:=GDevSwitch
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=sqlite3

include $(INCLUDE_DIR)/package.mk

define Package/GDevSwitch
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=BandLuxe Control Tool
  DEPENDS:=+libsqlite3
endef

define Package/GDevSwitch/description
 The GDevSwitch will handle Zero-CD with ECM device switching.
endef

TARGET_CFLAGS:=$(TARGET_CPPFLAGS)

define Build/Configure
	$(call Build/Configure/Default, \
		--disable-xmlserver \
		--enable-rcmdserver --enable-querydiag \
		--disable-sms \
		--enable-netserver --enable-atemu \
		--enable-debug --enable-sharememory \
		--enable-atduo --enable-pr30 \
		, \
		config_TARGET_SQLITE3_INC="-I$(STAGING_DIR)/usr/include" \
		config_TARGET_SQLITE3_LIBS="-L$(STAGING_DIR)/usr/lib -lsqlite3" \
	)
endef

define Build/Compile

	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)\
		-I$(BUILD_DIR)/../linux-imx25_imx25pdk/linux-2.6.31/include"

endef

define Package/GDevSwitch/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/GDevSwitch $(1)/usr/bin/
endef

$(eval $(call BuildPackage,GDevSwitch))
