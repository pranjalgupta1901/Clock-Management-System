################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/DS3231.c \
../source/PES_Final_Project.c \
../source/i2c.c \
../source/mtb.c \
../source/oled_driver.c \
../source/project_tasks.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/DS3231.d \
./source/PES_Final_Project.d \
./source/i2c.d \
./source/mtb.d \
./source/oled_driver.d \
./source/project_tasks.d \
./source/semihost_hardfault.d 

OBJS += \
./source/DS3231.o \
./source/PES_Final_Project.o \
./source/i2c.o \
./source/mtb.o \
./source/oled_driver.o \
./source/project_tasks.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\board" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\source" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\freertos" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\drivers" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\CMSIS" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\utilities" -I"C:\Users\admin\Documents\MCUXpressoIDE_11.8.0_1165\workspace\PES_Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/DS3231.d ./source/DS3231.o ./source/PES_Final_Project.d ./source/PES_Final_Project.o ./source/i2c.d ./source/i2c.o ./source/mtb.d ./source/mtb.o ./source/oled_driver.d ./source/oled_driver.o ./source/project_tasks.d ./source/project_tasks.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

