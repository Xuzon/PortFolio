#ifndef WIN32_KEYBOARD_INTERFACE
#define WIN32_KEYBOARD_INTERFACE

#ifdef _WIN32
#include <windows.h>
#endif

#include "keyboardinterface.h"

class WIN32KeyboardInterface : public KeyboardInterface
{
public:
    WIN32KeyboardInterface()
    {
        for (int i = 0; i < KC_MAX_KEYS; ++i) {
            m_keyState[i] = 0;
            m_lastKeyState[i] = 0;
        }
    }

    virtual bool isKeyPressed(BOGLGPKeyCode code)
    {
        return (!m_lastKeyState[code] && m_keyState[code]);
    }

    virtual bool isKeyHeldDown(BOGLGPKeyCode code)
    {
        return (m_lastKeyState[code] && m_keyState[code]);
    }

    virtual void handleKeyDown(BOGLGPKeyCode code)
    {
        m_keyState[code] = 1;
    }

    virtual void handleKeyUp(BOGLGPKeyCode code)
    {
        m_keyState[code] = 0;
    }

    virtual BOGLGPKeyCode translateKey(unsigned int code) //Translate a OS key to a KeyCode
    {
        switch(code)
        {
        case VK_UP:
            return KC_UP;
        break;
        case VK_DOWN:
            return KC_DOWN;
        break;
        case VK_LEFT:
            return KC_LEFT;
        break;
        case VK_RIGHT:
            return KC_RIGHT;
        break;
        case 'W':
            return KC_w;
        break;
        case 'S':
            return KC_s;
        break;
        case 'A':
            return KC_a;
        break;
        case 'D':
            return KC_d;
        break;
        case VK_SPACE:
            return KC_SPACE;
        break;
        default:
            return KC_INVALID;
        }
    }

    virtual void update()
    {
        memcpy(m_lastKeyState, m_keyState, sizeof(short) * KC_MAX_KEYS);
    }

private:
    short m_keyState[KC_MAX_KEYS];
    short m_lastKeyState[KC_MAX_KEYS];
};

#endif