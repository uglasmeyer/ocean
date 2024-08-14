################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../External.cpp \
../GUIinterface.cpp \
../Instrument.cpp \
../Interpreter.cpp \
../Logfacility.cpp \
../Notes.cpp \
../Notesbase.cpp \
../Oscbase.cpp \
../Record.cpp \
../String.cpp \
../Variation.cpp \
../Wavedisplay.cpp \
../common.cpp \
../kbd.cpp \
../mixer.cpp \
../osc.cpp \
../processor.cpp \
../spectrum.cpp \
../synthmem.cpp 

CPP_DEPS += \
./External.d \
./GUIinterface.d \
./Instrument.d \
./Interpreter.d \
./Logfacility.d \
./Notes.d \
./Notesbase.d \
./Oscbase.d \
./Record.d \
./String.d \
./Variation.d \
./Wavedisplay.d \
./common.d \
./kbd.d \
./mixer.d \
./osc.d \
./processor.d \
./spectrum.d \
./synthmem.d 

OBJS += \
./External.o \
./GUIinterface.o \
./Instrument.o \
./Interpreter.o \
./Logfacility.o \
./Notes.o \
./Notesbase.o \
./Oscbase.o \
./Record.o \
./String.o \
./Variation.o \
./Wavedisplay.o \
./common.o \
./kbd.o \
./mixer.o \
./osc.o \
./processor.o \
./spectrum.o \
./synthmem.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I/home/sirius/git/Ocean/libSynthesizer/include -O2 -g -Wall -c -fmessage-length=0 -fPIC -fstack-protector-all -Wformat=2 -Wformat-security -Wstrict-overflow -fPIE -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./External.d ./External.o ./GUIinterface.d ./GUIinterface.o ./Instrument.d ./Instrument.o ./Interpreter.d ./Interpreter.o ./Logfacility.d ./Logfacility.o ./Notes.d ./Notes.o ./Notesbase.d ./Notesbase.o ./Oscbase.d ./Oscbase.o ./Record.d ./Record.o ./String.d ./String.o ./Variation.d ./Variation.o ./Wavedisplay.d ./Wavedisplay.o ./common.d ./common.o ./kbd.d ./kbd.o ./mixer.d ./mixer.o ./osc.d ./osc.o ./processor.d ./processor.o ./spectrum.d ./spectrum.o ./synthmem.d ./synthmem.o

.PHONY: clean--2e-

