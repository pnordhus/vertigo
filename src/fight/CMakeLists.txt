set(SOURCES
    scenario.cpp
    scenario.h
    surface.cpp
    surface.h
    vector.cpp
    vector.h
)

set(MOC_SOURCES
    scenario.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(fight ${SOURCES} ${MOC_SOURCES})
target_link_libraries(fight game)
