# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/zaf/MyProject/MyMuduo/src/net

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zaf/MyProject/MyMuduo/src/net

# Include any dependencies generated for this target.
include test/CMakeFiles/rpc_client.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/rpc_client.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/rpc_client.dir/flags.make

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o: test/CMakeFiles/rpc_client.dir/flags.make
test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o: test/rpc/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyProject/MyMuduo/src/net/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o"
	cd /home/zaf/MyProject/MyMuduo/src/net/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rpc_client.dir/rpc/client.cpp.o -c /home/zaf/MyProject/MyMuduo/src/net/test/rpc/client.cpp

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpc_client.dir/rpc/client.cpp.i"
	cd /home/zaf/MyProject/MyMuduo/src/net/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyProject/MyMuduo/src/net/test/rpc/client.cpp > CMakeFiles/rpc_client.dir/rpc/client.cpp.i

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpc_client.dir/rpc/client.cpp.s"
	cd /home/zaf/MyProject/MyMuduo/src/net/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyProject/MyMuduo/src/net/test/rpc/client.cpp -o CMakeFiles/rpc_client.dir/rpc/client.cpp.s

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.requires:

.PHONY : test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.requires

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.provides: test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/rpc_client.dir/build.make test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.provides.build
.PHONY : test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.provides

test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.provides.build: test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o


# Object files for target rpc_client
rpc_client_OBJECTS = \
"CMakeFiles/rpc_client.dir/rpc/client.cpp.o"

# External object files for target rpc_client
rpc_client_EXTERNAL_OBJECTS =

test/rpc_client: test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o
test/rpc_client: test/CMakeFiles/rpc_client.dir/build.make
test/rpc_client: libmy_tiny_muduo_net.a
test/rpc_client: test/CMakeFiles/rpc_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zaf/MyProject/MyMuduo/src/net/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rpc_client"
	cd /home/zaf/MyProject/MyMuduo/src/net/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rpc_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/rpc_client.dir/build: test/rpc_client

.PHONY : test/CMakeFiles/rpc_client.dir/build

test/CMakeFiles/rpc_client.dir/requires: test/CMakeFiles/rpc_client.dir/rpc/client.cpp.o.requires

.PHONY : test/CMakeFiles/rpc_client.dir/requires

test/CMakeFiles/rpc_client.dir/clean:
	cd /home/zaf/MyProject/MyMuduo/src/net/test && $(CMAKE_COMMAND) -P CMakeFiles/rpc_client.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/rpc_client.dir/clean

test/CMakeFiles/rpc_client.dir/depend:
	cd /home/zaf/MyProject/MyMuduo/src/net && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zaf/MyProject/MyMuduo/src/net /home/zaf/MyProject/MyMuduo/src/net/test /home/zaf/MyProject/MyMuduo/src/net /home/zaf/MyProject/MyMuduo/src/net/test /home/zaf/MyProject/MyMuduo/src/net/test/CMakeFiles/rpc_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/rpc_client.dir/depend
