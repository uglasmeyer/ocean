################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OceanGUI_main.cpp \
../filedialog.cpp \
../moc_File_Dialog.cpp \
../moc_Mainwindow.cpp \
../moc_Rtsp_dialog_class.cpp \
../moc_Spectrum_dialog_class.cpp \
../oceanGUI.cpp \
../oscilloscopeWidget.cpp \
../rtsp_dialog_class.cpp \
../spectrum_dialog_class.cpp 

CPP_DEPS += \
./OceanGUI_main.d \
./filedialog.d \
./moc_File_Dialog.d \
./moc_Mainwindow.d \
./moc_Rtsp_dialog_class.d \
./moc_Spectrum_dialog_class.d \
./oceanGUI.d \
./oscilloscopeWidget.d \
./rtsp_dialog_class.d \
./spectrum_dialog_class.d 

OBJS += \
./OceanGUI_main.o \
./filedialog.o \
./moc_File_Dialog.o \
./moc_Mainwindow.o \
./moc_Rtsp_dialog_class.o \
./moc_Spectrum_dialog_class.o \
./oceanGUI.o \
./oscilloscopeWidget.o \
./rtsp_dialog_class.o \
./spectrum_dialog_class.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./OceanGUI_main.d ./OceanGUI_main.o ./filedialog.d ./filedialog.o ./moc_File_Dialog.d ./moc_File_Dialog.o ./moc_Mainwindow.d ./moc_Mainwindow.o ./moc_Rtsp_dialog_class.d ./moc_Rtsp_dialog_class.o ./moc_Spectrum_dialog_class.d ./moc_Spectrum_dialog_class.o ./oceanGUI.d ./oceanGUI.o ./oscilloscopeWidget.d ./oscilloscopeWidget.o ./rtsp_dialog_class.d ./rtsp_dialog_class.o ./spectrum_dialog_class.d ./spectrum_dialog_class.o

.PHONY: clean--2e-

