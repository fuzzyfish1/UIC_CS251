# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug

# Include any dependencies generated for this target.
include GoogleTests/lib/googletest/CMakeFiles/gtest.dir/depend.make
# Include the progress variables for this target.
include GoogleTests/lib/googletest/CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include GoogleTests/lib/googletest/CMakeFiles/gtest.dir/flags.make

GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.obj: GoogleTests/lib/googletest/CMakeFiles/gtest.dir/flags.make
GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.obj: GoogleTests/lib/googletest/CMakeFiles/gtest.dir/includes_CXX.rsp
GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.obj: ../GoogleTests/lib/googletest/src/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.obj"
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\gtest.dir\src\gtest-all.cc.obj -c C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\GoogleTests\lib\googletest\src\gtest-all.cc

GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest.dir/src/gtest-all.cc.i"
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\GoogleTests\lib\googletest\src\gtest-all.cc > CMakeFiles\gtest.dir\src\gtest-all.cc.i

GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/src/gtest-all.cc.s"
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\GoogleTests\lib\googletest\src\gtest-all.cc -o CMakeFiles\gtest.dir\src\gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/src/gtest-all.cc.obj"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

lib/libgtest.a: GoogleTests/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.obj
lib/libgtest.a: GoogleTests/lib/googletest/CMakeFiles/gtest.dir/build.make
lib/libgtest.a: GoogleTests/lib/googletest/CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ..\..\..\lib\libgtest.a"
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && $(CMAKE_COMMAND) -P CMakeFiles\gtest.dir\cmake_clean_target.cmake
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\gtest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
GoogleTests/lib/googletest/CMakeFiles/gtest.dir/build: lib/libgtest.a
.PHONY : GoogleTests/lib/googletest/CMakeFiles/gtest.dir/build

GoogleTests/lib/googletest/CMakeFiles/gtest.dir/clean:
	cd /d C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest && $(CMAKE_COMMAND) -P CMakeFiles\gtest.dir\cmake_clean.cmake
.PHONY : GoogleTests/lib/googletest/CMakeFiles/gtest.dir/clean

GoogleTests/lib/googletest/CMakeFiles/gtest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3 C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\GoogleTests\lib\googletest C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest C:\Users\Fuzzyfish\CLionProjects\CS251\251progy3\cmake-build-debug\GoogleTests\lib\googletest\CMakeFiles\gtest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : GoogleTests/lib/googletest/CMakeFiles/gtest.dir/depend
