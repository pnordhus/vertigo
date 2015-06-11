set(QT_USE_QTMAIN true)
set(QT_USE_QTOPENGL true)
include(${QT_USE_FILE})

include_directories(${OPENAL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    fight/boundingbox.cpp
    fight/boundingbox.h
    fight/building.cpp
    fight/building.h
    fight/collisionmanager.cpp
    fight/collisionmanager.h
    fight/collisionmesh.cpp
    fight/collisionmesh.h
    fight/condition.cpp
    fight/condition.h
    fight/conditionmanager.cpp
    fight/conditionmanager.h
    fight/mine.cpp
    fight/mine.h
    fight/module.cpp
    fight/module.h
    fight/modulemanager.cpp
    fight/modulemanager.h
    fight/navpoint.cpp
    fight/navpoint.h
    fight/object.cpp
    fight/object.h
    fight/scenario.cpp
    fight/scenario.h
    fight/turret.cpp
    fight/turret.h
    fight/turretbase.cpp
    fight/turretbase.h
    fight/vector.cpp
    fight/vector.h

    fight/effects/billboard.cpp
    fight/effects/billboard.h
    fight/effects/effect.cpp
    fight/effects/effect.h
    fight/effects/effectmanager.cpp
    fight/effects/effectmanager.h
    fight/effects/projectile.cpp
    fight/effects/projectile.h
    fight/effects/trash.cpp
    fight/effects/trash.h

    fight/surface/betaspline.cpp
    fight/surface/betaspline.h
    fight/surface/element.cpp
    fight/surface/element.h
    fight/surface/surface.cpp
    fight/surface/surface.h
    fight/surface/tesselator.cpp
    fight/surface/tesselator.h

    game/area.cpp
    game/area.h
    game/boat.cpp
    game/boat.h
    game/briefing.cpp
    game/briefing.h
    game/chapter.cpp
    game/chapter.h
    game/departure.cpp
    game/departure.h
    game/depot.cpp
    game/depot.h
    game/desktop.cpp
    game/desktop.h
    game/dialog.cpp
    game/dialog.h
    game/dialogframe.cpp
    game/dialogframe.h
    game/encom.cpp
    game/encom.h
    game/items.cpp
    game/items.h
    game/main.cpp
    game/mainmenu.cpp
    game/mainmenu.h
    game/menu.cpp
    game/menu.h
    game/minimovie.cpp
    game/minimovie.h
    game/mission.cpp
    game/mission.h
    game/movie.cpp
    game/movie.h
    game/notebook.cpp
    game/notebook.h
    game/renderer.cpp
    game/renderer.h
    game/room.cpp
    game/room.h
    game/station.cpp
    game/station.h
    game/task.cpp
    game/task.h
    game/vertigo.cpp
    game/vertigo.h
    game/window.cpp
    game/window.h

    gfx/colortable.cpp
    gfx/colortable.h
    gfx/font.cpp
    gfx/font.h
    gfx/fontmanager.cpp
    gfx/fontmanager.h
    gfx/image.cpp
    gfx/image.h
    gfx/rle.cpp
    gfx/rle.h
    gfx/texture.cpp
    gfx/texture.h
    gfx/texturemanager.cpp
    gfx/texturemanager.h
    gfx/video.cpp
    gfx/video.h

    sfx/sound.cpp
    sfx/sound.h
    sfx/soundsystem.cpp
    sfx/soundsystem.h
    sfx/stream.cpp
    sfx/stream.h

    txt/desfile.cpp
    txt/desfile.h
    txt/stringtable.cpp
    txt/stringtable.h

    ui/arrow.cpp
    ui/arrow.h
    ui/button.cpp
    ui/button.h
    ui/frame.cpp
    ui/frame.h
    ui/itemlist.cpp
    ui/itemlist.h
    ui/label.cpp
    ui/label.h
    ui/list.cpp
    ui/list.h
    ui/widget.cpp
    ui/widget.h
)

add_executable(vertigo WIN32 ${SOURCES})
target_link_libraries(vertigo ${QT_LIBRARIES} ${OPENGL_LIBRARY} ${OPENAL_LIBRARY})
install(TARGETS vertigo DESTINATION bin)
