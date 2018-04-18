#!/bin/bash
# NOTE: Paths beginning with '/' are assumed to be absolute paths from root
# Prepend with '.' to use a local path

if [ $# -ne 2 ] ; then
    echo "ERROR"
    echo "Usage: <program> <out_file>"
    exit 1
fi

if [ "X$DRROOT" = "X" ] ; then
	echo "Need to specify DRROOT (contains bin32/bin64 directory)."
	echo "Run as:"
	echo "  DRROOT=/path/to/dynamorio $0 $*"
	exit 1
fi

DIR=$(pwd)

cd $DRROOT/bin*
if [ "${1:0:1}" = "/" ] ; then
    drrun -t drcachetrace -- $1
else
    drrun -t drcachetrace -- "${DIR}/$1"
fi
mv /tmp/tmp.vzf $2