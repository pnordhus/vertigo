set(SOURCES
    area.cpp
    area.h
    chapter.cpp
    chapter.h
    desktop.cpp
    desktop.h
    dialog.cpp
    dialog.h
    dialogframe.cpp
    dialogframe.h
    main.cpp
    mainmenu.cpp
    mainmenu.h
    menu.cpp
    menu.h
    minimovie.cpp
    minimovie.h
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
    vertigo.cpp
    vertigo.h
    window.cpp
    window.h
)

set(MOC_SOURCES
    chapter.h
    desktop.h
    mainmenu.h
    movie.h
    notebook.h
    room.h
    vertigo.h
    window.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_executable(vertigo ${SOURCES} ${MOC_SOURCES})
target_link_libraries(vertigo sfx txt ui ${QT_LIBRARIES} ${OPENGL_LIBRARY})
