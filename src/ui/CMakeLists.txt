set(SOURCES
    button.cpp
    button.h
    label.cpp
    label.h
    widget.cpp
    widget.h
)

set(MOC_SOURCES
    button.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(ui ${SOURCES} ${MOC_SOURCES})
target_link_libraries(ui gfx)
