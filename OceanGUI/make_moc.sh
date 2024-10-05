#! /usr/bin/env bash

cd /home/sirius/git/Ocean/OceanGUI/

make clean
make -j1 compiler_uic_make_all
make mocables
#make OceanGUI
