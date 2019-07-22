project("wav2mp3")

message("${lame_INCLUDE_DIRS}")


include_directories(
        "${CMAKE_SOURCE_DIR}/include"
        "${lame_INCLUDE_DIRS}"
)

add_executable(wav2mp3 src/main.cpp)

if(MACOSX)
    link_directories("${lame_DEFINITIONS}")
    target_link_libraries(wav2mp3 ${lame_DEFINITIONS}/libmp3lame.a)
endif(MACOSX)


install(TARGETS wav2mp3 RUNTIME DESTINATION ${INSTALL_DIR})