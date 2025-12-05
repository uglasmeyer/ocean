QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += -std=c++2b

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
 \#    OceanGUI_main.cpp
    cutterdialog.cpp \
    keyboad_dialog.cpp

HEADERS += \
    Cutterdialog.h \
    include/Keyboad_dialog.h \
    include/File_Dialog.h \
    include/Mainwindow.h \
    include/Oszilloscopewidget.h \
    include/Spectrum_dialog.h \
    include/Rtsp_dialog.h \
    include/Dispatcher.h

FORMS += \
    File_Dialog_class.ui \
    cutterdialog.ui \
    keyboad_dialog.ui \
    mainwindow.ui \
    Rtsp_dialog_class.ui \
    spectrum_dialog_class.ui


CONFIG += lrelease

INCLUDEPATH += /home/sirius/git/Ocean/include

LIBS += -L"/home/sirius/OceanDev/lib" -lOcean
#  LIBS += /home/sirius/git/Ocean/Release/libOcean.a
#  http://doc.qt.io/qt-5/third-party-libraries.html

OBJECTS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


