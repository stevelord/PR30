/*
 * Driver for keys on GPIO lines capable of generating interrupts.
 *
 * Copyright 2005 Phil Blundell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/workqueue.h>

#include <asm/gpio.h>

struct gpio_button_data {
	struct gpio_keys_button *button;
	struct input_dev *input;
	struct timer_list timer;
	struct work_struct work;
};

struct gpio_keys_drvdata {
	struct input_dev *input;
	struct gpio_button_data data[0];
};

#if 1
typedef enum BR_EXTRA_KEY_EVENT
{
	BR_EXTRA_KEY_SLEEP,
	BR_EXTRA_KEY_WAKEUP,
	BR_EXTRA_KEY_EVENT_NUM
} BR_EXTRA_KEY_EVENT_TYPE;

typedef struct BR_extra_key_data {
	struct work_struct work;
	unsigned int code;
	unsigned int valid;
} BR_extra_key_data_type;
#endif

#if 1
static struct input_dev *gpio_key_input = NULL;
static BR_extra_key_data_type BR_extra_key_data[BR_EXTRA_KEY_EVENT_NUM];

static void BR_extra_key_event_report_work(struct work_struct *work)
{
	BR_extra_key_data_type *p_data =
		container_of(work, BR_extra_key_data_type, work);

	printk("%s():code=%d\n", __FUNCTION__, p_data->code );

	if ( gpio_key_input )
	{
		input_event(gpio_key_input, EV_KEY, p_data->code, 1);
		input_event(gpio_key_input, EV_KEY, p_data->code, 0);
	}
}

static int BR_extra_key_event_register( BR_EXTRA_KEY_EVENT_TYPE key, unsigned int code )
{
	if ( key >= BR_EXTRA_KEY_EVENT_NUM ) 
		return -1;

	if ( !BR_extra_key_data[key].valid )
	{
		INIT_WORK(&(BR_extra_key_data[key].work), BR_extra_key_event_report_work);

		BR_extra_key_data[key].code = code;
		BR_extra_key_data[key].valid = 1;
	}
	
	return 0;
}

static int BR_extra_key_event_unregister( BR_EXTRA_KEY_EVENT_TYPE key )
{
	if ( key >= BR_EXTRA_KEY_EVENT_NUM ) 
		return -1;

	if ( BR_extra_key_data[key].valid )
	{
		cancel_work_sync( &(BR_extra_key_data[key].work) );
	}

	return 0;
}

static void BR_extra_key_event_init( void )
{
	int i;

	for ( i = 0 ; i < BR_EXTRA_KEY_EVENT_NUM ; i++ )
	{
		BR_extra_key_data[i].valid = 0;
	}
}

void BR_extra_key_report_event( unsigned int code )
{
	int i;

	for ( i = 0 ; i < BR_EXTRA_KEY_EVENT_NUM ; i++ )
	{
		if ( (BR_extra_key_data[i].valid) && (BR_extra_key_data[i].code == code) )
		{
			schedule_work( &(BR_extra_key_data[i].work) );

			break;
		}
	}
}

EXPORT_SYMBOL(BR_extra_key_report_event);
#endif

static void gpio_keys_report_event(struct work_struct *work)
{
	struct gpio_button_data *bdata =
		container_of(work, struct gpio_button_data, work);
	struct gpio_keys_button *button = bdata->button;
	struct input_dev *input = bdata->input;
	unsigned int type = button->type ?: EV_KEY;
	int state = (gpio_get_value(button->gpio) ? 1 : 0) ^ button->active_low;

	input_event(input, type, button->code, !!state);
	input_sync(input);
}

static void gpio_keys_timer(unsigned long _data)
{
	struct gpio_button_data *data = (struct gpio_button_data *)_data;

	schedule_work(&data->work);
}

static irqreturn_t gpio_keys_isr(int irq, void *dev_id)
{
	struct gpio_button_data *bdata = dev_id;
	struct gpio_keys_button *button = bdata->button;

	BUG_ON(irq != gpio_to_irq(button->gpio));

	if (button->debounce_interval)
		mod_timer(&bdata->timer,
			jiffies + msecs_to_jiffies(button->debounce_interval));
	else
		schedule_work(&bdata->work);

	return IRQ_HANDLED;
}

static int __devinit gpio_keys_probe(struct platform_device *pdev)
{
	struct gpio_keys_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_keys_drvdata *ddata;
	struct input_dev *input;
	int i, error;
	int wakeup = 0;

#if 1
	BR_extra_key_event_init( );
#endif

	ddata = kzalloc(sizeof(struct gpio_keys_drvdata) +
			pdata->nbuttons * sizeof(struct gpio_button_data),
			GFP_KERNEL);
	input = input_allocate_device();
	if (!ddata || !input) {
		error = -ENOMEM;
		goto fail1;
	}

	platform_set_drvdata(pdev, ddata);

	input->name = pdev->name;
	input->phys = "gpio-keys/input0";
	input->dev.parent = &pdev->dev;

	input->id.bustype = BUS_HOST;
	input->id.vendor = 0x0001;
	input->id.product = 0x0001;
	input->id.version = 0x0100;

	/* Enable auto repeat feature of Linux input subsystem */
	if (pdata->rep)
		__set_bit(EV_REP, input->evbit);

	ddata->input = input;

	for (i = 0; i < pdata->nbuttons; i++) {
		struct gpio_keys_button *button = &pdata->buttons[i];
		struct gpio_button_data *bdata = &ddata->data[i];
		int irq;
		unsigned int type = button->type ?: EV_KEY;

		bdata->input = input;
		bdata->button = button;
		setup_timer(&bdata->timer,
			    gpio_keys_timer, (unsigned long)bdata);
		INIT_WORK(&bdata->work, gpio_keys_report_event);

		error = gpio_request(button->gpio, button->desc ?: "gpio_keys");
		if (error < 0) {
			pr_err("gpio-keys: failed to request GPIO %d,"
				" error %d\n", button->gpio, error);
			goto fail2;
		}

		error = gpio_direction_input(button->gpio);
		if (error < 0) {
			pr_err("gpio-keys: failed to configure input"
				" direction for GPIO %d, error %d\n",
				button->gpio, error);
			gpio_free(button->gpio);
			goto fail2;
		}

		irq = gpio_to_irq(button->gpio);
		if (irq < 0) {
			error = irq;
			pr_err("gpio-keys: Unable to get irq number"
				" for GPIO %d, error %d\n",
				button->gpio, error);
			gpio_free(button->gpio);
			goto fail2;
		}

		error = request_irq(irq, gpio_keys_isr,
				    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
				    button->desc ? button->desc : "gpio_keys",
				    bdata);
		if (error) {
			pr_err("gpio-keys: Unable to claim irq %d; error %d\n",
				irq, error);
			gpio_free(button->gpio);
			goto fail2;
		}

		if (button->wakeup)
			wakeup = 1;

		input_set_capability(input, type, button->code);
	}

#if 1
	input_set_capability(input, EV_KEY, KEY_SLEEP);
	BR_extra_key_event_register(BR_EXTRA_KEY_SLEEP , KEY_SLEEP);

	input_set_capability(input, EV_KEY, KEY_WAKEUP );
	BR_extra_key_event_register(BR_EXTRA_KEY_WAKEUP , KEY_WAKEUP);
#endif

	error = input_register_device(input);
	if (error) {
		pr_err("gpio-keys: Unable to register input device, "
			"error: %d\n", error);
		goto fail2;
	}

	device_init_wakeup(&pdev->dev, wakeup);

#if 1
	gpio_key_input = input;
#endif
	return 0;

 fail2:
	while (--i >= 0) {
		free_irq(gpio_to_irq(pdata->buttons[i].gpio), &ddata->data[i]);
		if (pdata->buttons[i].debounce_interval)
			del_timer_sync(&ddata->data[i].timer);
		cancel_work_sync(&ddata->data[i].work);
		gpio_free(pdata->buttons[i].gpio);
	}

	platform_set_drvdata(pdev, NULL);

#if 1
	for ( i = 0 ; i < BR_EXTRA_KEY_EVENT_NUM ; i++ )
		BR_extra_key_event_unregister( i );
#endif
 fail1:
	input_free_device(input);
	kfree(ddata);

	return error;
}

static int __devexit gpio_keys_remove(struct platform_device *pdev)
{
	struct gpio_keys_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_keys_drvdata *ddata = platform_get_drvdata(pdev);
	struct input_dev *input = ddata->input;
	int i;

	device_init_wakeup(&pdev->dev, 0);

	for (i = 0; i < pdata->nbuttons; i++) {
		int irq = gpio_to_irq(pdata->buttons[i].gpio);
		free_irq(irq, &ddata->data[i]);
		if (pdata->buttons[i].debounce_interval)
			del_timer_sync(&ddata->data[i].timer);
		cancel_work_sync(&ddata->data[i].work);
		gpio_free(pdata->buttons[i].gpio);
	}
#if 1
	for ( i = 0 ; i < BR_EXTRA_KEY_EVENT_NUM ; i++ )
		BR_extra_key_event_unregister( i );
#endif

	input_unregister_device(input);

#if 0
	kfree(ddata);
#endif

	return 0;
}


#ifdef CONFIG_PM
static int gpio_keys_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct gpio_keys_platform_data *pdata = pdev->dev.platform_data;
	int i;

	if (device_may_wakeup(&pdev->dev)) {
		for (i = 0; i < pdata->nbuttons; i++) {
			struct gpio_keys_button *button = &pdata->buttons[i];
			if (button->wakeup) {
				int irq = gpio_to_irq(button->gpio);
				enable_irq_wake(irq);
			}
		}
	}

	return 0;
}

static int gpio_keys_resume(struct platform_device *pdev)
{
	struct gpio_keys_platform_data *pdata = pdev->dev.platform_data;
	int i;

	if (device_may_wakeup(&pdev->dev)) {
		for (i = 0; i < pdata->nbuttons; i++) {
			struct gpio_keys_button *button = &pdata->buttons[i];
			if (button->wakeup) {
				int irq = gpio_to_irq(button->gpio);
				disable_irq_wake(irq);
			}
		}
	}

#if 1
	BR_extra_key_report_event( KEY_WAKEUP );
#endif

	return 0;
}
#else
#define gpio_keys_suspend	NULL
#define gpio_keys_resume	NULL
#endif

static struct platform_driver gpio_keys_device_driver = {
	.probe		= gpio_keys_probe,
	.remove		= __devexit_p(gpio_keys_remove),
	.suspend	= gpio_keys_suspend,
	.resume		= gpio_keys_resume,
	.driver		= {
		.name	= "gpio-keys",
		.owner	= THIS_MODULE,
	}
};

static int __init gpio_keys_init(void)
{
	return platform_driver_register(&gpio_keys_device_driver);
}

static void __exit gpio_keys_exit(void)
{
	platform_driver_unregister(&gpio_keys_device_driver);
}

module_init(gpio_keys_init);
module_exit(gpio_keys_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Phil Blundell <pb@handhelds.org>");
MODULE_DESCRIPTION("Keyboard driver for CPU GPIOs");
MODULE_ALIAS("platform:gpio-keys");
