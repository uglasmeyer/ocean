#! /usr/bin/env bash

cat <<-EOF

MIT License

Copyright (c) 2025 Ulrich Glasmeyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

EOF

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

cp $OCEANDIR/etc/OceanGUI.desktop $HOME/Desktop
chmod 755 $HOME/Desktop/OceanGUI.desktop
mkdir -p $HOME/.local/share/applications
cp $OCEANDIR/etc/OceanGUI.desktop $HOME/.local/share/applications/
chmod 755 $HOME/.local/share/applications/OceanGUI.desktop

desktop-file-validate $HOME/.local/share/applications/OceanGUI.desktop

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
