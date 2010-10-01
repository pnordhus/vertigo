set(SOURCES
    main.cpp
    mainmenu.cpp
    mainmenu.h
    movie.cpp
    movie.h
    renderer.cpp
    renderer.h
    vertigo.cpp
    vertigo.h
    window.cpp
    window.h
)

set(MOC_SOURCES
    mainmenu.h
    movie.h
    vertigo.h
    window.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_executable(vertigo ${SOURCES} ${MOC_SOURCES})
target_link_libraries(vertigo txt ui ${QT_LIBRARIES} ${OPENGL_LIBRARY})
