/*
 * This is a module which is used for logging packets.
 */


#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/route.h>

#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>

//	+++	Gemtek
#include <linux/netfilter_ipv4/ipt_notify_userapps.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

/*	For putting processes to sleep and waking them up	*/
//#include <linux/sched.h>
//#include <linux/wrapper.h>

//	---	Gemtek

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("iptables syslog logging module");

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

#define CHECK_TIMER				10000			/*	10 second  */  
#define BASE_TIMER				60000			/*	60 second  */  

/* Use lock to serialize, so printks don't overlap */
static DEFINE_SPINLOCK(notify_userapps_lock);

DECLARE_WAIT_QUEUE_HEAD(OnDemandWaitQ);

static unsigned long expired_time = 0;
static unsigned int flag_timeout  = 0;

static struct timer_list	trigger_timer;

static int proc_return_metrics ( char *page , char **start , off_t off , int count , int *eof , int len )
{
	if ( len <= off + count ) *eof = 1;
	*start = page + off;
	len -= off;
	if ( len > count ) len = count;
	if ( len < 0 ) len = 0;
	return	len;
}

static struct proc_dir_entry *create_proc_read_write_entry(const char *name,
	mode_t mode, struct proc_dir_entry *base, 
	read_proc_t *read_proc, write_proc_t *write_proc, void * data)
{
		struct proc_dir_entry *res=create_proc_entry(name,mode,base);
	if (res) {
		res->read_proc=read_proc;
		res->write_proc=write_proc;
		res->data=data;
	}
	return res;
}

static int read_proc_DialOnDemand(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;
	len = sprintf ( buf , "%u\n", expired_time);

	return	proc_return_metrics ( buf , start , off , count , eof , len );
}

static int read_proc_DialOnDemand_Timeout(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int len = 0;
	
	len = sprintf ( buf , "%d\n", flag_timeout);

	return	proc_return_metrics ( buf , start , off , count , eof , len );
}

static int write_proc_DialOnDemand(struct file *file, const char *buffer, unsigned long count, void *data)
{
	int len = 0;
	char buf[16];
	int timer = 0;
		
	if(count >= 15)
		len = 15;
	else
		len = count;

	memset(buf, 0, sizeof(buf));

	if(copy_from_user(buf, buffer, len))
	{
		printk(KERN_EMERG "write_proc_DialOnDemand : fail.\n");		
		return -EFAULT;
	}
	spin_lock_bh(&notify_userapps_lock);
	timer = simple_strtol(buf,NULL,10);
	expired_time = jiffies + (timer * BASE_TIMER * HZ / 1000);
	spin_unlock_bh(&notify_userapps_lock);
	printk(KERN_EMERG "write_proc_DialOnDemand = %s t=%d\n", buf, timer);
	return len;
}  

static int write_proc_DialOnDemand_Timeout(struct file *file, const char *buffer, unsigned long count, void *data)
{
	//	Clean timeout flag for next dial on demand session
	int len = 0;
	char buf[16]={0};
		
	if(count >= 15)
		len = 15;
	else
		len = count;

	memset(buf, 0, sizeof(buf));

	if(copy_from_user(buf, buffer, len))
	{
		printk(KERN_EMERG "write_proc_DialOnDemand_Timeout : fail.\n");		
		return -EFAULT;
	}
	spin_lock_bh(&notify_userapps_lock);
	flag_timeout = simple_strtol(buf,NULL,10);
	if(flag_timeout!=0)
	   flag_timeout = 1;
        else
	   flag_timeout = 0;
	spin_unlock_bh(&notify_userapps_lock);
	printk(KERN_EMERG "write_proc_DialOnDemand_Timeout = %s t=%d\n", buf, flag_timeout);
	return len;
} 

static void trigger_monitor(unsigned long d)
{
	/*
		PLF_DEV
	*/	
	
	// Note : use time_after to handle jiffies overflow issue.
	//if (expired_time!=0 && expired_time <= jiffies + 1000)
	if (expired_time!=0 && time_after(jiffies + 1000, expired_time))
	{
		// printk("<2>ipt_notify_userapps : Timeout, expired_time = %u, jiffies = %u\n", expired_time, jiffies);
		/* 
			FixME : I should use kernel API 'call_usermodehelper' to ask user-space program 
							to set nvram parameter
		*/
		spin_lock_bh(&notify_userapps_lock);
		expired_time = 0;	
		flag_timeout = 1;
		spin_unlock_bh(&notify_userapps_lock);
	 	//printk("<2>ipt_notify_userapps : Timeout at %u\n", jiffies);
	}
	
	trigger_timer.expires  = jiffies + (CHECK_TIMER * HZ / 1000);
	trigger_timer.data = ((unsigned long) 0);
	mod_timer ( &trigger_timer , trigger_timer.expires );
 	//printk("<2>ipt_notify_userapps : Register trigger_timer at %u\n", jiffies);
}

static unsigned int ipt_notify_userapps_target(struct sk_buff *skb,
					       const struct xt_target_param *par)
{
	unsigned int timer = 0;
	const struct ipt_notify_userapps_info *triggerinfo = par->targinfo;
	
	timer = triggerinfo->timer;
	
	//printk("<2>ipt_trigger : On Demand Timer = %u\n", timer);
	spin_lock_bh(&notify_userapps_lock);
	expired_time = jiffies + (timer * BASE_TIMER * HZ / 1000);
	flag_timeout = 0;
	spin_unlock_bh(&notify_userapps_lock);
	wake_up_interruptible(&OnDemandWaitQ);
	//printk("<2>ipt_trigger : expired_time = %u, register time = %u\n", expired_time, jiffies);
	return XT_CONTINUE;
}

static int ipt_notify_userapps_checkentry(const struct xt_tgchk_param *par)
{

	return 1;
}

static struct xt_target ipt_notify_userapps_reg = {
	.name		= "NOTIFY_USERAPPS",
	.family		= AF_INET,
	.target		= ipt_notify_userapps_target,
	.targetsize	= sizeof(struct ipt_notify_userapps_info),
	.checkentry	= ipt_notify_userapps_checkentry,
	.me		= THIS_MODULE,
};

static int __init ipt_notify_userapps_init(void)
{
	int ret;

	ret = xt_register_target(&ipt_notify_userapps_reg);
	if (ret < 0)
		return ret;
		
	if(create_proc_read_write_entry("DialOnDemand",0,NULL, read_proc_DialOnDemand, write_proc_DialOnDemand, NULL) == NULL)
		printk("<2>ipt_notify_userapps : register proc fail\n");
	else
		printk("<2>ipt_notify_userapps : register proc success\n");

	if(create_proc_read_write_entry("DialOnDemand_Timeout",0,NULL, read_proc_DialOnDemand_Timeout, write_proc_DialOnDemand_Timeout, NULL) == NULL)
		printk("<2>ipt_notify_userapps : register proc fail\n");
	else
		printk("<2>ipt_notify_userapps : register proc success\n");
		
	init_timer(&trigger_timer);
	trigger_timer.function = trigger_monitor;
	trigger_timer.expires  = jiffies + (CHECK_TIMER * HZ / 1000);
 	add_timer(&trigger_timer);

	return 0;
}

static void __exit ipt_notify_userapps_fini(void)
{
	xt_unregister_target(&ipt_notify_userapps_reg);
}

module_init(ipt_notify_userapps_init);
module_exit(ipt_notify_userapps_fini);
