set(SOURCES
    area.cpp
    area.h
    boat.cpp
    boat.h
    briefing.cpp
    briefing.h
    chapter.cpp
    chapter.h
    departure.cpp
    departure.h
    depot.cpp
    depot.h
    desktop.cpp
    desktop.h
    dialog.cpp
    dialog.h
    dialogframe.cpp
    dialogframe.h
    encom.cpp
    encom.h
    items.cpp
    items.h
    mainmenu.cpp
    mainmenu.h
    menu.cpp
    menu.h
    minimovie.cpp
    minimovie.h
    mission.cpp
    mission.h
    movie.cpp
    movie.h
    notebook.cpp
    notebook.h
    renderer.cpp
    renderer.h
    room.cpp
    room.h
    station.cpp
    station.h
    task.cpp
    task.h
    vertigo.cpp
    vertigo.h
    window.cpp
    window.h
)

set(MOC_SOURCES
    briefing.h
    chapter.h
    departure.h
    depot.h
    desktop.h
    dialog.h
    encom.h
    mainmenu.h
    minimovie.h
    movie.h
    notebook.h
    renderer.h
    room.h
    vertigo.h
    window.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_library(game ${SOURCES} ${MOC_SOURCES})
target_link_libraries(game sfx txt ui ${QT_LIBRARIES} ${OPENGL_LIBRARY})

add_executable(vertigo WIN32 main.cpp)
target_link_libraries(vertigo game)
