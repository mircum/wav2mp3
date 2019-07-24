project("wav2mp3")

include_directories(
        "${lame_INCLUDE_DIRS}"
)

add_executable(wav2mp3 src/main.cpp)

if(MACOSX)
    #link_directories("${lame_DEFINITIONS}")
    target_link_libraries(wav2mp3 ${lame_DEFINITIONS}/libmp3lame.a)
endif(MACOSX)

if(WINDOWS)
    include_directories(
            "${dirent_win_INCLUDE_DIRS}"
    )
    target_link_libraries(wav2mp3 ${lame_DEFINITIONS}/libmp3lame-static.lib)
endif(WINDOWS)

install(TARGETS wav2mp3 RUNTIME DESTINATION ${INSTALL_DIR})