#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <memory>
#include "uncopyable.h"

class FreeTypeFont;
class GameWorld;
class BOGLGPWindow;

class Example : private Uncopyable
{
public:
    Example(BOGLGPWindow* window);
    virtual ~Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

    BOGLGPWindow* getWindow() { return m_window; }
    void setWindow(BOGLGPWindow* window) { m_window = window; }

    void updateFPS(float dt);
private:
    float m_angle;

    std::auto_ptr<FreeTypeFont> m_font;
    std::auto_ptr<GameWorld> m_world;
    BOGLGPWindow* m_window;

    float m_FPS;
};

#endif
