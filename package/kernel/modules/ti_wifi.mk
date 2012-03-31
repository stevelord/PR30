
TI_WIFI_MENU:=TI WIFI Support

define KernelPackage/ti-wifi
  SUBMENU:=$(TI_WIFI_MENU)
  TITLE:=TI WIFI core
#  KCONFIG:=CONFIG_USB_GADGET=y
  FILES:=$(LINUX_DIR)/ti/WiLink62_AP/out/*.$(LINUX_KMOD_SUFFIX)
#  AUTOLOAD:=$(call AutoLoad,50,sdio tiap_drv)
endef

$(eval $(call KernelPackage,ti-wifi))

define KernelPackage/tiwifi/Depends
  SUBMENU:=$(TI_WIFI_MENU)
  DEPENDS:=kmod-ti-wifi $(1)
endef

