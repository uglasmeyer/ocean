#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
RESOURCEDIR=$BASEDIR/Resource/
INSTBASE=$HOME/OceanBase/



echo installing resources
cd $RESOURCEDIR
cp Ocean.png $INSTBASE/lib
cp oceansetup.sh $INSTBASE/bin

echo updating git Instruments
cd $RESOURCEDIR
for F in *kbd
do
	cp ${INSTBASE}/etc/Instruments/*kbd .
	[ ! -f $INSTBASE/etc/Instruments/$F ] && cp $F $INSTBASE/etc/Instruments
done

echo updating git Notes
cd $RESOURCEDIR/Notes
for F in *kbd
do
	cp ${INSTBASE}/etc/Notes/*kbd .
	[ ! -f $INSTBASE/etc/Notes/$F ] && cp $F $INSTBASE/etc/Notes
done

cd $RESOURCEDIR
for F in main.synth *.inc
do
	cp ${INSTBASE}/etc/include/$F .
	[ ! -f $INSTBASE/etc/include/$F ] && cp $F $INSTBASE/etc/include
done

cd $RESOURCEDIR
cp *odt $INSTBASE/doc
cp oceandeploy.sh $HOME