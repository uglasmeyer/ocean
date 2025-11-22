#! /usr/bin/env bash

cd `dirname $0`
/usr/lib/qt6/bin/qmake -o Makefile_Qt6 OceanGUI.pro -spec linux-clang CONFIG+=debug CONFIG+=qml_debug

#make clean
NPROC=`nproc`
make -f Makefile_Qt6 -j$NPROC compiler_uic_make_all
make mocables
