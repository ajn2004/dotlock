cmake_minimum_required(VERSION 3.10)
project(DotlockSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

# Use sdl2-config to get flags and libs
execute_process(
  COMMAND sdl2-config --cflags
  OUTPUT_VARIABLE SDL2_CFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND sdl2-config --libs
  OUTPUT_VARIABLE SDL2_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Apply the flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SDL2_CFLAGS}")

# ImGui setup
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_BACKENDS}/imgui_impl_sdl2.cpp
  ${IMGUI_BACKENDS}/imgui_impl_sdlrenderer2.cpp
)

include_directories(
  ${IMGUI_DIR}
  ${IMGUI_BACKENDS}
  ${CMAKE_SOURCE_DIR}/include
)

set(APP_SOURCES
  src/main.cpp
  src/simulation.cpp
  src/gui.cpp
  src/feedback.cpp
)

add_executable(dotlock ${APP_SOURCES} ${IMGUI_SOURCES})

# Link SDL2 properly
target_link_libraries(dotlock PRIVATE ${SDL2_LIBS})
