#! /usr/bin/env bash

cd /home/sirius/git/Ocean/OceanGUI/
/usr/lib/qt6/qmake -o Makefile OceanGUI.pro -spec linux-clang CONFIG+=debug CONFIG+=qml_debug


make clean
make -j1 compiler_uic_make_all
make mocables
cd Release && make -j6 all
