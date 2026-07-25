#! /usr/bin/env bash

DBGDIR=$HOME/OceanDbg/
export OCEANDIR=$DBGDIR
PATH=$OCEANDIR/bin/x86_64/:$PATH
LD_LIBRARY_PATH=$OCEANDIR/lib/x86_64:$LD_LIBRARY_PATH

( cd $OCEANSRC/Debug; make )

if [ "$1" == "" ]
then
	gdb Synthesizer
else
	gdb $1
fi