#! /usr/bin/env bash

cd /home/sirius/git/Ocean/OceanGUI/
/usr/lib/qt6/bin/qmake -o Makefile OceanGUI.pro -spec linux-clang CONFIG+=debug CONFIG+=qml_debug


#make clean
NPROC=`nproc`
make -j$NPROC compiler_uic_make_all
make mocables
#cd Shlib && make -j6 all
