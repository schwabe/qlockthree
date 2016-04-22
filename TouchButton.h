/**
 * Button
 * Kleine Klasse zum Entprellen der Tasten.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.7
 * @created  18.2.2011
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.1:  - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.2:  - Optimierung hinsichtlich Speicherbedarf.
 * V 1.3:  - Verbessertes Debugging.
 * V 1.4:  - Doppel-Tasten-Abfrage ermoeglicht.
 * V 1.5:  - Ueberlauf in millis() beruecksichtigt.
 * V 1.6:  - Schalten gegen LOW ermoeglicht.
 * V 1.7:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include "Arduino.h"
#include "Button.h"

#define TOUCHSAMPLES 100
#define TOUCHTHRESHOLD 40

class TouchButton : public Button {
public:
    /**
     * Initialisierung mit dem Pin, an dem der Taster haengt.
     *
     * @param  pin: der Pin, an dem die Touchfläche hängt
     */
    TouchButton(byte pin): pin(pin), samples(0), idleAvg(0) {}

    virtual bool getState();

private:
    uint8_t pin;
    int samples;
    double idleAvg;
};

#endif
