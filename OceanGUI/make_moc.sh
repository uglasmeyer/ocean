#! /usr/bin/env bash

cd /home/sirius/git/Ocean/OceanGUI/
#touch mainwindow.ui
#touch File_Dialog.ui
make compiler_uic_make_all
make mocables
make OceanGUI