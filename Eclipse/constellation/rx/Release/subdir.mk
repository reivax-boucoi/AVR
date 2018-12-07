################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Kmean.cpp \
../Kmean_test.cpp \
../rx.cpp \
../uart.cpp 

OBJS += \
./Kmean.o \
./Kmean_test.o \
./rx.o \
./uart.o 

CPP_DEPS += \
./Kmean.d \
./Kmean_test.d \
./rx.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -std=c++17 -mmcu=attiny1634 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


