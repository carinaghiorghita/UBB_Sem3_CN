# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/Carina/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/Carina/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/d/fac/sem3/compnet/lab1/3_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/3_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/3_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3_server.dir/flags.make

CMakeFiles/3_server.dir/main.c.o: CMakeFiles/3_server.dir/flags.make
CMakeFiles/3_server.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/3_server.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3_server.dir/main.c.o   -c /cygdrive/d/fac/sem3/compnet/lab1/3_server/main.c

CMakeFiles/3_server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3_server.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/d/fac/sem3/compnet/lab1/3_server/main.c > CMakeFiles/3_server.dir/main.c.i

CMakeFiles/3_server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3_server.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/d/fac/sem3/compnet/lab1/3_server/main.c -o CMakeFiles/3_server.dir/main.c.s

# Object files for target 3_server
3_server_OBJECTS = \
"CMakeFiles/3_server.dir/main.c.o"

# External object files for target 3_server
3_server_EXTERNAL_OBJECTS =

3_server.exe: CMakeFiles/3_server.dir/main.c.o
3_server.exe: CMakeFiles/3_server.dir/build.make
3_server.exe: CMakeFiles/3_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable 3_server.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3_server.dir/build: 3_server.exe

.PHONY : CMakeFiles/3_server.dir/build

CMakeFiles/3_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3_server.dir/clean

CMakeFiles/3_server.dir/depend:
	cd /cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/d/fac/sem3/compnet/lab1/3_server /cygdrive/d/fac/sem3/compnet/lab1/3_server /cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug /cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug /cygdrive/d/fac/sem3/compnet/lab1/3_server/cmake-build-debug/CMakeFiles/3_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/3_server.dir/depend

