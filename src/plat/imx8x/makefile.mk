#
# Punch BOOT
#
# Copyright (C) 2018 Jonas Persson <jonpe960@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#
#


PB_ARCH_NAME = armv8a

PLAT_C_SRCS  += plat/imx/gpt.c
PLAT_C_SRCS  += plat/imx/lpuart.c
PLAT_C_SRCS  += plat/imx8x/plat.c
PLAT_C_SRCS  += plat/imx8x/sci/ipc.c
PLAT_C_SRCS  += plat/imx8x/sci/mx8_mu.c
PLAT_C_SRCS  += plat/imx8x/sci/svc/pad/pad_rpc_clnt.c
PLAT_C_SRCS  += plat/imx8x/sci/svc/pm/pm_rpc_clnt.c
PLAT_C_SRCS  += plat/imx8x/sci/svc/timer/timer_rpc_clnt.c
PLAT_C_SRCS  += plat/imx8x/sci/svc/misc/misc_rpc_clnt.c

CFLAGS += -D__PLAT_IMX8X__
CFLAGS += -I plat/imx8x/include

plat_clean:
	@-rm -rf plat/imx8x/*.o

plat_final:
	@mkimage_imx8 -commit > head.hash
	@cat $(TARGET).bin head.hash > $(TARGET)_hash.bin
	@mkimage_imx8 -soc QX -rev B0 \
				  -c -scfw scfw_tcm.bin \
				  -ap $(TARGET)_hash.bin a35 0x80000000 -out pb.imx
