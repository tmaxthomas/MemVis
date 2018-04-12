#!/bin/bash
# NOTE: Paths beginning with '/' are assumed to be absolute paths from root
# Prepend with '.' to use a local path


if [ $# -ne 2 ] ; then
    echo "ERROR\n"
    echo "Usage: <program> <out_file>"
    exit
fi

DIR=$(pwd)

cd /usr/local/DynamoRIO/bin64
if [ "${1:0:1}" = "/" ] ; then
    drrun -t drcachetrace -- $1 
else
    drrun -t drcachetrace -- "${DIR}/$1"
fi 
mv /tmp/tmp.vzf ${DIR}/$2
