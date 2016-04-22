#include "Button.h"
#include "Configuration.h"

bool Button::pressed()
{
    isPressed = getState();
 
    if (isPressed && (millis() - BUTTON_TRESHOLD) >) _lastPressTime ) {
        _lastPressTime = millis();
        return true;
    } else {
        return false;
    }
}
