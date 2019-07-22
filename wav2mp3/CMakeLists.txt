project("wav2mp3")

include_directories(
        "${CMAKE_SOURCE_DIR}/include"
)


add_executable(wav2mp3 src/main.cpp)


install(TARGETS wav2mp3 RUNTIME DESTINATION ${INSTALL_DIR})