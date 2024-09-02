#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
RESOURCEDIR=$BASEDIR/Resource/
INSTBASE=$HOME/Synthesizer/
INSTDIR=${INSTBASE}bin/

#cd $BASEDIR/OceanGUI/
#./make_moc.sh

make_inst()
{
#	return
	OBJ=$1
	cd $BASEDIR/$OBJ/Release
	echo linking $OBJ
	rm ${INSTDIR}$OBJ
	make ${INSTDIR}$OBJ
}

OBJS="comstack Composer OceanGUI Synthesizer"
cd ${BASEDIR}Audioserver/Release
echo linking
rm ${INSTDIR}AudioServer
make ${INSTDIR}AudioServer

for OBJ in $OBJS
do
	make_inst $OBJ
done

echo installing resources
cd $RESOURCEDIR
cp Ocean.png $INSTBASE/lib

cd $RESOURCEDIR
for F in *kbd
do
	cp ${INSTBASE}/etc/Instruments/*kbd .
	[ ! -f $INSTBASE/etc/Instruments/$F ] && cp $F $INSTBASE/etc/Instruments
done

cd $RESOURCEDIR
for F in main.synth *.inc
do
	cp ${INSTBASE}/etc/include/$F .
	[ ! -f $INSTBASE/etc/include/$F ] && cp $F $INSTBASE/etc/include
done

cd $RESOURCEDIR
cp *odt $INSTBASE/doc