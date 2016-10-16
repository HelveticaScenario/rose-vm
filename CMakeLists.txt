project(rosebud CXX)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(LuaJIT REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
add_executable(rosebud main.cpp system_sdl2.cpp runtime.cpp editor.cpp cartridge.cpp editor_cartridge.cpp)
target_link_libraries(rosebud ${SDL2_LIBRARY} ${LUA_LIBRARIES})
set_property(TARGET rosebud PROPERTY CXX_STANDARD 14)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  # Required for luajit.
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -image_base 100000000")
  set(CMAKE_MODULE_LINKER_FLAGS
    "${CMAKE_MODULE_LINKER_FLAGS} -image_base 100000000")
endif()
