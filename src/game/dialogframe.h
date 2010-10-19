#ifndef GAME_DIALOGFRAME_H
#define GAME_DIALOGFRAME_H


#include "ui/frame.h"


namespace game {


class DialogFrame : public ui::Frame
{
public:
    DialogFrame(const QString &roomName, ui::Widget *parent = NULL);
};


} // namespace game


#endif // GAME_DIALOGFRAME_H
