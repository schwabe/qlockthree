#include "TouchButton.h"
#include "Configuration.h"


/**
 * Wurde der Taster gedrueckt?
 */
boolean TouchButton::getState() {
    boolean _retVal = false;

    int t = touchRead(pin);
    samples++;

    const float alpha=0.1;

    // Moving exponential average
    idleAvg = (alpha * t) + (1.0 - alpha) * idleAvg;


    if (t > idleAvg + TOUCHTHRESHOLD)
        Serial.printf("%d: t %d, tavg %.1f, %d\r\n", pin, t, idleAvg,(int) (t- idleAvg));

    return (t > idleAvg + TOUCHTHRESHOLD);

}
