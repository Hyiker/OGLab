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
include lib/glfw/tests/CMakeFiles/icon.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/glfw/tests/CMakeFiles/icon.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/glfw/tests/CMakeFiles/icon.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw/tests/CMakeFiles/icon.dir/flags.make

lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o: lib/glfw/tests/CMakeFiles/icon.dir/flags.make
lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o: ../lib/glfw/tests/icon.c
lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o: lib/glfw/tests/CMakeFiles/icon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sidicefhh/Projects/OGLab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o -MF CMakeFiles/icon.dir/icon.c.o.d -o CMakeFiles/icon.dir/icon.c.o -c /Users/sidicefhh/Projects/OGLab/lib/glfw/tests/icon.c

lib/glfw/tests/CMakeFiles/icon.dir/icon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/icon.dir/icon.c.i"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sidicefhh/Projects/OGLab/lib/glfw/tests/icon.c > CMakeFiles/icon.dir/icon.c.i

lib/glfw/tests/CMakeFiles/icon.dir/icon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/icon.dir/icon.c.s"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sidicefhh/Projects/OGLab/lib/glfw/tests/icon.c -o CMakeFiles/icon.dir/icon.c.s

lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o: lib/glfw/tests/CMakeFiles/icon.dir/flags.make
lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o: ../lib/glfw/deps/glad.c
lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o: lib/glfw/tests/CMakeFiles/icon.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sidicefhh/Projects/OGLab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o -MF CMakeFiles/icon.dir/__/deps/glad.c.o.d -o CMakeFiles/icon.dir/__/deps/glad.c.o -c /Users/sidicefhh/Projects/OGLab/lib/glfw/deps/glad.c

lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/icon.dir/__/deps/glad.c.i"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sidicefhh/Projects/OGLab/lib/glfw/deps/glad.c > CMakeFiles/icon.dir/__/deps/glad.c.i

lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/icon.dir/__/deps/glad.c.s"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sidicefhh/Projects/OGLab/lib/glfw/deps/glad.c -o CMakeFiles/icon.dir/__/deps/glad.c.s

# Object files for target icon
icon_OBJECTS = \
"CMakeFiles/icon.dir/icon.c.o" \
"CMakeFiles/icon.dir/__/deps/glad.c.o"

# External object files for target icon
icon_EXTERNAL_OBJECTS =

lib/glfw/tests/icon.app/Contents/MacOS/icon: lib/glfw/tests/CMakeFiles/icon.dir/icon.c.o
lib/glfw/tests/icon.app/Contents/MacOS/icon: lib/glfw/tests/CMakeFiles/icon.dir/__/deps/glad.c.o
lib/glfw/tests/icon.app/Contents/MacOS/icon: lib/glfw/tests/CMakeFiles/icon.dir/build.make
lib/glfw/tests/icon.app/Contents/MacOS/icon: lib/glfw/src/libglfw3.a
lib/glfw/tests/icon.app/Contents/MacOS/icon: lib/glfw/tests/CMakeFiles/icon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sidicefhh/Projects/OGLab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable icon.app/Contents/MacOS/icon"
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/icon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw/tests/CMakeFiles/icon.dir/build: lib/glfw/tests/icon.app/Contents/MacOS/icon
.PHONY : lib/glfw/tests/CMakeFiles/icon.dir/build

lib/glfw/tests/CMakeFiles/icon.dir/clean:
	cd /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/icon.dir/cmake_clean.cmake
.PHONY : lib/glfw/tests/CMakeFiles/icon.dir/clean

lib/glfw/tests/CMakeFiles/icon.dir/depend:
	cd /Users/sidicefhh/Projects/OGLab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sidicefhh/Projects/OGLab /Users/sidicefhh/Projects/OGLab/lib/glfw/tests /Users/sidicefhh/Projects/OGLab/build /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests /Users/sidicefhh/Projects/OGLab/build/lib/glfw/tests/CMakeFiles/icon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw/tests/CMakeFiles/icon.dir/depend

