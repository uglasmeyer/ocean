#! /usr/bin/env bash
GITDIR=$HOME/git/Ocean/
RESOURCEDIR=$GITDIR/Resource/
INSTBASE=$HOME/OceanBase/

LINES=$(cat `find $GITDIR -name "*.h" ; find $GITDIR -name "*.cpp"` | wc -l)
H_FILES=$(find $GITDIR -name "*.h" | wc -l)
CPP_FILES=$(find $GITDIR -name "*.cpp" | wc -l)

echo "lines of code: " $LINES 
echo "header file  : " $H_FILES
echo "cpp files    : " $CPP_FILES

exit

echo installing resources
cd $RESOURCEDIR
cp Ocean.png $INSTBASE/lib
cp oceansetup.sh $INSTBASE/bin
cp clearipc.sh $INSTBASE/bin




echo updating git Instruments
DIR=$INSTBASE/etc/Instruments
cd $RESOURCEDIR/Instruments
for F in *snd
do
	cp $DIR/*snd .
done

echo updating git Notes
cd $RESOURCEDIR/Notes
for F in *nte
do
	cp ${INSTBASE}/etc/Notes/*nte .
done

echo updating git etc directory
cd $RESOURCEDIR
for F in synthesizer.cfg
do
#	cp ${INSTBASE}/etc/$F .
	cp $F $INSTBASE/etc
done

echo updating git include directory
cd $RESOURCEDIR
for F in main.synth *.inc
do
	cp ${INSTBASE}/etc/include/$F .
	[ ! -f $INSTBASE/etc/include/$F ] && cp $F $INSTBASE/etc/include
done

cd $RESOURCEDIR
cp *odt  $INSTBASE/doc
cp oceandeploy.sh $HOME



