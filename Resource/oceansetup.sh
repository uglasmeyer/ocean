#! /usr/bin/env bash

cd ..
OCEANDIR=`pwd`
cd bin || exit
echo using OCEANDIR=$OCEANDIR

ARCH=`uname -p`
echo using architecture $ARCH
cd $ARCH || exit

BIN=`echo *`
echo having binaries $BIN
cd ..

for bin in $BIN
do
	if [ ! -f $bin ]
	then
		echo executing ln -s $ARCH/$bin $bin 
		ln -s $ARCH/$bin $bin
	fi
done

BASHRC=~/.bashrc
TMPRC=/tmp/rc.tmp
RCFILE=$OCEANDIR/etc/ocean.rc

echo generating ocean.rc
echo export OCEANDIR=$OCEANDIR >> $RCFILE
echo export PATH=$OCEANDIR/bin:$PATH >> $RCFILE

echo updating $BASHRC
cat $BASHRC | grep -v ocean.rc > $TMPRC
echo . $OCEANDIR/etc/ocean.rc >> $TMPRC
mv $TMPRC $BASHRC
