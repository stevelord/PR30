#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5624 2006-11-23 00:29:07Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=batman-adv

# Versions to download for maintenance build
PKG_REV_MAINT:=1611
PKG_KMOD_VERSION_MAINT:=5c4633c532a7a9b5adfe5dbf3f5e97fff307216e
# just for the package version numbering
PKG_VERSION_MAINT:=0.2.1
# Versions to download for development build
PKG_REV_DEV:=1679
PKG_KMOD_VERSION_DEV:=de32ed5ba4b6487dbd3c4cad6c9ba8240d79c421
# just for the package version numbering
PKG_VERSION_DEV:=0.3.0a
## Make sure git-commit hash and svn revision number fit! ##

PKG_RELEASE:=1
PKG_SOURCE_SUBDIR:=$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

ifneq ($(CONFIG_BATMAN_ADV_DEV),)
	PKG_REV:=$(PKG_REV_DEV)
	PKG_VERSION:=$(PKG_VERSION_DEV)
	PKG_KMOD_VERSION=$(PKG_KMOD_VERSION_DEV)
	PKG_KMOD_URL=http://git.open-mesh.org/snapshot/$(PKG_KMOD_VERSION)/
	PKG_KMOD_FNAME=batman-adv-dev-$(PKG_REV).tar.gz
	PKG_TOOL_VERSION=$(PKG_REV)
	PKG_TOOL_PROTO=svn
	PKG_TOOL_URL=http://downloads.open-mesh.org/svn/batman/trunk/batctl
	PKG_TOOL_FNAME:=batctl-dev-$(PKG_TOOL_VERSION).tar.gz
else
	PKG_REV:=$(PKG_REV_MAINT)
	PKG_VERSION:=$(PKG_VERSION_MAINT)
	PKG_KMOD_VERSION=$(PKG_KMOD_VERSION_MAINT)
	PKG_KMOD_URL=http://git.open-mesh.org/snapshot/$(PKG_KMOD_VERSION)/
	PKG_KMOD_FNAME=batman-adv-maint-$(PKG_REV).tar.gz
	PKG_TOOL_VERSION=$(PKG_REV)
	PKG_TOOL_PROTO=svn
	PKG_TOOL_URL=http://downloads.open-mesh.org/svn/batman/branches/batctl-0.2.x/
	PKG_TOOL_FNAME:=batctl-maint-$(PKG_TOOL_VERSION).tar.gz
endif

PKG_KMOD_BUILD_DIR:=$(PKG_BUILD_DIR)/batman-adv-kernelland
PKG_TOOL_BUILD_DIR:=$(PKG_BUILD_DIR)/batctl

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

# have to set this variable after including the Makefiles
ifneq ($(CONFIG_BATMAN_ADV_DEV),)
	STAMP_PREPARED:=$(STAMP_PREPARED)_dev
else
	STAMP_PREPARED:=$(STAMP_PREPARED)_maint
endif
STAMP_PREPARED:=$(STAMP_PREPARED)$(if $(CONFIG_KMOD_BATMAN_ADV_BATCTL),_batctl)
STAMP_BUILT:=$(STAMP_BUILT)$(if $(CONFIG_KMOD_BATMAN_ADV_DEBUG_LOG),_debug)

define Package/batman-adv/Default
  URL:=http://www.open-mesh.org/
  MAINTAINER:=Marek Lindner <lindner_marek@yahoo.de>
endef

define KernelPackage/batman-adv-kernelland
$(call Package/batman-adv/Default)
  SUBMENU:=Network Support
  DEPENDS:=@LINUX_2_6
  TITLE:=B.A.T.M.A.N. layer 2
  FILES:=$(PKG_KMOD_BUILD_DIR)/batman-adv.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,50,batman-adv)
endef

define KernelPackage/batman-adv-kernelland/config
	source "$(SOURCE)/Config.in"
endef

define KernelPackage/batman-adv-kernelland/description
B.A.T.M.A.N. layer 2
endef

define Download/batman-adv-kernelland
  FILE:=$(PKG_KMOD_FNAME)
  URL:=$(PKG_KMOD_URL)
endef
$(eval $(call Download,batman-adv-kernelland))

ifneq ($(DEVELOPER)$(CONFIG_KMOD_BATMAN_ADV_BATCTL),)
define Download/batctl
  FILE:=$(PKG_TOOL_FNAME)
  URL:=$(PKG_TOOL_URL)
  PROTO:=$(PKG_TOOL_PROTO)
  VERSION:=$(PKG_TOOL_VERSION)
  SUBDIR:=$(PKG_SOURCE_SUBDIR)/batctl
endef
$(eval $(call Download,batctl))
endif

ifneq ($(DEVELOPER)$(CONFIG_KMOD_BATMAN_ADV_BATCTL),)
ifneq ($(DEVELOPER)$(CONFIG_BATMAN_ADV_DEV),)
define Download/batman
  FILE:=batman-$(PKG_REV).tar.gz
  URL:=http://downloads.open-mesh.org/svn/batman/trunk/batman
  PROTO:=svn
  VERSION:=$(PKG_REV)
  SUBDIR:=$(PKG_SOURCE_SUBDIR)/batman
endef
$(eval $(call Download,batman))
endif
endif

define Build/Prepare
	tar xzf "$(DL_DIR)/$(PKG_KMOD_FNAME)" -C "$(PKG_BUILD_DIR)"
ifneq ($(DEVELOPER)$(CONFIG_KMOD_BATMAN_ADV_BATCTL),)
	tar xzf "$(DL_DIR)/$(PKG_TOOL_FNAME)" -C "$(BUILD_DIR)"
ifneq ($(DEVELOPER)$(CONFIG_BATMAN_ADV_DEV),)
	tar xzf "$(DL_DIR)/batman-$(PKG_REV).tar.gz" -C "$(BUILD_DIR)"
endif
endif
	mv "$(PKG_BUILD_DIR)/batman-adv" "$(PKG_KMOD_BUILD_DIR)"
	$(call PatchDir,$(PKG_BUILD_DIR),$(PATCH_DIR),)
ifneq ($(CONFIG_BATMAN_ADV_DEV),)
	$(call PatchDir,$(PKG_BUILD_DIR),$(PATCH_DIR)/dev,)
else
	$(call PatchDir,$(PKG_BUILD_DIR),$(PATCH_DIR)/maint,)
endif
	cp $(PKG_KMOD_BUILD_DIR)/Makefile.kbuild $(PKG_KMOD_BUILD_DIR)/Makefile
endef

MAKE_BATADV_KERNELLAND_ARGS += \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	ARCH="$(LINUX_KARCH)" \
	PATH="$(TARGET_PATH)" \
	$(if $(CONFIG_KMOD_BATMAN_ADV_DEBUG_LOG),EXTRA_CFLAGS="-DCONFIG_BATMAN_ADV_DEBUG") \
	SUBDIRS="$(PKG_KMOD_BUILD_DIR)" \
	LINUX_VERSION="$(LINUX_VERSION)" \
	REVISION="$(PKG_REV)" modules

MAKE_BATCTL_ARGS += \
	CFLAGS="$(TARGET_CFLAGS)" \
	CCFLAGS="$(TARGET_CFLAGS)" \
	OFLAGS="$(TARGET_CFLAGS)" \
	REVISION="$(PKG_REV)" \
	CC="$(TARGET_CC)" \
	NODEBUG=1 \
	UNAME="Linux" \
	INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
	STRIP="/bin/true" \
	batctl install

define Build/Configure
endef

ifneq ($(DEVELOPER)$(CONFIG_PACKAGE_kmod-batman-adv-kernelland),)
        BUILD_KMOD_ADV = $(MAKE) -C "$(LINUX_DIR)" $(MAKE_BATADV_KERNELLAND_ARGS)
        BUILD_BATCTL = $(MAKE) -C $(PKG_TOOL_BUILD_DIR) $(MAKE_BATCTL_ARGS)
endif

define Build/Compile
	$(BUILD_KMOD_ADV)
ifneq ($(DEVELOPER)$(CONFIG_KMOD_BATMAN_ADV_BATCTL),)
	$(BUILD_BATCTL)
endif
endef

define Build/Clean
        rm -rf $(PKG_KMOD_BUILD_DIR)
endef

define KernelPackage/batman-adv-kernelland/install
	$(INSTALL_DIR) $(1)/etc/config $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/batman-adv $(1)/etc/init.d
	$(INSTALL_DATA) ./files/etc/config/batman-adv $(1)/etc/config
ifneq ($(DEVELOPER)$(CONFIG_KMOD_BATMAN_ADV_BATCTL),)
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/batctl $(1)/usr/sbin/
endif
endef

$(eval $(call KernelPackage,batman-adv-kernelland))
