/**
 * DigitalButton
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
#include "DigitalButton.h"

// #define DEBUG
#include "Debug.h"

#include "Configuration.h"

/**
 * Initialisierung mit dem Pin, an dem der Taster haengt.
 *
 * @param  pin: der Pin, an dem der Taster haengt
 *         pressedAgainst: wogegen schaltet der Taster? (HIGH/LOW)
 */
DigitalButton::DigitalButton(byte pin, byte pressedAgainst) {
    _pin1 = pin;
    _pressedAgainst = pressedAgainst;
    if (_pressedAgainst == HIGH) {
        pinMode(_pin1, INPUT);
    } else {
        pinMode(_pin1, INPUT_PULLUP);
    }
}

/**
 * Wurde der Taster gedrueckt?
 */
bool DigitalButton::getState() {
    return (digitalRead(_pin1) == _pressedAgainst);
}
