set(SOURCES
    label.cpp
    label.h
    widget.cpp
    widget.h
)

add_library(ui ${SOURCES})
target_link_libraries(ui gfx)
