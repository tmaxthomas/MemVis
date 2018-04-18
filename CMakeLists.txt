cmake_minimum_required(VERSION 3.0)
project(IVizFinal C CXX)

# Allow C++11 features
if (UNIX)
    list (APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

include_directories(${CMAKE_SOURCE_DIR})

set (VIZFILE_SRC
        vizFile/vizFile.cpp
        vizFile/vizFile.hpp
        vizFile/io.hpp
        vizFile/types.hpp
)

add_library(VizFile STATIC ${VIZFILE_SRC})

set (MEMTEST_SRC
        memTest/main.c
)

add_executable(MemTest ${MEMTEST_SRC})
set_target_properties(MemTest PROPERTIES CMAKE_OSX_ARCHITECTURES "i386" CMAKE_XCODE_ATTRIBUTE_ARCHS "\$(ARCHS_STANDARD_32_BIT)")

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_XCODE_ATTRIBUTE_ARCHS "\$(ARCHS_STANDARD_64_BIT)")

set (VIZRENDERQT_SRC
	vizRenderQt/main.cpp
	vizRenderQt/mainwindow.cpp
	vizRenderQt/mainwindow.h
	vizRenderQt/memDrawing.hpp
	vizRenderQt/memDrawing.cpp
	vizRenderQt/ui_mainwindow.h
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt)
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)

add_executable(VizRenderQT ${VIZRENDERQT_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(VizRenderQT Qt5::Widgets Qt5::Gui Qt5::Core VizFile)
