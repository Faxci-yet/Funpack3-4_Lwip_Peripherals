################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/drivers/freemaster_flexcan.c \
../freemaster/drivers/freemaster_mcan.c \
../freemaster/drivers/freemaster_mscan.c \
../freemaster/drivers/freemaster_net_lwip_tcp.c \
../freemaster/drivers/freemaster_net_lwip_udp.c \
../freemaster/drivers/freemaster_net_segger_rtt.c \
../freemaster/drivers/freemaster_serial_lpsci.c \
../freemaster/drivers/freemaster_serial_lpuart.c \
../freemaster/drivers/freemaster_serial_miniusart.c \
../freemaster/drivers/freemaster_serial_uart.c \
../freemaster/drivers/freemaster_serial_usart.c \
../freemaster/drivers/freemaster_serial_usb.c 

C_DEPS += \
./freemaster/drivers/freemaster_flexcan.d \
./freemaster/drivers/freemaster_mcan.d \
./freemaster/drivers/freemaster_mscan.d \
./freemaster/drivers/freemaster_net_lwip_tcp.d \
./freemaster/drivers/freemaster_net_lwip_udp.d \
./freemaster/drivers/freemaster_net_segger_rtt.d \
./freemaster/drivers/freemaster_serial_lpsci.d \
./freemaster/drivers/freemaster_serial_lpuart.d \
./freemaster/drivers/freemaster_serial_miniusart.d \
./freemaster/drivers/freemaster_serial_uart.d \
./freemaster/drivers/freemaster_serial_usart.d \
./freemaster/drivers/freemaster_serial_usb.d 

OBJS += \
./freemaster/drivers/freemaster_flexcan.o \
./freemaster/drivers/freemaster_mcan.o \
./freemaster/drivers/freemaster_mscan.o \
./freemaster/drivers/freemaster_net_lwip_tcp.o \
./freemaster/drivers/freemaster_net_lwip_udp.o \
./freemaster/drivers/freemaster_net_segger_rtt.o \
./freemaster/drivers/freemaster_serial_lpsci.o \
./freemaster/drivers/freemaster_serial_lpuart.o \
./freemaster/drivers/freemaster_serial_miniusart.o \
./freemaster/drivers/freemaster_serial_uart.o \
./freemaster/drivers/freemaster_serial_usart.o \
./freemaster/drivers/freemaster_serial_usb.o 


# Each subdirectory must supply rules for building sources it contributes
freemaster/drivers/%.o: ../freemaster/drivers/%.c freemaster/drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DMCUXPRESSO_SDK -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\source" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\p3t1755" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\utilities" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers\flash" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\silicon_id" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\port" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\contrib\apps\ping" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\device" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\startup" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\uart" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\lists" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\gpio" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\CMSIS" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\board" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\controls" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\drivers\tsi" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\electrodes" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\filters" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\keydetectors" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\modules" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\safety" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\system" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\xtalk" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster\platforms" -Og -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freemaster-2f-drivers

clean-freemaster-2f-drivers:
	-$(RM) ./freemaster/drivers/freemaster_flexcan.d ./freemaster/drivers/freemaster_flexcan.o ./freemaster/drivers/freemaster_mcan.d ./freemaster/drivers/freemaster_mcan.o ./freemaster/drivers/freemaster_mscan.d ./freemaster/drivers/freemaster_mscan.o ./freemaster/drivers/freemaster_net_lwip_tcp.d ./freemaster/drivers/freemaster_net_lwip_tcp.o ./freemaster/drivers/freemaster_net_lwip_udp.d ./freemaster/drivers/freemaster_net_lwip_udp.o ./freemaster/drivers/freemaster_net_segger_rtt.d ./freemaster/drivers/freemaster_net_segger_rtt.o ./freemaster/drivers/freemaster_serial_lpsci.d ./freemaster/drivers/freemaster_serial_lpsci.o ./freemaster/drivers/freemaster_serial_lpuart.d ./freemaster/drivers/freemaster_serial_lpuart.o ./freemaster/drivers/freemaster_serial_miniusart.d ./freemaster/drivers/freemaster_serial_miniusart.o ./freemaster/drivers/freemaster_serial_uart.d ./freemaster/drivers/freemaster_serial_uart.o ./freemaster/drivers/freemaster_serial_usart.d ./freemaster/drivers/freemaster_serial_usart.o ./freemaster/drivers/freemaster_serial_usb.d ./freemaster/drivers/freemaster_serial_usb.o

.PHONY: clean-freemaster-2f-drivers

