/**
 * Ereignisse
 * Definition der Zeichen, die am definierten Tag statt
 * des Datums in der Datumsanzeige angezeigt werden.
 * Die Zeichen sind wie die Woerter Bitmasken fuer die Matrix.
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Müller / raffix _AT_ web _DOT_ de
 * @version  1.0
 * @created  13.03.2016
 * @updated  09.04.2016
 *
 * Versionshistorie:
 * V 1.1:  - Startzeit und -dauer für einen Countdown hinzugefügt.
 */
#ifndef EREIGNISSE_H
#define EREIGNISSE_H

#include <avr/pgmspace.h>

/*
 * Beispiel für einen Eintrag:
 * 1,1,31,12,23,45,15
 * -- Symbol in der Datumsanzeige --
 * Die ersten beiden Ziffern (1,1) geben an, dass am 01.01. das
 * definierte Symbol ganztägig in der Datumsanzeige angezeigt wird.
 * Dies hat mit dem Countdown zunächst nichts zu tun.
 * -- Countdown --
 * Neujahrs-Countdown 15 Minuten lang
 * 31,12,23,45,15 bedeutet, am 31.12. um 23:45 Uhr startet
 * der Countdown und geht 15 Minuten lang, also bis 01.01. 0:00 Uhr.
 * Ist der Countdown beendet, wird das definierte Symbol blinkend
 * COUNTDOWN_BLINK_DURATION Sekunden lang angezeigt.
 * Die Zeit ist in der Configuration.h anpassbar.
 */
struct EventdayObject {
  byte day;
  byte month;
  // countdownStart* gibt an, wann der Countdown startet
  byte countdownStartDay;
  byte countdownStartMonth;
  byte countdownStartHour;
  byte countdownStartMinute;
  // Wie lange soll der Countdown laufen?
  byte countdownMinutes;
  int symbol[10];
};

extern const EventdayObject eventdayObject[] PROGMEM;
const EventdayObject eventdayObject[] = {
    { 1,
      1,
      31,
      12,
      23,
      45,
      15,
      {
        0b00110001100,
        0b01001010010,
        0b00010010010,
        0b00100010010,
        0b01111001100,
        0b00010011110,
        0b00110000010,
        0b00010000100,
        0b00010001000,
        0b00111010000
      }
    }
    ,
    { 0,
      0,
      0,
      0,
      0,
      0,
      0,
      {
        0b00000000000,
        0b00010001000,
        0b00101010100,
        0b01000100010,
        0b01000000010,
        0b00100000100,
        0b00010001000,
        0b00001010000,
        0b00000100000,
        0b00000000000
      }
    }  
};

#endif
