#include <iostream>
#include <string>

#ifdef __unix__
#include <sys/time.h>
#endif

#include "glxwindow.h"
#include "example.h"
#include "xkeyboardinterface.h"
#include "xmouseinterface.h"

using std::string;

typedef GLXContext ( * PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

unsigned int GetTickCount() {
    struct timeval t;
    gettimeofday(&t, NULL);

    unsigned long secs = t.tv_sec * 1000;
	secs += (t.tv_usec / 1000);
	return secs;
}

SimpleGLXWindow::SimpleGLXWindow():
m_example(NULL),
m_isRunning(true),
m_lastTime(0),
m_display(NULL),
m_XWindow(0),
m_glContext(0),
m_screenID(0),
m_isFullscreen(false),
m_width(0),
m_height(0),
m_bpp(0),
m_GL3Supported(false),
m_keyboard(NULL),
m_mouse(NULL)
{
    m_keyboard = new XKeyboardInterface();
}

SimpleGLXWindow::~SimpleGLXWindow()
{
    delete m_keyboard;
    delete m_mouse;
}

bool SimpleGLXWindow::create(int width, int height, int bpp, bool fullscreen)
{
    m_display = XOpenDisplay(0);  //Open default display
    if (m_display == NULL)
    {
        std::cerr << "Could not open the display" << std::endl;
        return false;
    }

    m_screenID = DefaultScreen(m_display); //Get the default screen id

    int n = 0, modeNum = 0;
    //Get a framebuffer config using the default attributes
    GLXFBConfig framebufferConfig = (*glXChooseFBConfig(m_display, DefaultScreen(m_display), 0, &n));

    XF86VidModeModeInfo **modes;
    if (!XF86VidModeGetAllModeLines(m_display, m_screenID, &modeNum, &modes))
    {
        std::cerr << "Could not query the video modes" << std::endl;
        return false;
    }

    m_XF86DeskMode = *modes[0];

    int bestMode = -1;
    for (int i = 0; i < modeNum; i++)
    {
        if ((modes[i]->hdisplay == width) &&
            (modes[i]->vdisplay == height))
        {
            bestMode = i;
        }
    }

    if (bestMode == -1)
    {
        std::cerr << "Could not find a suitable graphics mode" << std::endl;
        return false;
    }


    int doubleBufferedAttribList [] = {
        GLX_RGBA, GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        GLX_DEPTH_SIZE, 16,
        None
    };

    XVisualInfo* vi = NULL;
    //Attempt to create a double buffered window
    vi = glXChooseVisual(m_display, m_screenID, doubleBufferedAttribList);

    if (vi == NULL)
    {
        std::cerr << "Could not create a double buffered window" << std::endl;
        return false;
    }

    //Create a GL 2.1 context
    GLXContext gl2Context = glXCreateContext(m_display, vi, 0, GL_TRUE);

    if (gl2Context == NULL)
    {
        std::cerr << "Could not create a GL 2.1 context, please check your graphics drivers" << std::endl;
        return false;
    }

    //Get a pointer to the GL 3.0 context creation
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");

    if (glXCreateContextAttribs == NULL)
    {
        std::cerr << "OpenGL 3.0 is not supported, falling back to 2.1" << std::endl;
        m_glContext = gl2Context;
        m_GL3Supported = false;
    }
    else
    {
        //Create a GL 3.0 context

        int attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,//we want a 3.0 context
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            0 //zero indicates the end of the array
        };

        m_glContext = glXCreateContextAttribs(m_display, framebufferConfig, 0, true, &attribs[0]);
        glXDestroyContext(m_display, gl2Context); //We can destroy the GL 2.0 context once the 3.0 one has bene created
        m_GL3Supported = true;
    }

    Colormap cmap = XCreateColormap(m_display, RootWindow(m_display, vi->screen),vi->visual, AllocNone);
    m_XSetAttr.colormap = cmap;
    m_XSetAttr.border_pixel = 0;
    m_XSetAttr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                                StructureNotifyMask;

    m_XSetAttr.override_redirect = False;

    unsigned long windowAttributes = CWBorderPixel | CWColormap | CWEventMask;

    if (fullscreen)
    {
        windowAttributes = CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect;

        XF86VidModeSwitchToMode(m_display, m_screenID, modes[bestMode]);
        XF86VidModeSetViewPort(m_display, m_screenID, 0, 0);
        m_XSetAttr.override_redirect = True;
    }

    m_XWindow = XCreateWindow(m_display, RootWindow(m_display, vi->screen),
                                  0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
                                  CWBorderPixel | CWColormap | CWEventMask, &m_XSetAttr);

    string title = "Invasion!";

    if (fullscreen)
    {
        XWarpPointer(m_display, None, m_XWindow, 0, 0, 0, 0, 0, 0);
        XMapRaised(m_display, m_XWindow);
        XGrabKeyboard(m_display, m_XWindow, True, GrabModeAsync, GrabModeAsync, CurrentTime);
        XGrabPointer(m_display, m_XWindow, True, ButtonPressMask,
                     GrabModeAsync, GrabModeAsync, m_XWindow, None, CurrentTime);

        m_isFullscreen = true;
    }
    else
    {
        Atom wmDelete = XInternAtom(m_display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(m_display, m_XWindow, &wmDelete, 1);
        XSetStandardProperties(m_display, m_XWindow, title.c_str(), None, NULL, NULL, 0, NULL);
        XMapRaised(m_display, m_XWindow);
    }


    XFree(modes);

    m_mouse = new XMouseInterface(&m_XWindow);
    m_mouse->showCursor(false);
    //Make the new context current
    glXMakeCurrent(m_display, m_XWindow, m_glContext);

    int posx = 0;
    int posy = 0;
    Window winDummy;
    unsigned int borderDummy;

    m_width = (unsigned) width;
    m_height = (unsigned) height;
    m_bpp = (unsigned) bpp;

    XGetGeometry(m_display, m_XWindow, &winDummy,
                 &posx, &posy, &m_width, &m_height,
                 &borderDummy, &m_bpp);

    m_lastTime = GetTickCount(); //Initialize the time
    return true;
}

void SimpleGLXWindow::destroy()
{
    m_mouse->showCursor(true);
    if (m_glContext)
    {
        glXMakeCurrent(m_display, None, NULL);
        glXDestroyContext(m_display, m_glContext);
        m_glContext = NULL;
    }

    if (m_isFullscreen)
    {
        XF86VidModeSwitchToMode(m_display, m_screenID, &m_XF86DeskMode);
        XF86VidModeSetViewPort(m_display, m_screenID, 0, 0);
    }

    XCloseDisplay(m_display);
}

void SimpleGLXWindow::processEvents()
{
    getKeyboard()->update();
    getMouse()->update();

    XEvent event;

    while (XPending(m_display) > 0)
    {
        XNextEvent(m_display, &event);
        switch (event.type)
        {
        case Expose:
            if (event.xexpose.count != 0)
                break;
            break;
        case ConfigureNotify:
        {
            int width = event.xconfigure.width;
            int height = event.xconfigure.height;
            getAttachedExample()->onResize(width, height);
        }
        break;
        case KeyPress:
        {
            if (XLookupKeysym(&event.xkey,0) == XK_Escape) {
                m_isRunning = false;
            }

            //Register the key press with the keyboard interface
            m_keyboard->handleKeyDown(m_keyboard->translateKey(XLookupKeysym(&event.xkey,0)));
        }
        break;
        case KeyRelease:
        {
            BOGLGPKeyCode code = m_keyboard->translateKey(XLookupKeysym(&event.xkey,0));
            m_keyboard->handleKeyUp(code);
        }
        break;

        case ClientMessage:
            if (string(XGetAtomName(m_display, event.xclient.message_type)) == string("WM_PROTOCOLS"))
            {
                m_isRunning = false;
            }
            break;
        default:
            break;
        }
    }

}

bool SimpleGLXWindow::isRunning()
{
    return m_isRunning;
}

void SimpleGLXWindow::attachExample(Example* example)
{
    m_example = example;
}


float SimpleGLXWindow::getElapsedSeconds()
{
    unsigned int currentTime = GetTickCount();
    unsigned int diff = currentTime - m_lastTime;
    m_lastTime = currentTime;
    return float(diff) / 1000.0f;
}
