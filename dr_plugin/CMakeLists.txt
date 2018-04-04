cmake_minimum_required(VERSION 3.0)
project(IVizFinal CXX)

set (VIZFILE_SRC
        main.cpp
)

add_definitions(-Wall)

add_library(memtrace SHARED main.cpp)
find_package(DynamoRIO)
if(NOT DynamoRIO_FOUND)
        message(FATAL_ERROR "DynamoRIO package required to build")
endif(NOT DynamoRIO_FOUND)
configure_DynamoRIO_client(memtrace)
use_DynamoRIO_extension(memtrace drmgr)
use_DynamoRIO_extension(memtrace drreg)
use_DynamoRIO_extension(memtrace drutil)
use_DynamoRIO_extension(memtrace drx)
