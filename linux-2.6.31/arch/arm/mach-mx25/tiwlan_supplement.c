/**
 * @file          tiwlan_supplicant.c
 */


#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>

#include <mach/memory.h>
#include <mach/gpio.h>
#include <mach/mmc.h>

#include "iomux.h"
#include "crm_regs.h"

#define GPIO_PAD_CTL1 (PAD_CTL_DRV_NORMAL | PAD_CTL_PKE_ENABLE | \
             PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V | PAD_CTL_PUE_PULL)

int mmc_io_rw_direct(struct mmc_card *card, int write, unsigned fn,
        unsigned addr, u8 in, u8* out);

unsigned char tiwlan_direct_readb(struct sdio_func *func, int func_no ,unsigned int addr,
        int *err_ret)
{
        int ret;
        unsigned char val;

        BUG_ON(!func);

        if (err_ret)
                *err_ret = 0;

        ret = mmc_io_rw_direct(func->card, 0, func_no, addr, 0, &val);
        if (ret) {
                if (err_ret)
                        *err_ret = ret;
                return 0xFF;
        }

        return val;
}
EXPORT_SYMBOL(tiwlan_direct_readb);

void tiwlan_direct_writeb(struct sdio_func *func, int func_no , unsigned char b, unsigned int addr, int *err_ret)
{
        int ret;

        BUG_ON(!func);

        ret = mmc_io_rw_direct(func->card, 1, func_no, addr, b, NULL);
        if (err_ret)
                *err_ret = ret;
}

EXPORT_SYMBOL(tiwlan_direct_writeb);

#define DEFAULT_FORCE_RESCAN_DELAY      200
static struct mmc_host * g_tiwlan_mmc_host=NULL; 
void tiwlan_force_presence_change(void);
void tiwlan_set_mmc_host( struct mmc_host * tiwlan_mmc );

void tiwlan_force_presence_change()
{
	pr_debug(KERN_INFO "%s: Enter ...g_tiwlan_mmc_host:%p\n",__func__,g_tiwlan_mmc_host);
       	if ( g_tiwlan_mmc_host) mmc_detect_change( g_tiwlan_mmc_host , msecs_to_jiffies(DEFAULT_FORCE_RESCAN_DELAY));
}

void tiwlan_set_mmc_host( struct mmc_host * tiwlan_mmc ) 
{
	
	pr_debug(KERN_INFO "%s: Enter ...tiwlan_mmc:%p\n",__func__,tiwlan_mmc);
	g_tiwlan_mmc_host = tiwlan_mmc;
}

EXPORT_SYMBOL(tiwlan_force_presence_change);
EXPORT_SYMBOL(tiwlan_set_mmc_host);


int tiwlan_wifi_power(int on) { 
       if (on) gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D10),1);
       else gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D10),0);
       return 0;
}
EXPORT_SYMBOL(tiwlan_wifi_power);


int tiwlan_ext_int_init() { 
    	return IOMUX_TO_IRQ(MX25_PIN_D11);
}
EXPORT_SYMBOL(tiwlan_ext_int_init);

static int __init tiwlan_supp_init(void)
{
        int ret=0;


        pr_debug(KERN_INFO "%s:TIWLAN Supplement init\n",__func__);

        // TIWLAN WIFI_IRQ 
        mxc_request_iomux(MX25_PIN_D11, MUX_CONFIG_ALT5);
        gpio_request(IOMUX_TO_GPIO(MX25_PIN_D11), "d11");
        gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_D11)); 
      
	// TIWLAN WIFI_EN 
       mxc_request_iomux(MX25_PIN_D10, MUX_CONFIG_GPIO);
       mxc_iomux_set_pad(MX25_PIN_D10, GPIO_PAD_CTL1);
       gpio_request(IOMUX_TO_GPIO(MX25_PIN_D10), "d10");
       gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D10),1);

        /* Register the sdio driver */
       pr_debug(KERN_INFO "%s:TIWLAN Supplement Done\n",__func__);
        return ret;
}

static void __exit tiwlan_supp_exit(void)
{
        /* Unregister sdio driver */
        //platform_driver_unregister(&sdioDrv_struct);
}

module_init(tiwlan_supp_init);
module_exit(tiwlan_supp_exit);





