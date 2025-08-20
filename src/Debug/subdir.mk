################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../adsr.cpp \
../app.cpp \
../appstate.cpp \
../apptest.cpp \
../config.cpp \
../dynamic.cpp \
../eventhandler.cpp \
../external.cpp \
../fft.cpp \
../frequency.cpp \
../instrument.cpp \
../interface.cpp \
../interpreter.cpp \
../kbd.cpp \
../keyboard.cpp \
../keyhandler.cpp \
../logfacility.cpp \
../memory.cpp \
../memorybase.cpp \
../mixer.cpp \
../musicxml.cpp \
../notes.cpp \
../notesbase.cpp \
../notescore.cpp \
../osc.cpp \
../oscbase.cpp \
../oscgroup.cpp \
../oscwaveform.cpp \
../processor.cpp \
../progressbar.cpp \
../semaphore.cpp \
../spectrum.cpp \
../statistic.cpp \
../string.cpp \
../system.cpp \
../table.cpp \
../time.cpp \
../variation.cpp \
../viewinterface.cpp \
../wavedisplay.cpp 

CPP_DEPS += \
./adsr.d \
./app.d \
./appstate.d \
./apptest.d \
./config.d \
./dynamic.d \
./eventhandler.d \
./external.d \
./fft.d \
./frequency.d \
./instrument.d \
./interface.d \
./interpreter.d \
./kbd.d \
./keyboard.d \
./keyhandler.d \
./logfacility.d \
./memory.d \
./memorybase.d \
./mixer.d \
./musicxml.d \
./notes.d \
./notesbase.d \
./notescore.d \
./osc.d \
./oscbase.d \
./oscgroup.d \
./oscwaveform.d \
./processor.d \
./progressbar.d \
./semaphore.d \
./spectrum.d \
./statistic.d \
./string.d \
./system.d \
./table.d \
./time.d \
./variation.d \
./viewinterface.d \
./wavedisplay.d 

OBJS += \
./adsr.o \
./app.o \
./appstate.o \
./apptest.o \
./config.o \
./dynamic.o \
./eventhandler.o \
./external.o \
./fft.o \
./frequency.o \
./instrument.o \
./interface.o \
./interpreter.o \
./kbd.o \
./keyboard.o \
./keyhandler.o \
./logfacility.o \
./memory.o \
./memorybase.o \
./mixer.o \
./musicxml.o \
./notes.o \
./notesbase.o \
./notescore.o \
./osc.o \
./oscbase.o \
./oscgroup.o \
./oscwaveform.o \
./processor.o \
./progressbar.o \
./semaphore.o \
./spectrum.o \
./statistic.o \
./string.o \
./system.o \
./table.o \
./time.o \
./variation.o \
./viewinterface.o \
./wavedisplay.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++23 -I"/home/sirius/git/Ocean/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./adsr.d ./adsr.o ./app.d ./app.o ./appstate.d ./appstate.o ./apptest.d ./apptest.o ./config.d ./config.o ./dynamic.d ./dynamic.o ./eventhandler.d ./eventhandler.o ./external.d ./external.o ./fft.d ./fft.o ./frequency.d ./frequency.o ./instrument.d ./instrument.o ./interface.d ./interface.o ./interpreter.d ./interpreter.o ./kbd.d ./kbd.o ./keyboard.d ./keyboard.o ./keyhandler.d ./keyhandler.o ./logfacility.d ./logfacility.o ./memory.d ./memory.o ./memorybase.d ./memorybase.o ./mixer.d ./mixer.o ./musicxml.d ./musicxml.o ./notes.d ./notes.o ./notesbase.d ./notesbase.o ./notescore.d ./notescore.o ./osc.d ./osc.o ./oscbase.d ./oscbase.o ./oscgroup.d ./oscgroup.o ./oscwaveform.d ./oscwaveform.o ./processor.d ./processor.o ./progressbar.d ./progressbar.o ./semaphore.d ./semaphore.o ./spectrum.d ./spectrum.o ./statistic.d ./statistic.o ./string.d ./string.o ./system.d ./system.o ./table.d ./table.o ./time.d ./time.o ./variation.d ./variation.o ./viewinterface.d ./viewinterface.o ./wavedisplay.d ./wavedisplay.o

.PHONY: clean--2e-

