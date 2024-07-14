#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
INSTDIR=$HOME/Synthesizer/bin


make_inst()
{
OBJ=$1
cd $BASEDIR/$OBJ/Release
#rm $OBJ
make all
echo installing $OBJ
cp -f $OBJ $INSTDIR
}

OBJS="Synthesizer comstack Composer AudioServer OceanGUI"

for OBJ in $OBJS;
do
make_inst $OBJ
done

