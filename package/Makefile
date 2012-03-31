#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

curdir:=package

-include $(TMP_DIR)/.packagedeps
$(curdir)/builddirs:=$(sort $(package-) $(package-y) $(package-m))
ifeq ($(SDK),1)
  $(curdir)/builddirs-install:=.
else
  $(curdir)/builddirs-default:=. $(sort $(package-y) $(package-m))
  $(curdir)/builddirs-prereq:=. $(sort $(prereq-y) $(prereq-m))
  $(curdir)/builddirs-install:=. $(sort $(package-y))
endif
ifneq ($(IGNORE_ERRORS),)
  $(curdir)/builddirs-ignore-compile:= $(if $(filter m y, $(IGNORE_ERRORS)),$(foreach m,$(IGNORE_ERRORS),$(package-$(m))),$(package-m))
endif

$(curdir)/install:=$(curdir)/install-cleanup

$(curdir)/cleanup: $(TMP_DIR)/.build
	- find $(STAGING_DIR_ROOT) -type d | $(XARGS) chmod 0755
	rm -rf $(TARGET_DIR) $(STAGING_DIR_ROOT)

ifdef CONFIG_USE_MKLIBS
  define mklibs
	rm -rf $(TMP_DIR)/mklibs-progs $(TMP_DIR)/mklibs-out
	# first find all programs and add them to the mklibs list
	find $(STAGING_DIR_ROOT) -type f -perm +100 -exec \
		file -r -N -F '' {} + | \
		awk ' /executable.*dynamically/ { print $$1 }' > $(TMP_DIR)/mklibs-progs
	# find all loadable objects that are not regular libraries and add them to the list as well
	find $(STAGING_DIR_ROOT) -type f -name \*.so\* -exec \
		file -r -N -F '' {} + | \
		awk ' /shared object/ { print $$1 }' >> $(TMP_DIR)/mklibs-progs
	mkdir -p $(TMP_DIR)/mklibs-out
	$(STAGING_DIR_HOST)/bin/mklibs.py -D \
		-d $(TMP_DIR)/mklibs-out \
		--sysroot $(STAGING_DIR_ROOT) \
		-L /lib \
		-L /usr/lib \
		-L /usr/lib/ebtables \
		--ldlib $(patsubst $(STAGING_DIR_ROOT)/%,/%,$(firstword $(wildcard \
			$(foreach name,ld-uClibc.so.* ld-linux.so.* ld-*.so, \
			  $(STAGING_DIR_ROOT)/lib/$(name) \
			)))) \
		--target $(REAL_GNU_TARGET_NAME) \
		`cat $(TMP_DIR)/mklibs-progs` 2>&1
	$(RSTRIP) $(TMP_DIR)/mklibs-out
	for lib in `ls $(TMP_DIR)/mklibs-out/*.so.* 2>/dev/null`; do \
		LIB="$${lib##*/}"; \
		DEST="`ls "$(TARGET_DIR)/lib/$$LIB" "$(TARGET_DIR)/usr/lib/$$LIB" 2>/dev/null`"; \
		[ -n "$$DEST" ] || continue; \
		echo "Copying stripped library $$lib to $$DEST"; \
		cp "$$lib" "$$DEST" || exit 1; \
	done
  endef
endif

$(curdir)/rootfs-prepare: $(TMP_DIR)/.build
	@-$(MAKE) package/preconfig
	@if [ -d $(TOPDIR)/files ]; then \
		$(CP) $(TOPDIR)/files/. $(TARGET_DIR); \
	fi
	@mkdir -p $(TARGET_DIR)/etc/rc.d
	@( \
		cd $(TARGET_DIR); \
		for script in ./etc/init.d/*; do \
			grep '#!/bin/sh /etc/rc.common' $$script >/dev/null || continue; \
			IPKG_INSTROOT=$(TARGET_DIR) $$(which bash) ./etc/rc.common $$script enable; \
		done || true \
	)
	@-find $(TARGET_DIR) -name CVS   | $(XARGS) rm -rf
	@-find $(TARGET_DIR) -name .svn  | $(XARGS) rm -rf
	@-find $(TARGET_DIR) -name '.#*' | $(XARGS) rm -f
	@echo "[BANDRICH]Build WiFi drivers and programs"
	@make -C $(TOPDIR)/linux-2.6.31/ti/WiLink62_AP/platforms/os/linux ARCH=arm HOST_PLATFORM=mx25 BUILD_SUPPL=y CCX=n XCC=n BMTRACE=n TEST=n STRIP=y BMTRACE=n TNETW=1273 AP_MODE_ENABLED=y TI_HOST_OS=LINUX CROSS_COMPILE=$(TOOLCHAIN_DIR)/usr/bin/$(TARGET_CROSS) KERNEL_DIR=$(TOPDIR)/linux-2.6.31 OPENSSL_DIR=$(BUILD_DIR)/openssl-0.9.8m clean all V=1
	@echo "[BANDRICH]Copy WiFi drivers to /lib/modules/2.6.31"
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/sdio.ko $(TARGET_DIR)/lib/modules/2.6.31
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/tiap_drv.ko $(TARGET_DIR)/lib/modules/2.6.31	
	@echo "[BANDRICH]Copy WiFi programs and files to /usr/bin"
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/hostapd $(TARGET_DIR)/usr/bin
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/tiwlan_loader $(TARGET_DIR)/usr/bin
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/wlan_cu $(TARGET_DIR)/usr/bin
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/tiwlan.ini $(TARGET_DIR)/usr/bin
	@cp $(TOPDIR)/linux-2.6.31/ti/wifi_images/firmware_ap.bin $(TARGET_DIR)/usr/bin
	@echo "[BANDRICH]Build brlcd and copy to /usr/bin"
	@make -C $(TOPDIR)/BR_Packages/BrLCDMain clean
	@make -C $(TOPDIR)/BR_Packages/BrLCDMain
	@cp $(TOPDIR)/BR_Packages/BrLCDMain/brlcd $(TARGET_DIR)/usr/bin
	@cp $(TOPDIR)/BR_Packages/BrModuleDownload/brdloadr $(TARGET_DIR)/usr/bin
	@echo "[BANDRICH]Customization"
	@cp -r $(TOPDIR)/BR_customization/bandluxe/* $(TARGET_DIR)/
	@-find $(TARGET_DIR) -type d -iname '.svn' -exec rm -rf {} \;
	$(if $(CONFIG_CLEAN_IPKG),rm -rf $(TARGET_DIR)/usr/lib/opkg)
	$(call mklibs)

$(curdir)/index: FORCE
	@(cd $(PACKAGE_DIR); $(SCRIPT_DIR)/ipkg-make-index.sh . > Packages && \
		gzip -9c Packages > Packages.gz \
	)

$(curdir)/flags-install:= -j1

$(eval $(call stampfile,$(curdir),package,prereq,.config))
$(eval $(call stampfile,$(curdir),package,cleanup,$(TMP_DIR)/.build))
$(eval $(call stampfile,$(curdir),package,compile,$(TMP_DIR)/.build))
$(eval $(call stampfile,$(curdir),package,install,$(TMP_DIR)/.build))
$(eval $(call stampfile,$(curdir),package,rootfs-prepare,$(TMP_DIR)/.build))

$(eval $(call subdir,$(curdir)))
