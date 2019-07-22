project("lame")

if(MACOSX)
    install(FILES
            lib/macosx/libmp3lame.0.dylib
            lib/macosx/libmp3lame.dylib
            DESTINATION ${INSTALL_DIR})
endif(MACOSX)