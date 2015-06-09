set(SOURCES
    billboard.cpp
    billboard.h
    effect.cpp
    effect.h
    effectmanager.cpp
    effectmanager.h
    projectile.cpp
    projectile.h
    trash.cpp
    trash.h
)

add_library(effects ${SOURCES})
target_link_libraries(effects txt)
