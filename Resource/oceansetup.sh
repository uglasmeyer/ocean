#! /usr/bin/env bash

Exit(  )
{
	echo $1
	exit
}

cd ..
OCEANDIR=`pwd`
ls $OCEANDIR/bin/oceansetup.sh 2>/dev/null || Exit "cannot find oceansetup in $OCEANDIR" 

cd bin 2>/dev/null || Exit "$OCEANDIR has no bin/ directory" 
echo using OCEANDIR=$OCEANDIR

ARCH=`uname -p`
cd $ARCH 2>/dev/null || Exit "$OCEANDIR has no ${ARCH}/ directory" 
echo using architecture $ARCH

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
RCDIR=$OCEANDIR/etc/
RCFILE=${RCDIR}ocean.rc


echo generating ocean.rc
[ "$PATH_BACKUP" == "" ] && PATH_BACKUP=$PATH
[ ! -d $RCDIR ] && mkdir $RCDIR
echo export OCEANTESTCASE=oceantestcase 	> $RCFILE
echo export OCEANDIR=$OCEANDIR 				>> $RCFILE
echo export PATH=$OCEANDIR/bin:$PATH_BACKUP >> $RCFILE

echo updating $BASHRC
cat $BASHRC | grep -v ocean.rc\
			| grep -v PATH_BACKUP> $TMPRC
echo PATH_BACKUP=$PATH			>> $TMPRC
echo . $OCEANDIR/etc/ocean.rc 	>> $TMPRC
mv $TMPRC $BASHRC
