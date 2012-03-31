
rm out/*
cd platforms/os/linux
sh mx25_make.sh

cd ../../..

cp external_drivers/mx25/Linux/sdio/sdio.ko platforms/os/linux/tiap_drv.ko out
cp fw/Latest/Fw1273_CHIP.bin out/firmware.bin
cp platforms/os/linux/tiwlan.ini platforms/os/linux/tiwlan_dual.ini out
ls -la out/tiap_drv.ko out/sdio.ko

