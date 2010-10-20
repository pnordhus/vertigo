set(SOURCES
    button.cpp
    button.h
    frame.cpp
    frame.h
    label.cpp
    label.h
    widget.cpp
    widget.h
)

set(MOC_SOURCES
    button.h
    frame.h
    widget.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(ui ${SOURCES} ${MOC_SOURCES})
target_link_libraries(ui gfx)
