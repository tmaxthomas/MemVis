#!/bin/bash -x

if [ $# -ne 2 ]; then
    echo "ERROR\n"
    echo "Usage: <program> <out_file>"
    exit
fi

DIR=$(pwd)

cd /usr/local/DynamoRIO/bin64

drrun -c ${DIR}/libprojplugin.so /tmp/tmp.vzf -- "$1"
drrun -t drcachetrace -- "$1" 

mv /tmp/tmp.vzf ${DIR}/$2
