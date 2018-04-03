cmake_minimum_required(VERSION 3.0)
project(IVizFinal CXX)

# Allow C++11 features
if (UNIX)
    list (APPEND CMAKE_CXX_FLAGS "-std=c++11 -m32 ${CMAKE_CXX_FLAGS}")
endif()

set (VIZFILE_SRC
        vizFile/vizFile.cpp
        vizFile/vizFile.hpp
        vizFile/io.hpp
        vizFile/types.hpp
)

add_library(VizFile SHARED ${VIZFILE_SRC})
