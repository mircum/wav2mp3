project("wav2mp3")

include_directories(
        "${lame_INCLUDE_DIRS}"
)

add_executable(wav2mp3 src/main.cpp)

if(WAV2MP3_MACOS)
    target_link_libraries(wav2mp3 ${lame_DEFINITIONS}/libmp3lame.a)
endif(WAV2MP3_MACOS)

if(WAV2MP3_WINDOWS)
    include_directories(
            "${dirent_win_INCLUDE_DIRS}"
    )
    target_link_libraries(wav2mp3 ${lame_DEFINITIONS}/libmp3lame-static.lib)
endif(WAV2MP3_WINDOWS)

install(TARGETS wav2mp3 RUNTIME DESTINATION ${INSTALL_DIR})