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
CMAKE_SOURCE_DIR = /home/leon/emg_project/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leon/emg_project/build

# Include any dependencies generated for this target.
include sinsun_robot/CMakeFiles/sct_test.dir/depend.make

# Include the progress variables for this target.
include sinsun_robot/CMakeFiles/sct_test.dir/progress.make

# Include the compile flags for this target's objects.
include sinsun_robot/CMakeFiles/sct_test.dir/flags.make

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o: sinsun_robot/CMakeFiles/sct_test.dir/flags.make
sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o: /home/leon/emg_project/src/sinsun_robot/src/sct_test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/leon/emg_project/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o"
	cd /home/leon/emg_project/build/sinsun_robot && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sct_test.dir/src/sct_test.cpp.o -c /home/leon/emg_project/src/sinsun_robot/src/sct_test.cpp

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sct_test.dir/src/sct_test.cpp.i"
	cd /home/leon/emg_project/build/sinsun_robot && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/leon/emg_project/src/sinsun_robot/src/sct_test.cpp > CMakeFiles/sct_test.dir/src/sct_test.cpp.i

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sct_test.dir/src/sct_test.cpp.s"
	cd /home/leon/emg_project/build/sinsun_robot && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/leon/emg_project/src/sinsun_robot/src/sct_test.cpp -o CMakeFiles/sct_test.dir/src/sct_test.cpp.s

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.requires:
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.requires

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.provides: sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.requires
	$(MAKE) -f sinsun_robot/CMakeFiles/sct_test.dir/build.make sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.provides.build
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.provides

sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.provides.build: sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o

# Object files for target sct_test
sct_test_OBJECTS = \
"CMakeFiles/sct_test.dir/src/sct_test.cpp.o"

# External object files for target sct_test
sct_test_EXTERNAL_OBJECTS =

/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: sinsun_robot/CMakeFiles/sct_test.dir/build.make
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/libroscpp.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/librosconsole.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/librosconsole_log4cxx.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/librosconsole_backend_interface.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/liblog4cxx.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/libxmlrpcpp.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/libroscpp_serialization.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/librostime.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /opt/ros/indigo/lib/libcpp_common.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/leon/emg_project/devel/lib/sinsun_robot/sct_test: sinsun_robot/CMakeFiles/sct_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/leon/emg_project/devel/lib/sinsun_robot/sct_test"
	cd /home/leon/emg_project/build/sinsun_robot && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sct_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sinsun_robot/CMakeFiles/sct_test.dir/build: /home/leon/emg_project/devel/lib/sinsun_robot/sct_test
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/build

sinsun_robot/CMakeFiles/sct_test.dir/requires: sinsun_robot/CMakeFiles/sct_test.dir/src/sct_test.cpp.o.requires
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/requires

sinsun_robot/CMakeFiles/sct_test.dir/clean:
	cd /home/leon/emg_project/build/sinsun_robot && $(CMAKE_COMMAND) -P CMakeFiles/sct_test.dir/cmake_clean.cmake
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/clean

sinsun_robot/CMakeFiles/sct_test.dir/depend:
	cd /home/leon/emg_project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leon/emg_project/src /home/leon/emg_project/src/sinsun_robot /home/leon/emg_project/build /home/leon/emg_project/build/sinsun_robot /home/leon/emg_project/build/sinsun_robot/CMakeFiles/sct_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sinsun_robot/CMakeFiles/sct_test.dir/depend

