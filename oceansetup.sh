#! /usr/bin/env bash

if [ "$OCEANDIR" != "" ]
then
	ARCH=$(uname -m)
	export OCEANP=$OCEANDIR/bin/${ARCH}:
	PATH=$(echo $PATH | sed "s|"$OCEANP"||g")
	OCEANP=$OCEANDIR/bin:
	PATH=$(echo $PATH | sed "s|"$OCEANP"||g")
	export OCEANL=$OCEANDIR/lib/$ARCH:
	LD_LIBRARY_PATH=$(echo $LD_LIBRARY_PATH | sed "s|$OCEANL||g")
fi

RCFILE=etc/ocean.rc

export OCEANDIR=$(pwd)
OCEANP=$OCEANDIR/bin/${ARCH}:$OCEANDIR/bin:
OCEANL=$OCEANDIR/lib/$ARCH:
cat <<-EOF   > $RCFILE

# $RCFILE created on $(date)

export MAKEFLAGS=-j$(nproc)

export ARCH=$(uname -m)

export OCEANDIR=$(pwd)

export PATH=${OCEANP}$PATH

export LD_LIBRARY_PATH=${OCEANL}$LD_LIBRARY_PATH

EOF
