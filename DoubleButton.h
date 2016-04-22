/**
 * DoubleButton
 */
#ifndef DOUBLEBUTTON_H
#define DOUBLEBUTTON_H

#include "Arduino.h"
#include "Button.h"

class DoubleButton : public Button {
public:
    Button & b1;
    Button & b2;

    DoubleButton(Button & button1, Button & button2): b1(button1), b2(button2)
    {

    }

protected:
    virtual bool getState()
    {
        return b1.isPressed && b2.isPressed;
    }

};

#endif
