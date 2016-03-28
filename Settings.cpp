/**
 * Settings
 * Die vom Benutzer getaetigten Einstellungen werden hier verwaltet
 * und im EEPROM persistiert.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.3c
 * @created  23.1.2013
 * @updated  17.04.2016 (Ergänzungen von A. Mueller)
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - LDR-Modus aufgenommen.
 * V 1.2:  - Brightness aufgenommen.
 *         - EnableAlarm aufgenommen.
 *         - DcfSignalIsInverted aufgenommen.
 *         - TimeShift aufgenommen.
 * V 1.3:  - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt.
 * V 1.3a: - Einstellung für automatischen Rücksprung und Nachtschaltung aufgenommen.
 * V 1.3b: - Einstellung der Nachtschaltung um Sonderzeiten am Wochenende erweitert.
 * V 1.3c: - Codeoptimierung bei den Einstellungen zur Nachtschaltung
 */
#include "Settings.h"
#include <EEPROM.h>
#include "Renderer.h"

#define SETTINGS_MAGIC_NUMBER 0xCA
#define SETTINGS_VERSION 6

/**
 *  Konstruktor.
 */
Settings::Settings() {
    _language = LANGUAGE_DE_DE;
    _renderCornersCw = true;
    _use_ldr = true;
    _brightness = 75;
    _enableAlarm = false;
    _dcfSignalIsInverted = false;
    _timeShift = 0;
    _jumpToTime = 10;
    byte offTimeMoFrHours = 3;
    byte offTimeMoFrMinutes = 0;
    byte onTimeMoFrHours = 4;
    byte onTimeMoFrMinutes = 30;
    byte offTimeSaSoHours = 3;
    byte offTimeSaSoMinutes = 0;
    byte onTimeSaSoHours = 4;
    byte onTimeSaSoMinutes = 30;
    
    _NightTimes[0] = new TimeStamp(offTimeMoFrMinutes, offTimeMoFrHours, 0, 0, 0, 0);
    _NightTimes[1] = new TimeStamp(onTimeMoFrMinutes, onTimeMoFrHours, 0, 0, 0, 0);
    _NightTimes[2] = new TimeStamp(offTimeSaSoMinutes, offTimeSaSoHours, 0, 0, 0, 0);
    _NightTimes[3] = new TimeStamp(onTimeSaSoMinutes, onTimeSaSoHours, 0, 0, 0, 0);

    // Versuche alte Einstellungen zu laden...
    loadFromEEPROM();
}

/**
 * Die Sprache. Werte siehe Renderer.h
 */
byte Settings::getLanguage() {
    return _language;
}

void Settings::setLanguage(byte language) {
    _language = language;
}

/**
 * Die Laufrichtung der Eck-LEDs.
 */
boolean Settings::getRenderCornersCw() {
    return _renderCornersCw;
}

void Settings::setRenderCornersCw(boolean cw) {
    _renderCornersCw = cw;
}

/**
 * Der LDR-Modus.
 */
boolean Settings::getUseLdr() {
    return _use_ldr;
}

void Settings::setUseLdr(boolean useLdr) {
    _use_ldr = useLdr;
}

/**
 * Die Helligkeit.
 */
byte Settings::getBrightness() {
    return _brightness;
}

void Settings::setBrightness(byte brightness) {
    _brightness = brightness;
}

/**
 * Wecker enablen?
 */
boolean Settings::getEnableAlarm() {
    return _enableAlarm;
}

void Settings::setEnableAlarm(boolean enableAlarm) {
    _enableAlarm = enableAlarm;
}

/**
 * Ist das DCF-Signal invertiert?
 */
boolean Settings::getDcfSignalIsInverted() {
    return _dcfSignalIsInverted;
}

void Settings::setDcfSignalIsInverted(boolean dcfSignalIsInverted) {
    _dcfSignalIsInverted = dcfSignalIsInverted;
}

/**
 * Zeitverschiebung
 */
signed char Settings::getTimeShift() {
    return _timeShift;
}

void Settings::setTimeShift(signed char timeShift) {
    _timeShift = timeShift;
}

/**
 * Automatische Umschaltung zurück auf Uhrzeit
 */
byte Settings::getJumpToTime() {
    return _jumpToTime;
}

void Settings::setJumpToTime(byte jumpToTime) {
    _jumpToTime = jumpToTime;
}

/**
 * Pointer auf onTime oder offTime
 */
TimeStamp* Settings::getNightTimeStamp(byte _position) {
    /* 
     * 0: offTimeMoFr
     * 1: onTimeMoFr
     * 2: offTimeSaSo
     * 3: onTimeSaSo
     */
     return _NightTimes[_position];
}

/**
 * Die Einstellungen laden.
 */
void Settings::loadFromEEPROM() {
    if ((EEPROM.read(0) == SETTINGS_MAGIC_NUMBER) && (EEPROM.read(1) == SETTINGS_VERSION)) {
        // es sind gueltige Einstellungen vorhanden...
        _language = EEPROM.read(2);
        _renderCornersCw = EEPROM.read(3);
        _use_ldr = EEPROM.read(4);
        _brightness = EEPROM.read(5);
        _enableAlarm = EEPROM.read(6);
        _dcfSignalIsInverted = EEPROM.read(7);
        _timeShift = EEPROM.read(8);
        _jumpToTime = EEPROM.read(9);
        _loadSaveNightTimes(false);
    }
}

/**
 * Die Einstellungen speichern.
 */
void Settings::saveToEEPROM() {
    if (EEPROM.read(0) != SETTINGS_MAGIC_NUMBER) {
        EEPROM.write(0, SETTINGS_MAGIC_NUMBER);
    }
    if (EEPROM.read(1) != SETTINGS_VERSION) {
        EEPROM.write(1, SETTINGS_VERSION);
    }
    if (EEPROM.read(2) != _language) {
        EEPROM.write(2, _language);
    }
    if (EEPROM.read(3) != _renderCornersCw) {
        EEPROM.write(3, _renderCornersCw);
    }
    if (EEPROM.read(4) != _use_ldr) {
        EEPROM.write(4, _use_ldr);
    }
    if (EEPROM.read(5) != _brightness) {
        EEPROM.write(5, _brightness);
    }
    if (EEPROM.read(6) != _enableAlarm) {
        EEPROM.write(6, _enableAlarm);
    }
    if (EEPROM.read(7) != _dcfSignalIsInverted) {
        EEPROM.write(7, _dcfSignalIsInverted);
    }
    if (EEPROM.read(8) != _timeShift) {
        EEPROM.write(8, _timeShift);
    }
    if (EEPROM.read(9) != _jumpToTime) {
        EEPROM.write(9, _jumpToTime);
    }
    _loadSaveNightTimes(true);
}

void Settings::_loadSaveNightTimes(boolean save) {
    byte forStart = 10;
    byte forEnd = 17;
    byte getValue;
    for (byte i = forStart; i <= forEnd; i++) {
        byte value = EEPROM.read(i);
        byte i_forStart   = i - forStart;
        byte i_forStart_2 = i_forStart / 2;
        if (!(i_forStart % 2)) {
            // Hours
            if (save) {
                getValue = _NightTimes[i_forStart_2]->getHours();
            } else {
                _NightTimes[i_forStart_2]->setHours(value);
            }
        } else {
            // Minutes
            if (save) {
                getValue = _NightTimes[i_forStart_2]->getMinutes();
            } else {
                _NightTimes[i_forStart_2]->setMinutes(value);
            }
            
        }
        if (save && value != getValue) {
            EEPROM.write(i, getValue);
        }
    }
}
