/*
 * Copyright 2004-2010 BndRich Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/**
 * 
 * @defgroup SLCDC 			SLCDC driver
 **/
/**@{*/
/**
 * @brief   slcdc driver source code for RiTdisplay P13506 OLED
 *
 * P13506 Feature
 * Panel matrix: 128x64 blue color
 * Driver IC : SOLOMON SSD1305
 * Graphic Display RAM : 132x64
 * Page Size : 8
 * Control Interface : SPI interface
 * Oscillator : Internal circuit
 * 
 * 
 * This is the basic release for SLCDC driver, which is to support RiTdisplay 
 * P13506. This driver acts as a standard character device, which can be 
 * dinamically loaded.
 * For example, you can refer to slcdctest.c file.
 *
 * Modification History:
 * 15,Dec,2003 Karen Kang
 *
 * 21,Feb,2006 Vasanthan S
 *
 * 
 *
 * @bug
 **/

#ifndef __KERNEL__
#  define __KERNEL__
#endif


#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/pm.h>

#include "../console/fbcon.h"
#include <linux/mempool.h>
#include <linux/timer.h>

#include <asm/bitops.h>
#include <linux/io.h>
#include <asm/uaccess.h>	/* get_user,copy_to_user */

#include <mach/hardware.h>
#include <mach/irqs.h>

#include <asm/page.h>
#include <asm/pgtable.h>

#ifdef CONFIG_ARCH_MX2ADS
#include <asm/arch/mx2.h>
#endif

#define MODULE_NAME "slcdc"






#ifdef DBMX_DEBUG

#define TRACE(fmt, args...) \
	{ \
		printk(fmt, ## args);\
		printk("\n"); \
	}

#else
#define TRACE(fmt, args...)
#endif


#define FAILED(fmt, args...) \
	{ \
		printk("%s:%d:%s\n",__FILE__, __LINE__,__FUNCTION__); \
		printk(fmt, ## args);\
		printk("\n"); \
	}

#define INFO(fmt, args...) \
	{ \
		printk(fmt, ## args);\
		printk("\n"); \
	}


/*@brief definition for SLCDC_LCD_TXCONFIG */
#define SLCDC_IMG_8L	0x00020000

/*@name SLCDC connection configuration
 *@brief config macro for serial mode or parallel mode. SLCDC can operate in
 *       serial or parallel mode. This macro reflects the hardware configuration
 *       and is not a software configuration. 
 */
/**@{*/
#define SLCDC_SERIAL_MODE
/**@}*/

/*@name SLCDC IO MUX configuration
 *@brief configuration macro for the pin mux detail. This tells which pins are 
 *       configured for SLCDC. For more information refer processor data sheet.
 *       Most cases only one of the following macros should be enabled.
 */
/**@{*/

#define BR_FEATURE_LCD_VERTICAL_FLIP
//#define BR_FEATURE_LCD_HORIZONTAL_FLIP
#define BR_FEATURE_LCD_FORMAT_CONVERT // Convert from share memory to LCD frame buffer for 1 bpp
#define BR_FEATURE_SMEM_BPP	8  // 1
#define BR_FEATURE_LCD_USE_DMA

/**@}*/

/*@brief configuration macro which tells whether the interrupt should be used
 *        or not for SLCDC. SLCDC will provide an interrupt on a completed 
 *        transfer, which can be used for refreshing the data.
 */
#define USING_INTERRUPT_SLCDC

// definition for SOLOMON SSD1305
#define SLCDC_CMD_DISON			0xAF	/*!< 1. display on */
#define SLCDC_CMD_DISOFF		0xAE	/*!< 2. display off */
#define SLCDC_CMD_DISNOR		0xA6	/*!< 3. normal display */
#define SLCDC_CMD_DISINV		0xA7	/*!< 4. inverse display */
#define SLCDC_CMD_SD_PSET		0xB0	/*!< 12.page address set */
#define SLCDC_CMD_SD_CSET		0x04	/*!< 14.column address set */


#define SLCDC_IRQ	            MXC_INT_SLCDC

#define SLCDC_CMD_MEM_SIZE		4
#define _SLCDC_CMD_MEM_SIZE_		(SLCDC_CMD_MEM_SIZE+4)

#define SLCDC_WIDTH			128
#define SLCDC_HIGH			64
#define SLCDC_BPP			1
#define SLCDC_PAGE_NUM			(SLCDC_HIGH*SLCDC_BPP/8)

#define GDRAM_WIDTH			132
#define GDRAM_HIGH			64

#define SLCDC_WIDTH_DELTA		(GDRAM_WIDTH-SLCDC_WIDTH)


#define SLCDC_PIXEL_MEM_SIZE	(SLCDC_WIDTH*SLCDC_HIGH*SLCDC_BPP/8)
#define _SLCDC_DATA_SIZE_		(SLCDC_PIXEL_MEM_SIZE + 32)

#define SMEM_PIXEL_MEM_SIZE	(SLCDC_WIDTH*SLCDC_HIGH*BR_FEATURE_SMEM_BPP/8)


//bit mask definition in STAT/CTRL register
#define SLCDC_TRANSFER_BUSY		0x4
#define SLCDC_TRANSFER_ERROR	0x10

//<<<<<< Global Variable
#if 1
static struct clk *slcdc_clk;

#ifdef BR_FEATURE_LCD_USE_DMA
u8 *g_slcdc_org_dbuffer_address = NULL;
u8 *g_slcdc_org_dbuffer_phyaddress = NULL;

u8 *g_slcdc_dbuffer_address = NULL;  /*used for SLCDC data buffer */

u8 *g_slcdc_dbuffer_phyaddress = NULL;  /* physical address for SLCDC data buffer*/
#endif

u8 *g_slcdc_org_cbuffer_address = NULL;
u8 *g_slcdc_org_cbuffer_phyaddress = NULL;

u8 *g_slcdc_cbuffer_address = NULL;  /* used for SLCDC command buffer */

u8 *g_slcdc_cbuffer_phyaddress = NULL;  /* physical address for SLCDC command buffer */
#else // original
/*used for SLCDC data buffer */
__attribute__ ((aligned(4)))
u16 *g_slcdc_dbuffer_address;

/* physical address for SLCDC data buffer*/
__attribute__ ((aligned(4)))
u16 *g_slcdc_dbuffer_phyaddress;

/* used for SLCDC command buffer */
__attribute__ ((aligned(4)))
u16 *g_slcdc_cbuffer_address;

/* physical address for SLCDC command buffer */
__attribute__ ((aligned(4)))
u16 *g_slcdc_cbuffer_phyaddress;
#endif

//static wait_queue_head_t slcdc_wait;

extern void gpio_slcdc_reset( int level );
extern void gpio_slcdc_active( void );
extern void gpio_slcdc_inactive( void );


int slcdc_open(struct inode *inode, struct file *filp);
int slcdc_release(struct inode *inode, struct file *filp);

static void __init _init_fbinfo(struct fb_info *info);

//>>>>>> Global Variable


/**@brief Local LCD controller parameters*/
struct slcdcfb_par {
	unsigned long screen_memory_size;	/*!< Screen memory size */
	unsigned int palette_size;	/*!<Palette size */
	unsigned int max_xres;	/*!<Maximum x resolution */
	unsigned int max_yres;	/*!<Maximum x resolution */
	unsigned int xres;	/*!<X resolution */
	unsigned int yres;	/*!<Y resolution */
	unsigned int xres_virtual;	/*!<Vitual x resolution */
	unsigned int yres_virtual;	/*!<Vitual y resolution */
	unsigned int max_bpp;	/*!<Maximum bit per pixel */
	unsigned int bits_per_pixel;	/*!<Bits per pixel */
	unsigned int currcon;	/*!<Current console ID */
	unsigned int visual;	/*!<Vitual color type */
	unsigned int TFT:1;	/*!<TFT flag */
	unsigned int color:1;	/*!<Color flag */
	unsigned int sharp:1;	/*!< Sharp LCD flag */
};

/* Frame buffer device API */
static int slcdcfb_set_var(struct fb_info *info);

#if 0
/* perform fb specific mmap */
static int slcdcfb_mmap(struct fb_info *info, struct vm_area_struct *vma);
/* perform fb specific ioctl (optional) */
static int slcdcfb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg);
#endif

#if 0
static void mx2fb_init_pm_reg8(struct platform_device *pdev);
static void mx2fb_exit_pm_reg8(struct platform_device *pdev);
static int mx2fb_enable_pm_reg8( void );
static int mx2fb_disable_pm_reg8( void );
#endif

static int mx2fb_pan_display(struct fb_var_screeninfo *var,
			     struct fb_info *info);

static int mx2fb_remove(struct platform_device *pdev);
static int mx2fb_probe(struct platform_device *pdev);
static void mx2fb_update_lcdc(struct fb_info *info);	
static void slcdc_timer_start( void );
static void slcdc_timer_func(unsigned long args);

#ifdef CONFIG_PM
static int mx2fb_suspend(struct platform_device *pdev, pm_message_t state);
static int mx2fb_resume(struct platform_device *pdev);
#else
#define mx2fb_suspend	0
#define mx2fb_resume	0
#endif


static int slcdcfb_open(struct fb_info *info, int user);
static int slcdcfb_release(struct fb_info *info, int user);

static int _check_var(struct fb_var_screeninfo *var, struct fb_info *info);
static void _set_fix(struct fb_info *info);
static int mx2fb_set_par(struct fb_info *info);
static int slcdcfb_blank(int blank, struct fb_info *info);

/*
 *  Framebuffer file operations
 */
static struct fb_ops slcdcfb_ops = {
	.owner = THIS_MODULE,
	.fb_open = slcdcfb_open,
	.fb_release = slcdcfb_release,
	.fb_check_var = _check_var,
	.fb_set_par = mx2fb_set_par,
	.fb_blank = slcdcfb_blank,
	.fb_pan_display = mx2fb_pan_display,
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea = cfb_copyarea,
	.fb_imageblit = cfb_imageblit,
#if 1
#else
	.fb_ioctl = slcdcfb_ioctl,
	.fb_mmap = slcdcfb_mmap,
#endif
};

/*!
 * @brief This structure contains pointers to the power management
 * callback functions.
 */
static struct platform_driver mx2fb_driver = {
	.driver = {
		   .name =  MODULE_NAME,
		   .owner = THIS_MODULE,
		   .bus = &platform_bus_type,
		   },
	.probe = mx2fb_probe,
	.remove = mx2fb_remove,
	.suspend = mx2fb_suspend,
	.resume = mx2fb_resume,
};

static u8 slcdc_brightness_level = 128;

static struct slcdcfb_par current_par;
static struct fb_info slcdc_fb_info = {.par = &current_par};


#define MIN_XRES        64
#define MIN_YRES        64

#define SLCDC_REFRESH_RATE (HZ / 20)   // (HZ / 100)

static struct timer_list slcdc_timer;
//static int start_fb_timer_flag = 0;	//0 is stop, 1 is start

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
#define BOOT_PROGRESS_BAR_WIDTH	3
#define BOOT_PROGRESS_BAR_START	56

#define BOOT_PROGRESS_TOTAL_TIME	23   // sec
#define BOOT_PROGRESS_STEP_SIZE	4

#define cal_boot_progress_time_duration( step_size, remain_time )  (HZ * step_size * remain_time / 100)
#define cal_boot_progress_remain_time( total_time, cur_percent )   ((100-cur_percent) * total_time / 100)

static int   boot_progress_phase = 1;
static long boot_progress_cur_percent = 0;
static long boot_progress_toal_time= BOOT_PROGRESS_TOTAL_TIME;
static long boot_progress_time_duration = SLCDC_REFRESH_RATE;

static void fill_bootup_progress_bar( u8 *data_buffer, long percent , long start_y, long bar_width );
#endif

#ifdef CONFIG_LOGO
#include <video/bmp_layout.h>

static unsigned char cmap_1bpp[2]; /* for 1bpp */

static int fill_bitmap_on_lcd(ulong bmp_image, int x, int y);

extern int get_bootup_logo_num( void );
extern char *get_bootup_logo_ptr( int logo_index );
#endif

#if 0
static const char fb_power_oled[] = "REG8_OLED";
static struct regulator *regulator_oled;
#endif

void slcdc_delay(int num)
{
	udelay(num);
}

/**
 *@brief slcdc gpio configure routine for serial mode
 *
 * Function Name: slcdc_gpio_serial
 *
 *
 * Description:This routine will implement gpio configurations for serial mode 
 *             both for\n LCDC mux and SDHC2 mux, you can use macro 
 *             SLCDC_MUX_SSI3_SLCDC2 or SLCDC_MUX_SD_SLCDC1 \n to choose the 
 *             right way according to your hardware configuration.
 *
 *
 *@return		None
 *
 * Modification History:
 * 	Dec,2003			Karen update for MX21 TO2 
 *  Jun,2004			Shirley update for LCDC mux
 *  Mar,2006            Update for MX27 mux 
 **/

void slcdc_gpio_serial(void)
{
	TRACE("%s()",__FUNCTION__);

	gpio_slcdc_active();
}

void slcdc_reset(int level)
{
	TRACE("%s(): level=%d",__FUNCTION__, level );

	// Reset OLED in the u-boot
//	level = 1;

	gpio_slcdc_reset( level );
}

/**
 *@brief slcdc hardware initialization
 *
 * Function Name: slcdc_init_dev
 *
 * Description  : This routine will enable the SLCDC and the clock for it
 *             
 **/
void slcdc_init_dev(void)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_clk = clk_get(NULL, "slcdc_clk");
	if (IS_ERR(slcdc_clk))
	{
		TRACE("%s():slcdc_clk init failure!", __FUNCTION__ );
		return;
	}

	clk_enable(slcdc_clk);
}

/**
 *@brief slcdc hardware disable
 *
 * Function Name: slcdc_exit_dev()
 *
 * Description  : This routine will disable the SLCDC and the clock for it
 *             
 **/
void slcdc_exit_dev(void)
{
	clk_disable(slcdc_clk);

	clk_put(slcdc_clk);
}

/**
 *@brief slcdc register initialization
 *
 * Function Name: slcdc_init_reg
 *
 * Description:This routine will setup the SLCDC register for first time 
 *             
 **/
void slcdc_init_reg(void)
{
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x00));	/* _reg_SLCDC_DBADDR */
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x04));	/* _reg_SLCDC_DBUF_SIZE */
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x08));	/* _reg_SLCDC_CBADDR */
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x0C));	/* _reg_SLCDC_CBUF_SIZE */
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x10));	/* _reg_SLCDC_CBUF_SSIZE */
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x14));	/* _reg_SLCDC_FIFO_CONFIG */
#if 1
	__raw_writel(SLCDC_WIDTH, IO_ADDRESS(SLCDC_BASE_ADDR + 0x18));	/* _reg_SLCDC_LCD_CONFIG */
#else // original
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x18));	/* _reg_SLCDC_LCD_CONFIG */
#endif	
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x1C));
						     /*_reg_SLCDC_LCD_TXCONFIG*/
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
	__raw_writel(0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x24));
						    /*_reg_SLCDC_LCD_CLKCONFIG*/
}

void slcdc_disable_interrupt( void )
{
	volatile unsigned long reg;

	/* disable interrupt */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
					       /*_reg_SLCDC_LCD_CTRL_STAT*/
	reg = reg & 0xffffff7f;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
}

void slcdc_enable_interrupt( void )
{
	volatile unsigned long reg;

	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
	reg |= ~0xffffff7f;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
}

/**
 *@brief slcdc initial configuration
 *
 * Function Name: slcdc_config
 *
 * Description:This routine will fist time configuration like buffer address
 *             FIFO configuration etc. 
 *             
 **/
void slcdc_config(int datlen)
{
	u32 xfrmode, sckpol, worddefcom, imgend = 0, worddefwrite = 0, worddefdat = 0;
	u32 cspol = 0x0; // SLCDC will drive LCD_CS to 0 during tranfer of data to external LCD controller
	volatile unsigned long reg;

	TRACE("%s()",__FUNCTION__);

	if (datlen == 8) {
		imgend = 0x2;	/*8-bit little endian; */
		worddefdat = 0;	/* 8-bit data */
#if 1
		worddefwrite = 0;  /* 8-bit data for LCD WRITE DATA register transfer */
#else // original
		worddefwrite = 10;
#endif
	} else if (datlen == 16) {
		imgend = 0x1;	/* 16-bit little endian; */
		worddefdat = 1;	/* 16-bit data */
		worddefwrite = 1;
	} else {
		FAILED(":invaild parameter, 8 or 16 is the value required");
	}
#if 1
	worddefcom = 0; /* 8-bit command */
#else // original
	worddefcom = 1; /* 16-bit command */
#endif

#ifdef SLCDC_SERIAL_MODE
	xfrmode = 0;		/* serial mode */
#else
	xfrmode = 1;		/* paralle mode */
#endif

	sckpol = 1;		/* Serial data will transition on the falling edge of serial clock  */

	// Therefore, Data shoud be latched by 
	// the display device on the rising edge of serial clcok.

	/* config to be little endian serial 16bit */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x1C));    /*_reg_SLCDC_LCD_TXCONFIG*/
#if 1
	reg = (imgend << 16) | (worddefdat << 4) | (worddefcom << 3) | (xfrmode << 2) | (cspol << 1) | (sckpol);
#else // original
	reg = (imgend << 16) | (worddefdat << 4) | (worddefcom << 3) | (xfrmode << 2) | (sckpol);
#endif
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x1C));

	/* printk("SLCDC_TXCONFIG = %x \n",_reg_SLCDC_LCD_TXCONFIG); */
	/* config dma setting */
	__raw_writel(5, IO_ADDRESS(SLCDC_BASE_ADDR + 0x14));	/* _reg_SLCDC_FIFO_CONFIG,
								   burst length is 4 32-bit words */
	/* config buffer address setting */
	__raw_writel((u32) g_slcdc_cbuffer_phyaddress, IO_ADDRESS(SLCDC_BASE_ADDR + 0x08));	/* _reg_SLCDC_CBADDR */

	/* config clk setting */
	// LCD_CLK = HCLK_SLCDC * DIV / 128
#if 1
	// approximate LCD_CLK = 130Mhz * 1 / 128 = 1Mhz
	__raw_writel((u32) 0x1, IO_ADDRESS(SLCDC_BASE_ADDR + 0x24));  /*_reg_SLCDC_LCD_CLKCONFIG*/
#else // original
	// approximate LCD_CLK = 26Mhz * 3 / 128 = 609Khz
	__raw_writel((u32) 0x3, IO_ADDRESS(SLCDC_BASE_ADDR + 0x24));  /*_reg_SLCDC_LCD_CLKCONFIG*/
#endif
	/* set GO 0 */
	__raw_writel((u32) 0x0, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						   /*_reg_SLCDC_LCD_CTRL_STAT*/

	slcdc_delay(5000);
}

/**
 *@brief slcdc send single command routine 
 *
 * Function Name: slcdc_send_single_cmd()
 *
 * Description:This help routine sends command to the SLCD from SLCDC
 *             
 *@return		0 on success, any other value otherwise
 **/
/* for command transfer, it is very short, will not use interrupt */
int slcdc_write_data_reg( u8 data , u8 is_cmd )
{
	u32 status;
	u32 wrtite_data = data;
	volatile unsigned long reg;

	if ( !is_cmd ) wrtite_data |= 0x10000;

	/* disable interrupt */
	slcdc_disable_interrupt();

	/* set automode 00 for command */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	reg = reg & 0x000001ff;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

	/* Direct write data register */
	__raw_writel( wrtite_data, IO_ADDRESS(SLCDC_BASE_ADDR + 0x28)); /* _reg_SLCDC_LCD_WRITE_DATA */

	/* polling for data transfer finish */

	status = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	while ((!(status & SLCDC_TRANSFER_ERROR))
	       && (status & SLCDC_TRANSFER_BUSY)) {
		status = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	}

	if (status & SLCDC_TRANSFER_ERROR) {
		TRACE("send cmd error status=0x%x \n", status);
		return 1;
	} else {
		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
		reg |= 0x40;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
		reg |= 0x20;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
		return 0;
	}
}

int slcdc_direct_write_cmd( u8 data )
{
	return slcdc_write_data_reg( data, 1 );
}

int slcdc_direct_write_data( u8 data )
{
	return slcdc_write_data_reg( data, 0 );
}


/**
 *@brief set LCD start address
 *
 * Function Name: slcdc_goto_lcd_start_point()
 *
 *
 * Description: This is set LCD start address
 *
 *
 *@return	NONE
 *   
 **/
void slcdc_goto_lcd_start_point( void )
{
	slcdc_direct_write_cmd(0xB0);  // Set page start address (PAGE0~PAGE7)
	slcdc_delay(2000);

#ifdef BR_FEATURE_LCD_HORIZONTAL_FLIP
	slcdc_direct_write_cmd(0);  // Lower column address setting
#else // original
	slcdc_direct_write_cmd(SLCDC_WIDTH_DELTA);  // Lower column address setting
#endif
	slcdc_delay(2000);
	slcdc_direct_write_cmd(0x10);  // Higher column address setting
	slcdc_delay(2000);
}

/**
 *@brief set LCD brightness function
 *
 * Function Name: mx2fb_set_brightness()
 *
 *
 * Description: This is will set LCD brightness
 *
 *@param	contrast: 0 ~ 255
 *
 *@return	NONE
 *   
 **/
void mx2fb_set_brightness(uint8_t level)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_brightness_level = level;

	slcdc_direct_write_cmd( 0x81 ); //Contrast setting
	slcdc_direct_write_cmd( level ); //Contrast value
	slcdc_delay(2000);
}

EXPORT_SYMBOL(mx2fb_set_brightness);

/**
 *@brief get LCD brightness function
 *
 * Function Name: mx2fb_get_brightness()
 *
 *
 * Description: This is will get LCD brightness
 *
 *@param	NONE
 *
 *@return	brightness level
 *   
 **/
u8 mx2fb_get_brightness( void )
{
	return slcdc_brightness_level;
}

EXPORT_SYMBOL(mx2fb_get_brightness);

/**
 *@brief clean LCD screen
 *
 * Function Name: slcdc_clean_lcd()
 *
 *
 * Description: This is function will clean LCD screen
 *
 *
 *@return	NONE
 *   
 **/
void slcdc_clean_lcd( void )
{
	u32 index;

	slcdc_goto_lcd_start_point();

	for ( index = 0 ; index < SLCDC_PIXEL_MEM_SIZE ; index++ )
	{
		slcdc_direct_write_data(  0x00 );
	}
}

/**
 *@brief use frame buffer to fill LCD function
 *
 * Function Name: slcdc_fill_lcd()
 *
 *
 * Description: This is will use frame buffer to fill LCD
 *
 *@param	data_buffer: frame buffer memory address
 *@param	high  : frame buffer high
 *@param	width : frame buffer width
 *
 *@return	NONE
 *   
 **/
void slcdc_fill_lcd( u8 *data_buffer, u16 high, u16 width )
{
	u8   target , shift;
	u16 x, y, page, page_num;
	u16 src_index;

	if ( data_buffer == NULL )	return;

	slcdc_goto_lcd_start_point();

	page_num = high / 8;

#if (BR_FEATURE_SMEM_BPP == 8)
	for ( page = 0, y =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			target = 0;

			for ( shift =0 ; shift < 8 ; shift++ )
			{
				src_index = (y+shift) * width + x;

				if ( data_buffer[src_index] != 0 )  target |= (0x01 << shift);
			}
			slcdc_direct_write_data( target );
		}
		y += 8;
	}
#elif (BR_FEATURE_SMEM_BPP == 1)
#if defined (BR_FEATURE_LCD_FORMAT_CONVERT)
	for ( page = 0, y =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			target = 0;

			for ( shift =0 ; shift < 8 ; shift++ )
			{
				src_index = (y+shift) * width + x;
				// ((data_buffer[src_index/8] >> (x%8)) & 0x01) << shift;
				target |= ((data_buffer[src_index>>3] >> (x&0x07)) & 0x01) << shift;
			}
			slcdc_direct_write_data( target );
		}
		y += 8;
	}
#else
	for ( page = 0, src_index =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			slcdc_direct_write_data(  data_buffer[src_index++] );
		}
	}
#endif // end of BR_FEATURE_LCD_FORMAT_CONVERT
#endif // end of BR_FEATURE_SMEM_BPP
}

/**
 *@brief frame buffer format convert function
 *
 * Function Name: mx2fb_format_convert()
 *
 *
 * Description: This is will convert frame buffer format to LCD format
 *
 *@param	target: target buffer
 *@param	src   : source buffer
 *@param	high  : frame buffer high
 *@param	width : frame buffer width
 *
 *@return	1 : content change
 *                  0 : content don't change
 *   
 **/
static u8 mx2fb_format_convert(u8 *target_ptr, u8 *src_ptr, u16 high, u16 width )
{
	u8   content_changed = 0;
	u8   target, shift;
	u16 x, y, page, page_num;
	u16 src_index, target_index = 0;

	if ( (target_ptr == NULL) || (src_ptr == NULL) )	
		return content_changed;

	page_num = high / 8;

#if (BR_FEATURE_SMEM_BPP == 8)
	for ( page = 0, y =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			target = 0;

			for ( shift =0 ; shift < 8 ; shift++ )
			{
				src_index = (y+shift) * width + x;

				if ( src_ptr[src_index] != 0 )  target |= (0x01 << shift);
			}

			if ( target != target_ptr[target_index] )
			{
				target_ptr[target_index] = target;
				content_changed = 1;
			}
			target_index++;
		}
		y += 8;
	}
#elif (BR_FEATURE_SMEM_BPP == 1)
#ifdef BR_FEATURE_LCD_FORMAT_CONVERT
	for ( page = 0, y =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			target = 0;

			for ( shift =0 ; shift < 8 ; shift++ )
			{
				src_index = (y+shift) * width + x;

				// ((src_ptr[src_index/8] >> (x%8)) & 0x01) << shift;
				target |= ((src_ptr[src_index>>3] >> (x&0x07)) & 0x01) << shift;
			}

			if ( target != target_ptr[target_index] )
			{
				target_ptr[target_index] = target;
				content_changed = 1;
			}
			target_index++;
		}
		y += 8;
	}
#else
	for ( page = 0, src_index =0 ; page < page_num ; page++ )
	{
		for ( x = 0 ; x < width ; x++ )
		{
			if ( target_ptr[src_index] != src_ptr[src_index] )
			{
				target_ptr[src_index] = src_ptr[src_index];
				content_changed = 1;
			}
			src_index++;
		}
	}
#endif // end of BR_FEATURE_LCD_FORMAT_CONVERT
#endif // end of BR_FEATURE_SMEM_BPP

	return content_changed;
}

/**
 *@brief slcdc send command routine 
 *
 * Function Name: slcdc_send_cmd
 *
 * Description:This help routine sends command to the SLCD from SLCDC
 *             
 *@return		0 on success, any other value otherwise
 **/
/* for command transfer, it is very short, will not use interrupt */
int slcdc_send_cmd(u32 length)
{
	u32 status;
	volatile unsigned long reg;

#if 1
	__raw_writel((u32) g_slcdc_cbuffer_phyaddress,
		     IO_ADDRESS(SLCDC_BASE_ADDR + 0x00)); 	/* _reg_SLCDC_DBADDR */
#endif

	/* disable interrupt */
	slcdc_disable_interrupt();

	/* set length */
	__raw_writel(length, IO_ADDRESS(SLCDC_BASE_ADDR + 0x04));	/* _reg_SLCDC_DBUF_SIZE */

	/* set automode 00 for command */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	reg = reg & 0x000001ff;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

	/* set GO */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	reg |= 0x1;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

	/* polling for data transfer finish */

	status = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	while ((!(status & SLCDC_TRANSFER_ERROR))
	       && (status & SLCDC_TRANSFER_BUSY)) {
		status = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
	}

	if (status & SLCDC_TRANSFER_ERROR) {
		TRACE("send cmd error status=0x%x \n", status);
		return 1;
	} else {
		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
		reg |= 0x40;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));	/* _reg_SLCDC_LCD_CTRL_STAT */
		reg |= 0x20;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
		return 0;
	}
}

/**
 *@brief slcdc isr 
 *
 * Function Name: slcdc_isr
 *
 * Description: This ISR routine takes interrupt from SLCDC and does refresh of
 *              display data if necessary
 *             
 *@return		0 on success, any other value otherwise
 **/
static irqreturn_t slcdc_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	volatile u32 reg;

	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/

	/* clear interrupt */
	reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
	reg |= 0x40;
	__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

//	wake_up_interruptible(&slcdc_wait);

//	if (start_fb_timer_flag == 1) 
	{
		/*        while((_reg_SLCDC_LCD_CTRL_STAT &0x00000004)!=0);        */
		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
		reg |= 0x40;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
		reg |= 0x20;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

		slcdc_timer_start( );

//		TRACE("%s()", __FUNCTION__ );
	}

	return IRQ_HANDLED;
}

#if 0
void slcd_fill_test_pattern( u8 *data_buffer )
{
	unsigned x , y;
	unsigned location = 0;

	if ( current_par.max_bpp == 8 )
	{
		for ( y = 0 ; y < SLCDC_HIGH ; y++)
		{
			for ( x = 0 ; x < SLCDC_WIDTH ; x++ )
			{
				if ( ((y >> 1) & 0x01) == 0 ) data_buffer[location] = 0x1;
				else data_buffer[location] = 0x0;

				location++;
			}
		}
	}
	else if ( current_par.max_bpp == 1 )
	{
		unsigned page , page_num;

		page_num = SLCDC_HIGH / 8;

		for ( page = 0 ; page < page_num ; page++)
		{
			for ( x = 0 ; x < SLCDC_WIDTH ; x++ )
			{
				data_buffer[location] = 0x33;

				location++;
			}
		}
	}
}
#endif

/**
 *@brief slcdc buffer initialization
 *
 * Function Name: slcdc_init_buffer
 *
 *
 * Description:This routine will allocate physical memory for SLCDC data and 
 *             for SLCDC command. 
 *             
 *@return		0 on success, appropriate error value on error
 **/
int slcdc_init_buffer( struct fb_info *info )
{
	TRACE("%s()", __FUNCTION__ );

#ifdef BR_FEATURE_LCD_USE_DMA
 	/* allocate data buffer for slcdc */
	g_slcdc_org_dbuffer_address =
		dma_alloc_coherent(0, (_SLCDC_DATA_SIZE_) ,	
					(dma_addr_t *) & g_slcdc_org_dbuffer_phyaddress, GFP_KERNEL | GFP_DMA);

	if (!g_slcdc_org_dbuffer_address) {
		TRACE(KERN_ERR MODULE_NAME ": not enough memory for data buffer\n");
		return -ENOMEM;
	}

	g_slcdc_dbuffer_address	= (u8 *) ((u_long) g_slcdc_org_dbuffer_address & 0xfffffff4);
	g_slcdc_dbuffer_phyaddress = (u8 *) ((u_long) g_slcdc_org_dbuffer_phyaddress & 0xfffffff4);

 	memset( g_slcdc_dbuffer_address, 0, SLCDC_PIXEL_MEM_SIZE);
#endif

	//---------------------------------------------------------------------
 	/* allocate data buffer for share memory */
	info->fix.smem_len = SMEM_PIXEL_MEM_SIZE;
	info->screen_size = SMEM_PIXEL_MEM_SIZE;

	info->screen_base =
		dma_alloc_coherent(0, info->fix.smem_len ,	
					(dma_addr_t *) &(info->fix.smem_start), GFP_KERNEL | GFP_DMA);

	if (!info->screen_base) {
		TRACE(KERN_ERR MODULE_NAME ": not enough memory for smem buffer\n");
		return -ENOMEM;
	}

#ifdef CONFIG_LOGO
	if ( get_bootup_logo_num() > 0 ) 
	{
		char *img_ptr = NULL;

		img_ptr = get_bootup_logo_ptr( 0 );
		if ( img_ptr )	fill_bitmap_on_lcd( (ulong)img_ptr , 0, 0);
	}
#else
	/* Clear the screen */
	memset((char *)info->screen_base, 0, info->fix.smem_len);
#endif

	//---------------------------------------------------------------------
	/* allocate command buffer */
	g_slcdc_org_cbuffer_address = 
		dma_alloc_coherent( info->device, (_SLCDC_CMD_MEM_SIZE_),	
						(dma_addr_t *) & g_slcdc_org_cbuffer_phyaddress, GFP_KERNEL | GFP_DMA);

	if (!g_slcdc_org_cbuffer_address) {
		TRACE(KERN_ERR MODULE_NAME ": not enough memory	for command buffer\n");
		return -ENOMEM;
	}
	g_slcdc_cbuffer_address	=
		(u8 *) ((u_long) g_slcdc_org_cbuffer_address & 0xfffffff4);
	g_slcdc_cbuffer_phyaddress =
		(u8 *) ((u_long) g_slcdc_org_cbuffer_phyaddress & 0xfffffff4);

	return 0;
}


/**
 *@brief slcdc buffer de initialization
 *
 * Function Name: slcdc_free_buffer
 *
 *
 * Description:This routine will deallocate the physical memory allocated by 
 *             slcdc_init_buffer. 
 *             
 *@return		0 on success
 **/
int slcdc_free_buffer(struct fb_info *info)
{
	TRACE("%s()",__FUNCTION__);

	/* free command buffer */
	if (g_slcdc_cbuffer_address != NULL) 
	{
		dma_free_coherent(0, (_SLCDC_CMD_MEM_SIZE_), 
					g_slcdc_org_cbuffer_address, (dma_addr_t)g_slcdc_org_cbuffer_phyaddress );
		
		g_slcdc_org_cbuffer_address = NULL;
		g_slcdc_org_cbuffer_phyaddress = NULL;
	
		g_slcdc_cbuffer_address = NULL;
		g_slcdc_cbuffer_phyaddress = NULL;
	}

#ifdef BR_FEATURE_LCD_USE_DMA
 	/* free data buffer for slcdc */
	if (g_slcdc_dbuffer_address != NULL) 
	{
		dma_free_coherent(0, (_SLCDC_DATA_SIZE_), 
					g_slcdc_org_dbuffer_address, (dma_addr_t)g_slcdc_org_dbuffer_phyaddress );
		
		g_slcdc_org_dbuffer_address = NULL;
		g_slcdc_org_dbuffer_phyaddress = NULL;

		g_slcdc_dbuffer_address = NULL;
		g_slcdc_dbuffer_phyaddress = NULL;
	}
#endif

 	/* free data buffer for share memory */
	if ( info->screen_base != NULL )
	{
		dma_free_coherent(0, info->fix.smem_len, info->screen_base,
				  (dma_addr_t) info->fix.smem_start);
			  
		info->screen_base = 0;
		info->fix.smem_start = 0;
		info->fix.smem_len = 0;
	}

	return 0;
}


#if 0
/**
 *@brief slcdc mmap function called from framebuffer
 *
 * Function Name: slcdcfb_mmap
 *
 *
 * Description: This is the memory map routine for this driver,will setup the
 *              memory map used in this driver. 
 *             
 *@return		0 on success any other value for failure
 **/
static int slcdcfb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	unsigned long page, pos;
	unsigned long start = (unsigned long)vma->vm_start;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

	TRACE("%s():size=%lu",__FUNCTION__, size );
		
	if (size > info->screen_size )
	{
		TRACE("%s():size=%lu over memory range %lu",__FUNCTION__, size, info->screen_size );
		return -EINVAL;
	}

	pos = (unsigned long)(info->screen_base);

	while (size > 0) {
		page = virt_to_phys((void *)pos);

		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		/* This is an IO map - tell maydump to skip this VMA  */
		vma->vm_flags |= VM_IO;

		if (remap_pfn_range
		    (vma, start, page >> PAGE_SHIFT, PAGE_SIZE, PAGE_SHARED))
			return -EAGAIN;
		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	return 0;
}
#endif

/**
 *@brief slcdc display-on function
 *
 * Function Name: slcdc_display_on
 *
 * Description:This helper routine will send the command to the SLCD for display
 *             on. 
 *             
 **/
void slcdc_display_on(void)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_direct_write_cmd( 0xAF ); // Display on
	slcdc_delay(2000);
}

/**
 *@brief slcdc display-off function
 *
 * Function Name: slcdc_display_off
 *
 * Description:This helper routine will send the command to the SLCD for display
 *             off. 
 *             
 **/
void slcdc_display_off(void)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_direct_write_cmd( 0xAE ); // Display off
	slcdc_delay(2000);
}

/**
 *@brief slcdc display-off function
 *
 * Function Name: slcdc_display_normal
 *
 * Description:This helper routine will send the command to the SLCD for display
 *             normal. 
 *             
 **/
void slcdc_display_normal(void)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_direct_write_cmd( 0xA6 );
	slcdc_delay(100);
}

void slcdc_config_panel(void)
{
	u8 *databuffer;

	//---------------------------------------------------
	// set data format
	slcdc_config(8);
	slcdc_reset(1);		/* pull reset signal high */
	slcdc_delay(0xffff);

	//---------------------------------------------------
	databuffer = g_slcdc_cbuffer_address;

	*databuffer = 0xAE; // Display off
	slcdc_send_cmd(1);
	slcdc_delay(2000);

	databuffer[0] = 0xAD; //Internal DC-DC on/off setting
	databuffer[1] = 0x8E; //Internal DC-DC off; external Vcc supply
	slcdc_send_cmd(2);
	slcdc_delay(2000);

#if 0
	//---------------------------------------------------
	// Power on OLED_12V 
	mx2fb_enable_pm_reg8( );
	slcdc_delay(4000);
#endif

	//---------------------------------------------------
	databuffer[0] = 0xA8; //MUX Ratio
	databuffer[1] = 0x3F; //64 duty
	slcdc_send_cmd(2);
	slcdc_delay(2000);

	databuffer[0] = 0xD3; //Set Display offset
	databuffer[1] = 0x00; //offset = 0
	slcdc_send_cmd(2);
	slcdc_delay(2000);

	databuffer[0] = 0x40; //Start line
	slcdc_send_cmd(1);
	slcdc_delay(2000);

	//---------------------------------------------------
#ifdef BR_FEATURE_LCD_HORIZONTAL_FLIP
	 //Set Segment re-map (0xA1:column address 0 is mapped to SEG0)
	databuffer[0] = 0xA0;
#else // original
	//Set Segment re-map (0xA1:column address 131 is mapped to SEG0)
	databuffer[0] = 0xA1;
#endif
	slcdc_send_cmd(1);
	slcdc_delay(2000);

#ifdef BR_FEATURE_LCD_VERTICAL_FLIP
	//Set COM Output Scan Direction (0xC0: Normal mode. Scan from COM0 to COM[N-1])
	databuffer[0] = 0xC0; 
#else // original
	//Set COM Output Scan Direction (0xC8: Remappped mode. Scan from COM[N-1] to COM0)
	databuffer[0] = 0xC8; 
#endif
	slcdc_send_cmd(1);
	slcdc_delay(2000);

	databuffer[0] = 0xA6; //Set normal/inverse display (0xA6:Normal display)
	slcdc_send_cmd(1);
	slcdc_delay(2000);
	
	databuffer[0] = 0xA4; //Set entire display on/off (0xA4:Resume to RAM content display)
	slcdc_send_cmd(1);
	slcdc_delay(2000);

	databuffer[0] = 0x81; //Contrast setting
	databuffer[1] = slcdc_brightness_level; //Contrast value
	slcdc_send_cmd(2);
	slcdc_delay(2000);

	//---------------------------------------------------
#if 1
	// MUX=64 (in 0xA8 register)
	// D = 1+0
	// Fosc=400Khz (0x90=>400Khz)
	// Ffrm=Fosc/(D*54*MUX)=400Khz/(1*54*64)=118Hz
	databuffer[0] = 0xD5; // Frame rate
	databuffer[1] = 0x90; // DCLK=Fosc/D=400Khz/1=400khz
#else // original
	// MUX=64 (in 0xA8 register)
	// D = 1+0
	// Fosc=300Khz (0x10=>300Khz)
	// Ffrm=Fosc/(D*54*MUX)=300Khz/(1*54*64)=86Hz
	databuffer[0] = 0xD5; // Frame rate
	databuffer[1] = 0x10; // DCLK=Fosc/D=300Khz/1=300khz
#endif
	slcdc_send_cmd(2);
	slcdc_delay(2000);

#if 1
	// use default value 0x00
#else // original
	databuffer[0] = 0xD8; //Mode setting
	databuffer[1] = 0x00; //Mono mode; normal power mode
	slcdc_send_cmd(2);
	slcdc_delay(2000);
#endif

	databuffer[0] = 0x20; //Set Memory Addressing Mode
	databuffer[1] = 0x00;  //0x00: Horizontal Addressing Mode
	slcdc_send_cmd(2);
	slcdc_delay(2000);

	//---------------------------------------------------
	// Set GDDRAM range
#ifdef BR_FEATURE_LCD_HORIZONTAL_FLIP
	databuffer[0] = 0x21; //Set Column address
	databuffer[1] = 0;    // Column start address
	databuffer[2] = (SLCDC_WIDTH - 1);  //Column end address
#else // original
	databuffer[0] = 0x21; //Set Column address
	databuffer[1] = SLCDC_WIDTH_DELTA;    // Column start address
	databuffer[2] = (SLCDC_WIDTH - 1)+SLCDC_WIDTH_DELTA;  //Column end address
#endif
	slcdc_send_cmd(3);
	slcdc_delay(2000);

	databuffer[0] = 0x22; //Set Page address
	databuffer[1] = 0;    // Page start address
	databuffer[2] = (SLCDC_PAGE_NUM - 1);  //Page end address
	slcdc_send_cmd(3);
	slcdc_delay(2000);

	//---------------------------------------------------
	// Clean screen
	slcdc_clean_lcd();

	//---------------------------------------------------
	databuffer[0] = 0xAF; //Display on
	slcdc_send_cmd(1);
	slcdc_delay(2000);
}

#if 0
static int slcdcfb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
	TRACE("%s()",__FUNCTION__);
	
	switch (cmd) {
	case SLCDC_CMD_DISON:
		slcdc_display_on();
		break;

	case SLCDC_CMD_DISOFF:
		slcdc_display_off();
		break;

	case SLCDC_CMD_DISNOR:
		slcdc_display_normal();
		break;

	case SLCDC_CMD_DISINV:
		slcdc_direct_write_cmd( 0xA7 ); // inverse display
		slcdc_delay(100);
		break;

	default:
		break;
	}

	return 0;
}
#endif

/*!
 * @brief Update LCDC registers
 * @param info	framebuffer information pointer
 */
static void mx2fb_update_lcdc(struct fb_info *info)
{
	volatile unsigned long reg;
	/* Causes the MPU to be a data entry mode,allowing it to serite data in the 
	   display memory. Inputting any other cmds other than NOP cancels the data 
	   entry mode. */

//	TRACE("%s()",__FUNCTION__);

	if ( info == NULL )
	{
		TRACE("%s(): info==NULL",__FUNCTION__);
		return;
	}

#ifdef BR_FEATURE_LCD_USE_DMA
	if ( !mx2fb_format_convert(g_slcdc_dbuffer_address, (u8 *)(info->screen_base), SLCDC_HIGH, SLCDC_WIDTH ) )
	{
//		if (start_fb_timer_flag == 1)
			slcdc_timer_start( );
	}
	else
	{
//		TRACE("%s(): content changed",__FUNCTION__);

		slcdc_goto_lcd_start_point();

		__raw_writel((u32) g_slcdc_dbuffer_phyaddress, IO_ADDRESS(SLCDC_BASE_ADDR + 0x00));	/* _reg_SLCDC_DBADDR */

		slcdc_delay(0xffff);

		/* enable interrupt */
#ifdef USING_INTERRUPT_SLCDC
		slcdc_enable_interrupt();
#endif
		/* set length */
		__raw_writel(SLCDC_PIXEL_MEM_SIZE, IO_ADDRESS(SLCDC_BASE_ADDR + 0x04));	/* _reg_SLCDC_DBUF_SIZE */

		/* set automode 01 for data */
		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/
		reg |= 0x00000800;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));

		/* set GO  */
		reg = __raw_readl(IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
						    /*_reg_SLCDC_LCD_CTRL_STAT*/

		reg |= 0x1;
		__raw_writel(reg, IO_ADDRESS(SLCDC_BASE_ADDR + 0x20));
		slcdc_delay(0xffff);
	}
#else // single write mode
	slcdc_fill_lcd( (u8 *)(info->screen_base), SLCDC_HIGH, SLCDC_WIDTH );

//	if (start_fb_timer_flag == 1)
		slcdc_timer_start( );
#endif
}

static void slcdc_timer_start( void )
{
	unsigned long expires = SLCDC_REFRESH_RATE;

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	if ( (boot_progress_phase) && (boot_progress_time_duration != 0) ) 
		expires = boot_progress_time_duration;
#endif

	mod_timer(&slcdc_timer, jiffies +  expires );
}

static void slcdc_timer_stop( void )
{
	del_timer( &slcdc_timer );
}

/**
 *@brief slcdc timer call back function
 *
 * Function Name: slcdc_timer_func
 *
 * Description:This helper routine prepare the SLCDC for data transfer
 *             
 **/
static void slcdc_timer_func(unsigned long args)
{
//	TRACE("%s()",__FUNCTION__);

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	if ( boot_progress_phase && (boot_progress_cur_percent < 100) )
	{
		boot_progress_cur_percent += BOOT_PROGRESS_STEP_SIZE;
		if ( boot_progress_cur_percent > 100 )
			boot_progress_cur_percent = 100;

		fill_bootup_progress_bar( (u8 *)(slcdc_fb_info.screen_base), 
				boot_progress_cur_percent, 
				BOOT_PROGRESS_BAR_START, 
				BOOT_PROGRESS_BAR_WIDTH );
	}
#endif

	mx2fb_update_lcdc( &slcdc_fb_info );
}

/**
 *@brief slcdc timer initialization 
 *
 * Function Name: init_slcdc_timer
 *
 * Description:This helper routine prepare the SLCDC timer for refreshing the
 *             screen
 *             
 **/
static void init_slcdc_timer(void)
{
	TRACE("%s()",__FUNCTION__);

	init_timer(&slcdc_timer);
#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	boot_progress_time_duration = cal_boot_progress_time_duration( BOOT_PROGRESS_STEP_SIZE, BOOT_PROGRESS_TOTAL_TIME );

	slcdc_timer.expires = jiffies + boot_progress_time_duration;
#else // original
	slcdc_timer.expires = jiffies + SLCDC_REFRESH_RATE;
#endif
	slcdc_timer.data = 0;
	slcdc_timer.function = slcdc_timer_func;
	add_timer(&slcdc_timer);
//	start_fb_timer_flag = 1;
}


/*////////////// Frame Buffer Suport /////////////////////////////////// */

/**
 * _check_var - Validates a var passed in. 
 * @var: frame buffer variable screen structure
 * @info: frame buffer structure that represents a single frame buffer 
 *
 * Checks to see if the hardware supports the state requested by var passed
 * in. This function does not alter the hardware state! If the var passed in
 * is slightly off by what the hardware can support then we alter the var
 * PASSED in to what we can do. If the hardware doesn't support mode change
 * a -EINVAL will be returned by the upper layers.
 *
 * Returns negative errno on error, or zero on success.
 */
static int _check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	const struct slcdcfb_par *par = (const struct slcdcfb_par *)info->par;

	TRACE("%s()",__FUNCTION__);

	if ( par == NULL )
	{
		TRACE("%s():par==NULL",__FUNCTION__);
		return -EINVAL;
	}

	if (var->xres > current_par.max_xres)
		var->xres = current_par.max_xres;
	if (var->yres > current_par.max_yres)
		var->yres = current_par.max_yres;

	var->xres_virtual = var->xres_virtual < par->xres
	    ? par->xres : var->xres_virtual;
	var->yres_virtual = var->yres_virtual < par->yres
	    ? par->yres : var->yres_virtual;
	var->bits_per_pixel = par->bits_per_pixel;

	switch (var->bits_per_pixel) {
#if 1
	case 1:
	case 2:
	case 4:
	case 8:
		var->grayscale  = 1;
		var->red.offset	= 0;
		var->red.length	= var->bits_per_pixel;
		var->green	= var->red;
		var->blue	= var->red;
		var->transp.length = 0;
		break;
#else
	case 2:
	case 4:
	case 8:
		var->red.length = 4;
		var->green = var->red;
		var->blue = var->red;
		var->transp.length = 0;
		break;
#endif
	case 12:		/*  RGB 444 */
	case 16:		/* RGB 565 */
		TRACE("16->a");
		var->red.length = 4;
		var->blue.length = 4;
		var->green.length = 4;
		var->transp.length = 0;
#ifdef __LITTLE_ENDIAN
		TRACE("16->b");
		var->red.offset = 8;
		var->green.offset = 4;
		var->blue.offset = 0;
		var->transp.offset = 0;
#endif				/* __LITTLE_ENDIAN */
		break;

	default:
		return -EINVAL;
	}

	/* *var->screen_start_address=(u_char*)((u_long)g_slcdc_dbuffer_phyaddress );
	 *var->v_screen_start_address=(u_char*)((u_long)g_slcdc_dbuffer_address  ); */

	var->height = -1;
	var->width = -1;
	var->nonstd = 0;

	var->pixclock = -1;
	var->left_margin = -1;
	var->right_margin = -1;
	var->upper_margin = -1;
	var->lower_margin = -1;
	var->hsync_len = -1;
	var->vsync_len = -1;

	var->vmode = FB_VMODE_NONINTERLACED;
	var->sync = 0;

	return 0;
}

/**
 *@brief Use current_par to set a var structure
 *
 *@param 	var Input var data
 *@param	par LCD controller parameters
 *
 *@return  If no error, return 0
 *
 */
static int _encode_var(struct fb_var_screeninfo *var, struct slcdcfb_par *par)
{
	TRACE("%s()",__FUNCTION__);

	/* Don't know if really want to zero var on entry.
	    Look at set_var to see.  If so, may need to add extra params to par   */

	// The slcdc_fb_info.var is important value
	// Don't reset it

	var->xres = par->xres;
	var->yres = par->yres;
	var->xres_virtual = par->xres_virtual;
	var->yres_virtual = par->yres_virtual;
	var->bits_per_pixel = par->bits_per_pixel;

	switch (var->bits_per_pixel) {
#if 1
	case 1:
	case 2:
	case 4:
	case 8:
		var->grayscale  = 1;
		var->red.offset	= 0;
		var->red.length	= var->bits_per_pixel;
		var->green	= var->red;
		var->blue	= var->red;
		var->transp.length = 0;
		break;
#else
	case 2:
	case 4:
	case 8:
		var->red.length = 4;
		var->green = var->red;
		var->blue = var->red;
		var->transp.length = 0;
		break;
#endif
	case 12:		/* This case should differ for Active/Passive mode */
	case 16:
		TRACE("16->a");
		var->red.length = 4;
		var->blue.length = 4;
		var->green.length = 4;
		var->transp.length = 0;
#ifdef __LITTLE_ENDIAN
		TRACE("16->b");
		var->red.offset = 8;
		var->green.offset = 4;
		var->blue.offset = 0;
		var->transp.offset = 0;
#endif				/* __LITTLE_ENDIAN */
		break;
	}

	return 0;
}

/**
 *@brief Get the video params out of 'var'. If a value doesn't fit, 
 * 		round it up,if it's too big, return -EINVAL.
 *
 *@warning Round up in the following order: bits_per_pixel, xres,
 * 	yres, xres_virtual, yres_virtual, xoffset, yoffset, grayscale,
 * 	bitfields, horizontal timing, vertical timing. 			
 *
 *@param 	var Input var data
 *@param	par LCD controller parameters
 *  
 *@return If no error, return 0
 */
static int mx2fb_set_par(struct fb_info *info)
{
	struct fb_var_screeninfo *var = &info->var;
	struct slcdcfb_par *par = (struct slcdcfb_par *)info->par;

	TRACE("%s()",__FUNCTION__);
	
	_set_fix( info );

	if ((par->xres = var->xres) < MIN_XRES)
		par->xres = MIN_XRES;
	if ((par->yres = var->yres) < MIN_YRES)
		par->yres = MIN_YRES;
	if (par->xres > current_par.max_xres)
		par->xres = current_par.max_xres;
	if (par->yres > current_par.max_yres)
		par->yres = current_par.max_yres;
	par->xres_virtual = var->xres_virtual < par->xres
	    ? par->xres : var->xres_virtual;
	par->yres_virtual = var->yres_virtual < par->yres
	    ? par->yres : var->yres_virtual;
	par->bits_per_pixel = var->bits_per_pixel;

	switch (par->bits_per_pixel) {
	case 1:
		par->visual = FB_VISUAL_MONO10; /* Monochr. 1=White 0=Black */
		par->palette_size = 0;
		break;

	case 4:
		par->visual = FB_VISUAL_PSEUDOCOLOR;
		par->palette_size = 16;
		break;

	case 8:
		par->visual = FB_VISUAL_PSEUDOCOLOR;
		par->palette_size = 256;
		break;

	case 12:		/* RGB 444 */
	case 16:		/* RGB 565 */
		par->visual = FB_VISUAL_TRUECOLOR;
		par->palette_size = 0;
		break;

	default:
		return -EINVAL;
	}

	/* update_lcd ? */

	return 0;
}

/**
 *@brief Set current_par by var, also set display data, specially the console 
 * 	   related file operations, then enable the SLCD controller, and set cmap to
 * 	   hardware.
 *
 *@param	var	Iuput data pointer
 *@param	con	Console ID
 *@param	info	Frame buffer information 
 * 
 *@return   If no error, return 0.
 *
 **/
static int slcdcfb_set_var(struct fb_info *info)
{
	int err;
	struct fb_var_screeninfo *var = &info->var;

	TRACE("%s()",__FUNCTION__);

	if ((err = mx2fb_set_par(info))) {
		TRACE("%s():set par error!", __FUNCTION__ );
		return err;
	}

	// Store adjusted par_var into var structure
	// The current_par adjust in mx2fb_set_par()
	_encode_var(var, info->par );

	if ((var->activate & FB_ACTIVATE_MASK) == FB_ACTIVATE_TEST)
		return 0;
	else if (((var->activate & FB_ACTIVATE_MASK) != FB_ACTIVATE_NOW) &&
		 ((var->activate & FB_ACTIVATE_MASK) != FB_ACTIVATE_NXTOPEN))
		return -EINVAL;

	return 0;
}

/**
 *@brief Blank the screen, if blank, disable LCD controller, while if no blank
 * 		set cmap and enable LCD controller
 *
 *@param	blank Blank flag
 *@param	info	Frame buffer database
 *
 *@return  VOID
 */
static int slcdcfb_blank(int blank, struct fb_info *info)
{
	TRACE("%s()",__FUNCTION__);

	if (blank) {
		slcdc_display_off();
	} else {
		slcdc_display_on();
	}

	return 0;
}

/**
 *@brief Initialize frame buffer. While 16bpp is used to store a 12 bits pixels 
 *      packet, it is not a really 16bpp system, maybe in-compatiable with
 * 		other system or GUI.There are some field in var which specify
 *		the red/green/blue offset in a 16bit word, just little endian is
 * 		concerned
 *
 *@return  VOID
 **/

/*!
 * @brief Set fixed framebuffer parameters based on variable settings.
 *
 * @param info	framebuffer information pointer
 * @return	Negative errno on error, or zero on success.
 */
static void _set_fix(struct fb_info *info)
{
	struct fb_fix_screeninfo *fix = &(info->fix);
	struct fb_var_screeninfo *var = &(info->var);

	strcpy(fix->id, "SLCDC");

	fix->line_length = var->xres_virtual * var->bits_per_pixel / 8;
	fix->type = FB_TYPE_PACKED_PIXELS;
	fix->accel = FB_ACCEL_NONE; /* No hardware acceleration */    
	fix->visual = FB_VISUAL_MONO10; /* Monochr. 1=White 0=Black */
#if 1
	fix->xpanstep = 0;
	fix->ypanstep = 0;
#else // original
	fix->xpanstep = 1;
	fix->ypanstep = 1;
#endif
	fix->ywrapstep = 0;
}

static void __init _init_fbinfo(struct fb_info *info)
{
	TRACE("%s()",__FUNCTION__);

	info->node = -1;
	info->flags = 0;	/* Low-level driver is not a module */
	info->fbops = &slcdcfb_ops;
#if 0
	info->monspecs = monspecs;
#endif

	/*
	 * * setup initial parameters
	 * */
	memset(&info->var, 0, sizeof(info->var));
	memset(&current_par, 0, sizeof(current_par));

	info->var.transp.length = 0;
	info->var.nonstd = 0;
	info->var.activate = FB_ACTIVATE_NOW;
	info->var.xoffset = 0;
	info->var.yoffset = 0;
	info->var.height = -1;
	info->var.width = -1;
	info->var.vmode = FB_VMODE_NONINTERLACED;

	info->var.grayscale = 1;
	info->var.sync = 0;
	// dotclock = GDRAM_WIDTH * GDRAM_HIGH * 60 = 132*64*60 = 506880
	// pixclock = 1/dotclock = 1972853(ps)
	info->var.pixclock = 1972853; // pico second

	/* 
	 * xres and yres might be set when loading the module, 
	 * if this driver is built as module 
	 */
	current_par.max_xres = SLCDC_WIDTH;
	current_par.max_yres = SLCDC_HIGH;
	current_par.max_bpp = BR_FEATURE_SMEM_BPP;
	
	current_par.screen_memory_size = SMEM_PIXEL_MEM_SIZE;
	current_par.currcon = -1;

	//----------------------------------------------------------------------------
	info->var.xres = current_par.max_xres;
	info->var.yres = current_par.max_yres;
	info->var.xres_virtual = info->var.xres;
	info->var.yres_virtual = info->var.yres;
	info->var.bits_per_pixel = current_par.max_bpp;

	current_par.xres = info->var.xres;
	current_par.yres = info->var.yres;
	current_par.xres_virtual = info->var.xres;
	current_par.yres_virtual = info->var.yres;
	
}

#if 0
static void mx2fb_init_pm_reg8(struct platform_device *pdev)
{
	TRACE("%s()",__FUNCTION__);

	regulator_oled = regulator_get(&pdev->dev, fb_power_oled);
}

static void mx2fb_exit_pm_reg8(struct platform_device *pdev)
{
	TRACE("%s()",__FUNCTION__);

	regulator_put( regulator_oled );
}

static int mx2fb_enable_pm_reg8( void )
{
	int ret = 0;

	TRACE("%s()",__FUNCTION__);

	if ( !IS_ERR(regulator_oled) ) 
	{
		if ( regulator_is_enabled( regulator_oled ) )
			regulator_disable( regulator_oled );

		ret = regulator_set_voltage( regulator_oled, 12000000, 12000000 );
		if ( ret < 0) // 12V
		{
			TRACE("%s:set voltage fail. result=%d", fb_power_oled, ret);
			return ret;
		}

		if ( (ret = regulator_enable(regulator_oled)) < 0)
		{
			TRACE("%s:enable fail. result=%d", fb_power_oled, ret);
			return ret;
		}
	}
	else
	{
		return -EINVAL;
	}

	return 0;
}

static int mx2fb_disable_pm_reg8( void )
{
	TRACE("%s()",__FUNCTION__);

	if ( !IS_ERR(regulator_oled) ) 
	{
		if ( regulator_is_enabled( regulator_oled ) )
			regulator_disable( regulator_oled );
	}
	else
	{
		return -EINVAL;
	}

	return 0;
}
#endif

/*!
 * @brief Pans the display.
 *
 * @param var	Frame buffer variable screen structure
 * @param info	Frame buffer structure that represents a single frame buffer
 *
 * @return	Negative errno on error, or zero on success.
 *
 * Pan (or wrap, depending on the `vmode' field) the display using the
 * 'xoffset' and 'yoffset' fields of the 'var' structure. If the values
 * don't fit, return -EINVAL.
 */
static int mx2fb_pan_display(struct fb_var_screeninfo *var,
			     struct fb_info *info)
{
	TRACE("%s()",__FUNCTION__);

	if (var->xoffset < 0 || var->yoffset < 0
	    || var->xoffset + info->var.xres > info->var.xres_virtual
	    || var->yoffset + info->var.yres > info->var.yres_virtual)
		return -EINVAL;

	info->var.xoffset = var->xoffset;
	info->var.yoffset = var->yoffset;

	mx2fb_update_lcdc(info);

	if (var->vmode & FB_VMODE_YWRAP) {
		info->var.vmode |= FB_VMODE_YWRAP;
	} else {
		info->var.vmode &= ~FB_VMODE_YWRAP;
	}

	return 0;
}


//=============================================================*/

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
static void fill_bootup_progress_bar( u8 *data_buffer, long percent , long start_y, long bar_width )
{
	long x, y, end_x;
	long end_y;
	long location = 0;

	if ( current_par.max_bpp == 8 )
	{
		end_y = start_y + bar_width;
		if ( (percent == 0 ) || (percent > 100) || (end_y > SLCDC_HIGH)) 
			return;

		end_x =  (percent * SLCDC_WIDTH) / 100;
		for ( y = start_y ; y < end_y ; y++)
		{
			location = y * SLCDC_WIDTH;
			for ( x = 0 ; x < SLCDC_WIDTH ; x++ )
			{
				if (x < end_x) data_buffer[location] = 1;
				else data_buffer[location] = 0;
					
				location++;
			}
		}
	}
}
//==========================================================

static ssize_t slcdc_show_remain_time(struct device *dev, struct device_attribute *attr, char *buf)
{
	long remain_time;
	
	remain_time = cal_boot_progress_remain_time( boot_progress_toal_time, boot_progress_cur_percent );

	return sprintf(buf, "%ld\n", remain_time );
}

static ssize_t slcdc_set_remain_time(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	long current_remain_time;
	long remain_time;

	if ( boot_progress_phase && (boot_progress_cur_percent < 100))
	{
		slcdc_timer_stop();

		if ( (remain_time = simple_strtoul(buf, NULL, 10)) == 0 )
		{
			boot_progress_cur_percent = 100;
			fill_bootup_progress_bar( (u8 *)(slcdc_fb_info.screen_base), 
					boot_progress_cur_percent, 
					BOOT_PROGRESS_BAR_START, 
					BOOT_PROGRESS_BAR_WIDTH );

			mx2fb_update_lcdc( &slcdc_fb_info );
		}
		else
		{
			current_remain_time = cal_boot_progress_remain_time( boot_progress_toal_time, boot_progress_cur_percent );

			boot_progress_toal_time = (boot_progress_toal_time - current_remain_time)
										+ remain_time;

			boot_progress_time_duration = cal_boot_progress_time_duration( BOOT_PROGRESS_STEP_SIZE, remain_time );

			slcdc_timer_start();
		}

		printk("%s() end:cur_percent=%ld, time_duration=%ld\n", __FUNCTION__, boot_progress_cur_percent, boot_progress_time_duration );
	}
	
	return count;
}

static DEVICE_ATTR(remain_time, S_IWUGO | S_IRUGO, slcdc_show_remain_time, slcdc_set_remain_time);

void slcdc_device_create_file( void )
{
	int	status = -EINVAL;
	
	status = device_create_file( slcdc_fb_info.dev, &dev_attr_remain_time);
	if (status != 0)
	{
		printk(KERN_ERR "%s():error status=%d\n", __FUNCTION__, status );
	}
}

void slcdc_device_remove_file( void )
{
	// slcdc_fb_info.device
	device_remove_file( slcdc_fb_info.dev, &dev_attr_remain_time);
}
#endif

//==========================================================
#ifdef CONFIG_LOGO
static void init_1bpp_cmap( void )
{
	unsigned i;

	for ( i = 0 ; i < 2 ; i++ )
	{
		cmap_1bpp[i] = i;
	}
}

static void update_1bpp_cmap( bmp_color_table_entry_t *p_color_table )
{
	unsigned i;
	bmp_color_table_entry_t *p_lut = p_color_table;

	for ( i = 0 ; i < 2 ; i++ )
	{
		cmap_1bpp[i] = 1 * (p_lut[i].blue+p_lut[i].green+p_lut[i].red)/(3*0xFF);
	}
}

static void fill_1bpp_img_on_8bpp_lcd(unsigned char *img_ptr, unsigned img_width, unsigned img_height,
		unsigned char *lcd_ptr, unsigned lcd_width, unsigned lcd_height )
{
	long x, img_y, lcd_y;
	long lcd_index = 0, img_index = 0;
	long lw;
	unsigned char target;

	lw = (((img_width + 7) / 8) + 3) & ~0x3;
	for (img_y = (img_height-1), lcd_y = 0; (img_y >= 0) && (lcd_y < lcd_height) ; img_y--, lcd_y++ ) 
	{
		img_index = img_y * (lw*8);
		lcd_index = lcd_y * lcd_width;
		for (x = 0; (x < img_width) && (x < lcd_width); x++) 
		{
			target = ((img_ptr[img_index>>3] >> (7-(x&7))) & 0x01);
			lcd_ptr[lcd_index] = cmap_1bpp[target];
			lcd_index++;
			img_index++;
		}
	}
}

//=============================================================================
/*
 * Display the BMP file located at address bmp_image.
 * Only uncompressed.
 */
static int fill_bitmap_on_lcd(ulong bmp_image, int x, int y)
{
	bmp_image_t *bmp;
	unsigned long width;
	unsigned long height;
	unsigned short bpp;
	unsigned long compression;
	unsigned long data_offset;
	unsigned char *dataptr;

	TRACE("%s()",__FUNCTION__);

	bmp = (bmp_image_t *) bmp_image;
	if ((bmp->header.signature[0] == 'B') &&
	    (bmp->header.signature[1] == 'M')) 
	{
		compression  = le32_to_cpu(bmp->header.compression);
		width        = le32_to_cpu(bmp->header.width);
		height       = le32_to_cpu(bmp->header.height);
		bpp          = le16_to_cpu(bmp->header.bit_count);
		data_offset  = le32_to_cpu(bmp->header.data_offset);
		dataptr = (unsigned char *) bmp + data_offset;

		TRACE("BMP: compression=%lu, width=%lu, height=%lu, bpp=%u, data_offset=%lu", 
			compression, width, height, bpp, data_offset);

		switch (bpp) 
		{
		case 1:
			if ( data_offset > sizeof( bmp->header ) )
				update_1bpp_cmap( bmp->color_table );
			fill_1bpp_img_on_8bpp_lcd(dataptr, width, height, (u8 *)(slcdc_fb_info.screen_base), SLCDC_WIDTH, SLCDC_HIGH);
			break;
		default:
			TRACE("Error: %u bit per pixel not supported by this LCD", bpp);
			return 0;
		}
	} 
	else	
	{
		TRACE("Error: no valid bmp at 0x%p", bmp);
		return 0;
	}

	return 1;
}
#endif


#ifdef CONFIG_PM
/*
 * Power management hooks. Note that we won't be called from IRQ context,
 * unlike the blank functions above, so we may sleep.
 */

/*!
 * @brief Suspends the framebuffer and blanks the screen.
 * Power management support
 */
static int mx2fb_suspend(struct platform_device *pdev, pm_message_t state)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_timer_stop();
		
	slcdc_display_off();

	slcdc_disable_interrupt();

	return 0;
}

/*!
 * @brief Resumes the framebuffer and unblanks the screen.
 * Power management support
 */
static int mx2fb_resume(struct platform_device *pdev)
{
	TRACE("%s()",__FUNCTION__);

	mx2fb_update_lcdc(&slcdc_fb_info);

	slcdc_display_on();

//	slcdc_timer_start();

	return 0;
}
#endif				/* CONFIG_PM */


/**
 *@brief slcdc close function
 *
 * Function Name: slcdc_release
 *
 *
 * Description: This is the release routine for the driver. And this function \n
 *	   will be called while the module being closed. In this function, it will\n
 *	   unregister the apm	
 *
 *@param	inode the pointer to the inode descripter
 *@param	filp  the pointer to the file descripter
 *
 *@return	int   return status
 *			@li 0  sucessful
 *			@li other failed
 * Modification History:
 * 	Dec,2003			Karen first version 
 *   
 **/

int slcdc_release(struct inode *inode, struct file *filp)
{
	TRACE("%s()",__FUNCTION__);

	slcdc_timer_stop();

#if 0
	mx2fb_disable_pm_reg8( );
#endif

	slcdc_reset(0);		/*pull reset low */

	slcdc_exit_dev();

	gpio_slcdc_inactive();

	return 0;
}

/**
 *@brief slcdc open function
 *
 * Function Name: slcdc_open
 *
 *
 * Description: This is the open routine for the driver. And this function \n
 *	   will be called while the module being opened. In this function, it will\n
 *			@li	configure GPIO for serial/parallel
 *			@li	slcdc reset
 *			@li init slcd registers 
 *			@li init waitqueue
 *			@li get rca, select the card
 *			@li send some command for panel configuration
 *
 *@param	inode the pointer to the inode descripter
 *@param	filp  the pointer to the file descripter
 *
 *@return	int   return status
 *			@li 0  sucessful
 *			@li other failed
 * Modification History:
 * 	Dec,2003,			Karen first version
 *  June,2004,			Shirley update for Parallel mode 
 *   
 **/

int slcdc_open(struct inode *inode, struct file *filp)
{
	TRACE("%s()",__FUNCTION__);

	/* init dev */
	slcdc_gpio_serial();

	slcdc_init_dev();
	slcdc_reset(0);		/*pull reset low */
	/* init slcd registers */
	slcdc_init_reg();

	/* init waitqueue */
//	init_waitqueue_head(&slcdc_wait);

	/* send some command for panel configuration */
	slcdc_config_panel();

	/* init slcdc timer, and start timer */
	init_slcdc_timer();

	return 0;
}

/**
 *@brief slcdc framebuffer open call
 *
 * Function Name: slcdcfb_open
 *
 * Description  : This function is called by the framebuffer when the 
 *                application requests. 
 *             
 *@return		0 on success any other value for failure
 **/
static int slcdcfb_open(struct fb_info *info, int user)
{
	TRACE("%s()",__FUNCTION__);

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	if ( boot_progress_phase )
	{
		boot_progress_phase = 0;

		/* Clear the screen */
		memset((char *)info->screen_base, 0, info->fix.smem_len);
	}
#endif
	slcdc_timer_start( );

	return 0;
}

/**
 *@brief slcdc framebuffer release call
 *
 * Function Name: slcdcfb_release
 *
 * Description  : This function is called by the framebuffer when the 
 *                application closes the link to framebuffer. 
 *             
 *@return		0 on success any other value for failure
 **/
static int slcdcfb_release(struct fb_info *info, int user)
{
	TRACE("%s()",__FUNCTION__);

	return 0;
}

static int mx2fb_remove(struct platform_device *pdev)
{
	TRACE("%s()",__FUNCTION__);

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	slcdc_device_remove_file( );
#endif

	free_irq(SLCDC_IRQ, 0);

	/*Do some cleanup work */
	unregister_framebuffer(&slcdc_fb_info);

	slcdc_free_buffer(&slcdc_fb_info);

	slcdc_release(NULL, 0);

#if 0
	mx2fb_exit_pm_reg8( NULL );
#endif

	return 0;
}

/*!
 * @brief Probe routine for the framebuffer driver. It is called during the
 *        driver binding process.
 *
 * @return Appropriate error code to the kernel common code
 */
static int mx2fb_probe(struct platform_device *pdev)
{
	int tmp;

	TRACE("%s()",__FUNCTION__);

#if 0
	mx2fb_init_pm_reg8( pdev );
#endif

	slcdc_fb_info.device = &(pdev->dev);

	_init_fbinfo(&slcdc_fb_info);

	slcdc_init_buffer(&slcdc_fb_info);

	slcdcfb_set_var(&slcdc_fb_info);	/* current_par.allow_modeset = 0; */

	tmp = slcdc_open(NULL, 0);

	register_framebuffer(&slcdc_fb_info);

	// Register IRQ
 	tmp = request_irq(SLCDC_IRQ, (void *)slcdc_isr, 0, MODULE_NAME, 0);
	if ( tmp ) 
	{
		TRACE("%s():cannot init irq=%d", __FUNCTION__, SLCDC_IRQ);

		return -EINVAL;
	}

#ifdef BR_FEATURE_SHOW_BOOT_UP_PROGRESS
	slcdc_device_create_file( );
#endif
#ifdef CONFIG_LOGO
	init_1bpp_cmap();
#endif

	return 0;
}

/**
 *@brief slcdc init function
 *
 * Function Name: slcdc_init
 *
 *
 *@return	int   return status
 *			@li 0  sucess
 *			@li other failure
 *
 * Description: This is the initialization routine for the driver. And this 
 *              function \n will be called while the module being installed. 
 *              In this function, it will \n register char device,request 
 *              slcdc irq, initialize the buffer,register to\npower management. 
 *
 * Modification History:
 * 	Dec,2003,			Karen update for MX21 TO2 
 *   
 **/

int __init slcdc_init(void)
{
	INFO("RiTdisplay P13506 OLED(%dx%d)", SLCDC_WIDTH , SLCDC_HIGH );

	return platform_driver_register(&mx2fb_driver);
}

/**
 *@brief slcdc cleanup function
 *
 * Function Name: slcdc_cleanup
 *
 *@return	None
 *
 * Description: This is the cleanup routine for the driver. And this function \n
 *	 will be called while the module being removed. In this function, it will \n
 *	 cleanup all the registered entries 
 *
 * Modification History:
 * 	Dec 2003,			Karen update for MX21 TO2 
 *   
 **/

void __exit slcdc_cleanup(void)
{
	TRACE("%s()",__FUNCTION__);

	platform_driver_unregister(&mx2fb_driver);
}

module_init(slcdc_init);
module_exit(slcdc_cleanup);
/*\@}*/

MODULE_AUTHOR("BandRich, Inc.");
MODULE_DESCRIPTION("SLCDC driver");
MODULE_LICENSE("GPL");
