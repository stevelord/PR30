/*
 *  Copyright 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/kernel.h>
#include <linux/suspend.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include "crm_regs.h"

#if 1
#include <asm/mach/map.h>
#include <asm/tlb.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/proc_fs.h>
#include "sleep.h"
#endif


#if defined(CONFIG_CPU_FREQ)
static struct cpu_wp *cpu_wp_tbl;
static struct clk *cpu_clk;

static int org_freq;
extern int cpufreq_suspended;
extern int set_cpu_freq(int wp);
#endif

/*!
 * @defgroup MSL_MX25 i.MX25 Machine Specific Layer (MSL)
 */

/*!
 * @file mach-mx25/pm.c
 * @brief This file contains suspend operations
 *
 * @ingroup MSL_MX25
 */
static unsigned int cgcr0, cgcr1, cgcr2;

#if 1
#define MXC_SUSPEND_CODE_SIZE (6 * 1024)

#define SUSPEND_IRAM_BASE_ADDR (USB_IRAM_BASE_ADDR + USB_IRAM_SIZE)

unsigned long iram_phy_addr;
void *iram_virtual_addr;

static void *mxc_suspend_iram_init(dma_addr_t *phys_addr, size_t bytes)
{
	void *iram_base;

	iram_base = (void *)ioremap((uint32_t) SUSPEND_IRAM_BASE_ADDR, bytes);

	*phys_addr = (dma_addr_t) SUSPEND_IRAM_BASE_ADDR;

	return iram_base;
}
#endif

static int mx25_suspend_enter(suspend_state_t state)
{
	unsigned int reg;
#if 0
	void (*mx25_cpu_standby_ptr) (void);
#endif

	switch (state) {
	case PM_SUSPEND_MEM:
		mxc_cpu_lp_set(STOP_POWER_OFF);
		break;
	case PM_SUSPEND_STANDBY:
		mxc_cpu_lp_set(WAIT_UNCLOCKED_POWER_OFF);
		break;
	default:
		return -EINVAL;
	}

#if 0
//	local_flush_tlb_all();
	flush_cache_all();

	local_fiq_disable();

	if ( iram_virtual_addr != NULL )
	{
		if ( state == PM_SUSPEND_MEM )
		{
			/* copy suspend function into SRAM */
			memcpy(iram_virtual_addr, mx23_cpu_standby, mx23_standby_alloc_sz);
			mx25_cpu_standby_ptr = iram_virtual_addr;
			mx25_cpu_standby_ptr();
		}
	}
	else
	{
		printk("%s():iram_virtual_addr=NULL\n", __FUNCTION__ );
	}

	local_fiq_enable();
#else // original
	/* Executing CP15 (Wait-for-Interrupt) Instruction */
	cpu_do_idle();
#endif

	reg = (__raw_readl(MXC_CCM_CGCR0) & ~MXC_CCM_CGCR0_STOP_MODE_MASK) |
	    cgcr0;
	__raw_writel(reg, MXC_CCM_CGCR0);

	reg = (__raw_readl(MXC_CCM_CGCR1) & ~MXC_CCM_CGCR1_STOP_MODE_MASK) |
	    cgcr1;
	__raw_writel(reg, MXC_CCM_CGCR1);

	reg = (__raw_readl(MXC_CCM_CGCR2) & ~MXC_CCM_CGCR2_STOP_MODE_MASK) |
	    cgcr2;
	__raw_writel(reg, MXC_CCM_CGCR2);

	return 0;
}

/*
 * Called after processes are frozen, but before we shut down devices.
 */
static int mx25_suspend_prepare(void)
{
#if defined(CONFIG_CPU_FREQ)
	struct cpufreq_freqs freqs;
	org_freq = clk_get_rate(cpu_clk);
	freqs.old = org_freq / 1000;
	freqs.new = cpu_wp_tbl[0].cpu_rate / 1000;
	freqs.cpu = 0;
	freqs.flags = 0;

	cpufreq_suspended = 1;
	if (clk_get_rate(cpu_clk) != cpu_wp_tbl[0].cpu_rate) {
		set_cpu_freq(cpu_wp_tbl[0].cpu_rate);
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}
#endif

	cgcr0 = __raw_readl(MXC_CCM_CGCR0) & MXC_CCM_CGCR0_STOP_MODE_MASK;
	cgcr1 = __raw_readl(MXC_CCM_CGCR1) & MXC_CCM_CGCR1_STOP_MODE_MASK;
	cgcr2 = __raw_readl(MXC_CCM_CGCR2) & MXC_CCM_CGCR2_STOP_MODE_MASK;

	return 0;
}

/*
 * Called after devices are re-setup, but before processes are thawed.
 */
static void mx25_suspend_finish(void)
{
#if defined(CONFIG_CPU_FREQ)
	struct cpufreq_freqs freqs;

	freqs.old = clk_get_rate(cpu_clk) / 1000;
	freqs.new = org_freq / 1000;
	freqs.cpu = 0;
	freqs.flags = 0;

	cpufreq_suspended = 0;

	if (org_freq != clk_get_rate(cpu_clk)) {
		set_cpu_freq(org_freq);
		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
		cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	}
#endif
}

static int mx25_pm_valid(suspend_state_t state)
{
	return state > PM_SUSPEND_ON && state <= PM_SUSPEND_MAX;
}

struct platform_suspend_ops mx25_suspend_ops = {
	.valid = mx25_pm_valid,
	.prepare = mx25_suspend_prepare,
	.enter = mx25_suspend_enter,
	.finish = mx25_suspend_finish,
};

static int __init mx25_pm_init(void)
{
#if defined(CONFIG_CPU_FREQ) 
	int cpu_wp_nr;
#endif

	pr_info("Static Power Management for Freescale i.MX25\n");

#if 1
	iram_virtual_addr = mxc_suspend_iram_init((dma_addr_t *)&iram_phy_addr, MXC_SUSPEND_CODE_SIZE );
#endif

	suspend_set_ops(&mx25_suspend_ops);

#if defined(CONFIG_CPU_FREQ)
	cpu_wp_tbl = get_cpu_wp(&cpu_wp_nr);

	cpu_clk = clk_get(NULL, "cpu_clk");
	if (IS_ERR(cpu_clk)) {
		printk("%s(): failed to get cpu_clk\n", __FUNCTION__ );
		return PTR_ERR(cpu_clk);
	}
#endif

	return 0;
}

late_initcall(mx25_pm_init);
