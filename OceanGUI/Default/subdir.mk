################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OceanGUI_main.cpp \
../common.cpp \
../cutterdialog.cpp \
../datagraphic.cpp \
../dispatcher.cpp \
../filedialog.cpp \
../keyboad_dialog.cpp \
../mainwindow_init.cpp \
../moc_Cutterdialog.cpp \
../moc_DataGraphic.cpp \
../moc_Dispatcher.cpp \
../moc_File_Dialog.cpp \
../moc_Keyboad_dialog.cpp \
../moc_Mainwindow.cpp \
../moc_Rtsp_dialog.cpp \
../moc_Spectrum_dialog.cpp \
../oceanGUI.cpp \
../qrc_resource.cpp \
../rtsp_dialog_class.cpp \
../spectrum_dialog.cpp 

CPP_DEPS += \
./OceanGUI_main.d \
./common.d \
./cutterdialog.d \
./datagraphic.d \
./dispatcher.d \
./filedialog.d \
./keyboad_dialog.d \
./mainwindow_init.d \
./moc_Cutterdialog.d \
./moc_DataGraphic.d \
./moc_Dispatcher.d \
./moc_File_Dialog.d \
./moc_Keyboad_dialog.d \
./moc_Mainwindow.d \
./moc_Rtsp_dialog.d \
./moc_Spectrum_dialog.d \
./oceanGUI.d \
./qrc_resource.d \
./rtsp_dialog_class.d \
./spectrum_dialog.d 

OBJS += \
./OceanGUI_main.o \
./common.o \
./cutterdialog.o \
./datagraphic.o \
./dispatcher.o \
./filedialog.o \
./keyboad_dialog.o \
./mainwindow_init.o \
./moc_Cutterdialog.o \
./moc_DataGraphic.o \
./moc_Dispatcher.o \
./moc_File_Dialog.o \
./moc_Keyboad_dialog.o \
./moc_Mainwindow.o \
./moc_Rtsp_dialog.o \
./moc_Spectrum_dialog.o \
./oceanGUI.o \
./qrc_resource.o \
./rtsp_dialog_class.o \
./spectrum_dialog.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./OceanGUI_main.d ./OceanGUI_main.o ./common.d ./common.o ./cutterdialog.d ./cutterdialog.o ./datagraphic.d ./datagraphic.o ./dispatcher.d ./dispatcher.o ./filedialog.d ./filedialog.o ./keyboad_dialog.d ./keyboad_dialog.o ./mainwindow_init.d ./mainwindow_init.o ./moc_Cutterdialog.d ./moc_Cutterdialog.o ./moc_DataGraphic.d ./moc_DataGraphic.o ./moc_Dispatcher.d ./moc_Dispatcher.o ./moc_File_Dialog.d ./moc_File_Dialog.o ./moc_Keyboad_dialog.d ./moc_Keyboad_dialog.o ./moc_Mainwindow.d ./moc_Mainwindow.o ./moc_Rtsp_dialog.d ./moc_Rtsp_dialog.o ./moc_Spectrum_dialog.d ./moc_Spectrum_dialog.o ./oceanGUI.d ./oceanGUI.o ./qrc_resource.d ./qrc_resource.o ./rtsp_dialog_class.d ./rtsp_dialog_class.o ./spectrum_dialog.d ./spectrum_dialog.o

.PHONY: clean--2e-

