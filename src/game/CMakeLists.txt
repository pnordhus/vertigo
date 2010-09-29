set(SOURCES
    main.cpp
    mainmenu.cpp
    mainmenu.h
    renderer.cpp
    renderer.h
    vertigo.cpp
    vertigo.h
    window.cpp
    window.h
)

set(MOC_SOURCES
    vertigo.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_SOURCES})

add_executable(vertigo ${SOURCES} ${MOC_SOURCES})
target_link_libraries(vertigo gfx ${QT_LIBRARIES} ${OPENGL_LIBRARY})
