project("lame")


set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

if(MACOSX)
    #add_library(${PROJECT_NAME} STATIC)
    set(${PROJECT_NAME}_DEFINITIONS ${PROJECT_SOURCE_DIR}/lib/macosx
            CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

endif(MACOSX)

if(WINDOWS)
    #add_library(${PROJECT_NAME} STATIC)
    set(${PROJECT_NAME}_DEFINITIONS ${PROJECT_SOURCE_DIR}/lib/win
            CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

endif(WINDOWS)
