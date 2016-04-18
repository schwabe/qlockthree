/**
 * LedDriverDefault
 * Implementierung auf der Basis 74HC595 und UDN2981A.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.4
 * @created  18.1.2013
 * @updated  18.1.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - printSignature() eingefuehrt.
 *         - Benennung verbessert.
 * V 1.2:  - Anpassung Helligkeit 0-100%
 * V 1.3:  - Getter fuer Helligkeit nachgezogen.
 * V 1.4:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 */
#include "LedDriverDefault.h"
#include "Configuration.h"

// #define DEBUG
#include "Debug.h"

// Hilfsvariable, um das Display auf den Kopf zu stellen
#ifdef UPSIDE_DOWN
#define DISPLAY_SHIFT  9-
#else
#define DISPLAY_SHIFT  0+
#endif

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 * @param clock Pin, an dem die Clock-Line haengt.
 * @param latch Pin, an dem die Latch-Line haengt.
 * @param outputEnable Pin, an dem OutputEnable haengt.
 * @param linesToWrite Wieviel Zeilen muessen aus dem Bildspeicher uebernommen werden?
 */
LedDriverDefault::LedDriverDefault(byte data, byte clock, byte latch, byte outputEnable, byte linesToWrite) {
    _shiftRegister = new ShiftRegister(data, clock, latch);
    _outputEnablePin = outputEnable;
    pinMode(_outputEnablePin, OUTPUT);
    digitalWrite(_outputEnablePin, HIGH);
    _linesToWrite = linesToWrite;
}

/**
 * init() wird im Hauptprogramm in init() aufgerufen.
 * Hier sollten die LED-Treiber in eine definierten
 * Ausgangszustand gebracht werden.
 */
void LedDriverDefault::init() {
}

void LedDriverDefault::printSignature() {
    Serial.println(F("Default - 74HC595"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverDefault::writeScreenBufferToMatrix(word matrix[16], boolean onChange) {
    word row = 1;

    // Jetzt die Daten...
    // wir brauchen keine 16, 10 wuerden reichen... dann gehen aber nicht
    // mehr alle Modi! Also via Variable, die im Modus-Wechsel geaendert wird...
    for (byte k = 0; k < _linesToWrite; k++) {
#ifdef SKIP_BLANK_LINES
        if (matrix[DISPLAY_SHIFT k] != 0) {
#endif
            // Zeile einschalten...
            _shiftRegister->prepareShiftregisterWrite();
            _shiftRegister->shiftOut(~matrix[DISPLAY_SHIFT k]);
            _shiftRegister->shiftOut(row);
            _shiftRegister->finishShiftregisterWrite();

            // hier kann man versuchen, das Taktverhaeltnis zu aendern (Anzeit)...
            // delayMicroseconds mit Werten <= 3 macht Probleme...
            if (_brightnessInPercent > 3) {
                delayMicroseconds(_brightnessInPercent * PWM_DURATION);
            }

            // Zeile ausschalten (einfach ganze Matrix gegenlaeufig schalten)...
            _shiftRegister->prepareShiftregisterWrite();
            _shiftRegister->shiftOut(65535);
            _shiftRegister->shiftOut(0);
            _shiftRegister->finishShiftregisterWrite();

            // hier kann man versuchen, das Taktverhaeltnis zu aendern (Auszeit)...
            // delayMicroseconds mit Werten <= 3 macht Probleme...
            if (_brightnessInPercent < 97) {
                delayMicroseconds((100 - _brightnessInPercent) * PWM_DURATION);
            }

#ifdef SKIP_BLANK_LINES
        }
#endif

        row = row << 1;
    }
}

/**
 * Die Helligkeit des Displays anpassen.
 *
 * @param brightnessInPercent Die Helligkeit.
 */
void LedDriverDefault::setBrightness(byte brightnessInPercent) {
    _brightnessInPercent = brightnessInPercent;
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverDefault::getBrightness() {
    return _brightnessInPercent;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverDefault::setLinesToWrite(byte linesToWrite) {
    _linesToWrite = linesToWrite;
}

/**
 * Das Display ausschalten.
 */
void LedDriverDefault::shutDown() {
    digitalWrite(_outputEnablePin, HIGH);
}

/**
 * Das Display einschalten.
 */
void LedDriverDefault::wakeUp() {
    digitalWrite(_outputEnablePin, LOW);
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverDefault::clearData() {
    _shiftRegister->prepareShiftregisterWrite();
    _shiftRegister->shiftOut(65535);
    _shiftRegister->shiftOut(0);
    _shiftRegister->finishShiftregisterWrite();
}
