# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/zt/work/Code2019/CodeCraft2019

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zt/work/Code2019/CodeCraft2019/build

# Include any dependencies generated for this target.
include CMakeFiles/road.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/road.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/road.dir/flags.make

CMakeFiles/road.dir/class/road.cpp.o: CMakeFiles/road.dir/flags.make
CMakeFiles/road.dir/class/road.cpp.o: ../class/road.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zt/work/Code2019/CodeCraft2019/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/road.dir/class/road.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/road.dir/class/road.cpp.o -c /home/zt/work/Code2019/CodeCraft2019/class/road.cpp

CMakeFiles/road.dir/class/road.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/road.dir/class/road.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zt/work/Code2019/CodeCraft2019/class/road.cpp > CMakeFiles/road.dir/class/road.cpp.i

CMakeFiles/road.dir/class/road.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/road.dir/class/road.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zt/work/Code2019/CodeCraft2019/class/road.cpp -o CMakeFiles/road.dir/class/road.cpp.s

CMakeFiles/road.dir/class/road.cpp.o.requires:
.PHONY : CMakeFiles/road.dir/class/road.cpp.o.requires

CMakeFiles/road.dir/class/road.cpp.o.provides: CMakeFiles/road.dir/class/road.cpp.o.requires
	$(MAKE) -f CMakeFiles/road.dir/build.make CMakeFiles/road.dir/class/road.cpp.o.provides.build
.PHONY : CMakeFiles/road.dir/class/road.cpp.o.provides

CMakeFiles/road.dir/class/road.cpp.o.provides.build: CMakeFiles/road.dir/class/road.cpp.o

# Object files for target road
road_OBJECTS = \
"CMakeFiles/road.dir/class/road.cpp.o"

# External object files for target road
road_EXTERNAL_OBJECTS =

libroad.so: CMakeFiles/road.dir/class/road.cpp.o
libroad.so: CMakeFiles/road.dir/build.make
libroad.so: CMakeFiles/road.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libroad.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/road.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/road.dir/build: libroad.so
.PHONY : CMakeFiles/road.dir/build

CMakeFiles/road.dir/requires: CMakeFiles/road.dir/class/road.cpp.o.requires
.PHONY : CMakeFiles/road.dir/requires

CMakeFiles/road.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/road.dir/cmake_clean.cmake
.PHONY : CMakeFiles/road.dir/clean

CMakeFiles/road.dir/depend:
	cd /home/zt/work/Code2019/CodeCraft2019/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zt/work/Code2019/CodeCraft2019 /home/zt/work/Code2019/CodeCraft2019 /home/zt/work/Code2019/CodeCraft2019/build /home/zt/work/Code2019/CodeCraft2019/build /home/zt/work/Code2019/CodeCraft2019/build/CMakeFiles/road.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/road.dir/depend

