#ifndef _GLWINDOW_H
#define _GLWINDOW_H

#include <windows.h>
#include <ctime>
#include "boglgpwindow.h"

class Example; //Declare our Example class

class GLWindow : public BOGLGPWindow
{
public:
    GLWindow(HINSTANCE hInstance); //default constructor
    virtual ~GLWindow();

    bool create(int width, int height, int bpp, bool fullscreen);
    void destroy();
    void processEvents();
    void attachExample(Example* example);

    bool isRunning(); //Is the window running?

    void swapBuffers() { SwapBuffers(m_hdc); }

    static LRESULT CALLBACK StaticWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

    float getElapsedSeconds();

    KeyboardInterface* getKeyboard() const { return m_keyboard; }
    MouseInterface* getMouse() const { return m_mouse; }

private:
    Example* m_example; //A link to the example program
    bool m_isRunning; //Is the window still running?
    bool m_isFullscreen;

    HWND m_hwnd; //Window handle
    HGLRC m_hglrc; //Rendering context
    HDC m_hdc; //Device context
    RECT m_windowRect; //Window bounds
    HINSTANCE m_hinstance; //Application instance
    WNDCLASSEX m_windowClass;

    void setupPixelFormat(void);
    Example* getAttachedExample() { return m_example; }

    float m_lastTime;

    KeyboardInterface* m_keyboard;
    MouseInterface* m_mouse;
};

#endif
