/*
 * host_platform.c
 *
 * Copyright(c) 1998 - 2009 Texas Instruments. All rights reserved.      
 * All rights reserved.                                                  
 *                                                                       
 * Redistribution and use in source and binary forms, with or without    
 * modification, are permitted provided that the following conditions    
 * are met:                                                              
 *                                                                       
 *  * Redistributions of source code must retain the above copyright     
 *    notice, this list of conditions and the following disclaimer.      
 *  * Redistributions in binary form must reproduce the above copyright  
 *    notice, this list of conditions and the following disclaimer in    
 *    the documentation and/or other materials provided with the         
 *    distribution.                                                      
 *  * Neither the name Texas Instruments nor the names of its            
 *    contributors may be used to endorse or promote products derived    
 *    from this software without specific prior written permission.      
 *                                                                       
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT      
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "tidef.h"
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/delay.h>

#include "host_platform.h"
#include "ioctl_init.h"
#include "WlanDrvIf.h"
#include "Device1273.h"

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/spba.h>
#include <asm/irq.h>
#include <asm/setup.h>
//#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/keypad.h>
#include <mach/memory.h>
#include <mach/gpio.h>
#include <mach/mmc.h>


#define OS_API_MEM_ADRR  	0x0000000
#define OS_API_REG_ADRR  	0x300000

#define SDIO_ATTEMPT_LONGER_DELAY_LINUX  150

int tiwlan_ext_int_init();
int tiwlan_wifi_power(int on);

#if 0 /* needed for first time new host ramp*/ 
static void dump_omap_registers(void);
#endif

extern void tiwlan_force_presence_change(void);


static int mx25_TNETW_Power(int power_on)
{

	if (power_on) {
	        tiwlan_wifi_power(1);
		printk(KERN_ERR "WiFi: ON\n") ;
	} else {
	        tiwlan_wifi_power(0);
		printk(KERN_ERR "WiFi: Off\n");
	}

	return 0;    
}

/*-----------------------------------------------------------------------------

Routine Name:

        hPlatform_hardResetTnetw

Routine Description:

        set the GPIO to low after awaking the TNET from ELP.

Arguments:

        OsContext - our adapter context.


Return Value:

        None

-----------------------------------------------------------------------------*/

int hPlatform_hardResetTnetw(void)
{
  int err;
 printk(KERN_INFO "%s: Enter ..\n",__func__);
    /* Turn power OFF*/
  if ((err = mx25_TNETW_Power(0)) == 0)
  {
    mdelay(1);
    /* Turn power ON*/
    err = mx25_TNETW_Power(1);
    mdelay(200);
    tiwlan_force_presence_change();
  }
  return err;

} /* hPlatform_hardResetTnetw() */

/* Turn device power off */
int hPlatform_DevicePowerOff (void)
{
     int err;
    printk(KERN_INFO "%s: Enter ..\n",__func__); 
     err = mx25_TNETW_Power(0);
    
     mdelay(100);
    tiwlan_force_presence_change();
    
     return err;
}


/* Turn device power off according to a given delay */
int hPlatform_DevicePowerOffSetLongerDelay(void)
{
    int err;
    
    err = mx25_TNETW_Power(0);
    
    mdelay(SDIO_ATTEMPT_LONGER_DELAY_LINUX);
    
    return err;
}


/* Turn device power on */
int hPlatform_DevicePowerOn (void)
{
    int err;
 printk(KERN_INFO "%s: Enter ..\n",__func__);
    err = mx25_TNETW_Power(1);

    /* New Power Up Sequence */
    mdelay(15);
    err = mx25_TNETW_Power(0);
    mdelay(1);

    err = mx25_TNETW_Power(1);

    /* Should not be changed, 50 msec cause failures */
    // **DEBUG** mdelay(70);
    mdelay(150) ;
    tiwlan_force_presence_change();
    msleep(300);

    return err;
}

/*-----------------------------------------------------------------------------

Routine Name:

        Wlan_Hardware_Init

Routine Description:

        this function init  SDIO pin mux , GPIO irq and power enable/disable

Arguments:
	
	none

Return Value:

        status

-----------------------------------------------------------------------------*/

int hPlatform_Wlan_Hardware_Init(void)
{

	// Config MMC pin mux , GPIO irq and power enable/disable
#if 0 
#if defined(CONFIG_MACH_IDEA6410)
       s3c_gpio_cfgpin(IRQ_GPIO , S3C_GPIO_SFN(3));  // GPL8 func 0x11 EINT16
       s3c_gpio_cfgpin(PMENA_GPIO , S3C_GPIO_SFN(3));  // GPC6 func 0x1 Output
       s3c_gpio_setpull(IRQ_GPIO , S3C_GPIO_PULL_NONE);
       s3c_gpio_setpull(PMENA_GPIO, S3C_GPIO_PULL_NONE);
#elif defined(CONFIG_MACH_REAL6410)
       s3c_gpio_cfgpin(IRQ_GPIO , S3C_GPIO_SFN(3));  // GPL8 func 0x11 EINT18
       s3c_gpio_cfgpin(PMENA_GPIO , S3C_GPIO_SFN(1));  // GPC6 func 0x1 Output
       s3c_gpio_setpull(IRQ_GPIO , S3C_GPIO_PULL_NONE);
       s3c_gpio_setpull(PMENA_GPIO, S3C_GPIO_PULL_NONE);

#elif defined(CONFIG_MACH_MX31_WIMAP)
#elif defined(CONFIG_MACH_SMDK2416)
#else 
#error "Unsupport MACH" 
#endif
#endif


	return 0;
}

/*-----------------------------------------------------------------------------

Routine Name:

        InitInterrupt

Routine Description:

        this function init the interrupt to the Wlan ISR routine.

Arguments:

        tnet_drv - Golbal Tnet driver pointer.


Return Value:

        status

-----------------------------------------------------------------------------*/

TWlanDrvIfObj *g_drv ;
int hPlatform_initInterrupt(void *tnet_drv, void* handle_add ) 
{
	TWlanDrvIfObj *drv = tnet_drv;
    int rc=0;

	//if (drv->irq == 0 || handle_add == NULL)
	if (handle_add == NULL)
	{
	  print_err("hPlatform_initInterrupt() bad param drv->irq=%d handle_add=0x%x !!!\n",drv->irq,(int)handle_add);
	  return -EINVAL;
	}


          drv->irq = tiwlan_ext_int_init();
	  g_drv  = drv;

      // set_irq_type(drv->irq  , IRQ_TYPE_EDGE_FALLING);


/*
               rc = gpio_request(IRQ_GPIO , "wl1271_wlan_irq");
                if (rc < 0) {
                       printk("%s: failed to request GPIO %d,"
                                " error %d\n",__func__ , IRQ_GPIO , rc);
                }

                oc = gpio_direction_input(IRQ_GPIO);

                if (rc < 0) {
                        pr_err("%s: failed to configure input"
                                " direction for GPIO %d, error %d\n",
                                __func__, IRQ_GPIO , rc);
                        gpio_free(IRQ_GPIO);
                }

                drv->irq = gpio_to_irq(IRQ_GPIO);
                if (drv->irq  < 0) {
                        rc = drv->irq;
                        pr_err("%s: Unable to get irq number"
                                " for GPIO %d, error %d\n",
                                __func__,IRQ_GPIO , rc);
                        gpio_free(IRQ_GPIO);
                }
*/

#if 1
        if ((rc = request_irq(drv->irq, handle_add, IRQF_TRIGGER_FALLING, "WL1271_IRQ", drv)))
        {
            	print_err("TIWLAN: Failed to register interrupt handler\n");
                return rc;
        }
#endif



	return rc;

} /* hPlatform_initInterrupt() */

/*--------------------------------------------------------------------------------------*/

void hPlatform_freeInterrupt(void) 
{
       //free_irq(g_drv->irq,g_drv);
}

/****************************************************************************************
 *                        hPlatform_hwGetRegistersAddr()                                 
 ****************************************************************************************
DESCRIPTION:	

ARGUMENTS:		

RETURN:			

NOTES:         	
*****************************************************************************************/
void*
hPlatform_hwGetRegistersAddr(
        TI_HANDLE OsContext
        )
{
	return (void*)OS_API_REG_ADRR;
}

/****************************************************************************************
 *                        hPlatform_hwGetMemoryAddr()                                 
 ****************************************************************************************
DESCRIPTION:	

ARGUMENTS:		

RETURN:			

NOTES:         	
*****************************************************************************************/
void*
hPlatform_hwGetMemoryAddr(
        TI_HANDLE OsContext
        )
{
	return (void*)OS_API_MEM_ADRR;
}


void hPlatform_Wlan_Hardware_DeInit(void)
{
 // omap_free_gpio(PMENA_GPIO);

}

#if 0/* needed for first time new host ramp*/
static void dump_omap_registers(void)
{

	printk(KERN_ERR "CLK addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC2_CLK, omap_readl( CONTROL_PADCONF_MMC2_CLK ));
	printk(KERN_ERR "DAT0 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC2_DAT0, omap_readl( CONTROL_PADCONF_MMC2_DAT0 ));
	printk(KERN_ERR "DAT2 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC2_DAT2, omap_readl( CONTROL_PADCONF_MMC2_DAT2 ));

/*
	printk(KERN_ERR "AE10 which is 0x%x= 0x%x\n", CONTROL_PADCONF_MMC3_CMD, omap_readl( CONTROL_PADCONF_MMC3_CMD ));
	printk(KERN_ERR "AC3 which is addr 0x480021D0=%x\n", omap_readl( 0x480021D0 ));

	printk(KERN_ERR "DAT0 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC3_DAT0, omap_readl( CONTROL_PADCONF_MMC3_DAT0 ));
	printk(KERN_ERR "DAT2 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC3_DAT2, omap_readl( CONTROL_PADCONF_MMC3_DAT2 ));
	printk(KERN_ERR "DAT3 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC3_DAT3, omap_readl( CONTROL_PADCONF_MMC3_DAT3 ));
	printk(KERN_ERR "DAT4 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC2_DAT4, omap_readl( CONTROL_PADCONF_MMC2_DAT4 ));
	printk(KERN_ERR "DAT6 addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC2_DAT6, omap_readl( CONTROL_PADCONF_MMC2_DAT6 ));
	printk(KERN_ERR "CAM_D1 addr 0x%x value is =%x\n", CONTROL_PADCONF_CAM_D1, omap_readl( CONTROL_PADCONF_CAM_D1 ));
	printk(KERN_ERR "MCBSP1_CLKX addr 0x%x value is =%x\n", CONTROL_PADCONF_MCBSP1_CLKX, omap_readl( CONTROL_PADCONF_MCBSP1_CLKX ));
	printk(KERN_ERR "CMD addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC3_CMD, omap_readl( CONTROL_PADCONF_MMC3_CMD ));
	printk(KERN_ERR "MCBSP1_CLKX addr 0x%x value is =%x\n", CONTROL_PADCONF_MCBSP1_CLKX, omap_readl( CONTROL_PADCONF_MCBSP1_CLKX ));
	printk(KERN_ERR "CLK MCBSP1_CLKX addr 0x%x value is =%x\n", CONTROL_PADCONF_MMC3_CLK, omap_readl( CONTROL_PADCONF_MMC3_CLK ));
    printk(KERN_ERR "0x480021E0 value is =%x\n", omap_readl( 0x480021E0 ));
	*/
	return;
}
#endif
