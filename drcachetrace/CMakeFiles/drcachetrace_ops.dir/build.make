# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/max/Documents/vis/dynamorio-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/max/Documents/vis/dynamorio-master

# Include any dependencies generated for this target.
include clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/depend.make

# Include the progress variables for this target.
include clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/progress.make

# Include the compile flags for this target's objects.
include clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/flags.make

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/flags.make
clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o: clients/drcachesim/optionlist.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Documents/vis/dynamorio-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o -c /home/max/Documents/vis/dynamorio-master/clients/drcachesim/optionlist.cpp

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.i"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/max/Documents/vis/dynamorio-master/clients/drcachesim/optionlist.cpp > CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.i

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.s"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/max/Documents/vis/dynamorio-master/clients/drcachesim/optionlist.cpp -o CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.s

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.requires:

.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.requires

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.provides: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.requires
	$(MAKE) -f clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/build.make clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.provides.build
.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.provides

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.provides.build: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o


clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/flags.make
clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o: clients/drcachesim/common/options.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/max/Documents/vis/dynamorio-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -m64 -fno-strict-aliasing -fno-stack-protector -fvisibility=internal -std=c++11 -O3 -g3 -Wall -Werror -Wwrite-strings -Wno-unused-but-set-variable -m64 -std=c++11 -std=c++11 -o CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o -c /home/max/Documents/vis/dynamorio-master/clients/drcachesim/common/options.cpp

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/drcachetrace_ops.dir/common/options.cpp.i"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -m64 -fno-strict-aliasing -fno-stack-protector -fvisibility=internal -std=c++11 -O3 -g3 -Wall -Werror -Wwrite-strings -Wno-unused-but-set-variable -m64 -std=c++11 -std=c++11 -E /home/max/Documents/vis/dynamorio-master/clients/drcachesim/common/options.cpp > CMakeFiles/drcachetrace_ops.dir/common/options.cpp.i

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/drcachetrace_ops.dir/common/options.cpp.s"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -m64 -fno-strict-aliasing -fno-stack-protector -fvisibility=internal -std=c++11 -O3 -g3 -Wall -Werror -Wwrite-strings -Wno-unused-but-set-variable -m64 -std=c++11 -std=c++11 -S /home/max/Documents/vis/dynamorio-master/clients/drcachesim/common/options.cpp -o CMakeFiles/drcachetrace_ops.dir/common/options.cpp.s

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.requires:

.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.requires

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.provides: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.requires
	$(MAKE) -f clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/build.make clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.provides.build
.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.provides

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.provides.build: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o


# Object files for target drcachetrace_ops
drcachetrace_ops_OBJECTS = \
"CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o" \
"CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o"

# External object files for target drcachetrace_ops
drcachetrace_ops_EXTERNAL_OBJECTS =

clients/bin64/drcachetrace_ops: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o
clients/bin64/drcachetrace_ops: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o
clients/bin64/drcachetrace_ops: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/build.make
clients/bin64/drcachetrace_ops: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/max/Documents/vis/dynamorio-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin64/drcachetrace_ops"
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/drcachetrace_ops.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/build: clients/bin64/drcachetrace_ops

.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/build

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/requires: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/optionlist.cpp.o.requires
clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/requires: clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/common/options.cpp.o.requires

.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/requires

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/clean:
	cd /home/max/Documents/vis/dynamorio-master/clients/drcachesim && $(CMAKE_COMMAND) -P CMakeFiles/drcachetrace_ops.dir/cmake_clean.cmake
.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/clean

clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/depend:
	cd /home/max/Documents/vis/dynamorio-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/max/Documents/vis/dynamorio-master /home/max/Documents/vis/dynamorio-master/clients/drcachesim /home/max/Documents/vis/dynamorio-master /home/max/Documents/vis/dynamorio-master/clients/drcachesim /home/max/Documents/vis/dynamorio-master/clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : clients/drcachesim/CMakeFiles/drcachetrace_ops.dir/depend

