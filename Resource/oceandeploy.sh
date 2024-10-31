#! /usr/bin/env bash
VERSION_TXT=$OCEANDIR/etc/version.txt

EXCLUDE=/tmp/ecxlude
echo ".git
archive
wav
log
ifd_data*
auto
tmp" > $EXCLUDE

VERSION=`cat $VERSION_TXT | grep Version: | cut -d: -f2`
VERSION=`echo $VERSION`
tar --exclude-from=$EXCLUDE -cvf ocean_sound_lab_${VERSION}.tar OceanBase
