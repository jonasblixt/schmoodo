/**
 * Punch BOOT
 *
 * Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdio.h>
#include <board.h>
#include <plat.h>
#include <gpt.h>
#include <board/config.h>
#include <io.h>

void pb_main(void) 
{
    if (plat_early_init() != PB_OK)
        plat_reset();

    LOG_INFO ("PB: " VERSION " starting...");

    if (gpt_init() != PB_OK)
        flag_run_recovery = true;

    while (true)
    {
        LOG_INFO("Main loop");
    }
    
    plat_reset();
}
