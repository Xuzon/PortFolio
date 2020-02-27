#ifndef XMOUSEINTERFACE_H_INCLUDED
#define XMOUSEINTERFACE_H_INCLUDED

#ifdef __unix__

#include <stdexcept>
#include <cassert>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "mouseinterface.h"

class XMouseInterface : public MouseInterface
{
public:
    XMouseInterface(Window* window):
    m_window(window)
    {
        m_buttonState[0] = m_buttonState[1] = 0;
        m_lastButtonState[0] = m_lastButtonState[1] = 0;

    }

    virtual void getMousePos(int& x, int& y)
    {
        Display *dpy;
        Window root;
        Window ret_root;
        Window ret_child;
        int root_x;
        int root_y;
        int winX;
        int winY;
        unsigned int mask;

        dpy = XOpenDisplay(NULL);
        root = XDefaultRootWindow(dpy);

        if(XQueryPointer(dpy, *m_window, &ret_root, &ret_child, &root_x, &root_y,
					 &winX, &winY, &mask))
        {
            x = winX;
            y = winY;
		}

        XCloseDisplay(dpy);
    }

    virtual void setMousePos(int x, int y)
    {
        Display* mainDisplay = XOpenDisplay(NULL);

        if(!mainDisplay)
    	{
    		throw std::runtime_error("Could not get a pointer to the main display");
    	}

    	XWarpPointer(mainDisplay, None, *m_window, 0, 0, 0, 0, x, y);
        XCloseDisplay(mainDisplay);
    }

    virtual void showCursor(bool val)
    {
        Display* mainDisplay = XOpenDisplay(NULL);

        if (!val)
        {
            //XDefineCursor(mainDisplay, *m_window, None);

            Pixmap blank;
            XColor dummy;
            char data[1] = {0};

            /* make a blank cursor */
            blank = XCreateBitmapFromData (mainDisplay, *m_window, data, 1, 1);
            m_cursor = XCreatePixmapCursor(mainDisplay, blank, blank, &dummy, &dummy, 0, 0);
            XFreePixmap (mainDisplay, blank);

            XDefineCursor(mainDisplay, *m_window, m_cursor);
        }
        else
        {
            XUndefineCursor(mainDisplay, *m_window);
        }

        XCloseDisplay(mainDisplay);
    }

    virtual bool isButtonPressed(int button)
    {
        assert(button < 2);
        return (m_lastButtonState[button] == 0 && m_buttonState[button] == 1);
    }

    virtual void update()
    {
        memcpy(m_lastButtonState, m_buttonState, sizeof(short) * 2);

        Display *dpy;
        Window root;
        Window ret_root;
        Window ret_child;
        int root_x;
        int root_y;
        int winX;
        int winY;
        unsigned int mask;

        dpy = XOpenDisplay(NULL);
        root = XDefaultRootWindow(dpy);

        if(XQueryPointer(dpy, *m_window, &ret_root, &ret_child, &root_x, &root_y,
					 &winX, &winY, &mask))
        {
            m_buttonState[0] = (mask & Button1Mask) ? 1 : 0;
            m_buttonState[1] = (mask & Button2Mask) ? 1 : 0;
		}

        XCloseDisplay(dpy);
    }

private:
    Window* m_window;
    Cursor m_cursor;

    short m_buttonState[2];
    short m_lastButtonState[2];
};

#endif //unix

#endif // XMOUSEINTERFACE_H_INCLUDED
