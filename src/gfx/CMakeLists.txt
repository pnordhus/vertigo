set(SOURCES
	colortable.cpp
	colortable.h
    font.cpp
    font.h
    image.cpp
    image.h
	rle.cpp
	rle.h
    texture.cpp
    texture.h
    video.cpp
    video.h
)

add_library(gfx ${SOURCES})
target_link_libraries(gfx)
