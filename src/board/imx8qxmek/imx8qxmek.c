#include <stdio.h>
#include <pb.h>
#include <io.h>
#include <plat.h>
#include <stdbool.h>
#include <plat/imx8x/plat.h>



uint32_t board_early_init(void)
{



    return PB_OK;
}

uint32_t board_late_init(void)
{


    return PB_OK;
}


uint32_t board_get_debug_uart(void)
{
    return 0x0;
}

