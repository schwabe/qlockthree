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
#ifndef SETTINGS_H
#define SETTINGS_H

#include "Arduino.h"
#include "TimeStamp.h"

class Settings {
public:
    Settings();

    byte getLanguage();
    void setLanguage(byte language);

    boolean getRenderCornersCw();
    void setRenderCornersCw(boolean cw);

    boolean getUseLdr();
    void setUseLdr(boolean useLdr);

    byte getBrightness();
    void setBrightness(byte brightness);

    boolean getEnableAlarm();
    void setEnableAlarm(boolean enableAlarm);

    boolean getDcfSignalIsInverted();
    void setDcfSignalIsInverted(boolean dcfSignalIsInverted);

    signed char getTimeShift();
    void setTimeShift(signed char timeShift);

    byte getJumpToTime();
    void setJumpToTime(byte jumpToTime);

    TimeStamp* getNightTimeStamp(byte _position);

    void loadFromEEPROM();
    void saveToEEPROM();

private:
    byte _language;
    boolean _renderCornersCw;
    boolean _use_ldr;
    byte _brightness;
    boolean _enableAlarm;
    boolean _dcfSignalIsInverted;

    signed char _timeShift;
    byte _jumpToTime;
    TimeStamp* _NightTimes[4];

    void _loadSaveNightTimes(boolean save);
};

#endif

