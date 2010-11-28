add_subdirectory(effects)
add_subdirectory(surface)

set(SOURCES
    building.cpp
    building.h
    mine.cpp
    mine.h
    module.cpp
    module.h
    modulemanager.cpp
    modulemanager.h
    navpoint.cpp
    navpoint.h
    object.cpp
    object.h
    scenario.cpp
    scenario.h
    turret.cpp
    turret.h
    turretbase.cpp
    turretbase.h
    vector.cpp
    vector.h
)

set(MOC_SOURCES
    scenario.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(fight ${SOURCES} ${MOC_SOURCES})
target_link_libraries(fight effects surface game)
