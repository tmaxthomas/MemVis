cmake_minimum_required(VERSION 3.0)
project(IVizFinal CXX)

# Allow C++11 features
if (UNIX)
    list (APPEND CMAKE_CXX_FLAGS "-std=c++11 -m32 ${CMAKE_CXX_FLAGS}")
endif()

include_directories(${CMAKE_SOURCE_DIR})

set (VIZFILE_SRC
        vizFile/vizFile.cpp
        vizFile/vizFile.hpp
        vizFile/io.hpp
        vizFile/types.hpp
)

add_library(VizFile STATIC ${VIZFILE_SRC})

set (VIZTESTGEN_SRC
        vizTestGen/main.cpp
)

add_executable(VizTestGen ${VIZTESTGEN_SRC})
target_link_libraries(VizTestGen VizFile)

set (MEMTEST_SRC
        memTest/main.cpp
)

add_executable(MemTest ${MEMTEST_SRC})
target_compile_definitions(MemTest -mdynamic-no-pic -no_pie)
set(CMAKE_OSX_ARCHITECTURES "i386 x86_64")
set(CMAKE_XCODE_ATTRIBUTE_ARCHS "\$(ARCHS_STANDARD_32_64_BIT)")
