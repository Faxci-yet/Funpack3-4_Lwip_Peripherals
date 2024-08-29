################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freemaster/freemaster_appcmd.c \
../freemaster/freemaster_can.c \
../freemaster/freemaster_net.c \
../freemaster/freemaster_pdbdm.c \
../freemaster/freemaster_pipes.c \
../freemaster/freemaster_protocol.c \
../freemaster/freemaster_rec.c \
../freemaster/freemaster_scope.c \
../freemaster/freemaster_serial.c \
../freemaster/freemaster_sha.c \
../freemaster/freemaster_tsa.c \
../freemaster/freemaster_ures.c \
../freemaster/freemaster_utils.c 

C_DEPS += \
./freemaster/freemaster_appcmd.d \
./freemaster/freemaster_can.d \
./freemaster/freemaster_net.d \
./freemaster/freemaster_pdbdm.d \
./freemaster/freemaster_pipes.d \
./freemaster/freemaster_protocol.d \
./freemaster/freemaster_rec.d \
./freemaster/freemaster_scope.d \
./freemaster/freemaster_serial.d \
./freemaster/freemaster_sha.d \
./freemaster/freemaster_tsa.d \
./freemaster/freemaster_ures.d \
./freemaster/freemaster_utils.d 

OBJS += \
./freemaster/freemaster_appcmd.o \
./freemaster/freemaster_can.o \
./freemaster/freemaster_net.o \
./freemaster/freemaster_pdbdm.o \
./freemaster/freemaster_pipes.o \
./freemaster/freemaster_protocol.o \
./freemaster/freemaster_rec.o \
./freemaster/freemaster_scope.o \
./freemaster/freemaster_serial.o \
./freemaster/freemaster_sha.o \
./freemaster/freemaster_tsa.o \
./freemaster/freemaster_ures.o \
./freemaster/freemaster_utils.o 


# Each subdirectory must supply rules for building sources it contributes
freemaster/%.o: ../freemaster/%.c freemaster/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -D_POSIX_SOURCE -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DMCUXPRESSO_SDK -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\source" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\p3t1755" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\utilities" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\drivers\flash" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\silicon_id" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\port" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\src" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\src\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\lwip\contrib\apps\ping" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\device" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\startup" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\uart" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\lists" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\phy" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\component\gpio" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freertos\freertos-kernel\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\CMSIS" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\board" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\include" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\controls" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\drivers\tsi" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\electrodes" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\filters" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\keydetectors" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\modules" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\safety" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\system" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\touch\source\xtalk" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster\drivers" -I"E:\codeDZB\MCUXpressoIDE_11.9.0_2144\workspace\funpack_lwip_peripherals\freemaster\platforms" -Og -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-freemaster

clean-freemaster:
	-$(RM) ./freemaster/freemaster_appcmd.d ./freemaster/freemaster_appcmd.o ./freemaster/freemaster_can.d ./freemaster/freemaster_can.o ./freemaster/freemaster_net.d ./freemaster/freemaster_net.o ./freemaster/freemaster_pdbdm.d ./freemaster/freemaster_pdbdm.o ./freemaster/freemaster_pipes.d ./freemaster/freemaster_pipes.o ./freemaster/freemaster_protocol.d ./freemaster/freemaster_protocol.o ./freemaster/freemaster_rec.d ./freemaster/freemaster_rec.o ./freemaster/freemaster_scope.d ./freemaster/freemaster_scope.o ./freemaster/freemaster_serial.d ./freemaster/freemaster_serial.o ./freemaster/freemaster_sha.d ./freemaster/freemaster_sha.o ./freemaster/freemaster_tsa.d ./freemaster/freemaster_tsa.o ./freemaster/freemaster_ures.d ./freemaster/freemaster_ures.o ./freemaster/freemaster_utils.d ./freemaster/freemaster_utils.o

.PHONY: clean-freemaster

