# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/osmc/mcpelauncher-manifest/game-window

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/osmc/mcpelauncher-manifest/game-window/build

# Include any dependencies generated for this target.
include CMakeFiles/gamewindow.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gamewindow.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gamewindow.dir/flags.make

CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o: CMakeFiles/gamewindow.dir/flags.make
CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o: ../src/game_window_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o -c /home/osmc/mcpelauncher-manifest/game-window/src/game_window_manager.cpp

CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osmc/mcpelauncher-manifest/game-window/src/game_window_manager.cpp > CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.i

CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osmc/mcpelauncher-manifest/game-window/src/game_window_manager.cpp -o CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.s

CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o: CMakeFiles/gamewindow.dir/flags.make
CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o: ../src/game_window_error_handler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o -c /home/osmc/mcpelauncher-manifest/game-window/src/game_window_error_handler.cpp

CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osmc/mcpelauncher-manifest/game-window/src/game_window_error_handler.cpp > CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.i

CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osmc/mcpelauncher-manifest/game-window/src/game_window_error_handler.cpp -o CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.s

CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o: CMakeFiles/gamewindow.dir/flags.make
CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o: ../src/joystick_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o -c /home/osmc/mcpelauncher-manifest/game-window/src/joystick_manager.cpp

CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osmc/mcpelauncher-manifest/game-window/src/joystick_manager.cpp > CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.i

CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osmc/mcpelauncher-manifest/game-window/src/joystick_manager.cpp -o CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.s

CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o: CMakeFiles/gamewindow.dir/flags.make
CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o: ../src/window_sdl2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o -c /home/osmc/mcpelauncher-manifest/game-window/src/window_sdl2.cpp

CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osmc/mcpelauncher-manifest/game-window/src/window_sdl2.cpp > CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.i

CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osmc/mcpelauncher-manifest/game-window/src/window_sdl2.cpp -o CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.s

CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o: CMakeFiles/gamewindow.dir/flags.make
CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o: ../src/window_manager_sdl2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o -c /home/osmc/mcpelauncher-manifest/game-window/src/window_manager_sdl2.cpp

CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osmc/mcpelauncher-manifest/game-window/src/window_manager_sdl2.cpp > CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.i

CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osmc/mcpelauncher-manifest/game-window/src/window_manager_sdl2.cpp -o CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.s

# Object files for target gamewindow
gamewindow_OBJECTS = \
"CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o" \
"CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o" \
"CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o" \
"CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o" \
"CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o"

# External object files for target gamewindow
gamewindow_EXTERNAL_OBJECTS =

libgamewindow.a: CMakeFiles/gamewindow.dir/src/game_window_manager.cpp.o
libgamewindow.a: CMakeFiles/gamewindow.dir/src/game_window_error_handler.cpp.o
libgamewindow.a: CMakeFiles/gamewindow.dir/src/joystick_manager.cpp.o
libgamewindow.a: CMakeFiles/gamewindow.dir/src/window_sdl2.cpp.o
libgamewindow.a: CMakeFiles/gamewindow.dir/src/window_manager_sdl2.cpp.o
libgamewindow.a: CMakeFiles/gamewindow.dir/build.make
libgamewindow.a: CMakeFiles/gamewindow.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libgamewindow.a"
	$(CMAKE_COMMAND) -P CMakeFiles/gamewindow.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gamewindow.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gamewindow.dir/build: libgamewindow.a

.PHONY : CMakeFiles/gamewindow.dir/build

CMakeFiles/gamewindow.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gamewindow.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gamewindow.dir/clean

CMakeFiles/gamewindow.dir/depend:
	cd /home/osmc/mcpelauncher-manifest/game-window/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/osmc/mcpelauncher-manifest/game-window /home/osmc/mcpelauncher-manifest/game-window /home/osmc/mcpelauncher-manifest/game-window/build /home/osmc/mcpelauncher-manifest/game-window/build /home/osmc/mcpelauncher-manifest/game-window/build/CMakeFiles/gamewindow.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gamewindow.dir/depend

