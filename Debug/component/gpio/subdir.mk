################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/gpio/fsl_adapter_gpio.c 

C_DEPS += \
./component/gpio/fsl_adapter_gpio.d 

OBJS += \
./component/gpio/fsl_adapter_gpio.o 


# Each subdirectory must supply rules for building sources it contributes
component/gpio/%.o: ../component/gpio/%.c component/gpio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DMCUXPRESSO_SDK -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\source" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\utilities" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers\flash" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\silicon_id" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\port" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\src\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\lwip\contrib\apps\ping" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\device" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\startup" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\uart" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\lists" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\component\gpio" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\freertos\freertos-kernel\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\CMSIS" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxn947_lwip_ping_freertos\board" -Og -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-gpio

clean-component-2f-gpio:
	-$(RM) ./component/gpio/fsl_adapter_gpio.d ./component/gpio/fsl_adapter_gpio.o

.PHONY: clean-component-2f-gpio

