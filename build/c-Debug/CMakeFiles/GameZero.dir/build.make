# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/draugur/Documents/GameZero

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/draugur/Documents/GameZero/build/c-Debug

# Include any dependencies generated for this target.
include CMakeFiles/GameZero.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GameZero.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GameZero.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GameZero.dir/flags.make

GameZero_autogen/timestamp: /usr/lib/qt5/bin/moc
GameZero_autogen/timestamp: /usr/lib/qt5/bin/uic
GameZero_autogen/timestamp: CMakeFiles/GameZero.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target GameZero"
	/usr/bin/cmake -E cmake_autogen /home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles/GameZero_autogen.dir/AutogenInfo.json Debug
	/usr/bin/cmake -E touch /home/draugur/Documents/GameZero/build/c-Debug/GameZero_autogen/timestamp

CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o: CMakeFiles/GameZero.dir/flags.make
CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o: GameZero_autogen/mocs_compilation.cpp
CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o: CMakeFiles/GameZero.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o -MF CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o -c /home/draugur/Documents/GameZero/build/c-Debug/GameZero_autogen/mocs_compilation.cpp

CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.i"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/draugur/Documents/GameZero/build/c-Debug/GameZero_autogen/mocs_compilation.cpp > CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.i

CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.s"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/draugur/Documents/GameZero/build/c-Debug/GameZero_autogen/mocs_compilation.cpp -o CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.s

CMakeFiles/GameZero.dir/main.cpp.o: CMakeFiles/GameZero.dir/flags.make
CMakeFiles/GameZero.dir/main.cpp.o: /home/draugur/Documents/GameZero/main.cpp
CMakeFiles/GameZero.dir/main.cpp.o: CMakeFiles/GameZero.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/GameZero.dir/main.cpp.o"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GameZero.dir/main.cpp.o -MF CMakeFiles/GameZero.dir/main.cpp.o.d -o CMakeFiles/GameZero.dir/main.cpp.o -c /home/draugur/Documents/GameZero/main.cpp

CMakeFiles/GameZero.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GameZero.dir/main.cpp.i"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/draugur/Documents/GameZero/main.cpp > CMakeFiles/GameZero.dir/main.cpp.i

CMakeFiles/GameZero.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GameZero.dir/main.cpp.s"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/draugur/Documents/GameZero/main.cpp -o CMakeFiles/GameZero.dir/main.cpp.s

CMakeFiles/GameZero.dir/MainWindow.cpp.o: CMakeFiles/GameZero.dir/flags.make
CMakeFiles/GameZero.dir/MainWindow.cpp.o: /home/draugur/Documents/GameZero/MainWindow.cpp
CMakeFiles/GameZero.dir/MainWindow.cpp.o: CMakeFiles/GameZero.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/GameZero.dir/MainWindow.cpp.o"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GameZero.dir/MainWindow.cpp.o -MF CMakeFiles/GameZero.dir/MainWindow.cpp.o.d -o CMakeFiles/GameZero.dir/MainWindow.cpp.o -c /home/draugur/Documents/GameZero/MainWindow.cpp

CMakeFiles/GameZero.dir/MainWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GameZero.dir/MainWindow.cpp.i"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/draugur/Documents/GameZero/MainWindow.cpp > CMakeFiles/GameZero.dir/MainWindow.cpp.i

CMakeFiles/GameZero.dir/MainWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GameZero.dir/MainWindow.cpp.s"
	/usr/bin/clang++-18 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/draugur/Documents/GameZero/MainWindow.cpp -o CMakeFiles/GameZero.dir/MainWindow.cpp.s

# Object files for target GameZero
GameZero_OBJECTS = \
"CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/GameZero.dir/main.cpp.o" \
"CMakeFiles/GameZero.dir/MainWindow.cpp.o"

# External object files for target GameZero
GameZero_EXTERNAL_OBJECTS =

GameZero: CMakeFiles/GameZero.dir/GameZero_autogen/mocs_compilation.cpp.o
GameZero: CMakeFiles/GameZero.dir/main.cpp.o
GameZero: CMakeFiles/GameZero.dir/MainWindow.cpp.o
GameZero: CMakeFiles/GameZero.dir/build.make
GameZero: ChatServerClient/ServerClient/libServerClient.a
GameZero: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.15.13
GameZero: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.83.0
GameZero: ChatServerClient/ServerClient/libServerClient.a
GameZero: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.15.13
GameZero: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.13
GameZero: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.83.0
GameZero: CMakeFiles/GameZero.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable GameZero"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GameZero.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GameZero.dir/build: GameZero
.PHONY : CMakeFiles/GameZero.dir/build

CMakeFiles/GameZero.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GameZero.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GameZero.dir/clean

CMakeFiles/GameZero.dir/depend: GameZero_autogen/timestamp
	cd /home/draugur/Documents/GameZero/build/c-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/draugur/Documents/GameZero /home/draugur/Documents/GameZero /home/draugur/Documents/GameZero/build/c-Debug /home/draugur/Documents/GameZero/build/c-Debug /home/draugur/Documents/GameZero/build/c-Debug/CMakeFiles/GameZero.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GameZero.dir/depend

