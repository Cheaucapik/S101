# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\ocean\Desktop\Travail BUT 1e année\S101"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/S101.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/S101.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/S101.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/S101.dir/flags.make

CMakeFiles/S101.dir/S101.c.obj: CMakeFiles/S101.dir/flags.make
CMakeFiles/S101.dir/S101.c.obj: C:/Users/ocean/Desktop/Travail\ BUT\ 1e\ année/S101/S101.c
CMakeFiles/S101.dir/S101.c.obj: CMakeFiles/S101.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/S101.dir/S101.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/S101.dir/S101.c.obj -MF CMakeFiles\S101.dir\S101.c.obj.d -o CMakeFiles\S101.dir\S101.c.obj -c "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\S101.c"

CMakeFiles/S101.dir/S101.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/S101.dir/S101.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\S101.c" > CMakeFiles\S101.dir\S101.c.i

CMakeFiles/S101.dir/S101.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/S101.dir/S101.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\S101.c" -o CMakeFiles\S101.dir\S101.c.s

# Object files for target S101
S101_OBJECTS = \
"CMakeFiles/S101.dir/S101.c.obj"

# External object files for target S101
S101_EXTERNAL_OBJECTS =

S101.exe: CMakeFiles/S101.dir/S101.c.obj
S101.exe: CMakeFiles/S101.dir/build.make
S101.exe: CMakeFiles/S101.dir/linkLibs.rsp
S101.exe: CMakeFiles/S101.dir/objects1.rsp
S101.exe: CMakeFiles/S101.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable S101.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\S101.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/S101.dir/build: S101.exe
.PHONY : CMakeFiles/S101.dir/build

CMakeFiles/S101.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\S101.dir\cmake_clean.cmake
.PHONY : CMakeFiles/S101.dir/clean

CMakeFiles/S101.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\ocean\Desktop\Travail BUT 1e année\S101" "C:\Users\ocean\Desktop\Travail BUT 1e année\S101" "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug" "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug" "C:\Users\ocean\Desktop\Travail BUT 1e année\S101\cmake-build-debug\CMakeFiles\S101.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/S101.dir/depend

