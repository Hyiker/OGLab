# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.22.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.22.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sidicefhh/Projects/OGLab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sidicefhh/Projects/OGLab/build

# Include any dependencies generated for this target.
include lib/glew/CMakeFiles/libglewmx_static.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/glew/CMakeFiles/libglewmx_static.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/glew/CMakeFiles/libglewmx_static.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glew/CMakeFiles/libglewmx_static.dir/flags.make

lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o: lib/glew/CMakeFiles/libglewmx_static.dir/flags.make
lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o: ../lib/glew/src/glew.c
lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o: lib/glew/CMakeFiles/libglewmx_static.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sidicefhh/Projects/OGLab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o -MF CMakeFiles/libglewmx_static.dir/src/glew.c.o.d -o CMakeFiles/libglewmx_static.dir/src/glew.c.o -c /Users/sidicefhh/Projects/OGLab/lib/glew/src/glew.c

lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libglewmx_static.dir/src/glew.c.i"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sidicefhh/Projects/OGLab/lib/glew/src/glew.c > CMakeFiles/libglewmx_static.dir/src/glew.c.i

lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libglewmx_static.dir/src/glew.c.s"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sidicefhh/Projects/OGLab/lib/glew/src/glew.c -o CMakeFiles/libglewmx_static.dir/src/glew.c.s

# Object files for target libglewmx_static
libglewmx_static_OBJECTS = \
"CMakeFiles/libglewmx_static.dir/src/glew.c.o"

# External object files for target libglewmx_static
libglewmx_static_EXTERNAL_OBJECTS =

lib/libglewmxd.a: lib/glew/CMakeFiles/libglewmx_static.dir/src/glew.c.o
lib/libglewmxd.a: lib/glew/CMakeFiles/libglewmx_static.dir/build.make
lib/libglewmxd.a: lib/glew/CMakeFiles/libglewmx_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sidicefhh/Projects/OGLab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../libglewmxd.a"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && $(CMAKE_COMMAND) -P CMakeFiles/libglewmx_static.dir/cmake_clean_target.cmake
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libglewmx_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glew/CMakeFiles/libglewmx_static.dir/build: lib/libglewmxd.a
.PHONY : lib/glew/CMakeFiles/libglewmx_static.dir/build

lib/glew/CMakeFiles/libglewmx_static.dir/clean:
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glew && $(CMAKE_COMMAND) -P CMakeFiles/libglewmx_static.dir/cmake_clean.cmake
.PHONY : lib/glew/CMakeFiles/libglewmx_static.dir/clean

lib/glew/CMakeFiles/libglewmx_static.dir/depend:
	cd /Users/sidicefhh/Projects/OGLab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sidicefhh/Projects/OGLab /Users/sidicefhh/Projects/OGLab/lib/glew /Users/sidicefhh/Projects/OGLab/build /Users/sidicefhh/Projects/OGLab/build/lib/glew /Users/sidicefhh/Projects/OGLab/build/lib/glew/CMakeFiles/libglewmx_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glew/CMakeFiles/libglewmx_static.dir/depend

