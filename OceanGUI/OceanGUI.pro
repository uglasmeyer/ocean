QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += -std=c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    File_Dialog.cpp \
    OscilloscopeWidget.cpp \
    main.cpp \
    SndlabGUI.cpp \
    spectrum_dialog_class.cpp

HEADERS += \
    File_Dialog.h \
    mainwindow.h \
    oszilloscopewidget.h \
    spectrum_dialog_class.h

FORMS += \
    File_Dialog.ui \
    mainwindow.ui \
    spectrum_dialog_class.ui


CONFIG += lrelease

INCLUDEPATH += /home/sirius/git/Ocean/include \
    /home/sirius/git/Ocean/OceanGUI


LIBS += /home/sirius/git/Ocean/Release/libOcean.a
#  http://doc.qt.io/qt-5/third-party-libraries.html

OBJECTS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


