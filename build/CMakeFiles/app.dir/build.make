# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/mica/Desktop/UTD/Vk-Template

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mica/Desktop/UTD/Vk-Template/build

# Include any dependencies generated for this target.
include CMakeFiles/app.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app.dir/flags.make

CMakeFiles/app.dir/main.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/main.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/main.cpp

CMakeFiles/app.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/main.cpp > CMakeFiles/app.dir/main.cpp.i

CMakeFiles/app.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/main.cpp -o CMakeFiles/app.dir/main.cpp.s

CMakeFiles/app.dir/Helpers.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/Helpers.cpp.o: ../Helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app.dir/Helpers.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/Helpers.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/Helpers.cpp

CMakeFiles/app.dir/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/Helpers.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/Helpers.cpp > CMakeFiles/app.dir/Helpers.cpp.i

CMakeFiles/app.dir/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/Helpers.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/Helpers.cpp -o CMakeFiles/app.dir/Helpers.cpp.s

CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o: ../extern/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_demo.cpp

CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_demo.cpp > CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.i

CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_demo.cpp -o CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.s

CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o: ../extern/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_draw.cpp

CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_draw.cpp > CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.i

CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_draw.cpp -o CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.s

CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o: ../extern/imgui/imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_tables.cpp

CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_tables.cpp > CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.i

CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_tables.cpp -o CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.s

CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o: ../extern/imgui/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_widgets.cpp

CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_widgets.cpp > CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.i

CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui_widgets.cpp -o CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.s

CMakeFiles/app.dir/extern/imgui/imgui.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/imgui.cpp.o: ../extern/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/app.dir/extern/imgui/imgui.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/imgui.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui.cpp

CMakeFiles/app.dir/extern/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/imgui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui.cpp > CMakeFiles/app.dir/extern/imgui/imgui.cpp.i

CMakeFiles/app.dir/extern/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/imgui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/imgui.cpp -o CMakeFiles/app.dir/extern/imgui/imgui.cpp.s

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o: ../extern/imgui/backends/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_glfw.cpp

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_glfw.cpp > CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.i

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_glfw.cpp -o CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.s

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o: ../extern/imgui/backends/imgui_impl_vulkan.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o -c /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_vulkan.cpp

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_vulkan.cpp > CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.i

CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mica/Desktop/UTD/Vk-Template/extern/imgui/backends/imgui_impl_vulkan.cpp -o CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.s

# Object files for target app
app_OBJECTS = \
"CMakeFiles/app.dir/main.cpp.o" \
"CMakeFiles/app.dir/Helpers.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/imgui.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o" \
"CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o"

# External object files for target app
app_EXTERNAL_OBJECTS =

app: CMakeFiles/app.dir/main.cpp.o
app: CMakeFiles/app.dir/Helpers.cpp.o
app: CMakeFiles/app.dir/extern/imgui/imgui_demo.cpp.o
app: CMakeFiles/app.dir/extern/imgui/imgui_draw.cpp.o
app: CMakeFiles/app.dir/extern/imgui/imgui_tables.cpp.o
app: CMakeFiles/app.dir/extern/imgui/imgui_widgets.cpp.o
app: CMakeFiles/app.dir/extern/imgui/imgui.cpp.o
app: CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_glfw.cpp.o
app: CMakeFiles/app.dir/extern/imgui/backends/imgui_impl_vulkan.cpp.o
app: CMakeFiles/app.dir/build.make
app: /home/mica/Downloads/1.2.198.1/x86_64/lib/libvulkan.so
app: /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
app: CMakeFiles/app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app.dir/build: app

.PHONY : CMakeFiles/app.dir/build

CMakeFiles/app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app.dir/clean

CMakeFiles/app.dir/depend:
	cd /home/mica/Desktop/UTD/Vk-Template/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mica/Desktop/UTD/Vk-Template /home/mica/Desktop/UTD/Vk-Template /home/mica/Desktop/UTD/Vk-Template/build /home/mica/Desktop/UTD/Vk-Template/build /home/mica/Desktop/UTD/Vk-Template/build/CMakeFiles/app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/app.dir/depend

