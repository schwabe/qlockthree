/**
 * LedDriverLPD8806
 * Implementierung auf der Basis von LPD8806-Streifen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  9.2.2015
 * @updated  16.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 *
 * Verkabelung: Einspeisung oben links, dann schlangenfoermig runter,
 * dann Ecke unten links, oben links, oben rechts, unten rechts.
 *
 * Achtung! LPD8806-Streifen koennen nur in Vielfachen von 2 getrennt werden! Daher bleiben am Rand LEDs uebrig (und dunkel)!
 *
 */
#include "LedDriverLPD8806.h"
#include "Configuration.h"

// #define DEBUG
#include "Debug.h"

// eigentlich haben wir ja 114 LEDs, aber LPD8806 sind ja immer zweier...
#define NUM_PIXEL 128

/**
 * Initialisierung.
 *
 * @param data Pin, an dem die Data-Line haengt.
 */
LedDriverLPD8806::LedDriverLPD8806(byte dataPin, byte clockPin) {
    _dataPin = dataPin;
    _clockPin = clockPin;
    _strip = new LPD8806(NUM_PIXEL, _dataPin, _clockPin);
    _strip->begin();
    setColor(250, 255, 200);
}

/**
 * init() wird im Hauptprogramm in init() aufgerufen.
 * Hier sollten die LED-Treiber in eine definierten
 * Ausgangszustand gebracht werden.
 */
void LedDriverLPD8806::init() {
    setBrightness(50);
    clearData();
    wakeUp();
}

void LedDriverLPD8806::printSignature() {
    Serial.println(F("LPD8806"));
}

/**
 * Den Bildschirm-Puffer auf die LED-Matrix schreiben.
 *
 * @param onChange: TRUE, wenn es Aenderungen in dem Bildschirm-Puffer gab,
 *                  FALSE, wenn es ein Refresh-Aufruf war.
 */
void LedDriverLPD8806::writeScreenBufferToMatrix(word matrix[16], boolean onChange) {
    if (onChange || _dirty) {
        _dirty = false;
        _clear();

        uint32_t color = _strip->Color(_brightnessScaleColor(getRed()), _brightnessScaleColor(getBlue()), _brightnessScaleColor(getGreen()));

        for (byte y = 0; y < 10; y++) {
            for (byte x = 5; x < 16; x++) {
                word t = 1 << x;
                if ((matrix[y] & t) == t) {
                    _setPixel(15 - x, y, color);
                }
            }
        }

        // wir muessen die Eck-LEDs umsetzten...
        if ((matrix[1] & 0b0000000000011111) == 0b0000000000011111) {
            _setPixel(110, color); // 1
        }
        if ((matrix[0] & 0b0000000000011111) == 0b0000000000011111) {
            _setPixel(111, color); // 2
        }
        if ((matrix[3] & 0b0000000000011111) == 0b0000000000011111) {
            _setPixel(112, color); // 3
        }
        if ((matrix[2] & 0b0000000000011111) == 0b0000000000011111) {
            _setPixel(113, color); // 4
        }

        _strip->show();
    }
}

/**
 * Die Helligkeit des Displays anpassen.
 *
 * @param brightnessInPercent Die Helligkeit.
 */
void LedDriverLPD8806::setBrightness(byte brightnessInPercent) {
    if (brightnessInPercent != _brightnessInPercent) {
        _brightnessInPercent = brightnessInPercent;
        _dirty = true;
    }
}

/**
 * Die aktuelle Helligkeit bekommen.
 */
byte LedDriverLPD8806::getBrightness() {
    return _brightnessInPercent;
}

/**
 * Anpassung der Groesse des Bildspeichers.
 *
 * @param linesToWrite Wieviel Zeilen aus dem Bildspeicher sollen
 *                     geschrieben werden?
 */
void LedDriverLPD8806::setLinesToWrite(byte linesToWrite) {
}

/**
 * Das Display ausschalten.
 */
void LedDriverLPD8806::shutDown() {
    _clear();
    _strip->show();
}

/**
 * Das Display einschalten.
 */
void LedDriverLPD8806::wakeUp() {
}

/**
 * Den Dateninhalt des LED-Treibers loeschen.
 */
void LedDriverLPD8806::clearData() {
    _clear();
    _strip->show();
}

/**
 * Einen X/Y-koordinierten Pixel in der Matrix setzen.
 */
void LedDriverLPD8806::_setPixel(byte x, byte y, uint32_t c) {
    _setPixel(x + (y * 11), c);
}

/**
 * Einen Pixel im Streifen setzten (die Eck-LEDs sind am Ende).
 */
void LedDriverLPD8806::_setPixel(byte num, uint32_t c) {
    if (num < 110) {
        if ((num / 11) % 2 == 0) {
            _strip->setPixelColor(num + (num / 11), c);
        } else {
            _strip->setPixelColor(((num / 11) * 12) + 11 - (num % 11), c);
        }
    } else {
        switch (num) {
            case 110:
                _strip->setPixelColor(111 + 11, c);
                break;
            case 111:
                _strip->setPixelColor(112 + 12, c);
                break;
            case 112:
                _strip->setPixelColor(113 + 13, c);
                break;
            case 113:
                _strip->setPixelColor(110 + 10, c);
                break;
        }
    }
}

/**
 * Funktion fuer saubere 'Regenbogen'-Farben.
 * Kopiert aus den Adafruit-Beispielen (strand).
 */
uint32_t LedDriverLPD8806::_wheel(byte wheelPos) {
    if (wheelPos < 85) {
        return _strip->Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if (wheelPos < 170) {
        wheelPos -= 85;
        return _strip->Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
        wheelPos -= 170;
        return _strip->Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
}

/**
 * Hilfsfunktion fuer das Skalieren der Farben.
 */
byte LedDriverLPD8806::_brightnessScaleColor(byte colorPart) {
    return map(_brightnessInPercent, 0, 100, 0, colorPart / 2); // LPD8806 kann nur 7 bit Farben! (also 0..127, nicht 0..255)
}

/**
 * Streifen loeschen.
 */
void LedDriverLPD8806::_clear() {
    for (byte i = 0; i < NUM_PIXEL; i++) {
        _strip->setPixelColor(i, 0);
    }
}
