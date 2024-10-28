#! /usr/bin/env bash

ARCH=`uname -p
for BIN in $ARCH/*
do
	ln -s $ARCH/$BIN $BIN
done`

cat ~/.bashrc | grep -v OCEANDIR > /tmp/rc.tmp
cd ..
echo export OCEANDIR=`pwd` >> /tmp/rc.tmp
echo PATH=$OCEANDIR/bin:$PATH >> /tmp/rc.tmp
mv /tmp/rc.tmp ~/.bashrc
