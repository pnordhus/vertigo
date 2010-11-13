set(SOURCES
    betaspline.cpp
    betaspline.h
    element.cpp
    element.h
    surface.cpp
    surface.h
    tesselator.cpp
    tesselator.h
)

add_library(surface ${SOURCES})
target_link_libraries(surface gfx)
