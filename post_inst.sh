#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
RESOURCEDIR=$BASEDIR/Resource
INSTBASE=$HOME/Synthesizer
INSTDIR=$INSTBASE/bin


make_inst()
{
#	return
	OBJ=$1
	cd $BASEDIR/$OBJ/Release
	echo linking
	make $INSTDIR/$OBJ
}

OBJS="comstack Composer OceanGUI"
cd $BASEDIR/Audioserver/Release
echo linking
make $INSTDIR/AudioServer

for OBJ in $OBJS
do
	make_inst $OBJ
done

echo installing resources
cd $BASEDIR/Resource

cp Ocean.png $INSTBASE/lib

for F in *kbd
do
	[ ! -f $INSTBASE/etc/Instruments/$F ] && cp $F $INSTBASE/etc/Instruments
done

for F in main.synth *.inc
do
	[ ! -f $INSTBASE/etc/include/$F ] && cp $F $INSTBASE/etc/include
done
