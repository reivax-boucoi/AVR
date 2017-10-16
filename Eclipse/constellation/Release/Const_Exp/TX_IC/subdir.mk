################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Const_Exp/TX_IC/TX_IC.c 

C_DEPS += \
./Const_Exp/TX_IC/TX_IC.d 

OBJS += \
./Const_Exp/TX_IC/TX_IC.o 


# Each subdirectory must supply rules for building sources it contributes
Const_Exp/TX_IC/%.o: ../Const_Exp/TX_IC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=attiny1634 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


