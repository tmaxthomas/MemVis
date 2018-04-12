#!/bin/bash

cd ../dynamorio-master
cmake .
make -j4
mv drcachetrace.drrun64 /usr/local/DynamoRIO/tools
mv clients/bin64/drcachetrace /usr/local/DynamoRIO/tools/bin64
