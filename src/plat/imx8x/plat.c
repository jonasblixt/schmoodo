#include <stdio.h>
#include <pb.h>
#include <io.h>
#include <plat.h>
#include <board.h>
#include <plat/regs.h>
#include <plat/imx/lpuart.h>
#include <plat/imx/gpt.h>
#include <plat/sci/ipc.h>
#include <plat/sci/sci.h>
#include <plat/imx8qxp_pads.h>
#include <plat/iomux.h>



#define UART_PAD_CTRL	(PADRING_IFMUX_EN_MASK | PADRING_GP_EN_MASK | \
			(SC_PAD_CONFIG_OUT_IN << PADRING_CONFIG_SHIFT) | \
			(SC_PAD_ISO_OFF << PADRING_LPCONFIG_SHIFT) | \
			(SC_PAD_28FDSOI_DSE_DV_LOW << PADRING_DSE_SHIFT) | \
			(SC_PAD_28FDSOI_PS_PD << PADRING_PULL_SHIFT))

static struct gp_timer tmr0;
static struct lpuart_device uart_device;

sc_ipc_t ipc_handle;

/* Platform API Calls */
void plat_reset(void)
{
    sc_pm_reset(ipc_handle,SC_PM_RESET_TYPE_BOARD);
}

uint32_t  plat_get_us_tick(void)
{
    return gp_timer_get_tick(&tmr0);
}

void plat_wdog_init(void)
{
    sc_timer_set_wdog_timeout(ipc_handle, 15000);
    sc_timer_set_wdog_action(ipc_handle,SC_RM_PT_ALL,SC_TIMER_WDOG_ACTION_BOARD);
    sc_timer_start_wdog(ipc_handle, true);
}

void plat_wdog_kick(void)
{
    sc_timer_ping_wdog(ipc_handle);
}

uint32_t  plat_early_init(void)
{
    uint32_t err = PB_OK;

    
	sc_ipc_open(&ipc_handle, SC_IPC_BASE);
    
    plat_wdog_init();

	/* Power up UART0 */
	sc_pm_set_resource_power_mode(ipc_handle, SC_R_UART_0, SC_PM_PW_MODE_ON);

	/* Set UART0 clock root to 80 MHz */
	sc_pm_clock_rate_t rate = 80000000;
	sc_pm_set_clock_rate(ipc_handle, SC_R_UART_0, 2, &rate);

	/* Enable UART0 clock root */
	sc_pm_clock_enable(ipc_handle, SC_R_UART_0, 2, true, false);

	/* Configure UART pads */
	sc_pad_set(ipc_handle, SC_P_UART0_RX, UART_PAD_CTRL);
	sc_pad_set(ipc_handle, SC_P_UART0_TX, UART_PAD_CTRL);

    //board_early_init();

    uart_device.base = 0x5A060000;//board_get_debug_uart();
    uart_device.baudrate = 0x402008b;

    lpuart_init(&uart_device);
    
    LOG_INFO("IMX8X init");
    /* Setup GPT0 */
	sc_pm_set_resource_power_mode(ipc_handle, SC_R_GPT_0, SC_PM_PW_MODE_ON);
	rate = 24000000;
	sc_pm_set_clock_rate(ipc_handle, SC_R_GPT_0, 2, &rate);

	err = sc_pm_clock_enable(ipc_handle, SC_R_GPT_0, 2, true, false);

    if (err != SC_ERR_NONE)
    {
        LOG_ERR("Could not enable GPT0 clock");
        return PB_ERR;
    }

    tmr0.base = 0x5D140000;
    tmr0.pr = 24;

    gp_timer_init(&tmr0);

    return err;
}


/* UART Interface */

void plat_uart_putc(void *ptr, char c) 
{
    UNUSED(ptr);
    lpuart_putc(&uart_device, c);
}

