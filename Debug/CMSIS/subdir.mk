################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 

OBJS += \
./CMSIS/system_MKL25Z4.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\freertos" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/system_MKL25Z4.d ./CMSIS/system_MKL25Z4.o

.PHONY: clean-CMSIS

