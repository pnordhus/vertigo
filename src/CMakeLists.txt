if(Qt4_FOUND)
    set(QT_USE_QTMAIN true)
    set(QT_USE_QTOPENGL true)
    include(${QT_USE_FILE})
endif()

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OPENAL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    fight/boundingbox.cpp
    fight/boundingbox.h
    fight/collisionmanager.cpp
    fight/collisionmanager.h
    fight/collisionmesh.cpp
    fight/collisionmesh.h
    fight/condition.cpp
    fight/condition.h
    fight/conditionmanager.cpp
    fight/conditionmanager.h
    fight/module.cpp
    fight/module.h
    fight/modulemanager.cpp
    fight/modulemanager.h
    fight/scenario.cpp
    fight/scenario.h
    fight/sonar.cpp
    fight/sonar.h

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

    fight/objects/activeobject.cpp
    fight/objects/activeobject.h
    fight/objects/building.cpp
    fight/objects/building.h
    fight/objects/mine.cpp
    fight/objects/mine.h
    fight/objects/navpoint.cpp
    fight/objects/navpoint.h
    fight/objects/object.cpp
    fight/objects/object.h
    fight/objects/simpleobject.cpp
    fight/objects/simpleobject.h
    fight/objects/turret.cpp
    fight/objects/turret.h
    fight/objects/turretbase.cpp
    fight/objects/turretbase.h

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
    gfx/texture.cpp
    gfx/texture.h
    gfx/texturemanager.cpp
    gfx/texturemanager.h
    gfx/video.cpp
    gfx/video.h
    gfx/cliprect.cpp
    gfx/cliprect.h

    hud/hud.cpp
    hud/hud.h
    hud/crosshair.cpp
    hud/crosshair.h
    hud/heading.cpp
    hud/heading.h
    hud/beta.cpp
    hud/beta.h
    hud/meter.cpp
    hud/meter.h
    hud/digiblock.cpp
    hud/digiblock.h
    hud/radiomessage.cpp
    hud/radiomessage.h
    hud/energybar.cpp
    hud/energybar.h
    hud/shield.cpp
    hud/shield.h
    hud/activesonar.cpp
    hud/activesonar.h
    hud/master.cpp
    hud/master.h

    sfx/sound.cpp
    sfx/sound.h
    sfx/soundsystem.cpp
    sfx/soundsystem.h
    sfx/stream.cpp
    sfx/stream.h
    sfx/samplemap.cpp
    sfx/samplemap.h

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

    util/core.h
    util/deferreddeletable.cpp
    util/deferreddeletable.h
    util/error.cpp
    util/error.h
    util/event.hpp
    util/file.cpp
    util/file.h
)

add_executable(vertigo WIN32 ${SOURCES})
target_link_libraries(vertigo ${QT_LIBRARIES} ${SDL2_LIBRARY} ${OPENGL_LIBRARY} ${OPENAL_LIBRARY})
if (Qt5OpenGL_FOUND)
    target_link_libraries(vertigo Qt5::OpenGL)
endif()
install(TARGETS vertigo DESTINATION bin)
