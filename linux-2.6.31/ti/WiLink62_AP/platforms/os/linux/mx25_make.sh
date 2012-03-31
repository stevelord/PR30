export CROSS_COMPILE=$(pwd)/../../../../../../staging_dir/toolchain-arm_v5te_gcc-4.3.3+cs_uClibc-0.9.30.1_eabi/usr/bin/arm-openwrt-linux-uclibcgnueabi-
export ARCH=arm
export HOST_PLATFORM=mx25
export BUILD_SUPPL=y
export CCX=n
export XCC=n
export BMTRACE=n
export TEST=n
export STRIP=y
export BMTRACE=n
export TNETW=1273
export AP_MODE_ENABLED=y
export TI_HOST_OS=LINUX

export KERNEL_DIR=$(pwd)/../../../../..
export OPENSSL_DIR=$KERNEL_DIR/../build_dir/target-arm_v5te_uClibc-0.9.30.1_eabi/openssl-0.9.8m

# platform settings.
make clean
make V=1 


