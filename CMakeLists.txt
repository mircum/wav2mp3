cmake_minimum_required(VERSION 3.13)

# specified by -DBUILD_TYPE=(Debug|Release) or fetched by folder name. Default Debug
if (BUILD_TYPE STREQUAL "release")
    # release build
    add_compile_definitions(BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type: Release")
else()
    # debug build
    add_compile_definitions(BUILD_DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "Build type: Debug")
endif()

project(wav2mp3 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED YES)  
set(CXX_EXTENSIONS NO)


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WAV2MP3_WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(WAV2MP3_LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(WAV2MP3_MACOS TRUE)
endif()

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
set(INSTALL_DIR "${CMAKE_BINARY_DIR}/bin")

# handle all folders
add_subdirectory(lame)
add_subdirectory(dirent_win)
add_subdirectory(wav2mp3)

