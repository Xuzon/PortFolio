#ifndef KEYBOARDINTERFACE_H_INCLUDED
#define KEYBOARDINTERFACE_H_INCLUDED

enum BOGLGPKeyCode {
    KC_INVALID,
    KC_UP,
    KC_DOWN,
    KC_LEFT,
    KC_RIGHT,
    KC_SPACE,
    KC_w,
    KC_a,
    KC_s,
    KC_d,
    KC_MAX_KEYS
};

class KeyboardInterface : private Uncopyable {
public:
    virtual ~KeyboardInterface() {}
    virtual bool isKeyPressed(BOGLGPKeyCode code) = 0;
    virtual bool isKeyHeldDown(BOGLGPKeyCode code) = 0;

    virtual void handleKeyDown(BOGLGPKeyCode code) = 0;
    virtual void handleKeyUp(BOGLGPKeyCode code) = 0;
    virtual BOGLGPKeyCode translateKey(unsigned int code) = 0; //Translate a OS key to a KeyCode
    virtual void update() = 0;
};

#endif // KEYBOARDINTERFACE_H_INCLUDED
