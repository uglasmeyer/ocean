#! /usr/bin/env bash

ipcrm -a
ipcs 
rm $OCEANDIR/lib/ifd*
