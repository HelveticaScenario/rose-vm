project(rosebud C)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(CMakeToolsHelpers OPTIONAL)
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/opt/libarchive/lib/pkgconfig" )
find_package(LuaJIT REQUIRED)
find_package(SDL2 REQUIRED)


find_package(ZLIB REQUIRED)
file(GLOB_RECURSE SOURCES src/*.c)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  # Required for luajit.
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -image_base 100000000")
  set(CMAKE_MODULE_LINKER_FLAGS
    "${CMAKE_MODULE_LINKER_FLAGS} -image_base 100000000")

  set(LibArchive_INCLUDE_DIR "/usr/local/opt/libarchive/include")
  set(LibArchive_LIBRARIES "/usr/local/opt/libarchive/lib/libarchive.a")
endif()

include_directories(${SDL2_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
add_executable(rosebud ${SOURCES})
target_link_libraries(rosebud ${SDL2_LIBRARY} ${LUA_LIBRARIES} ${ZLIB_LIBRARIES} ${LibArchive_LIBRARIES} expat lzma bz2 iconv)





