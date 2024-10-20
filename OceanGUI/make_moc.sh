#! /usr/bin/env bash

cd /home/sirius/git/Ocean/OceanGUI/
/usr/lib/qt5/bin/qmake -o Makefile OceanGUI.pro -spec linux-clang CONFIG+=debug CONFIG+=qml_debug


make clean
make -j1 compiler_uic_make_all
make mocables
#make OceanGUI
