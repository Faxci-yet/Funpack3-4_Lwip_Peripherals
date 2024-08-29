################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/altcp.c \
../lwip/src/core/altcp_alloc.c \
../lwip/src/core/altcp_tcp.c \
../lwip/src/core/def.c \
../lwip/src/core/dns.c \
../lwip/src/core/inet_chksum.c \
../lwip/src/core/init.c \
../lwip/src/core/ip.c \
../lwip/src/core/mem.c \
../lwip/src/core/memp.c \
../lwip/src/core/netif.c \
../lwip/src/core/pbuf.c \
../lwip/src/core/raw.c \
../lwip/src/core/stats.c \
../lwip/src/core/sys.c \
../lwip/src/core/tcp.c \
../lwip/src/core/tcp_in.c \
../lwip/src/core/tcp_out.c \
../lwip/src/core/timeouts.c \
../lwip/src/core/udp.c 

C_DEPS += \
./lwip/src/core/altcp.d \
./lwip/src/core/altcp_alloc.d \
./lwip/src/core/altcp_tcp.d \
./lwip/src/core/def.d \
./lwip/src/core/dns.d \
./lwip/src/core/inet_chksum.d \
./lwip/src/core/init.d \
./lwip/src/core/ip.d \
./lwip/src/core/mem.d \
./lwip/src/core/memp.d \
./lwip/src/core/netif.d \
./lwip/src/core/pbuf.d \
./lwip/src/core/raw.d \
./lwip/src/core/stats.d \
./lwip/src/core/sys.d \
./lwip/src/core/tcp.d \
./lwip/src/core/tcp_in.d \
./lwip/src/core/tcp_out.d \
./lwip/src/core/timeouts.d \
./lwip/src/core/udp.d 

OBJS += \
./lwip/src/core/altcp.o \
./lwip/src/core/altcp_alloc.o \
./lwip/src/core/altcp_tcp.o \
./lwip/src/core/def.o \
./lwip/src/core/dns.o \
./lwip/src/core/inet_chksum.o \
./lwip/src/core/init.o \
./lwip/src/core/ip.o \
./lwip/src/core/mem.o \
./lwip/src/core/memp.o \
./lwip/src/core/netif.o \
./lwip/src/core/pbuf.o \
./lwip/src/core/raw.o \
./lwip/src/core/stats.o \
./lwip/src/core/sys.o \
./lwip/src/core/tcp.o \
./lwip/src/core/tcp_in.o \
./lwip/src/core/tcp_out.o \
./lwip/src/core/timeouts.o \
./lwip/src/core/udp.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/%.o: ../lwip/src/core/%.c lwip/src/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DMCUXPRESSO_SDK -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\source" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\p3t1755" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\utilities" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\drivers\flash" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\silicon_id" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\port" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\src" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\src\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\contrib\apps\ping" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\device" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\startup" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\uart" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\lists" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\gpio" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freertos\freertos-kernel\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\CMSIS" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\board" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\controls" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\drivers\tsi" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\electrodes" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\filters" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\keydetectors" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\modules" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\safety" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\system" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\xtalk" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster\platforms" -Og -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-core

clean-lwip-2f-src-2f-core:
	-$(RM) ./lwip/src/core/altcp.d ./lwip/src/core/altcp.o ./lwip/src/core/altcp_alloc.d ./lwip/src/core/altcp_alloc.o ./lwip/src/core/altcp_tcp.d ./lwip/src/core/altcp_tcp.o ./lwip/src/core/def.d ./lwip/src/core/def.o ./lwip/src/core/dns.d ./lwip/src/core/dns.o ./lwip/src/core/inet_chksum.d ./lwip/src/core/inet_chksum.o ./lwip/src/core/init.d ./lwip/src/core/init.o ./lwip/src/core/ip.d ./lwip/src/core/ip.o ./lwip/src/core/mem.d ./lwip/src/core/mem.o ./lwip/src/core/memp.d ./lwip/src/core/memp.o ./lwip/src/core/netif.d ./lwip/src/core/netif.o ./lwip/src/core/pbuf.d ./lwip/src/core/pbuf.o ./lwip/src/core/raw.d ./lwip/src/core/raw.o ./lwip/src/core/stats.d ./lwip/src/core/stats.o ./lwip/src/core/sys.d ./lwip/src/core/sys.o ./lwip/src/core/tcp.d ./lwip/src/core/tcp.o ./lwip/src/core/tcp_in.d ./lwip/src/core/tcp_in.o ./lwip/src/core/tcp_out.d ./lwip/src/core/tcp_out.o ./lwip/src/core/timeouts.d ./lwip/src/core/timeouts.o ./lwip/src/core/udp.d ./lwip/src/core/udp.o

.PHONY: clean-lwip-2f-src-2f-core

