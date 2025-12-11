#! /usr/bin/env bash

if [ "$OCEANDIR" != "" ]
then
	OCEANP=$OCEANDIR/bin/${ARCH}:$OCEANDIR/bin:
	PATH=$(echo $PATH | sed "s|"$OCEANP"||g")
	OCEANL=$OCEANDIR/lib/$ARCH:
	LD_LIBRARY_PATH=$(echo $LD_LIBRARY_PATH | sed "s|$OCEANL||g")
fi

RCFILE=etc/ocean.rc

cat <<-EOF   > $RCFILE

# $RCFILE created on $(date)

ARCH=$(uname -m)

export OCEANDIR=$(pwd)

OCEANP=$OCEANDIR/bin/${ARCH}:$OCEANDIR/bin:
export PATH=${OCEANP}$PATH

OCEANL=$OCEANDIR/lib/$ARCH:
export LD_LIBRARY_PATH=${OCEANL}$LD_LIBRARY_PATH

EOF