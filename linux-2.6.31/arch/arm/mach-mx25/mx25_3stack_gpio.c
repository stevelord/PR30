/*
 * Copyright 2008-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <mach/gpio.h>
#include "board-mx25_3stack.h"
#include "iomux.h"


/*!
 * @file mach-mx25/mx25_3stack_gpio.c
 *
 * @brief This file contains all the GPIO setup functions for the board.
 *
 * @ingroup GPIO_MX25
 */
static struct mxc_iomux_pin_cfg __initdata mxc_iomux_pins[] = {
};

static struct mxc_iomux_pin_cfg __initdata sim_iomux_pins[] = {
};

static int __initdata enable_sim = { 0 };
static int __init sim_setup(char *__unused)
{
	enable_sim = 1;
	return 1;
}

__setup("sim", sim_setup);

void gpio_charge_init(void);

/*!
 * This system-wide GPIO function initializes the pins during system startup.
 * All the statically linked device drivers should put the proper GPIO
 * initialization code inside this function. It is called by
 * \b fixup_mx25_3stack() during system startup. This function is board
 * specific.
 */
void __init mx25_3stack_gpio_init(void)
{
	int i, num = 0;
	struct mxc_iomux_pin_cfg *pin_ptr;

	for (i = 0; i < ARRAY_SIZE(mxc_iomux_pins); i++) {
		mxc_request_iomux(mxc_iomux_pins[i].pin,
				  mxc_iomux_pins[i].mux_mode);
		if (mxc_iomux_pins[i].pad_cfg)
			mxc_iomux_set_pad(mxc_iomux_pins[i].pin,
					  mxc_iomux_pins[i].pad_cfg);
		if (mxc_iomux_pins[i].in_select)
			mxc_iomux_set_input(mxc_iomux_pins[i].in_select,
					    mxc_iomux_pins[i].in_mode);
	}

	if (enable_sim) {
		pin_ptr = sim_iomux_pins;
		num = ARRAY_SIZE(sim_iomux_pins);
	}

	for (i = 0; i < num; i++) {
		mxc_request_iomux(pin_ptr[i].pin, pin_ptr[i].mux_mode);
		if (pin_ptr[i].pad_cfg)
			mxc_iomux_set_pad(pin_ptr[i].pin, pin_ptr[i].pad_cfg);
		if (pin_ptr[i].in_select)
			mxc_iomux_set_input(pin_ptr[i].in_select,
					pin_ptr[i].in_mode);
	}

	gpio_charge_init();

/* 
**  LED PIN define
*/
#define GPIO_PAD_CTL1 (PAD_CTL_DRV_NORMAL | PAD_CTL_PKE_ENABLE | \
             PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V | PAD_CTL_PUE_PULL)

	/* === LED === */
	/* CSI_MCLK ROUTER_LED_R*/
	mxc_request_iomux(MX25_PIN_CSI_MCLK, MUX_CONFIG_GPIO);
	mxc_iomux_set_pad(MX25_PIN_CSI_MCLK, GPIO_PAD_CTL1);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_MCLK), "csi_mclk");
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_MCLK), 0);
	gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_MCLK), true);

	/* CSI_VSYNC ROUTER_LED_G*/
	mxc_request_iomux(MX25_PIN_CSI_VSYNC, MUX_CONFIG_GPIO);
	mxc_iomux_set_pad(MX25_PIN_CSI_VSYNC, GPIO_PAD_CTL1);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_VSYNC), "csi_vsync");
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_VSYNC), 0);
	gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_VSYNC), true);

  /* CSI_HSYNC ROUTER_LED_B*/
  mxc_request_iomux(MX25_PIN_CSI_HSYNC, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_HSYNC, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_HSYNC), "csi_hsync");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_HSYNC), 1);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_HSYNC), true);

  /* CSI_PIXCLK WiFi_LED*/
  mxc_request_iomux(MX25_PIN_CSI_PIXCLK, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_PIXCLK, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_PIXCLK), "csi_pixclk");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_PIXCLK), 0);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_PIXCLK), true);

  /* CSI_D7 POWER_LED*/          
  mxc_request_iomux(MX25_PIN_CSI_D7, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D7, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D7), "csi_d7");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D7), 1);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D7), true);


  /* CSI_D8 3G_MODULE_LED_EN */
  mxc_request_iomux(MX25_PIN_CSI_D8, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D8, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D8), "csi_d8");
#if 0
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D8), 1);
#else // original
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D8), 0);
#endif  
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D8), true);
//  gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D8), 0);
  pr_info ("3G LED EN GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_CSI_D8));


/* ----END */

#if 1
/* 
**  USB PHY
*/
	/* USB_CLK_EN*/
	mxc_request_iomux(MX25_PIN_KPP_COL0, MUX_CONFIG_GPIO);
	mxc_iomux_set_pad(MX25_PIN_KPP_COL0, GPIO_PAD_CTL1);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_COL0), "kpp_col0");
#ifdef CONFIG_USB_EHCI_ARC_H2
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_COL0), 1); // Enable USB clock
#else // original
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_COL0), 0); // Disable USB clock
#endif

	/* RST_USBH2_PHY_B*/
	mxc_request_iomux(MX25_PIN_KPP_COL1, MUX_CONFIG_GPIO);
	mxc_iomux_set_pad(MX25_PIN_KPP_COL1, GPIO_PAD_CTL1);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_COL1), "kpp_col1");
#ifdef CONFIG_USB_EHCI_ARC_H2
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_COL1), 1); // Enable USB3317
#else // original
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_COL1), 0); // Reset USB3317
#endif
#if 1
	gpio_export (IOMUX_TO_GPIO(MX25_PIN_KPP_COL1), true);
#endif
/* ----END */
#endif

#if 1
  /* 3G Module */
  /* CSI_D4 WAKE_3G_N */
  mxc_request_iomux(MX25_PIN_CSI_D4, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D4, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D4), "csi_d4");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D4), 0);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D4), true);
  //gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D4), 0);
  pr_info ("WAKE_3G_N GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_CSI_D4));

  /* CSI_D3 3G_RST_N */
	mxc_request_iomux(MX25_PIN_CSI_D3, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D3, GPIO_PAD_CTL1);  
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), "csi_d3");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), 1);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D3), true);
  //gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), 1);
  pr_info ("3G RST GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_CSI_D3));

  /* CSI_D2 3G_W_DISABLE_N */
  mxc_request_iomux(MX25_PIN_CSI_D2, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D2, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D2), "csi_d2");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D2), 1);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D2), true);
  //gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D2), 1);
  pr_info ("3G W_DISABLE GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_CSI_D2));


  /* CSI_D6 3G_PWR_EN */
  mxc_request_iomux(MX25_PIN_CSI_D6, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_CSI_D6, GPIO_PAD_CTL1);  
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D6), "csi_d6");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_CSI_D6), 1);
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_CSI_D6), true);
  //gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D6), 1);
  pr_info ("3G_PWR_EN GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_CSI_D6));


#endif



/* ----START
**  WiFi gpio
*/
/* === WiFi === */

#if 0 
  /* WLAN_EN : WiFi enable control pin (Active high) */
  mxc_request_iomux(MX25_PIN_D10, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_D10, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_D10), "d10");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D10),1);

  /* WLAN_IRQ : Interrupt from WiFi module (Active low) */
  mxc_request_iomux(MX25_PIN_D11, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_D11, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_D11), "d11");
  gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_D11));
#endif
/* WLAN_SLEEP_CLK : Output 32.768KHZ clock for Wifi sleep clock */
  mxc_request_iomux(MX25_PIN_CLKO, MUX_CONFIG_FUNC);	
  mxc_iomux_set_pad(MX25_PIN_CLKO, PAD_CTL_DRV_3_3V | PAD_CTL_DRV_MAX);

/* === Bluetooth === */

  /* BT_HOST_WAKE : input*/
  mxc_request_iomux(MX25_PIN_D12, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_D12, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_D12), "d12");
  gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_D12));

  /* BT_RESET_N : output*/
  mxc_request_iomux(MX25_PIN_D13, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_D13, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_D13), "d13");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D13),0);

  /* BT_WAKEUP : output*/
  mxc_request_iomux(MX25_PIN_D14, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_D14, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_D14), "d14");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_D14),0);



/* ----END */

#if 1
  /* SD_PWR_EN for SDHC2 */
  mxc_request_iomux(MX25_PIN_KPP_ROW3, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_KPP_ROW3, PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
			PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V);
#ifdef CONFIG_MMC_IMX_ESDHCI 
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW3), 1); // SDHC2 power on
#else
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW3), 0); // SDHC2 power off
#endif
//  gpio_export (IOMUX_TO_GPIO(MX25_PIN_KPP_ROW3), true);

  /* OLED_RESET_N : Reset OLED */	
  mxc_request_iomux(MX25_PIN_VSTBY_ACK, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_VSTBY_ACK, (PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
					   PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V) );
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_VSTBY_ACK),1); // initial value
// gpio_export (IOMUX_TO_GPIO(MX25_PIN_VSTBY_ACK), true);
#endif

}


/* ===== 2010/05/26 Add by Anchor ===== START */
#define CHARGE_DEFAULT_VALUE	0X00
/*
	Bit 3		: /ENBAT
	Bit 2		: /EN1
	Bit 1		: /EN2
	Bit 0		: /ENO
*/
/* ===== 2010/05/26 Add by Anchor ===== END */
/* ===== 2010/05/26 Add by Anchor ===== START */
void gpio_charge_init(void)
{
	/* Charger_D4 /ENBAT */
  mxc_request_iomux(MX25_PIN_KPP_ROW2, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_KPP_ROW2, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2), "kpp_row2");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2), CHARGE_DEFAULT_VALUE >> 3 & 0x01 );
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2), true);
  pr_info ("Charger ENBAT GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2));


	/* Charger_D3 /EN1 */
  mxc_request_iomux(MX25_PIN_KPP_ROW1, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_KPP_ROW1, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1), "kpp_row1");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1), CHARGE_DEFAULT_VALUE >> 2 & 0x01 );
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1), true);  
  pr_info ("Charger EN1 GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1));

  	/* Charger_D2 /EN2 */
  mxc_request_iomux(MX25_PIN_GPIO_A, MUX_CONFIG_FUNC);
  mxc_iomux_set_pad(MX25_PIN_GPIO_A, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_A), "gpio_a");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_GPIO_A), CHARGE_DEFAULT_VALUE >> 1 & 0x01 );
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_GPIO_A), true);  
  pr_info ("Charger EN2 GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_GPIO_A));

  	/* Charger_D0  /ENO */
  mxc_request_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_FUNC);
  mxc_iomux_set_pad(MX25_PIN_GPIO_C, GPIO_PAD_CTL1);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), "gpio_c");
  gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), CHARGE_DEFAULT_VALUE >> 0 & 0x01 );
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_GPIO_C), true);  
  pr_info ("Charger ENO GPIO (%d)\n", IOMUX_TO_GPIO(MX25_PIN_GPIO_C));

	/* Charger Status */
  mxc_request_iomux(MX25_PIN_A14, MUX_CONFIG_GPIO);
  mxc_iomux_set_pad(MX25_PIN_A14, GPIO_PAD_CTL1);
  //mxc_iomux_set_pad(MX25_PIN_A14, PAD_CTL_DRV_NORMAL);
  gpio_request(IOMUX_TO_GPIO(MX25_PIN_A14), "A14");
  //gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_A14), CHARGE_DEFAULT_VALUE >> 0 & 0x01 );
  gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_A14));
  gpio_export (IOMUX_TO_GPIO(MX25_PIN_A14), true);  
  pr_info ("[Anchor] Charger Status (%d)\n", IOMUX_TO_GPIO(MX25_PIN_A14));

}
EXPORT_SYMBOL(gpio_charge_init);

int get_charger_status(void)
{
	unsigned int rc = 0;

	rc = gpio_get_value(IOMUX_TO_GPIO(MX25_PIN_A14));
	//printk("[Anchor]GPIO - %d\n",rc);
	return rc;
}
EXPORT_SYMBOL(get_charger_status);

void gpio_charge_set(unsigned char setValue)
{
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2), setValue >> 3 & 0x01 );
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1), setValue >> 2 & 0x01 );
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_GPIO_A), setValue >> 1 & 0x01 );
	gpio_direction_output(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), setValue >> 0 & 0x01 );
}
EXPORT_SYMBOL(gpio_charge_set);

/* ===== 2010/05/26 Add by Anchor ===== END */



/*!
 * Activate a UART port
 *
 * @param  port         a UART port
 * @param  no_irda      indicates if the port is used for SIR
 */
void gpio_uart_active(int port, int no_irda)
{
	/*
	 * Configure the IOMUX control registers for the UART signals
	 */
	switch (port) {
	case 0:
		/* UART 1 IOMUX Configs */
		mxc_request_iomux(MX25_PIN_UART1_RXD, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART1_TXD, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART1_RTS, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART1_CTS, MUX_CONFIG_FUNC);
		mxc_iomux_set_pad(MX25_PIN_UART1_RXD,
				  PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
		mxc_iomux_set_pad(MX25_PIN_UART1_TXD,
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);
		mxc_iomux_set_pad(MX25_PIN_UART1_RTS,
				  PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
		mxc_iomux_set_pad(MX25_PIN_UART1_CTS,
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);

		break;
	case 1:
		/* UART 2 IOMUX Configs */
		mxc_request_iomux(MX25_PIN_UART2_RXD, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART2_TXD, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART2_RTS, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_UART2_CTS, MUX_CONFIG_FUNC);
		mxc_iomux_set_pad(MX25_PIN_UART2_RXD,
				  PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
		mxc_iomux_set_pad(MX25_PIN_UART2_TXD, PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);
		mxc_iomux_set_pad(MX25_PIN_UART2_RTS,
				  PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PU);
		mxc_iomux_set_pad(MX25_PIN_UART2_CTS, PAD_CTL_PKE_ENABLE |
				  PAD_CTL_PUE_PUD | PAD_CTL_100K_PD);
		break;
	case 2:
		/* UART 3 IOMUX Configs */
		mxc_request_iomux(MX25_PIN_KPP_ROW0, MUX_CONFIG_ALT1); /*RXD*/
		mxc_request_iomux(MX25_PIN_KPP_ROW1, MUX_CONFIG_ALT1); /*TXD*/
		mxc_request_iomux(MX25_PIN_KPP_ROW2, MUX_CONFIG_ALT1); /*RTS*/
		mxc_request_iomux(MX25_PIN_KPP_ROW3, MUX_CONFIG_ALT1); /*CTS*/

		mxc_iomux_set_input(MUX_IN_UART3_IPP_UART_RTS_B,
				    INPUT_CTL_PATH1);
		mxc_iomux_set_input(MUX_IN_UART3_IPP_UART_RXD_MUX,
				    INPUT_CTL_PATH1);
		break;
	case 3:
		/* UART 4 IOMUX Configs */
		mxc_request_iomux(MX25_PIN_LD8, MUX_CONFIG_ALT2); /*RXD*/
		mxc_request_iomux(MX25_PIN_LD9, MUX_CONFIG_ALT2); /*TXD*/
		mxc_request_iomux(MX25_PIN_LD10, MUX_CONFIG_ALT2); /*RTS*/
		mxc_request_iomux(MX25_PIN_LD11, MUX_CONFIG_ALT2); /*CTS*/

		mxc_iomux_set_input(MUX_IN_UART4_IPP_UART_RTS_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_UART4_IPP_UART_RXD_MUX,
				    INPUT_CTL_PATH0);
	case 4:

    #if 0
		/* UART 5 IOMUX Configs */
		mxc_request_iomux(MX25_PIN_CSI_D2, MUX_CONFIG_ALT1); /*RXD*/
		mxc_request_iomux(MX25_PIN_CSI_D3, MUX_CONFIG_ALT1); /*TXD*/
		mxc_request_iomux(MX25_PIN_CSI_D4, MUX_CONFIG_ALT1); /*RTS*/
		mxc_request_iomux(MX25_PIN_CSI_D5, MUX_CONFIG_ALT1); /*CTS*/

		mxc_iomux_set_input(MUX_IN_UART5_IPP_UART_RTS_B,
				    INPUT_CTL_PATH1);
		mxc_iomux_set_input(MUX_IN_UART5_IPP_UART_RXD_MUX,
				    INPUT_CTL_PATH1);
    #endif
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_uart_active);

/*!
 * Inactivate a UART port
 *
 * @param  port         a UART port
 * @param  no_irda      indicates if the port is used for SIR
 */
void gpio_uart_inactive(int port, int no_irda)
{
	switch (port) {
	case 0:
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART1_RXD), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART1_TXD), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART1_RTS), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART1_CTS), NULL);

		mxc_free_iomux(MX25_PIN_UART1_RXD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART1_TXD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART1_RTS, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART1_CTS, MUX_CONFIG_GPIO);
		break;
	case 1:
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART2_RXD), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART2_TXD), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART2_RTS), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART2_CTS), NULL);

		mxc_free_iomux(MX25_PIN_UART2_RXD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART2_TXD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART2_RTS, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART2_CTS, MUX_CONFIG_GPIO);
		break;
	case 2:
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW0), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW1), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW2), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_KPP_ROW3), NULL);

		mxc_free_iomux(MX25_PIN_KPP_ROW0, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_KPP_ROW1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_KPP_ROW2, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_KPP_ROW3, MUX_CONFIG_GPIO);
		break;
	case 3:
		mxc_free_iomux(MX25_PIN_LD8, MUX_CONFIG_FUNC);
		mxc_free_iomux(MX25_PIN_LD9, MUX_CONFIG_FUNC);
		mxc_free_iomux(MX25_PIN_LD10, MUX_CONFIG_FUNC);
		mxc_free_iomux(MX25_PIN_LD11, MUX_CONFIG_FUNC);
		break;
	case 4:
    #if 0
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D2), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D4), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D5), NULL);

		mxc_free_iomux(MX25_PIN_CSI_D2, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D3, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D4, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D5, MUX_CONFIG_GPIO);
  #endif
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_uart_inactive);

/*!
 * Configure the IOMUX GPR register to receive shared SDMA UART events
 *
 * @param  port         a UART port
 */
void config_uartdma_event(int port)
{
}
EXPORT_SYMBOL(config_uartdma_event);
/*!
 * Activate an I2C device
 *
 * @param  i2c_num         an I2C device
 */
void gpio_i2c_active(int i2c_num)
{
#define I2C_PAD_CTL (PAD_CTL_HYS_SCHMITZ | PAD_CTL_PKE_ENABLE | \
		     PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_ODE_OpenDrain)

	switch (i2c_num) {
	case 0:
		/*I2C1*/
		mxc_request_iomux(MX25_PIN_I2C1_CLK, MUX_CONFIG_SION);
		mxc_request_iomux(MX25_PIN_I2C1_DAT, MUX_CONFIG_SION);
		mxc_iomux_set_pad(MX25_PIN_I2C1_CLK, I2C_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_I2C1_DAT, I2C_PAD_CTL);
		break;
	case 1:
		/*I2C2*/
    
/* 
** Change I2C2 pin from GPIO_C and CPIO_D to FEC_RDATA1 and FEC_RX_DV  
*/

#if 0    
		mxc_request_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_ALT2); /*SCL*/
		mxc_request_iomux(MX25_PIN_GPIO_D, MUX_CONFIG_ALT2); /*SDA*/
		mxc_iomux_set_pad(MX25_PIN_GPIO_C, I2C_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_GPIO_D, I2C_PAD_CTL);
		mxc_iomux_set_input(MUX_IN_I2C2_IPP_SCL_IN, INPUT_CTL_PATH1);
		mxc_iomux_set_input(MUX_IN_I2C2_IPP_SDA_IN, INPUT_CTL_PATH1);
#else
		/* Or use FEC pins if it is not used */
		mxc_request_iomux(MX25_PIN_FEC_RDATA1, MUX_CONFIG_ALT1); /*SCL*/
		mxc_request_iomux(MX25_PIN_FEC_RX_DV, MUX_CONFIG_ALT1); /*SDA*/
		mxc_iomux_set_pad(MX25_PIN_FEC_RDATA1, I2C_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_FEC_RX_DV, I2C_PAD_CTL);
		mxc_iomux_set_input(MUX_IN_I2C2_IPP_SCL_IN, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_I2C2_IPP_SDA_IN, INPUT_CTL_PATH0);
#endif

/* ----END */

		break;
	case 2:
		/*I2C3*/
		mxc_request_iomux(MX25_PIN_HSYNC, MUX_CONFIG_ALT2); /*SCL*/
		mxc_request_iomux(MX25_PIN_VSYNC, MUX_CONFIG_ALT2); /*SDA*/
		mxc_iomux_set_pad(MX25_PIN_HSYNC, I2C_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_VSYNC, I2C_PAD_CTL);
		mxc_iomux_set_input(MUX_IN_I2C3_IPP_SCL_IN, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_I2C3_IPP_SDA_IN, INPUT_CTL_PATH0);
		break;
	default:
		break;
	}
#undef I2C_PAD_CTL
}
EXPORT_SYMBOL(gpio_i2c_active);

/*!
 * Inactivate an I2C device
 *
 * @param  i2c_num         an I2C device
 */
void gpio_i2c_inactive(int i2c_num)
{
	switch (i2c_num) {
	case 0:
		/*I2C1*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_I2C1_CLK), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_I2C1_DAT), NULL);
		mxc_free_iomux(MX25_PIN_I2C1_CLK, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_I2C1_DAT, MUX_CONFIG_GPIO);
		break;
	case 1:
		/*I2C2*/

/*
** Change I2C2 pin from GPIO_C and CPIO_D to FEC_RDATA1 and FEC_RX_DV  
*/  

#if 0
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_D), NULL);
		mxc_free_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_GPIO_D, MUX_CONFIG_GPIO);
#else
		/* Or use FEC pins if not in use */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_RDATA1), NULL); /*SCL*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_RX_DV), NULL); /*SDA*/
		mxc_free_iomux(MX25_PIN_FEC_RDATA1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_FEC_RX_DV, MUX_CONFIG_GPIO);
#endif

/* ----END */

		break;
	case 2:
		/*I2C3*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_HSYNC), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_VSYNC), NULL);
		mxc_free_iomux(MX25_PIN_HSYNC, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_VSYNC, MUX_CONFIG_GPIO);
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_i2c_inactive);

/*!
 * Activate a CSPI device
 *
 * @param  cspi_mod         a CSPI device
 */
void gpio_spi_active(int cspi_mod)
{
#define SPI_PAD_CTL1 (PAD_CTL_HYS_SCHMITZ|PAD_CTL_PKE_ENABLE| \
		      PAD_CTL_100K_PU)
#define SPI_PAD_CTL2 (PAD_CTL_HYS_SCHMITZ|PAD_CTL_PKE_ENABLE| \
		      PAD_CTL_PUE_PUD|PAD_CTL_100K_PU)

	switch (cspi_mod) {
	case 0:
		/* SPI1 */
		mxc_request_iomux(MX25_PIN_CSPI1_MOSI, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_CSPI1_MISO, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_CSPI1_SS0, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_CSPI1_SS1, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_CSPI1_SCLK, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_CSPI1_RDY, MUX_CONFIG_FUNC);
#ifndef CONFIG_CAN_FLEXCAN	/* MX25 3-stack uses this pin for CAN2 */
		mxc_request_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_ALT5); /*SS2*/
#endif
		mxc_request_iomux(MX25_PIN_VSTBY_ACK, MUX_CONFIG_ALT2); /*SS3*/

		/* Or if VSTBY_ACK is being used */
		/*mxc_request_iomux(MX25_PIN_NF_CE0, MUX_CONFIG_ALT1);*/ /*SS3*/

		mxc_iomux_set_pad(MX25_PIN_CSPI1_MOSI, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CSPI1_MISO, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CSPI1_SS0, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CSPI1_SS1, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CSPI1_SCLK, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CSPI1_RDY, SPI_PAD_CTL1);
#ifndef CONFIG_CAN_FLEXCAN	/* MX25 3-stack uses this pin for CAN2 */
		mxc_iomux_set_pad(MX25_PIN_GPIO_C, SPI_PAD_CTL2);
#endif
		mxc_iomux_set_pad(MX25_PIN_VSTBY_ACK, SPI_PAD_CTL1);

		mxc_iomux_set_input(MUX_IN_CSPI1_IPP_IND_SS3_B,
				    INPUT_CTL_PATH1);
		break;
	case 1:

    #if 0
		/* SPI2 */
		mxc_request_iomux(MX25_PIN_LD12, MUX_CONFIG_ALT2); /*MOSI*/
		mxc_request_iomux(MX25_PIN_LD13, MUX_CONFIG_ALT2); /*MISO*/
		mxc_request_iomux(MX25_PIN_LD14, MUX_CONFIG_ALT2); /*SCLK*/
		mxc_request_iomux(MX25_PIN_LD15, MUX_CONFIG_ALT2); /*RDY*/
		mxc_request_iomux(MX25_PIN_OE_ACD, MUX_CONFIG_ALT2); /*SS0*/
		mxc_request_iomux(MX25_PIN_CONTRAST, MUX_CONFIG_ALT2); /*SS1*/
		mxc_request_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_ALT7); /*SS2*/
		mxc_request_iomux(MX25_PIN_UART2_RTS, MUX_CONFIG_ALT6); /*SS3*/

		mxc_iomux_set_pad(MX25_PIN_LD12, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_LD13, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_LD14, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_LD15, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_OE_ACD, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_CONTRAST, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_GPIO_C, SPI_PAD_CTL2);
		mxc_iomux_set_pad(MX25_PIN_UART2_RTS, SPI_PAD_CTL2);

		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_CSPI_CLK_IN,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_IND_DATAREADY_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_IND_MISO, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_IND_MOSI, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_IND_SS0_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI2_IPP_IND_SS1_B,
				    INPUT_CTL_PATH0);
    #endif
    
		break;
	case 2:

    #if 0
		/* SPI3 */
		mxc_request_iomux(MX25_PIN_EB0, MUX_CONFIG_ALT6); /*SS0*/
		mxc_request_iomux(MX25_PIN_EB1, MUX_CONFIG_ALT6); /*SS1*/
		mxc_request_iomux(MX25_PIN_CS4, MUX_CONFIG_ALT6); /*MOSI*/
		mxc_request_iomux(MX25_PIN_CS5, MUX_CONFIG_ALT6); /*MISO*/
		mxc_request_iomux(MX25_PIN_ECB, MUX_CONFIG_ALT6); /*SCLK*/
		mxc_request_iomux(MX25_PIN_LBA, MUX_CONFIG_ALT6); /*RDY*/
		mxc_request_iomux(MX25_PIN_GPIO_D, MUX_CONFIG_ALT7); /*SS2*/
		mxc_request_iomux(MX25_PIN_CSI_D9, MUX_CONFIG_ALT7); /*SS3*/

		mxc_iomux_set_pad(MX25_PIN_EB0, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_EB1, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CS4, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_CS5, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_ECB, SPI_PAD_CTL1);
		mxc_iomux_set_pad(MX25_PIN_LBA, SPI_PAD_CTL1);

		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_CSPI_CLK_IN,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_DATAREADY_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_MISO, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_MOSI, INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_SS0_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_SS1_B,
				    INPUT_CTL_PATH0);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_SS2_B,
				    INPUT_CTL_PATH1);
		mxc_iomux_set_input(MUX_IN_CSPI3_IPP_IND_SS3_B,
				    INPUT_CTL_PATH0);
    #endif
		break;
	default:
		break;
	}
#undef SPI_PAD_CTL1
#undef SPI_PAD_CTL2
}
EXPORT_SYMBOL(gpio_spi_active);

/*!
 * Inactivate a CSPI device
 *
 * @param  cspi_mod         a CSPI device
 */
void gpio_spi_inactive(int cspi_mod)
{
	switch (cspi_mod) {
	case 0:
		/* SPI1 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_MOSI), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_MISO), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_SS0), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_SS1), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_SCLK), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_RDY), NULL);
#ifndef CONFIG_CAN_FLEXCAN	/* MX25 3-stack uses this pin for CAN2 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), NULL); /*SS2*/
#endif
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_VSTBY_ACK), NULL); /*SS3*/

		mxc_free_iomux(MX25_PIN_CSPI1_MOSI, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSPI1_MISO, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSPI1_SS0, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSPI1_SS1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSPI1_SCLK, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSPI1_RDY, MUX_CONFIG_GPIO);
#ifndef CONFIG_CAN_FLEXCAN	/* MX25 3-stack uses this pin for CAN2 */
		mxc_free_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_GPIO);
#endif
		mxc_free_iomux(MX25_PIN_VSTBY_ACK, MUX_CONFIG_GPIO);
		break;
	case 1:
    #if 0
		/* SPI2 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD12), NULL); /*MOSI*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD13), NULL); /*MISO*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD14), NULL); /*SCLK*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD15), NULL); /*RDY*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_OE_ACD), NULL); /*SS0*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CONTRAST), NULL); /*SS1*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_C), NULL); /*SS2*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_UART2_RTS), NULL); /*SS3*/

		mxc_free_iomux(MX25_PIN_LD12, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD13, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD14, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD15, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_OE_ACD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CONTRAST, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_GPIO_C, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_UART2_RTS, MUX_CONFIG_GPIO);
    #endif
		break;
	case 2:

    #if 0
		/* SPI3 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_EB0), NULL); /*SS0*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_EB1), NULL); /*SS1*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CS4), NULL); /*MOSI*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CS5), NULL); /*MISO*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_ECB), NULL); /*SCLK*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LBA), NULL); /*RDY*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_GPIO_D), NULL); /*SS2*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D9), NULL); /*SS3*/

		mxc_free_iomux(MX25_PIN_EB0, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_EB1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CS4, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CS5, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_ECB, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LBA, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_GPIO_D, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D9, MUX_CONFIG_GPIO);
    #endif
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_spi_inactive);

/*!
 * Activate SDHC
 *
 * @param module SDHC module number
 */
void gpio_sdhc_active(int module)
{

#if 1
#define SDHC_PAD_CTL (PAD_CTL_PKE_ENABLE | PAD_CTL_PUE_PUD | \
		      PAD_CTL_47K_PU | PAD_CTL_SRE_FAST | PAD_CTL_DRV_1_8V )

#else  
#define SDHC_PAD_CTL (PAD_CTL_PKE_ENABLE | PAD_CTL_PUE_PUD | \
		      PAD_CTL_47K_PU | PAD_CTL_SRE_FAST)
#endif 
	switch (module) {
	case 0:
		/* SDHC1 */
		mxc_request_iomux(MX25_PIN_SD1_CMD,
				  MUX_CONFIG_FUNC | MUX_CONFIG_SION);
		mxc_request_iomux(MX25_PIN_SD1_CLK,
				  MUX_CONFIG_FUNC | MUX_CONFIG_SION);
		mxc_request_iomux(MX25_PIN_SD1_DATA0, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_SD1_DATA1, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_SD1_DATA2, MUX_CONFIG_FUNC);
		mxc_request_iomux(MX25_PIN_SD1_DATA3, MUX_CONFIG_FUNC);
#if 1 
#else // original
		mxc_request_iomux(MX25_PIN_A14, MUX_CONFIG_ALT5); /*SD1_WP*/
		mxc_request_iomux(MX25_PIN_A15, MUX_CONFIG_ALT5); /*SD1_DET*/
#endif

		mxc_iomux_set_pad(MX25_PIN_SD1_CMD, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_SD1_CLK, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_SD1_DATA0, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_SD1_DATA1, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_SD1_DATA2, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_SD1_DATA3, SDHC_PAD_CTL);
#if 1
#else // original
		mxc_iomux_set_pad(MX25_PIN_A14, PAD_CTL_DRV_NORMAL);
		mxc_iomux_set_pad(MX25_PIN_A15, PAD_CTL_DRV_NORMAL);

		/* Set write protect and card detect gpio as inputs */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_A14), "a14");
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_A15), "a15");
		gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_A14)); /*SD1_WP*/
		gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_A15)); /*SD1_DET*/
#endif

		break;
	case 1:
		/* SDHC2 */
		
#if 1
#define SDHC2_PAD_CTL (PAD_CTL_PKE_ENABLE | PAD_CTL_PUE_PUD | \
                       PAD_CTL_47K_PU | PAD_CTL_SRE_FAST| PAD_CTL_DRV_3_3V)

#define FEC_PAD_CTL1 (PAD_CTL_HYS_SCHMITZ | PAD_CTL_PUE_PUD | \
          PAD_CTL_PKE_ENABLE)
#define FEC_PAD_CTL2 (PAD_CTL_PUE_PUD)

mxc_request_iomux(MX25_PIN_FEC_MDC,    MUX_CONFIG_ALT1 | MUX_CONFIG_SION); /*SD2_CMD*/
mxc_request_iomux(MX25_PIN_FEC_MDIO,   MUX_CONFIG_ALT1 | MUX_CONFIG_SION); /*SD2_CLK*/
mxc_request_iomux(MX25_PIN_FEC_TDATA0, MUX_CONFIG_ALT1); /*SD2_D0*/
mxc_request_iomux(MX25_PIN_FEC_TDATA1, MUX_CONFIG_ALT1); /*SD2_D1*/
mxc_request_iomux(MX25_PIN_FEC_TX_EN,  MUX_CONFIG_ALT1); /*SD2_D2*/
mxc_request_iomux(MX25_PIN_FEC_RDATA0, MUX_CONFIG_ALT1); /*SD2_D3*/

mxc_iomux_set_pad(MX25_PIN_FEC_MDC,    FEC_PAD_CTL2); /*SD2_CMD*/
mxc_iomux_set_pad(MX25_PIN_FEC_MDIO,   FEC_PAD_CTL1 | PAD_CTL_22K_PU); /*SD2_CLK*/

mxc_iomux_set_pad(MX25_PIN_FEC_TDATA0, FEC_PAD_CTL2); /*SD2_D0*/
mxc_iomux_set_pad(MX25_PIN_FEC_TDATA1, FEC_PAD_CTL2); /*SD2_D1*/
mxc_iomux_set_pad(MX25_PIN_FEC_TX_EN,  FEC_PAD_CTL2); /*SD2_D2*/
mxc_iomux_set_pad(MX25_PIN_FEC_RDATA0, FEC_PAD_CTL1); /*SD2_D3*/

mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_CMD_IN,      INPUT_CTL_PATH2);  /*SD2_CMD*/
mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_CARD_CLK_IN, INPUT_CTL_PATH2);  /*SD2_CLK*/
mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_DAT0_IN,     INPUT_CTL_PATH2);  /*SD2_D0*/
mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_DAT1_IN,     INPUT_CTL_PATH2);  /*SD2_D1*/
mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_DAT2_IN,     INPUT_CTL_PATH2);  /*SD2_D2*/
mxc_iomux_set_input(MUX_IN_ESDHC2_IPP_DAT3_IN,     INPUT_CTL_PATH2);  /*SD2_D3*/

/* Set write protect and card detect gpio as inputs */
mxc_request_iomux(MX25_PIN_ECB, MUX_CONFIG_ALT5); /*SD2_DET*/
mxc_iomux_set_pad(MX25_PIN_ECB, PAD_CTL_DRV_NORMAL); /*SD2_DET*/  
gpio_request(IOMUX_TO_GPIO(MX25_PIN_ECB), "ecb"); /*SD2_DET*/
gpio_direction_input(IOMUX_TO_GPIO(MX25_PIN_ECB)); /*SD2_DET*/

#undef FEC_PAD_CTL1
#undef FEC_PAD_CTL2
#else // original
		mxc_request_iomux(MX25_PIN_LD8,
				  MUX_CONFIG_ALT6 | MUX_CONFIG_SION); /*CMD*/
		mxc_request_iomux(MX25_PIN_LD9,
				  MUX_CONFIG_ALT6 | MUX_CONFIG_SION); /*CLK*/
		mxc_request_iomux(MX25_PIN_LD10, MUX_CONFIG_ALT6); /*DAT0*/
		mxc_request_iomux(MX25_PIN_LD11, MUX_CONFIG_ALT6); /*DAT1*/
		mxc_request_iomux(MX25_PIN_LD12, MUX_CONFIG_ALT6); /*DAT2*/
		mxc_request_iomux(MX25_PIN_LD13, MUX_CONFIG_ALT6); /*DAT3*/

		mxc_iomux_set_pad(MX25_PIN_LD8, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_LD9, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_LD10, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_LD11, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_LD12, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_LD13, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_CSI_D2, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_CSI_D3, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_CSI_D4, SDHC_PAD_CTL);
		mxc_iomux_set_pad(MX25_PIN_CSI_D5, SDHC_PAD_CTL);

#endif
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_sdhc_active);

/*!
 * Inactivate SDHC
 *
 * @param module SDHC module number
 */
void gpio_sdhc_inactive(int module)
{
	switch (module) {
	case 0:
		/* SDHC1 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_CMD), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_CLK), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_DATA0), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_DATA1), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_DATA2), NULL);
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_SD1_DATA3), NULL);

		mxc_free_iomux(MX25_PIN_SD1_CMD, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_SD1_CLK, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_SD1_DATA0, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_SD1_DATA1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_SD1_DATA2, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_SD1_DATA3, MUX_CONFIG_GPIO);
#if 1
#else
		mxc_free_iomux(MX25_PIN_A14, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_A15, MUX_CONFIG_GPIO);
#endif
		break;
	case 1:
		/* SDHC2 */		
#if 1
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_MDC),    NULL); /* SD_CMD   */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_MDIO),   NULL); /* SD_CLK   */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_TDATA0), NULL); /* SD_DATA0 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_TDATA1), NULL); /* SD_DATA1 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_TX_EN),  NULL); /* SD_DATA2 */
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_FEC_RDATA0), NULL); /* SD_DATA3 */

		mxc_free_iomux(MX25_PIN_FEC_MDC,    MUX_CONFIG_GPIO); 
		mxc_free_iomux(MX25_PIN_FEC_MDIO,   MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_FEC_TDATA0, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_FEC_TDATA1, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_FEC_TX_EN,  MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_FEC_RDATA0, MUX_CONFIG_GPIO);

#if 1 
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_ECB), NULL); /*SD2_DET*/
		mxc_free_iomux(MX25_PIN_ECB, MUX_CONFIG_GPIO); /*SD2_DET*/
#endif

#else

		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD8), NULL); /*CMD*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD9), NULL); /*CLK*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD10), NULL); /*DAT0*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD11), NULL); /*DAT1*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD12), NULL); /*DAT2*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD13), NULL); /*DAT3*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D2), NULL); /*DAT4*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), NULL); /*DAT5*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D4), NULL); /*DAT6*/
		gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSI_D5), NULL); /*DAT7*/

		mxc_free_iomux(MX25_PIN_LD8, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD9, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD10, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD11, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD12, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_LD13, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D2, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D3, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D4, MUX_CONFIG_GPIO);
		mxc_free_iomux(MX25_PIN_CSI_D5, MUX_CONFIG_GPIO);
		
#endif
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(gpio_sdhc_inactive);

/*
 * Probe for the card. If present the GPIO data would be set.
 */
unsigned int sdhc_get_card_det_status(struct device *dev)
{
	unsigned int ret = 0;
#if 0
	ret = gpio_get_value(IOMUX_TO_GPIO(MX25_PIN_A15));
#endif
	return ret;
}
EXPORT_SYMBOL(sdhc_get_card_det_status);

/*!
 * Get pin value to detect write protection
 */
int sdhc_write_protect(struct device *dev)
{
	unsigned int rc = 0;

	/*Mark by Anchor 20100701*/
	//rc = gpio_get_value(IOMUX_TO_GPIO(MX25_PIN_A14));
	return rc;
}
EXPORT_SYMBOL(sdhc_write_protect);

/*
 *  USB Host2
 *
 *  This configuration uses the on-chip FS/LS serial transceiver.
 *  USBPHY2_{DP,DM} pins are not muxed.
 *  We just need to grab USBH2_PWR, USBH2_OC and the Bluetooth/USB
 *  mux control signal.
 */
int gpio_usbh2_active(void)
{
/*
** usbh2 
*/

#define USBH2_PAD_CTL (PAD_CTL_PKE_ENABLE | PAD_CTL_PUE_PUD | \
            PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V )
    mxc_request_iomux(MX25_PIN_LD0,    MUX_CONFIG_ALT6); /*CLK*/
    mxc_request_iomux(MX25_PIN_LD1,    MUX_CONFIG_ALT6); /*DIR*/
    mxc_request_iomux(MX25_PIN_LD2,    MUX_CONFIG_ALT6); /*STP*/
    mxc_request_iomux(MX25_PIN_LD3,    MUX_CONFIG_ALT6); /*NXT*/
    mxc_request_iomux(MX25_PIN_LD4,    MUX_CONFIG_ALT6); /*DATA0*/
    mxc_request_iomux(MX25_PIN_LD5,    MUX_CONFIG_ALT6); /*DATA1*/
    mxc_request_iomux(MX25_PIN_LD6,    MUX_CONFIG_ALT6); /*DATA2*/
    mxc_request_iomux(MX25_PIN_LD7,    MUX_CONFIG_ALT6); /*DATA3*/
    mxc_request_iomux(MX25_PIN_HSYNC,  MUX_CONFIG_ALT6); /*DATA4*/
    mxc_request_iomux(MX25_PIN_VSYNC,  MUX_CONFIG_ALT6); /*DATA5*/
    mxc_request_iomux(MX25_PIN_LSCLK,  MUX_CONFIG_ALT6); /*DATA6*/
    mxc_request_iomux(MX25_PIN_OE_ACD, MUX_CONFIG_ALT6); /*DATA7*/
    
    mxc_iomux_set_pad(MX25_PIN_LD0,    USBH2_PAD_CTL); /*CLK*/
    mxc_iomux_set_pad(MX25_PIN_LD1,    USBH2_PAD_CTL); /*DIR*/
    mxc_iomux_set_pad(MX25_PIN_LD2,    USBH2_PAD_CTL); /*STP*/
    mxc_iomux_set_pad(MX25_PIN_LD3,    USBH2_PAD_CTL); /*NXT*/
    mxc_iomux_set_pad(MX25_PIN_LD4,    USBH2_PAD_CTL); /*DATA0*/
    mxc_iomux_set_pad(MX25_PIN_LD5,    USBH2_PAD_CTL); /*DATA1*/
    mxc_iomux_set_pad(MX25_PIN_LD6,    USBH2_PAD_CTL); /*DATA2*/
    mxc_iomux_set_pad(MX25_PIN_LD7,    USBH2_PAD_CTL); /*DATA3*/
    mxc_iomux_set_pad(MX25_PIN_HSYNC,  USBH2_PAD_CTL); /*DATA4*/
    mxc_iomux_set_pad(MX25_PIN_VSYNC,  USBH2_PAD_CTL); /*DATA5*/
    mxc_iomux_set_pad(MX25_PIN_LSCLK,  USBH2_PAD_CTL); /*DATA6*/
    mxc_iomux_set_pad(MX25_PIN_OE_ACD, USBH2_PAD_CTL); /*DATA7*/
#undef USBH2_PAD_CTL
/* ----END */


	return 0;
}
EXPORT_SYMBOL(gpio_usbh2_active);

void gpio_usbh2_inactive(void)
{
/*
** usbh2
*/
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD0),    NULL); /* CLK */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD1),    NULL); /* DIR */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD2),    NULL); /* STP */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD3),    NULL); /* NXT */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD4),    NULL); /* DATA0 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD5),    NULL); /* DATA1 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD6),    NULL); /* DATA2 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LD7),    NULL); /* DATA3 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_HSYNC),  NULL); /* DATA4 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_VSYNC),  NULL); /* DATA5 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_LSCLK),  NULL); /* DATA6 */
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_OE_ACD), NULL); /* DATA7 */

	mxc_free_iomux(MX25_PIN_LD0,    MUX_CONFIG_GPIO); 
	mxc_free_iomux(MX25_PIN_LD1,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD2,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD3,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD4,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD5,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD6,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LD7,    MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_HSYNC,  MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_VSYNC,  MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_LSCLK,  MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_OE_ACD, MUX_CONFIG_GPIO);
/* ----END */
}
EXPORT_SYMBOL(gpio_usbh2_inactive);

/*
 *  USB OTG UTMI
 *
 *  This configuration uses the on-chip UTMI transceiver.
 *  USBPHY1_{VBUS,DP,DM,UID,RREF} pins are not muxed.
 *  We just need to grab the USBOTG_PWR and USBOTG_OC pins.
 */
int gpio_usbotg_utmi_active(void)
{
	return 0;
}
EXPORT_SYMBOL(gpio_usbotg_utmi_active);

void gpio_usbotg_utmi_inactive(void)
{
}
EXPORT_SYMBOL(gpio_usbotg_utmi_inactive);

void sgtl5000_enable_amp(void)
{
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_D13), 1);
}
EXPORT_SYMBOL(sgtl5000_enable_amp);

/*
** ADD LED function
*/



void gpio_router_led_active(int R, int G, int B)
{
	if(R)
	        /* CSI_MCLK ROUTER_LED_R*/
	        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_MCLK), 1);
	if(G)
        	/* CSI_VSYNC ROUTER_LED_G*/
	        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_VSYNC), 1);
	if(B)
        	/* CSI_HSYNC ROUTER_LED_B*/
	        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_HSYNC), 1);
}
EXPORT_SYMBOL(gpio_router_led_active);

void gpio_router_led_inactive(void)
{
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_MCLK),  0);
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_VSYNC), 0);
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_HSYNC), 0);
}
EXPORT_SYMBOL(gpio_router_led_inactive);

void gpio_wifi_led_active(void)
{
        /* CSI_PIXCLK WiFi_LED*/
        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_PIXCLK), 1);
}
EXPORT_SYMBOL(gpio_wifi_led_active);

void gpio_wifi_led_inactive(void)
{
        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_PIXCLK), 0);
}
EXPORT_SYMBOL(gpio_wifi_led_inactive);

void gpio_power_led_active(void)
{
        /* CSI_D7 POWER_LED*/
        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D7), 1);
}
EXPORT_SYMBOL(gpio_power_led_active);

void gpio_power_led_inactive(void)
{
        gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D7), 0);
}
EXPORT_SYMBOL(gpio_power_led_inactive);

#undef GPIO_PAD_CTL1 
/* ----END */


#if 1
/*!
 * Setup GPIO for SLCDC device to be active
 *
 */
void gpio_slcdc_active(void)
{
	mxc_request_iomux(MX25_PIN_I2C1_CLK,  MUX_CONFIG_ALT6);
	mxc_request_iomux(MX25_PIN_I2C1_DAT,  MUX_CONFIG_ALT6);
	mxc_request_iomux(MX25_PIN_CSPI1_SS0, MUX_CONFIG_ALT6);
	mxc_request_iomux(MX25_PIN_CSPI1_SS1, MUX_CONFIG_ALT6);

#if 1
// PAD_CTL_22K_PU | PAD_CTL_DRV_MAX
#define	SLCDC_PAD_CTL1 (PAD_CTL_HYS_SCHMITZ	| PAD_CTL_PKE_ENABLE | \
					   PAD_CTL_PUE_PULL | PAD_CTL_47K_PU | \
					   PAD_CTL_SRE_FAST | PAD_CTL_DRV_3_3V )
#else // original
#define	SLCDC_PAD_CTL1 (PAD_CTL_HYS_SCHMITZ	| PAD_CTL_PKE_ENABLE | \
					   PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_ODE_OpenDrain | PAD_CTL_DRV_3_3V)
#endif

#define	SLCDC_PAD_CTL2 (PAD_CTL_HYS_SCHMITZ	| PAD_CTL_PKE_ENABLE | \
					   PAD_CTL_PUE_PUD | PAD_CTL_100K_PU | PAD_CTL_DRV_3_3V)

	mxc_iomux_set_pad(MX25_PIN_I2C1_CLK,  SLCDC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_I2C1_DAT,  SLCDC_PAD_CTL1);
	mxc_iomux_set_pad(MX25_PIN_CSPI1_SS0, SLCDC_PAD_CTL2);
	mxc_iomux_set_pad(MX25_PIN_CSPI1_SS1, SLCDC_PAD_CTL2);

#undef SLCDC_PAD_CTL1
#undef SLCDC_PAD_CTL2
}
EXPORT_SYMBOL(gpio_slcdc_active);

/*!
 * Setup GPIO for SLCDC device to be inactive
 *
 */
void gpio_slcdc_inactive(void)
{
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_I2C1_CLK),	NULL);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_I2C1_DAT),	NULL);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_SS0),	NULL);
	gpio_request(IOMUX_TO_GPIO(MX25_PIN_CSPI1_SS1),	NULL);

	mxc_free_iomux(MX25_PIN_I2C1_CLK,  MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_I2C1_DAT,  MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_CSPI1_SS0, MUX_CONFIG_GPIO);
	mxc_free_iomux(MX25_PIN_CSPI1_SS1, MUX_CONFIG_GPIO);
}
EXPORT_SYMBOL(gpio_slcdc_inactive);

void gpio_slcdc_reset( int level )
{
	if ( level == 0 )
		gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_VSTBY_ACK), 0);
	else  gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_VSTBY_ACK), 1);
}
EXPORT_SYMBOL(gpio_slcdc_reset);

#endif

#if 1
void gpio_usbh2_reset_pin( int level )
{
	int value = (level == 0) ? 0 : 1;

	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_KPP_COL1), value); // RST_USBH2_PHY_B
}

EXPORT_SYMBOL(gpio_usbh2_reset_pin);

void gpio_usbh2_clk_en_pin( int level )
{
	int value = (level == 0) ? 0 : 1;

	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_KPP_COL0), value); // USB_CLK_EN
}

EXPORT_SYMBOL(gpio_usbh2_clk_en_pin);
#endif

#if 1
void gpio_3g_reset( void )
{
#if 1
	// 3G_PWR_EN
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D6), 0);
	mdelay( 100 );
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D6), 1);
#else // original
	// 3G_RST_N
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), 0);
	mdelay( 5 );
	gpio_set_value(IOMUX_TO_GPIO(MX25_PIN_CSI_D3), 1);
#endif
}

EXPORT_SYMBOL(gpio_3g_reset);
#endif
