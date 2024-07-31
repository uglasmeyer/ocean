#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
INSTBASE=$HOME/Synthesizer
INSTDIR=$INSTBASE/bin


make_inst()
{
	OBJ=$1
	cd $BASEDIR/$OBJ/Release
	echo linking
	make $INSTDIR/$OBJ
}

OBJS="Synthesizer comstack Composer AudioServer OceanGUI"

for OBJ in $OBJS;
do
	make_inst $OBJ
done

echo installing resources
cd $BASEDIR/Resource
cp Ocean.png $INSTBASE/lib

