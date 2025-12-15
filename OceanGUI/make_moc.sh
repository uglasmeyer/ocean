#! /usr/bin/env bash

#cd `dirname $0`
/usr/lib/qt6/bin/qmake -o Makefile_qt6 OceanGUI.pro -spec linux-clang

#make clean
make -f Makefile_qt6 compiler_uic_make_all
make -f Makefile_qt6 mocables
make -f Makefile_qt6 compiler_rcc_make_all
