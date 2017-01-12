project(rosebud CXX)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(CMakeToolsHelpers OPTIONAL)
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/opt/libarchive/lib/pkgconfig" )

find_package(SDL2 REQUIRED)


find_package(ZLIB REQUIRED)
file(GLOB_RECURSE SOURCES src/*.cpp)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(LibArchive_INCLUDE_DIR "/usr/local/opt/libarchive/include")
  set(LibArchive_LIBRARIES "/usr/local/opt/libarchive/lib/libarchive.a")
endif()

include_directories(${SDL2_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/third_party/v8/include  ${CMAKE_SOURCE_DIR}/src)
add_executable(rosebud ${SOURCES})
target_link_libraries(rosebud ${SDL2_LIBRARY} ${ZLIB_LIBRARIES}
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libv8_base.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libv8_libbase.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libv8_libsampler.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libv8_libplatform.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libv8_external_snapshot.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libicui18n.a
        ${CMAKE_SOURCE_DIR}/third_party/v8/out/x64.release/libicuuc.a
        ${LibArchive_LIBRARIES} expat lzma bz2 iconv)

set_property(TARGET rosebud PROPERTY CXX_STANDARD 14)





