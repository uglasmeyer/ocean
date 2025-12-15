#! /usr/bin/env bash

#cd `dirname $0`
/usr/lib/qt6/bin/qmake -o Makefile_Qt6 OceanGUI.pro -spec linux-clang# CONFIG+=debug CONFIG+=qml_debug

#make clean
make -f Makefile_Qt6 compiler_uic_make_all
make -f Makefile_Qt6 mocables
