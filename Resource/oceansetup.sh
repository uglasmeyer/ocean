#! /usr/bin/env bash

Exit(  )
{
	echo $1
	exit
}

cd ..
OCEANDIR=`pwd`
LIBDIR=$OCEANDIR/lib
BINDIR=$OCEANDIR/bin

ls $OCEANDIR/bin/oceansetup.sh 2>/dev/null || Exit "cannot find oceansetup in $OCEANDIR" 

cd bin 2>/dev/null || Exit "$OCEANDIR has no bin/ directory" 
echo using OCEANDIR=$OCEANDIR

ARCH=`uname -p`
cd $ARCH 2>/dev/null || Exit "$OCEANDIR has no ${ARCH}/ directory" 
echo using architecture $ARCH

BIN=`echo *`
echo having binaries $BIN
cd $BINDIR

for bin in $BIN
do
	if [ ! -f $bin ]
	then
		echo executing ln -s $ARCH/$bin $bin 
		ln -s $ARCH/$bin $bin
	fi
	if [ $bin == Synthesizer ]
	then
		if [ ! -f Keyboard ]
		then
			echo executing ln -s $ARCH/$bin Keyboard 
			ln -s $ARCH/$bin Keyboard
		fi
	fi
done

BASHRC=~/.bashrc
TMPRC=/tmp/rc.tmp
RCDIR=$OCEANDIR/etc/
RCFILE=${RCDIR}ocean.rc

