#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#ifdef _WIN32
#include <windows.h>
#include "glwindow.h"
#else
#include "glxwindow.h"
#endif

#include "example.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
#else
int main(int argc, char** argv)
#endif
{
    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;
    bool windowFullscreen = false;

    //This is our window
#ifdef _WIN32
    GLWindow programWindow(hInstance);

    if (MessageBox(NULL, "Would you like to run in fullscreen mode?", "Fullscreen?", MB_ICONINFORMATION | MB_YESNO) == IDYES)
    {
        windowFullscreen = true;
    }
#else
    SimpleGLXWindow programWindow;
#endif


    //Attempt to create the window
    if (!programWindow.create(windowWidth, windowHeight, windowBPP, windowFullscreen))
    {
        //If it fails
#ifdef _WIN32
        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    //The example OpenGL code
    Example example(&programWindow);

    //Attach our example to our window
    programWindow.attachExample(&example);

    if (!example.init()) //Initialize our example
    {
#ifdef _WIN32
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
#endif
        programWindow.destroy(); //Reset the display and exit
        return 1;
    }

    //This is the mainloop, we render frames until isRunning returns false
    while(programWindow.isRunning())
    {
        programWindow.processEvents(); //Process any window events

        //We get the time that passed since the last frame
        float elapsedTime = programWindow.getElapsedSeconds();

        example.prepare(elapsedTime); //Do any pre-rendering logic
        example.render(); //Render the scene

        programWindow.swapBuffers();
    }

    example.shutdown(); //Free any resources
    programWindow.destroy(); //Destroy the program window

    return 0; //Return success
}


