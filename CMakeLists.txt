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
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/build/${SYSTEM_NAME_STR_LOWER}/${BUILD_ARCH_STR_LOWER}/base-${BUILD_ENC_LOWER}-${BUILD_TYPE_LOWER}")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
set(INSTALL_DIR "${CMAKE_BINARY_DIR}/bin")



if(MACOSX)
    message("this is macosx ${CMAKE_INSTALL_PREFIX}")
endif(MACOSX)

# handle all folders
add_subdirectory(lame)
add_subdirectory(wav2mp3)

