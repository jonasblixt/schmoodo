#include <stdio.h>
#include <board.h>
#include <plat.h>
#include <board/config.h>
#include <io.h>

void pb_main(void) 
{
    plat_early_init();

    LOG_INFO ("SCHMOODO: " VERSION " starting...");

    while (true)
    {
        LOG_INFO("Main loop");
    }
    
    plat_reset();
}
