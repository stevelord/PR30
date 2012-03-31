#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/imx_adc.h>

#include "BR_Monitor.h"


#undef POWERMANAGERMENT
//#define POWERMANAGERMENT

static int BR_Monitor_major;

/*!
 * The suspendq is used by blocking application calls
 */
static wait_queue_head_t suspendq;
static wait_queue_head_t tsq;

unsigned long tsc_base;
static struct class *BR_Monitor_class;
static struct imx_adc_data *adc_data;
static bool imx_adc_ready;
static DECLARE_MUTEX(convert_mutex);

/*!
 * Number of users waiting in suspendq
 */
static int swait;

/*!
 * To indicate whether any of the adc devices are suspending
 */
static int suspend_flag;

extern int get_charger_status(void);

int is_imx_adc_ready(void)
{
	return imx_adc_ready;
}
EXPORT_SYMBOL(is_imx_adc_ready);

void tsc_clk_enable(void)
{
	unsigned long reg;

	clk_enable(adc_data->adc_clk);

	reg = __raw_readl(tsc_base + TGCR);
	reg |= TGCR_IPG_CLK_EN;
	__raw_writel(reg, tsc_base + TGCR);
}

void tsc_self_reset(void)
{
	unsigned long reg;

	reg = __raw_readl(tsc_base + TGCR);
	reg |= TGCR_TSC_RST;
	__raw_writel(reg, tsc_base + TGCR);

	while (__raw_readl(tsc_base + TGCR) & TGCR_TSC_RST)
		continue;
}

void tsc_intref_enable(void)
{
	unsigned long reg;

	reg = __raw_readl(tsc_base + TGCR);
	reg |= TGCR_INTREFEN;
	__raw_writel(reg, tsc_base + TGCR);
}
#if 1
void imx_tsc_init(void)
{
	unsigned long reg;
	int lastitemid;
	int dbtime;

	/* Level sense */
	//reg = __raw_readl(tsc_base + TCQCR);
	reg = __raw_readl(tsc_base + GCQCR);
	reg |= CQCR_PD_CFG;
	reg |= (0xf << CQCR_FIFOWATERMARK_SHIFT);  /* watermark */
	//__raw_writel(reg, tsc_base + TCQCR);
	__raw_writel(reg, tsc_base + GCQCR);

#if 0
	/* Configure 4-wire */
	reg = TSC_4WIRE_PRECHARGE;
	reg |= CC_IGS;
	__raw_writel(reg, tsc_base + TCC0);

	reg = TSC_4WIRE_TOUCH_DETECT;
	reg |= 3 << CC_NOS_SHIFT;	/* 4 samples */
	reg |= 32 << CC_SETTLING_TIME_SHIFT;	/* it's important! */
	__raw_writel(reg, tsc_base + TCC1);

	reg = TSC_4WIRE_X_MEASUMENT;
	reg |= 3 << CC_NOS_SHIFT;	/* 4 samples */
	reg |= 16 << CC_SETTLING_TIME_SHIFT;	/* settling time */
	__raw_writel(reg, tsc_base + TCC2);

	reg = TSC_4WIRE_Y_MEASUMENT;
	reg |= 3 << CC_NOS_SHIFT;	/* 4 samples */
	reg |= 16 << CC_SETTLING_TIME_SHIFT;	/* settling time */
	__raw_writel(reg, tsc_base + TCC3);
#endif
/*
	reg = (TCQ_ITEM_TCC0 << TCQ_ITEM7_SHIFT) |
	      (TCQ_ITEM_TCC0 << TCQ_ITEM6_SHIFT) |
	      (TCQ_ITEM_TCC1 << TCQ_ITEM5_SHIFT) |
	      (TCQ_ITEM_TCC0 << TCQ_ITEM4_SHIFT) |
	      (TCQ_ITEM_TCC3 << TCQ_ITEM3_SHIFT) |
	      (TCQ_ITEM_TCC2 << TCQ_ITEM2_SHIFT) |
	      (TCQ_ITEM_TCC1 << TCQ_ITEM1_SHIFT) |
	      (TCQ_ITEM_TCC0 << TCQ_ITEM0_SHIFT);
*/
	reg = (GCQ_ITEM_GCC0 << GCQ_ITEM7_SHIFT) |
	      (GCQ_ITEM_GCC0 << GCQ_ITEM6_SHIFT) |
	      (GCQ_ITEM_GCC1 << GCQ_ITEM5_SHIFT) |
	      (GCQ_ITEM_GCC0 << GCQ_ITEM4_SHIFT) |
	      (GCQ_ITEM_GCC3 << GCQ_ITEM3_SHIFT) |
	      (GCQ_ITEM_GCC2 << GCQ_ITEM2_SHIFT) |
	      (GCQ_ITEM_GCC1 << GCQ_ITEM1_SHIFT) |
	      (GCQ_ITEM_GCC0 << GCQ_ITEM0_SHIFT);
 
	//__raw_writel(reg, tsc_base + TCQ_ITEM_7_0);
	__raw_writel(reg, tsc_base + GCQ_ITEM_7_0);

	lastitemid = 5;
	reg = __raw_readl(tsc_base + GCQCR);
	reg = (reg & ~CQCR_LAST_ITEM_ID_MASK) |
	      (lastitemid << CQCR_LAST_ITEM_ID_SHIFT);
	__raw_writel(reg, tsc_base + GCQCR);
#if 0
	/* Config idle for 4-wire */
	reg = TSC_4WIRE_PRECHARGE;
	__raw_writel(reg, tsc_base + TICR);

	reg = TSC_4WIRE_TOUCH_DETECT;
	__raw_writel(reg, tsc_base + TICR);

	/* pen down enable */
	reg = __raw_readl(tsc_base + TGCR);
	reg |= TGCR_PD_EN | TGCR_PDB_EN;
	__raw_writel(reg, tsc_base + TGCR);
	reg = __raw_readl(tsc_base + TCQCR);
	reg &= ~CQCR_PD_MSK;
	__raw_writel(reg, tsc_base + TCQCR);
	reg = __raw_readl(tsc_base + TCQMR);
	reg &= ~TCQMR_PD_IRQ_MSK;
	__raw_writel(reg, tsc_base + TCQMR);

	/* Debounce time = dbtime*8 adc clock cycles */
	reg = __raw_readl(tsc_base + TGCR);
	dbtime = TGCR_PDBTIME128;
	reg &= ~TGCR_PDBTIME_MASK;
	reg |= dbtime << TGCR_PDBTIME_SHIFT;
	reg |= TGCR_HSYNC_EN;
	__raw_writel(reg, tsc_base + TGCR);
#endif
}
#endif
/*!
 * This function initializes all ADC registers with default values. This
 * function also registers the interrupt events.
 *
 * @return       This function returns IMX_ADC_SUCCESS if successful.
 */
int imx_adc_init(void)
{
	unsigned long reg;
	pr_debug("imx_adc_init()\n");
	if (suspend_flag)
		return -EBUSY;
#if 0
	/*
	Queue Control Registers (0x0904(GCQCR))
	Bit[19]	:		Pen down config
	Bit[18]	:		Pen down mask
	Bit[17]	:		FRST
	Bit[16]	:		QRST
	Bit[15:12]		
	Bit[11:8]	: 		
	Bit[7:4]	: 1111	Last queue item ID(ID count)
	Bit[3]	: 1		The conversion queue repeats
	Bit[2]	: 1		FQS starts the conversion queue
	Bit[1:0]	: 10		New conversion queue is started by FQS setting
	*/
	reg = __raw_readl(tsc_base + GCQCR);
	reg |= (CQCR_QSM_FQS | CQCR_FQS | CQCR_LAST_ITEM_ID_MASK | CQCR_QRST | CQCR_FRST | CQCR_PD_CFG) ;
	__raw_writel(reg, tsc_base + GCQCR);
#else
	tsc_clk_enable();

	/* Reset */
	tsc_self_reset();

	/* Internal reference */
	tsc_intref_enable();

	/* Set power mode */
	reg = __raw_readl(tsc_base + TGCR) & ~TGCR_POWER_MASK;
	reg |= TGCR_POWER_SAVE;
	__raw_writel(reg, tsc_base + TGCR);

	imx_tsc_init();
#endif
	return IMX_ADC_SUCCESS;
}
//EXPORT_SYMBOL(imx_adc_init);

enum IMX_ADC_STATUS imx_adc_deinit(void)
{
	return IMX_ADC_SUCCESS;
}
//EXPORT_SYMBOL(imx_adc_deinit);


static irqreturn_t imx_adc_interrupt(int irq, void *dev_id)
{
	return IRQ_HANDLED;
}

enum IMX_ADC_STATUS imx_adc_read_general(unsigned short *result)
{
	unsigned long reg;
	unsigned int data_num = 0;

	reg = __raw_readl(tsc_base + GCQCR);
	reg |= CQCR_FQS;
	__raw_writel(reg, tsc_base + GCQCR);

	while (!(__raw_readl(tsc_base + GCQSR) & CQSR_EOQ))
		continue;
	reg = __raw_readl(tsc_base + GCQCR);
	reg &= ~CQCR_FQS;
	__raw_writel(reg, tsc_base + GCQCR);
	reg = __raw_readl(tsc_base + GCQSR);
	reg |= CQSR_EOQ;
	__raw_writel(reg, tsc_base + GCQSR);

	while (!(__raw_readl(tsc_base + GCQSR) & CQSR_EMPT)) {
		result[data_num] = __raw_readl(tsc_base + GCQFIFO) >>
				 GCQFIFO_ADCOUT_SHIFT;
		data_num++;
	}
	return IMX_ADC_SUCCESS;
}

/*!
 * This function triggers a conversion and returns one sampling result of one
 * channel.
 *
 * @param        channel   The channel to be sampled
 * @param        result    The pointer to the conversion result. The memory
 *                         should be allocated by the caller of this function.
 *
 * @return       This function returns IMX_ADC_SUCCESS if successful.
 */
enum IMX_ADC_STATUS imx_adc_convert(enum t_channel channel,
				    unsigned short *result)
{
	int reg;
	int lastitemid;
	//struct t_touch_screen touch_sample;

	switch (channel) 
	{
		case GER_PURPOSE_ADC0:
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC0......\n");
			down(&convert_mutex);
			lastitemid = 0;
			reg = (0xf << CQCR_FIFOWATERMARK_SHIFT) |
		      		(lastitemid << CQCR_LAST_ITEM_ID_SHIFT) | CQCR_QSM_FQS;
			__raw_writel(reg, tsc_base + GCQCR);

			reg = TSC_GENERAL_ADC_GCC0;
			reg |= (3 << CC_NOS_SHIFT) | (16 << CC_SETTLING_TIME_SHIFT);
			__raw_writel(reg, tsc_base + GCC0);

			imx_adc_read_general(result);
			up(&convert_mutex);
			break;
		case GER_PURPOSE_ADC1:
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC1......\n");
			down(&convert_mutex);
			lastitemid = 0;
			reg = (0xf << CQCR_FIFOWATERMARK_SHIFT) |
		      		(lastitemid << CQCR_LAST_ITEM_ID_SHIFT) | CQCR_QSM_FQS;
			__raw_writel(reg, tsc_base + GCQCR);

			reg = TSC_GENERAL_ADC_GCC1;
			reg |= (3 << CC_NOS_SHIFT) | (16 << CC_SETTLING_TIME_SHIFT);
			__raw_writel(reg, tsc_base + GCC0);
			
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC1 imx_adc_read_general(result);......\n");
			imx_adc_read_general(result);
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC1 imx_adc_read_general(result) exit;......\n");
			up(&convert_mutex);
			break;
		case GER_PURPOSE_ADC2:
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC2......\n");
			down(&convert_mutex);
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC2 Down......\n");
			lastitemid = 0;
			reg = (0xf << CQCR_FIFOWATERMARK_SHIFT) |
		      		(lastitemid << CQCR_LAST_ITEM_ID_SHIFT) | CQCR_QSM_FQS;
			__raw_writel(reg, tsc_base + GCQCR);

			reg = TSC_GENERAL_ADC_GCC2;
			reg |= (3 << CC_NOS_SHIFT) | (16 << CC_SETTLING_TIME_SHIFT);
			__raw_writel(reg, tsc_base + GCC0);
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC2 read general......\n");
			imx_adc_read_general(result);
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert GER_PURPOSE_ADC2 Up......\n");
			up(&convert_mutex);
			break;
		default:
			//printk(KERN_INFO"[BR_Monitor]Entry imx_adc_convert default......\n");
			pr_debug("[BR_Monitor] imx_adc_convert : unsupported imx_adc_convert command 0x%x\n",
			 channel);
		return -EINVAL;
				break;
	}
	return IMX_ADC_SUCCESS;
}




static int BR_Monitor_open(struct inode *inode, struct file *file)
{
	int ret = 0;
	//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_open......\n");

#if 1	
	while (suspend_flag) {
		swait++;
		/* Block if the device is suspended */
		if (wait_event_interruptible(suspendq, !suspend_flag)){
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_open......return -ERESTARTSYS\n");
			return -ERESTARTSYS;
			}
	}
#endif


	return ret;
}

static int BR_Monitor_free(struct inode *inode, struct file *file)
{
	int ret = 0;
	//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_free\n");
	return ret;
}

static int BR_Monitor_ioctl(struct inode *inode, struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int rc ;

	//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl......\n");

	struct t_adc_convert_param *convert_param;

	if ((_IOC_TYPE(cmd) != 'p') && (_IOC_TYPE(cmd) != 'D'))
		return -ENOTTY;

	while (suspend_flag) {
		swait++;
		/* Block if the device is suspended */
		if (wait_event_interruptible(suspendq, !suspend_flag))
			return -ERESTARTSYS;
	}

	switch (cmd)
	{
		case IMX_ADC_INIT:
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl IMX_ADC_INIT\n");
			pr_debug("init adc\n");
			CHECK_ERROR(imx_adc_init());
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl IMX_ADC_INIT break\n");
			break;

		case IMX_ADC_DEINIT:
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl IMX_ADC_DEINIT\n");
			pr_debug("deinit adc\n");
			CHECK_ERROR(imx_adc_deinit());
			break;
	
		case IMX_ADC_CONVERT:
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl IMX_ADC_CONVERT\n");
			convert_param = kmalloc(sizeof(*convert_param), GFP_KERNEL);
			if (convert_param == NULL)
				return -ENOMEM;
			if (copy_from_user(convert_param,(struct t_adc_convert_param *)arg,sizeof(*convert_param)))
			{ 
				kfree(convert_param);
				return -EFAULT;
			}
			
			CHECK_ERROR_KFREE(imx_adc_convert(convert_param->channel,convert_param->result),(kfree(convert_param)));

			if (copy_to_user((struct t_adc_convert_param *)arg,convert_param, sizeof(*convert_param))) 
			{
				kfree(convert_param);
				return -EFAULT;
			}
			kfree(convert_param);
			break;
		case BR_ADC_GET_CHARGE_STATUS:
		 
		        rc = get_charger_status();

			
			if (copy_to_user((int *)arg,&rc, sizeof(int))) 
			{
				//kfree(convert_param);
				return -EFAULT;
			}
			
			break;
	
#if 0 /* not support*/
	case IMX_ADC_CONVERT_MULTICHANNEL:
		convert_param = kmalloc(sizeof(*convert_param), GFP_KERNEL);
		if (convert_param == NULL)
			return -ENOMEM;
		if (copy_from_user(convert_param,
				   (struct t_adc_convert_param *)arg,
				   sizeof(*convert_param))) {
			kfree(convert_param);
			return -EFAULT;
		}
		CHECK_ERROR_KFREE(imx_adc_convert_multichnnel
				  (convert_param->channel,
				   convert_param->result),
				  (kfree(convert_param)));

		if (copy_to_user((struct t_adc_convert_param *)arg,
				 convert_param, sizeof(*convert_param))) {
			kfree(convert_param);
			return -EFAULT;
		}
		kfree(convert_param);
		break;
#endif
		default:
			//printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_ioctl default\n");
			pr_debug("[BR_Monitor] BR_Monitor_ioctl : unsupported ioctl command 0x%x\n",
			 	cmd);
			return -EINVAL;
	}
	//printk(KERN_INFO"[BR_Monitor]Leave BR_Monitor_ioctl ret = %d\n",ret);
	return ret;
}


static struct file_operations BR_Monitor_fops = {
	.owner = THIS_MODULE,
	.open = BR_Monitor_open,		
	.release = BR_Monitor_free,
	.ioctl = BR_Monitor_ioctl,
};


static int BR_Monitor_probe(struct platform_device *pdev)
{
	int ret = 0;
	int retval;
	struct device *temp_class;
	struct resource *res;
	void __iomem *base;

	printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_probe......\n");

	/* ioremap the base address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "No TSC base address provided\n");
		goto err_out0;
	}

	base = ioremap(res->start, res->end - res->start);
	if (base == NULL) {
		dev_err(&pdev->dev, "failed to rebase TSC base address\n");
		goto err_out0;
	}

	
	tsc_base = (unsigned long)base;

	/* create the chrdev */
	BR_Monitor_major = register_chrdev(0, "BR_Monitor", &BR_Monitor_fops);

	if (BR_Monitor_major < 0) {
		dev_err(&pdev->dev, "Unable to get a major for BR_Monitor\n");
		return BR_Monitor_major;
	}

	init_waitqueue_head(&suspendq);
	init_waitqueue_head(&tsq);

	BR_Monitor_class = class_create(THIS_MODULE, "BR_Monitor");
	if (IS_ERR(BR_Monitor_class)) {
		dev_err(&pdev->dev, "Error creating BR Monitor class.\n");
		ret = PTR_ERR(BR_Monitor_class);
		goto err_out1;
	}

	temp_class = device_create(BR_Monitor_class, NULL,
				   MKDEV(BR_Monitor_major, 0), NULL, "BR_Monitor");
	if (IS_ERR(temp_class)) {
		dev_err(&pdev->dev, "Error creating BR_Monitor_class device.\n");
		ret = PTR_ERR(temp_class);
		goto err_out2;
	}

	adc_data = kmalloc(sizeof(struct imx_adc_data), GFP_KERNEL);
	if (adc_data == NULL)
		return -ENOMEM;
	adc_data->irq = platform_get_irq(pdev, 0);
	retval = request_irq(adc_data->irq, imx_adc_interrupt,
			     0, MOD_NAME, MOD_NAME);
	if (retval) {
		return retval;
	}

	adc_data->adc_clk = clk_get(&pdev->dev, "tchscrn_clk");

	ret = imx_adc_init();

	if (ret != IMX_ADC_SUCCESS) {
		dev_err(&pdev->dev, "Error in imx_adc_init.\n");
		goto err_out4;
	}
	
	imx_adc_ready = 1;

	/* By default, devices should wakeup if they can */
	/* So TouchScreen is set as "should wakeup" as it can */
	device_init_wakeup(&pdev->dev, 1);

	pr_info("i.MX ADC at 0x%x irq %d\n", (unsigned int)res->start,
		adc_data->irq);
	return ret;

err_out4:
	device_destroy(BR_Monitor_class, MKDEV(BR_Monitor_major, 0));

err_out2:
	class_destroy(BR_Monitor_class);

err_out1:
	unregister_chrdev(BR_Monitor_major, "BR_Monitor");

err_out0:
	return ret;
}

static int BR_Monitor_remove(struct platform_device *pdev)
{
	int ret = 0;
	
	printk(KERN_INFO"[BR_Monitor]Entry BR_Monitor_remove......\n");
	imx_adc_ready = 0;
	imx_adc_deinit();
	device_destroy(BR_Monitor_class, MKDEV(BR_Monitor_major, 0));
	class_destroy(BR_Monitor_class);
	unregister_chrdev(BR_Monitor_major, "BR_Monitor");
	free_irq(adc_data->irq, MOD_NAME);
	kfree(adc_data);

	return ret;
}

#ifdef POWERMANAGERMENT
static int BR_Monitor_resume(struct platform_device *pdev)
{
	int ret = 0;

	return ret;
}
static int BR_Monitor_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret = 0;

	return ret;
}
#endif /* POWERMANAGERMENT	 */





static struct platform_driver BR_Monitor_driver = {
	.driver = {
		   .name = "imx_adc",
		   },
	.probe = BR_Monitor_probe,
	.remove = BR_Monitor_remove,
	
#ifdef POWERMANAGERMENT	
	.resume = BR_Monitor_resume,
	.suspend = BR_Monitor_suspend,
#endif /* POWERMANAGERMENT	 */

};

/*
 * Initialization and Exit
 */
static int __init BR_Monitor_init(void)
{
	printk(KERN_INFO"[BR_Monitor] BR_Monitor driver loading...\n");
	return platform_driver_register(&BR_Monitor_driver);
}

static void __exit BR_Monitor_exit(void)
{
	platform_driver_unregister(&BR_Monitor_driver);
	printk(KERN_INFO"[BR_Monitor] BR_Monitor driver successfully unloaded\n");

}

/*
 * Module entry points
 */

module_init(BR_Monitor_init);
module_exit(BR_Monitor_exit);

MODULE_DESCRIPTION("BandRich Battery Monitor driver");
MODULE_AUTHOR("BandRich, Inc.");
MODULE_LICENSE("GPL");

