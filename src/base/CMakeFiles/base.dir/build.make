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
CMAKE_SOURCE_DIR = /home/zaf/MyTinyMuduo/base

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zaf/MyTinyMuduo/base

# Include any dependencies generated for this target.
include CMakeFiles/base.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/base.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/base.dir/flags.make

CMakeFiles/base.dir/MutexLock.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/MutexLock.cpp.o: MutexLock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/base.dir/MutexLock.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/MutexLock.cpp.o -c /home/zaf/MyTinyMuduo/base/MutexLock.cpp

CMakeFiles/base.dir/MutexLock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/MutexLock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/MutexLock.cpp > CMakeFiles/base.dir/MutexLock.cpp.i

CMakeFiles/base.dir/MutexLock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/MutexLock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/MutexLock.cpp -o CMakeFiles/base.dir/MutexLock.cpp.s

CMakeFiles/base.dir/MutexLock.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/MutexLock.cpp.o.requires

CMakeFiles/base.dir/MutexLock.cpp.o.provides: CMakeFiles/base.dir/MutexLock.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/MutexLock.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/MutexLock.cpp.o.provides

CMakeFiles/base.dir/MutexLock.cpp.o.provides.build: CMakeFiles/base.dir/MutexLock.cpp.o


CMakeFiles/base.dir/Logger.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/Logger.cpp.o: Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/base.dir/Logger.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/Logger.cpp.o -c /home/zaf/MyTinyMuduo/base/Logger.cpp

CMakeFiles/base.dir/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/Logger.cpp > CMakeFiles/base.dir/Logger.cpp.i

CMakeFiles/base.dir/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/Logger.cpp -o CMakeFiles/base.dir/Logger.cpp.s

CMakeFiles/base.dir/Logger.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/Logger.cpp.o.requires

CMakeFiles/base.dir/Logger.cpp.o.provides: CMakeFiles/base.dir/Logger.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/Logger.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/Logger.cpp.o.provides

CMakeFiles/base.dir/Logger.cpp.o.provides.build: CMakeFiles/base.dir/Logger.cpp.o


CMakeFiles/base.dir/Thread.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/Thread.cpp.o: Thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/base.dir/Thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/Thread.cpp.o -c /home/zaf/MyTinyMuduo/base/Thread.cpp

CMakeFiles/base.dir/Thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/Thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/Thread.cpp > CMakeFiles/base.dir/Thread.cpp.i

CMakeFiles/base.dir/Thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/Thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/Thread.cpp -o CMakeFiles/base.dir/Thread.cpp.s

CMakeFiles/base.dir/Thread.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/Thread.cpp.o.requires

CMakeFiles/base.dir/Thread.cpp.o.provides: CMakeFiles/base.dir/Thread.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/Thread.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/Thread.cpp.o.provides

CMakeFiles/base.dir/Thread.cpp.o.provides.build: CMakeFiles/base.dir/Thread.cpp.o


CMakeFiles/base.dir/Semaphore.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/Semaphore.cpp.o: Semaphore.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/base.dir/Semaphore.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/Semaphore.cpp.o -c /home/zaf/MyTinyMuduo/base/Semaphore.cpp

CMakeFiles/base.dir/Semaphore.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/Semaphore.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/Semaphore.cpp > CMakeFiles/base.dir/Semaphore.cpp.i

CMakeFiles/base.dir/Semaphore.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/Semaphore.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/Semaphore.cpp -o CMakeFiles/base.dir/Semaphore.cpp.s

CMakeFiles/base.dir/Semaphore.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/Semaphore.cpp.o.requires

CMakeFiles/base.dir/Semaphore.cpp.o.provides: CMakeFiles/base.dir/Semaphore.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/Semaphore.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/Semaphore.cpp.o.provides

CMakeFiles/base.dir/Semaphore.cpp.o.provides.build: CMakeFiles/base.dir/Semaphore.cpp.o


CMakeFiles/base.dir/LogLevel.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/LogLevel.cpp.o: LogLevel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/base.dir/LogLevel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/LogLevel.cpp.o -c /home/zaf/MyTinyMuduo/base/LogLevel.cpp

CMakeFiles/base.dir/LogLevel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/LogLevel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/LogLevel.cpp > CMakeFiles/base.dir/LogLevel.cpp.i

CMakeFiles/base.dir/LogLevel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/LogLevel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/LogLevel.cpp -o CMakeFiles/base.dir/LogLevel.cpp.s

CMakeFiles/base.dir/LogLevel.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/LogLevel.cpp.o.requires

CMakeFiles/base.dir/LogLevel.cpp.o.provides: CMakeFiles/base.dir/LogLevel.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/LogLevel.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/LogLevel.cpp.o.provides

CMakeFiles/base.dir/LogLevel.cpp.o.provides.build: CMakeFiles/base.dir/LogLevel.cpp.o


CMakeFiles/base.dir/LogEvent.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/LogEvent.cpp.o: LogEvent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/base.dir/LogEvent.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/LogEvent.cpp.o -c /home/zaf/MyTinyMuduo/base/LogEvent.cpp

CMakeFiles/base.dir/LogEvent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/LogEvent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/LogEvent.cpp > CMakeFiles/base.dir/LogEvent.cpp.i

CMakeFiles/base.dir/LogEvent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/LogEvent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/LogEvent.cpp -o CMakeFiles/base.dir/LogEvent.cpp.s

CMakeFiles/base.dir/LogEvent.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/LogEvent.cpp.o.requires

CMakeFiles/base.dir/LogEvent.cpp.o.provides: CMakeFiles/base.dir/LogEvent.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/LogEvent.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/LogEvent.cpp.o.provides

CMakeFiles/base.dir/LogEvent.cpp.o.provides.build: CMakeFiles/base.dir/LogEvent.cpp.o


CMakeFiles/base.dir/LogFormatter.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/LogFormatter.cpp.o: LogFormatter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/base.dir/LogFormatter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/LogFormatter.cpp.o -c /home/zaf/MyTinyMuduo/base/LogFormatter.cpp

CMakeFiles/base.dir/LogFormatter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/LogFormatter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/LogFormatter.cpp > CMakeFiles/base.dir/LogFormatter.cpp.i

CMakeFiles/base.dir/LogFormatter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/LogFormatter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/LogFormatter.cpp -o CMakeFiles/base.dir/LogFormatter.cpp.s

CMakeFiles/base.dir/LogFormatter.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/LogFormatter.cpp.o.requires

CMakeFiles/base.dir/LogFormatter.cpp.o.provides: CMakeFiles/base.dir/LogFormatter.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/LogFormatter.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/LogFormatter.cpp.o.provides

CMakeFiles/base.dir/LogFormatter.cpp.o.provides.build: CMakeFiles/base.dir/LogFormatter.cpp.o


CMakeFiles/base.dir/LogFileAppender.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/LogFileAppender.cpp.o: LogFileAppender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/base.dir/LogFileAppender.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/LogFileAppender.cpp.o -c /home/zaf/MyTinyMuduo/base/LogFileAppender.cpp

CMakeFiles/base.dir/LogFileAppender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/LogFileAppender.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/LogFileAppender.cpp > CMakeFiles/base.dir/LogFileAppender.cpp.i

CMakeFiles/base.dir/LogFileAppender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/LogFileAppender.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/LogFileAppender.cpp -o CMakeFiles/base.dir/LogFileAppender.cpp.s

CMakeFiles/base.dir/LogFileAppender.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/LogFileAppender.cpp.o.requires

CMakeFiles/base.dir/LogFileAppender.cpp.o.provides: CMakeFiles/base.dir/LogFileAppender.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/LogFileAppender.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/LogFileAppender.cpp.o.provides

CMakeFiles/base.dir/LogFileAppender.cpp.o.provides.build: CMakeFiles/base.dir/LogFileAppender.cpp.o


CMakeFiles/base.dir/LogEventWarp.cpp.o: CMakeFiles/base.dir/flags.make
CMakeFiles/base.dir/LogEventWarp.cpp.o: LogEventWarp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/base.dir/LogEventWarp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base.dir/LogEventWarp.cpp.o -c /home/zaf/MyTinyMuduo/base/LogEventWarp.cpp

CMakeFiles/base.dir/LogEventWarp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base.dir/LogEventWarp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zaf/MyTinyMuduo/base/LogEventWarp.cpp > CMakeFiles/base.dir/LogEventWarp.cpp.i

CMakeFiles/base.dir/LogEventWarp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base.dir/LogEventWarp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zaf/MyTinyMuduo/base/LogEventWarp.cpp -o CMakeFiles/base.dir/LogEventWarp.cpp.s

CMakeFiles/base.dir/LogEventWarp.cpp.o.requires:

.PHONY : CMakeFiles/base.dir/LogEventWarp.cpp.o.requires

CMakeFiles/base.dir/LogEventWarp.cpp.o.provides: CMakeFiles/base.dir/LogEventWarp.cpp.o.requires
	$(MAKE) -f CMakeFiles/base.dir/build.make CMakeFiles/base.dir/LogEventWarp.cpp.o.provides.build
.PHONY : CMakeFiles/base.dir/LogEventWarp.cpp.o.provides

CMakeFiles/base.dir/LogEventWarp.cpp.o.provides.build: CMakeFiles/base.dir/LogEventWarp.cpp.o


# Object files for target base
base_OBJECTS = \
"CMakeFiles/base.dir/MutexLock.cpp.o" \
"CMakeFiles/base.dir/Logger.cpp.o" \
"CMakeFiles/base.dir/Thread.cpp.o" \
"CMakeFiles/base.dir/Semaphore.cpp.o" \
"CMakeFiles/base.dir/LogLevel.cpp.o" \
"CMakeFiles/base.dir/LogEvent.cpp.o" \
"CMakeFiles/base.dir/LogFormatter.cpp.o" \
"CMakeFiles/base.dir/LogFileAppender.cpp.o" \
"CMakeFiles/base.dir/LogEventWarp.cpp.o"

# External object files for target base
base_EXTERNAL_OBJECTS =

libbase.a: CMakeFiles/base.dir/MutexLock.cpp.o
libbase.a: CMakeFiles/base.dir/Logger.cpp.o
libbase.a: CMakeFiles/base.dir/Thread.cpp.o
libbase.a: CMakeFiles/base.dir/Semaphore.cpp.o
libbase.a: CMakeFiles/base.dir/LogLevel.cpp.o
libbase.a: CMakeFiles/base.dir/LogEvent.cpp.o
libbase.a: CMakeFiles/base.dir/LogFormatter.cpp.o
libbase.a: CMakeFiles/base.dir/LogFileAppender.cpp.o
libbase.a: CMakeFiles/base.dir/LogEventWarp.cpp.o
libbase.a: CMakeFiles/base.dir/build.make
libbase.a: CMakeFiles/base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zaf/MyTinyMuduo/base/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library libbase.a"
	$(CMAKE_COMMAND) -P CMakeFiles/base.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/base.dir/build: libbase.a

.PHONY : CMakeFiles/base.dir/build

CMakeFiles/base.dir/requires: CMakeFiles/base.dir/MutexLock.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/Logger.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/Thread.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/Semaphore.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/LogLevel.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/LogEvent.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/LogFormatter.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/LogFileAppender.cpp.o.requires
CMakeFiles/base.dir/requires: CMakeFiles/base.dir/LogEventWarp.cpp.o.requires

.PHONY : CMakeFiles/base.dir/requires

CMakeFiles/base.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/base.dir/cmake_clean.cmake
.PHONY : CMakeFiles/base.dir/clean

CMakeFiles/base.dir/depend:
	cd /home/zaf/MyTinyMuduo/base && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zaf/MyTinyMuduo/base /home/zaf/MyTinyMuduo/base /home/zaf/MyTinyMuduo/base /home/zaf/MyTinyMuduo/base /home/zaf/MyTinyMuduo/base/CMakeFiles/base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/base.dir/depend

