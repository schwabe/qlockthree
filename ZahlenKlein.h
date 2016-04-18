/**
 * Zahlen Klein
 * Definition der Zahlen fuer die Datumsanzeige der QLOCKTWO.
 * Die Zahlen sind wie die Woerter Bitmasken fuer die Matrix.
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Mueller
 * @version  1.0
 * @created  13.03.2016
 * @updated  13.03.2016
 *
 * Versionshistorie:
 * V 1.0:  - 
 */
#ifndef ZAHLENKLEIN_H
#define ZAHLENKLEIN_H

#include <avr/pgmspace.h>

extern const char ziffernKlein[][5] PROGMEM;
const char ziffernKlein[][5] = {
    { // 0:0
        0b0110,
        0b1001,
        0b1001,
        0b1001,
        0b0110
    }
    ,
    { // 1:1
        0b0010,
        0b0110,
        0b0010,
        0b0010,
        0b0111
    }
    ,
    { // 2:2
        0b0110,
        0b1001,
        0b0010,
        0b0100,
        0b1111
    }
    ,
    { // 3:3
        0b1110,
        0b0001,
        0b0110,
        0b0001,
        0b1110
    }
    ,
    { // 4:4
        0b0010,
        0b0110,
        0b1010,
        0b1111,
        0b0010
    }
    ,
    { // 5:5
        0b1111,
        0b1000,
        0b1110,
        0b0001,
        0b1110
    }
    ,
    { // 6:6
        0b0110,
        0b1000,
        0b1110,
        0b1001,
        0b0110
    }
    ,
    { // 7:7
        0b1111,
        0b0001,
        0b0010,
        0b0100,
        0b1000
    }
    ,
    { // 8:8
        0b0110,
        0b1001,
        0b0110,
        0b1001,
        0b0110
    }
    ,
    { // 9:9
        0b0110,
        0b1001,
        0b0111,
        0b0001,
        0b0110
    }
};

#endif
