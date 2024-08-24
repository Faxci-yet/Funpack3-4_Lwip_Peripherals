################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/freertos-kernel/croutine.c \
../freertos/freertos-kernel/event_groups.c \
../freertos/freertos-kernel/list.c \
../freertos/freertos-kernel/queue.c \
../freertos/freertos-kernel/stream_buffer.c \
../freertos/freertos-kernel/tasks.c \
../freertos/freertos-kernel/timers.c 

C_DEPS += \
./freertos/freertos-kernel/croutine.d \
./freertos/freertos-kernel/event_groups.d \
./freertos/freertos-kernel/list.d \
./freertos/freertos-kernel/queue.d \
./freertos/freertos-kernel/stream_buffer.d \
./freertos/freertos-kernel/tasks.d \
./freertos/freertos-kernel/timers.d 

OBJS += \
./freertos/freertos-kernel/croutine.o \
./freertos/freertos-kernel/event_groups.o \
./freertos/freertos-kernel/list.o \
./freertos/freertos-kernel/queue.o \
./freertos/freertos-kernel/stream_buffer.o \
./freertos/freertos-kernel/tasks.o \
./freertos/freertos-kernel/timers.o 


# Each subdirectory must supply rules for building sources it contributes
freertos/freertos-kernel/%.o: ../freertos/freertos-kernel/%.c freertos/freertos-kernel/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DMCUXPRESSO_SDK -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\source" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\p3t1755" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\utilities" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers\flash" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\silicon_id" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\port" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\contrib\apps\ping" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\device" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\startup" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\uart" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\lists" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\gpio" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\CMSIS" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\board" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\controls" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\drivers\tsi" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\electrodes" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\filters" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\keydetectors" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\modules" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\safety" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\system" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\touch\source\xtalk" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freemaster\platforms" -Og -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freertos-2f-freertos-2d-kernel

clean-freertos-2f-freertos-2d-kernel:
	-$(RM) ./freertos/freertos-kernel/croutine.d ./freertos/freertos-kernel/croutine.o ./freertos/freertos-kernel/event_groups.d ./freertos/freertos-kernel/event_groups.o ./freertos/freertos-kernel/list.d ./freertos/freertos-kernel/list.o ./freertos/freertos-kernel/queue.d ./freertos/freertos-kernel/queue.o ./freertos/freertos-kernel/stream_buffer.d ./freertos/freertos-kernel/stream_buffer.o ./freertos/freertos-kernel/tasks.d ./freertos/freertos-kernel/tasks.o ./freertos/freertos-kernel/timers.d ./freertos/freertos-kernel/timers.o

.PHONY: clean-freertos-2f-freertos-2d-kernel

