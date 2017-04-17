/**
 * TeensyRTC
 * Emuliert die MyRTC mit Teensys interner RTC
 *
 * @autor    Arne Schwabe
 * @version  0.1
 * @created  10.1.2016
 * @updated  10.1.2016
 *
 * Versionshistorie:
 * V0.1 First version
 */
#ifndef TEENSYRTC_H
#define TEENSYRTC_H

#include "MyRTC.h"
#include "TimeLib.h"

#define DEBUG
#include "Debug.h"

inline time_t getTeensy3Time()
{
    return Teensy3Clock.get();
}

class TeensyRTC : public MyRTC {
public:
    TeensyRTC(byte statusLedPin) :  MyRTC::MyRTC(0x0, statusLedPin) {
        setSyncProvider(getTeensy3Time);
    };

    void readTime() {
        time_t t=now();
        _seconds = second(t);
        _minutes = minute(t);
        _hours = hour(t);
        _date = day(t);
        _month = month(t);
        // Year is only a byte
        _year = year(t) % 100;
        _dayOfWeek = weekday(t);
    };

    void writeTime() {

        tmElements_t tm;
        tm.Year =  30 + _year; // years after 1970
        tm.Month = _month;
        tm.Day = _date;
        tm.Hour = _hours;
        tm.Minute = _minutes;
        tm.Second = _seconds;
        time_t t = makeTime(tm);
        Serial.printf("Teensy setting RTC, %d, %d-%d-%d %d:%d:%d", t, _year, _month, _date, _hours, _minutes, _seconds);
        Teensy3Clock.set(t);
        setTime(t);
    }
};

#endif
