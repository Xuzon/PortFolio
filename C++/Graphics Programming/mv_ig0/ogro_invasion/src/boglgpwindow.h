#ifndef BOGLGPWINDOW_H_INCLUDED
#define BOGLGPWINDOW_H_INCLUDED

#include "uncopyable.h"

class KeyboardInterface;
class MouseInterface;

class Example;

class BOGLGPWindow : private Uncopyable {
public:
    virtual ~BOGLGPWindow() {}

    virtual bool create(int width, int height, int bpp, bool fullscreen) = 0;
    virtual void destroy() = 0;
    virtual void processEvents() = 0;
    virtual void attachExample(Example* example) = 0;
    virtual bool isRunning() = 0;
    virtual void swapBuffers() = 0;
    virtual float getElapsedSeconds() = 0;
    virtual KeyboardInterface* getKeyboard() const = 0;
    virtual MouseInterface* getMouse() const = 0;
};

#endif // BOGLGPWINDOW_H_INCLUDED
