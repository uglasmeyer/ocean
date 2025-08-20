################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../common/dataworld.cpp \
../common/register.cpp 

CPP_DEPS += \
./common/dataworld.d \
./common/register.d 

OBJS += \
./common/dataworld.o \
./common/register.o 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.cpp common/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++23 -I"/home/sirius/git/Ocean/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-common

clean-common:
	-$(RM) ./common/dataworld.d ./common/dataworld.o ./common/register.d ./common/register.o

.PHONY: clean-common

