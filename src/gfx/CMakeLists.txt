set(SOURCES
    image.cpp
    image.h
    texture.cpp
    texture.h
)

add_library(gfx ${SOURCES})
target_link_libraries(gfx util)
