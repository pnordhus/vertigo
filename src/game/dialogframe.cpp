#include "dialogframe.h"
#include "txt/stringtable.h"
#include "gfx/image.h"


namespace game {


DialogFrame::DialogFrame(const QString &roomName, ui::Widget *parent) :
    ui::Frame(parent)
{
    setupFrame(QSize(378, 159), txt::StringTable::get(txt::DialogTitle), false);

    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    QImage image = gfx::Image::load(QString("gfx:img/desktop/bground/d_%1.img").arg(roomName), colorTable);

    ui::Label *label;

    label = new ui::Label(this);
    label->setPosition(9, 15);
    label->setTexture(image.copy(0, 0, 360, 135));

    label = new ui::Label(this);
    label->setPosition(17, 67);
    label->setTexture(gfx::Image::load("gfx:img/desktop/gui/horline2.img", colorTable).copy(0, 0, 352, 3));
}


} // namespace game
