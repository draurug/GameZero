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

# Utility rule file for ServerClient_autogen_timestamp_deps.

# Include any custom commands dependencies for this target.
include ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/compiler_depend.make

# Include the progress variables for this target.
include ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/progress.make

ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps: /usr/lib/qt5/bin/moc
ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.83.0
ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps: /usr/lib/qt5/bin/uic

ServerClient_autogen_timestamp_deps: ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps
ServerClient_autogen_timestamp_deps: ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/build.make
.PHONY : ServerClient_autogen_timestamp_deps

# Rule to build all files generated by this target.
ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/build: ServerClient_autogen_timestamp_deps
.PHONY : ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/build

ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/clean:
	cd /home/draugur/Documents/GameZero/build/c-Debug/ChatServerClient/ServerClient && $(CMAKE_COMMAND) -P CMakeFiles/ServerClient_autogen_timestamp_deps.dir/cmake_clean.cmake
.PHONY : ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/clean

ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/depend:
	cd /home/draugur/Documents/GameZero/build/c-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/draugur/Documents/GameZero /home/draugur/Documents/GameZero/ChatServerClient/ServerClient /home/draugur/Documents/GameZero/build/c-Debug /home/draugur/Documents/GameZero/build/c-Debug/ChatServerClient/ServerClient /home/draugur/Documents/GameZero/build/c-Debug/ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : ChatServerClient/ServerClient/CMakeFiles/ServerClient_autogen_timestamp_deps.dir/depend

