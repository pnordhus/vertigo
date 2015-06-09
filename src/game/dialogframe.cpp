#include "dialogframe.h"
#include "txt/stringtable.h"
#include "gfx/image.h"


namespace game {


DialogFrame::DialogFrame(Dialog* dialog, const QString &roomName, std::function<void()> &&funcClose) :
    Frame(std::move(funcClose)),
    m_dialog(dialog)
{
    Q_ASSERT(dialog);

    setupFrame(QSize(378, 159), txt::StringTable::get(txt::DialogTitle), false);

    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    QImage image = gfx::Image::load(QString("gfx:img/desktop/bground/d_%1.img").arg(roomName), colorTable);
    if (image.isNull())
        image = gfx::Image::load("gfx:img/desktop/bground/d_ew1.img", colorTable);

    ui::Label *label;

    label = new ui::Label(this);
    label->setPosition(9, 15);
    label->setTexture(image.copy(0, 0, 360, 135));

    label = new ui::Label(this);
    label->setPosition(17, 67);
    label->setTexture(gfx::Image::load("gfx:img/desktop/gui/horline2.img", colorTable).copy(0, 0, 352, 3));

    m_dialog->setParentWidget(this);
    m_dialog->setPosition(25, 23);
    m_dialog->setWidth(360 - 32);
    m_dialog->funcClose = [this]() { closeFrame(); };
}


DialogFrame::~DialogFrame()
{
    m_dialog->setParentWidget(NULL);
    if (m_dialog->remove())
        delete m_dialog;
}


} // namespace game
