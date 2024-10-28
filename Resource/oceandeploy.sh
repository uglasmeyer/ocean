#! /usr/bin/env bash

EXCLUDE=/tmp/ecxlude
echo ".git
archive
wav
log
ifd_data*
auto
tmp" > $EXCLUDE

tar --exclude-from=$EXCLUDE -cvf Ocean.tar OceanBase
