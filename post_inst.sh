#! /usr/bin/env bash
BASEDIR=$HOME/git/Ocean/
INSTDIR=$HOME/Synthesizer/bin

cd $BASEDIR/Synthesizer/Release
rm Synthesizer
make all
cp Synthesizer $INSTDIR

cd $BASEDIR/Composer/Release
rm Composer
make all
cp Composer $INSTDIR

cd $BASEDIR/Audioserver/Release
rm AudioServer
make all
cp AudioServer $INSTDIR

cd $BASEDIR/comstack/Release
rm ifdstruct
make all
cp ifdstruct $INSTDIR


