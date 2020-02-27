#ifndef WIN32_MOUSE_INTERFACE
#define WIN32_MOUSE_INTERFACE

#include <cassert>
#include <windows.h>

#include "mouseinterface.h"

class WIN32MouseInterface : public MouseInterface
{
public:
    virtual void getMousePos(int& x, int& y)
    {
        POINT point;
        GetCursorPos(&point);
        x = point.x;
        y = point.y;
    }

    virtual void setMousePos(int x, int y)
    {
        SetCursorPos(x, y);
    }

    virtual void showCursor(bool val)
    {
        ShowCursor(val);
    }

    bool isButtonPressed(int button)
    {
        assert(button < 2);
        return (m_lastButtonState[button] == 0 && m_buttonState[button] == 1);
    }

    void update()
    {
        memcpy(m_lastButtonState, m_buttonState, sizeof(short) * 2);
        m_buttonState[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
        m_buttonState[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
    }

private:
    short m_buttonState[2];
    short m_lastButtonState[2];
};

#endif