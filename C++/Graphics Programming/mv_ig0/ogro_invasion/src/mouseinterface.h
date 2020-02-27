#ifndef MOUSEINTERFACE_H_INCLUDED
#define MOUSEINTERFACE_H_INCLUDED

class MouseInterface
{
public:
    virtual void getMousePos(int& x, int& y) = 0;
    virtual void setMousePos(int x, int y) = 0;
    virtual void showCursor(bool val) = 0;
    virtual bool isButtonPressed(int button) = 0;
    virtual void update() = 0;
};

#endif // MOUSEINTERFACE_H_INCLUDED
