/*
 * Driver for OTG input event handle
 *
 * Copyright 2010 BandRich, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/timer.h>
#include <linux/workqueue.h>


typedef enum OTG_SWITCH_EVENT
{
	OTG_SWITCH_SW_CDROM_TO_ECM,
       OTG_SWITCH_SW_ECM_TO_CDROM,
	OTG_SWITCH_SW_NUM
} OTG_SWITCH_EVENT_TYPE;

typedef struct otg_switch_data_type {
	struct work_struct work;
	unsigned int code;
	unsigned int valid;
} otg_switch_data_type;


static struct input_dev *otg_input = NULL;
static otg_switch_data_type otg_switch_data[OTG_SWITCH_SW_NUM];

static void otg_switch_report_event_work(struct work_struct *work)
{
	otg_switch_data_type *p_data =
		container_of(work, otg_switch_data_type, work);

	printk("%s():code=%d\n", __FUNCTION__, p_data->code );

	if ( otg_input )
	{
		input_event(otg_input, EV_SW, p_data->code, 1);
		input_event(otg_input, EV_SW, p_data->code, 0);
	}
}

static int otg_switch_event_register( OTG_SWITCH_EVENT_TYPE key, unsigned int code )
{
	if ( key >= OTG_SWITCH_SW_NUM ) 
		return -1;

	if ( !otg_switch_data[key].valid )
	{
		INIT_WORK(&(otg_switch_data[key].work), otg_switch_report_event_work);
		otg_switch_data[key].code = code;
		otg_switch_data[key].valid = 1;
	}
	
	return 0;
}

static int otg_switch_event_unregister( OTG_SWITCH_EVENT_TYPE key )
{
	if ( key >= OTG_SWITCH_SW_NUM ) 
		return -1;

	if ( otg_switch_data[key].valid )
	{
		cancel_work_sync( &(otg_switch_data[key].work) );
	}

	return 0;
}

static void otg_switch_event_init( void )
{
	int i;

	for ( i = 0 ; i < OTG_SWITCH_SW_NUM ; i++ )
	{
		otg_switch_data[i].valid = 0;
	}
}

void otg_switch_report_event( unsigned int code )
{
	int i;

	for ( i = 0 ; i < OTG_SWITCH_SW_NUM ; i++ )
	{
		if ( (otg_switch_data[i].valid) && (otg_switch_data[i].code == code) )
		{
			schedule_work( &(otg_switch_data[i].work) );

			break;
		}
	}
}

EXPORT_SYMBOL(otg_switch_report_event);


static int __devinit otg_switch_probe(struct platform_device *pdev)
{
	int error;

	otg_switch_event_init();

	otg_input = input_allocate_device();
	if (!otg_input) {
		error = -ENOMEM;
		goto fail1;
	}
	
	otg_input->name = "otg-switch";
	otg_input->phys = "otg-switch/input";
	otg_input->dev.parent = &pdev->dev;

	otg_input->id.bustype = BUS_HOST;
	otg_input->id.vendor = 0x1A8D;
	otg_input->id.product = 0x0001;
	otg_input->id.version = 0x0100;

	input_set_capability(otg_input, EV_SW, SW_CDROM_TO_ECM);
	otg_switch_event_register( OTG_SWITCH_SW_CDROM_TO_ECM, SW_CDROM_TO_ECM );
	
	input_set_capability(otg_input, EV_SW, SW_ECM_TO_CDROM);
	otg_switch_event_register( OTG_SWITCH_SW_ECM_TO_CDROM, SW_ECM_TO_CDROM );
	
	error = input_register_device(otg_input);
	if ( error ) {
		printk("%s():Unable to register input device!\n", __FUNCTION__ );
		goto fail1;
	}
	
	return 0;

fail1:
	input_free_device(otg_input);
	
	return error;
}

static int __devexit otg_switch_remove(struct platform_device *pdev)
{
	otg_switch_event_unregister( OTG_SWITCH_SW_CDROM_TO_ECM );
	otg_switch_event_unregister( OTG_SWITCH_SW_ECM_TO_CDROM );

	input_unregister_device(otg_input);

	return 0;
}


static struct platform_driver otg_switch_driver = {
	.probe		= otg_switch_probe,
	.remove		= __devexit_p(otg_switch_remove),
	.driver		= {
		.name	= "otg-switch",
		.owner	= THIS_MODULE,
	}
};

static int __init otg_switch_init(void)
{
	return platform_driver_register(&otg_switch_driver);
}

static void __exit otg_switch_exit(void)
{
	platform_driver_unregister(&otg_switch_driver);
}

module_init(otg_switch_init);
module_exit(otg_switch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BandRich, Inc.");
MODULE_DESCRIPTION("OTG switch device driver");
MODULE_ALIAS("platform:otg-input");
