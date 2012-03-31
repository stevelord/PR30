
USB_MENU:=USB Gadget Support

define KernelPackage/usb-gadget
  SUBMENU:=$(USB_MENU)
  TITLE:=USB Gadget core
  KCONFIG:=CONFIG_USB_GADGET=y
  FILES:=$(LINUX_DIR)/drivers/usb/gadget/gadgetfs.ko
  #AUTOLOAD:=$(call AutoLoad,60,gadgetfs,1)
endef

$(eval $(call KernelPackage,usb-gadget))

define KernelPackage/usb/Depends
  SUBMENU:=$(USB_MENU)
  DEPENDS:=kmod-usb-gadget $(1)
endef

define KernelPackage/usb-gadget-storage
$(call KernelPackage/usb/Depends,)
  TITLE:=File-backed Storage Gadget
  KCONFIG:=CONFIG_USB_GADGETFS
  FILES:=$(LINUX_DIR)/drivers/usb/gadget/g_file_storage.ko
endef
$(eval $(call KernelPackage,usb-gadget-storage))

define KernelPackage/usb-gadget-ndis
$(call KernelPackage/usb/Depends,)
  TITLE:=USB Gadget NDIS
  KCONFIG:=CONFIG_USB_ETH
  FILES:=$(LINUX_DIR)/drivers/usb/gadget/g_ether.ko
endef
$(eval $(call KernelPackage,usb-gadget-ndis))

define KernelPackage/usb-gadget-serial
$(call KernelPackage/usb/Depends,)
  TITLE:=USB Gadget SERIAL
  KCONFIG:=CONFIG_USB_G_SERIAL
  FILES:=$(LINUX_DIR)/drivers/usb/gadget/g_serial.ko
endef
$(eval $(call KernelPackage,usb-gadget-serial))
