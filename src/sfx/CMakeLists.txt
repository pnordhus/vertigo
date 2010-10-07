include_directories(${OPENAL_INCLUDE_DIR})

set(SOURCES
    soundsystem.cpp
    soundsystem.h
    stream.cpp
    stream.h
)

add_library(sfx ${SOURCES})
target_link_libraries(sfx util ${OPENAL_LIBRARY})
