/**
 * Clockthree
 * Die Firmware der Selbstbau-QLOCKTWO.
 *
 * @mc       Arduino/RBBB (ATMEGA328)
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  3.4.9b4
 * @created  1.11.2011
 * @updated  17.04.2016 (Ergänzungen von A. Mueller)
 *
 * Versionshistorie:
 * V 1.1:   - DCF77 auf reine Zeit ohne Strings umgestellt.
 *          - Ganzes Datum wird in die DS1307 geschrieben (wieder verworfen).
 * V 1.2:   - Schnelles Schreiben der Shift-Register eingefuehrt.
 * V 1.3:   - Auf eigene DCF77-Bibliothek umgestellt (MyDCF77) und DCF77Helper verbessert.
 * V 1.4:   - Glimmen/Nachleuchten dank Tipp von Volker beseitigt (http://www.mikrocontroller.net/topic/209988).
 *          - Erneute Fehlerbereinigung in der DCF77-Bibliothek.
 * V 1.5:   - Die Helligkeit laesst sich ueber die Variable 'brightness' einstellen (Bereich 1-9) bzw.
 *              ueber den Compiler-Schalter 'ENABLE_LDR' die Regelung einschalten.
 * V 1.6:   - Fehler in der DCF77Helper-Bibliothek behoben.
 * V 1.7:   - SKIP_BLANK_LINES eingefuehrt, damit hellere LEDs (Danke an Johannes).
 * V 1.8:   - UPSIDE_DOWN eingefuehrt, damit man die Kabel beim Anoden-Multiplexer nicht kreuzen muss.
 * V 1.9:   - Diverse Verbesserungen und Fehlerbehebungen, die das Flackern drastisch reduzieren.
 * V 1.9.1: - Fehler behoben, der die Nutzung eines analogen Pins fuer den DCF77-Empfaenger verhindert hat.
 * V 1.9.2: - Timing-Parameter fuer die PWM ueber DEFINE anpassbar.
 *          - LDR-Grenzwerte automatisch anpassbar.
 *          - Schreibfehler in setHoures()->setHours() behoben.
 *          - Kompatibilitaet zu Arduino-IDE 1.0 hergestellt.
 * V 1.9.3: - Glimmen wieder behoben.
 * V 1.9.4: - Modus zum Einstellen der Helligkeit eingefuehrt.
 * V 2.0:   - Sprachrelevante Dinge ausgelagert, so kann man mehr Sprachen haben und einfach durch einkommentieren aktivieren.
 *          - setWords in setMinutes und setMinutes in setCorners umbenannt, weil es mehr Sinn ergibt.
 *          - setCorners in eigene Datei ausgelagert, weil viele Bastler sich vertun und in der Routine Aenderungen vornehmen muessen.
 *          - LDR in eigene Klasse ausgelagert und Werte geglaettet. Am Anfang werden 1000 Werte gelesen, damit er sich einpegelt.
 *          - Signal vom DCF77-Empfaenger geglaettet, damit nicht ein einzelner falscher Peak das Telegramm zerstoert.
 *          - Status-LEDs koennen ueber DEFINEs ausgeschaltet werden.
 *          - DCF77- und SQW-LED blinken am Anfang drei Mal als 'Hello', damit man ohne Serial-Monitor sehen kann, ob der ATMEGA anlaeuft.
 *          - Serial-Output ist immer an, kleine Statusmeldung waehrend der Initialisierung, damit man beim Bespielen des ATMEGA ohne weitere
 *              Elektronik sehen kann, ob das Programm drauf ist und laeuft.
 *          - Aenderung, die ein Stellen der Zeit nach dem DCF77-Signal auch im Modus 'BLANK' ermoeglicht - in diesem Modus ist der Empfang
 *              weniger gestoert, da die LED-Matrix abgeschaltet ist.
 *          - Zeitgesteuertes Abschalten des Displays eingefuehrt (Stromsparen in der Nacht/Schlafzimmer/besserer Empfang des DCF77-Empfaengers).
 *          - Speaker auf D13 eingefuehrt.
 *          - Alarm-Mode eingefuehrt.
 *          - Bayrische Sprachvariante (DE_BA): 'viertel nach Zehn', aber 'dreiviertel Elf'.
 * V 2.0.1: - DCF77-Tresholds angepasst.
 * V 2.0.2: - Strings in PROGMEM (flash) ausgelagert.
 * V 2.0.3: - SPEAKER_IS_BUZZER eingefuehrt, damit kann man sagen, ob man einen Lautsprecher oder Piezo-Buzzer als Alarm verwendet.
 * V 2.0.4: - falsches BREAK dank Lars behoben.
 * V 2.0.5: - Franzoesisch hinzugefuegt, Woerter_??.h's anschaulicher gemacht. Weitere Sprachen angelegt, aber noch nicht die Logik implementiert (Hilfe?!).
 * V 2.0.6: - cleanWordsForAlarmSettingMode() eingefuehrt, damit das Stellen des Alarms sprachenunabhaengig ist.
 *          - Das DCF77-Signal kann per Compiler-Schalter invertiert werden.
 * V 2.0.7: - Neuer Modus: Umschaltung LDR auto/manuell dank Alexander.
 * V 2.0.8: - Genauigkeit verbessert, dank Peter (Interrupt auf FALLING). @Peter: Das Zurueckscheiben in die DS1307 passiert im Normalbetrieb ja
                nur beim Update ueber den DCF77-Empfaenger, daher habe ich es weggelassen.
 * V 2.0.9: - SplitSideDown rausgenommen, diesen seltenen Fehler braucht kein Mensch.
 * V 2.1:   - Neue Sprachen hinzugefuegt: Englisch und Italiensch. Spanisch ist angelegt, aber nicht implementiert, weil ich kein Wort Spanisch spreche.
 *          - Man kann jetzt einfach relativ weit oben im Code ueber Compiler-Schalter festlegen, welche Sprache man haben moechte.
 * V 2.1.1: - Bitmaps in den PROGMEM ausgelagert. So kann man alle Buchstaben halten.
 * V 2.1.2: - Bug im Alaram-Modus behoben.
 * V 2.1.3: - Zusaetzlich zu den Ziffern auch Staben eingefuehrt, A/M-Modus darauf umgestellt (Vorbereitung auf Konfigurationsmoeglichkeit
 *              fuer alle Sprachen in einer Firmware)
 * V 2.1.4: - Optimierung hinsichtlich Speicherbedarf.
 * V 3.0:   - LedDriver-Klasse erstellt, welche die Shift-Register und UDN2981A als Led-Treiber kapseln. Dadurch kann man auch andere
 *              LED-Treiber nehmen (MAX7219, AS1130 etc.) und einfach eine passende Klasse von der LedDriver-Klasse vererben und in den Code einklinken.
 *          - Alle Sprachen im 'Renderer' zusammengefasst, so dass sie ohne die Firmware neu zu flashen umschaltbar werden.
 *          - Niederlaendisch hinzugefuegt (Danke an Wekog24 und Rudolf K.).
 *          - Eine kleine Debug-Klasse eingefuehrt, damit die #debug-Statements nicht so stark den Code zerhacken und er besser lesbar wird.
 *          - Beim Starten wird der freie Speicher und die Version auf der Console ausgegeben.
 *          - Eine Alarm-Klasse eingefuehrt, das macht den 'Hauptcode' nochmal besser lesbar.
 *          - Configuration.h eingefueht, in der alle wichtigen Compiler-Schalter zusammengefuehrt sind, dadurch gibt es eine zentrale Stelle
 *              fuer die Anpassungen (Danke fuer das Konzept an Markus K.).
 *          - Settings.h eingefuehrt, in der die Spracheinstellung gehalten wird und im EEPROM gespeichert wird (Danke fuer das Konzept an Markus K.).
 *          - Modes zum Einstellen der Sprache und des Eck-Modus eingefuehrt.
 *          - Extended Modes eingefuehrt.
 *          - Saechsisch als Sprache aufgenommen, danke an Sven.
 *          - Bug im setzten der Eck-LEDs fuer den Ueberpixel behoben.
 *          - Brightness ist normaler Mode, aber nur wenn ldr_auto == FALSE.
 *          - Automatische Alarm-Abschaltung nach MAX_BUZZ_TIME_IN_MINUTES.
 *          - Stellen der Uhr nur im erweiterten Modus moeglich (sonst verstellt man sie leicht bei der Verwendung als Wecker).
 *          - LDR-Modus wird im EEPROM abgelegt.
 *          - kleinere Korrekturen, die nur in erweiterten Compiler-Ausgaben zu Warnings gefuehrt haben.
 *          - Kleinere Optimierungen, Danke an Michael K.
 *          - Bug aufgrund eines moeglichen Ueberlaufs von millis() in Button.cpp behoben.
 *          - Verbesserungen in der Helligkeitssteuerung.
 *          - ENABLE_LDR aus Configuration.h entfernt, da sich der LDR ueber das erweiterte Setup ausschalten laesst.
 * V 3.0.1: - Schweizer Layout angepasst.
 * V 3.1:   - Spanisch implementiert.
 *          - Der LDR skaliert jetzt selbst, dann flackert es weniger bei unguenstigen Lichtverhaeltnissen.
 * V 3.1.1. - Die Displayhelligkeit wird beim starten gesetzt.
 *          - Kleiner Fehler im Spanischen behoben.
 *          - Die alte Shiftregistermethode ist wieder aktiv, die schnelle kann in Configuration.h gesetzt werden.
 * V 3.1.2. - Led-Driver-Klassen sagen, wer sie sind (fuer die Ausgabe der Konfiguration beim Start).
 *          - Klassen-Namen verbessert.
 * V 3.2.   - Led-Driver fuer das Licht-Monster implementiert.
 *          - LDR-Klasse auf 0-100% umgebaut.
 *          - Brightness wird im EEPROM gesichert.
 *          - EnableAlarm und DCF77SignalIsInverted ins EEPROM ausgelagert und ueber das erweiterte Setup einstellbar.
 *          - Die Modi SCRAMBLE und ALL entfernt, dafuer den Modus TEST eingefuehrt. ALL ist fuer manche DisplayDriver gefaehrlich wegen des Stromverbrauchs. TEST schalte nacheinander alle LEDs einmal durch.
 *          - Moeglichkeit zur Zeitverschiebung eingefuehrt.
 * V 3.2.1. - Alle Deutsch-Varianten zusammengefasst, um Platz zu sparen.
 *          - Fehler im Italienischen behoben.
 *          - Fehler mit Doppelbelegung einer Variable im Qlockthree.ino behoben.
 *          - Der LDR wird nur alle 250ms gelesen, dann flackert er weniger bei unguenstigen Verhaeltnissen.
 * V 3.2.2. - DS1307 Multi-MCU-Faehig gemacht.
 *          - Initialisierung der DS1307 verbessert.
 * V 3.2.3. - A0-Hack: Verbindet man die freie A0-Leitung mit PON vom DCF77-Empfaenger, schaltet diese den Empfaenger korrekt
 *              ein und aus, je nach Displayzustand. Ist PON fest mit GND verbunden, passiert nichts (schadet aber auch  nicht).
 * V 3.2.4. - Der DCF-Empfaenger wird ueber A0 am Anfang eingeschaltet, dann nicht mehr ausgeschaltet.
 *          - Wird in den Blank-Modus geschaltet (Display aus), schaltet sich nach dem erfolgreichen Empfang der Zeit ueber DCF77
 *              das Display wieder ein (hilft beim Debuggen).
 *          - Der Nachtabschaltmodus ist ein eigener, nicht manuell erreichbarer Modus.
 * V 3.3.0. - LedDriverNeoPixel fuer den Aufbau mit WS2812B-Streifen eingefuehrt (Verkabelungsschema ist beim LedDriverNeoPixel)...
 *          - ...dazu die Klasse Button angepasst, so dass sie auch gegen GND schalten kann (damit Unterstuetzung fuer das BBRTCAD).
 *          - Achtung! Wenn die NeoPixel-Library im Librarys-Ordner ist, kann ich keinen Pointer auf den Stripe (in LedDriverNeoPixel.h)
 *              definieren (ich weiss leider nicht warum, es kommt eine seltsame Compiler-Meldung). Wenn sie im Qlockthree-Ordner ist, geht es.
 *          - IR-Fernbedienungs-Unterstuetzung eingefuehrt. Achtung! Wegen Platzmangels alle IRs ausser der Sparkfun (NEC) auskommentiert. Siehe weitere
 *              Infos weiter unten und bei der IR-Fernbedienung!
 *          - Achtung! Weil ich die IR-Receiver-Library (https://github.com/shirriff/Arduino-IRremote/) entschlacken musste, ist sie hier
 *              im Qlockthree-Ordner. Wenn sie auch im Librarys-Ordner ist, schlaegt die Compilierung fehl! Wer andere Codes als die von der
 *              SparkFun-Fernbedienung (NEC) nehmen mechte, muss die entsprechenden Typen in der Datei IRremote.cpp so ab Zeile 427 wieder
 *              einkommentieren. Die Implementierung hier ist beispielhaft zu sehen, um eigene Ideen verwirklichen zu koennen.
 *          - Test-Modus geaendert, damit er mit der IR-Fernbedienung klappt. Die Eck-LEDs zaehlen im Sekunden-Takt hoch, die Spalten werden
 *              im Sekunden-Takt nach rechts durchgeschaltet.
 *          - DCF-Empfang verbessert, dank Tipp aus dem Forum (unguenstiger INTERNAL_PULLUP beim Signal-Pin; PULL-Widerstaende muessen in Hardware
 *              je nach verwendetem Empfaenger realisiert werden).
 *          - Flackern in Ext-Menues und Manual-Brightness behoben, falsch durchdachtes linesToWrite-Statement (interessiert nur DefaultLedDriver,
 *              alle anderen ignorieren es eh).
 *          - Fehler in der DS1307-Klasse behoben. Nicht immer kommen die angeforderten 7 Bytes, dann verwerfen und neu anfordern (und nach
 *              8 Versuchen abbrechen). Der I2C-Bus ist wohl speziell bei LED-Stripes gerne mal gestoert, dann kommen die beruehmten '85' Sekunden.
 *          - Mode fuer die Visualisierung des DCF77-Empfangs (im Ext-Menue nach dem LED-Test) eingefuehrt. Man kann damit abschaetzen, wie gut der
 *              DCF77-Empfang ist. Mit der gelben LED muessen die Ecken durchschalten. Schalten sie wild durcheinander, gibt es Fehlsignale.
 *              Das Problem ist, dass ja die LED-Matrix den Empfang stoert. So stoert natuerlich potentiell auch die Visualisierung den Empfang.
 *              Mit einem WortWecker hat das dennoch ganz gut geklappt. Mit dem experimentellen LedDriverNeoPixel nicht.
 *          - Kleinen Fehler im Italienischen behoben, danke an die diversen Hinweisgeber!
 * V 3.3.1. - Stundenbegrenzung im Renderer (die ja wegen der Zeitverschiebungsmoeglichkeit existiert) auf den Bereich 0 <= h <= 24 ausgeweitet,
 *              dank Tipp aus dem Forum.
 * V 3.4.0. - Die DCF77-Decoding-Klasse vollstaendig umgestellt. Alle Beispiele im Internet gehen davon aus, dass man keine Vergleichszeit hat.
 *              Da wir hier eine Echtzeituhr haben, kann man statistische Methoden zur Auswertung verwenden. Wenn das Signal optisch brauchbar aussieht (gelbe LED),
 *              Synchronisiert die Zeit in weniger als 4 Minuten. Aber es muss eben brauchbar aussehen. Kurzes Aufblinken 1 Mal pro Sekunde, eine
 *              Pause pro Minute.
 * V 3.4.1. - Die Adafruit- und IR-Library umbenannt ("My" vorangestellt), damit sie nicht mit den offiziellen Librarys kollidieren. Warum sie hier lokal liegen,
 *              steht weiter oben.
 * V 3.4.2. - LDR-Bugs behoben und -Klasse optimiert. Dazu auch Getter fuer die Helligkeit in den Display-Drivern eingefuehrt.
 * V 3.4.3. - Vergessene DEBUGs auskommentiert, Check der DS1307 beim Start verbessert (Danke an Erich M.).
 * V 3.4.4. - Verbesserungen in der RTC (setzten der Zeit durch Compile-Zeit moeglich).
 *          - Adafruits NeoPixel-Library muss jetzt im Librarys-Ordner liegen.
 * V 3.4.5. - DS1307 nach MyRTC umbenannt, weil es nicht mehr nur um die DS1307 geht.
 *          - Einschalten des Rechtecksignals fuer DS1307 und DS3231 getrennt (Danke an Erich M.) - Konfiguration in der Configuration.h.
 *          - Unterstuetzung verschiedener IR-Fernbedienungen vereinfacht. Codes von NEC-compatiblen Fernbedienungen werden angezeigt, wenn eine Fernbedienung in
 *              der Configuration.h eingeschaltet ist (muss nicht die richtige sein) und #DEBUG an ist. Drei Beispiel-Remotes sind angelegt: Sparkfun,
 *              Mooncandles, Lunartec. Bei der Verwendung von Nicht-NEC-Fernbedienungen muss der entsprechende Part in MyIRremote.cpp einkommentiert werden,
 *              ein guter Start ist Zeile 428.
 *          - Im Zusammenhang mit den Fernbedienungen setColor() fuer die LED-Driver eingefuehrt. Damit kann man die Farben von RGB-Stripes aendern. Bei
 *              dem klassischen Aufbau mit einfarbeigen LEDs (75HC595; MAX7219 etc.) passiert natuerlich nichts.
 *          - Unterstuetzung fuer APA102-Stripes (DotStars) eingefuehrt, die Adafruit APA102-Library muss dazu im librarys-Ordner liegen.
 *          - FPS werden bei eingeschaltetem #DEBUG angezeigt.
 *          - Die Helligkeitsregelung laeuft jetzt sanfter. Timing-Werte fuer die verschiedenen LedDriver in der Configuration.h hinterlegt.
 * V 3.4.6. - Unterstuetzung fuer LPD8806-Stripes eingefuehrt, die Adafruit LPD8806-Library muss dazu im librarys-Ordner liegen. Achtung! Zur Zeit hat die LPD8806-Library
 *              von Adafruit einen Bug. Es fehlt das klassische #ifndef __CLASSNAME__ #define __CLASSNAME__ [Klasssendefinition] #endif um die Klasse im .h-File.
 * V 3.4.7. - Unterstuetzung fuer Buttons auf rein analoge Ports hinzugefuegt, wie sie der ATMEGA328 in SMD-Ausfuehrung hat.
 *          - Unterstuetzung fuer die alte Arduino-IDE (bis 1.0.6) entfernt, da sich die Firmware damit eh nicht mehr kompilieren laesst.
 *          - Library fuer MAX7219 (LedControl) ausgelagert, sie muss jetzt im Librarys-Ordner liegen.
 * V 3.4.8. - HelperSeconds-Behandlung in Interrupt-Funktion verschoben, damit die nicht aufgrund von Tastendruecken hochgezaehlt werden, danke an Meikel.
 * V 3.4.8a - Zusätzliche Features hinzugefügt:
 *            * Datumsanzeige
 *            * Automatischer Rücksprung auf Uhr (Rücksprung standardmäßig von Sekunden-, Datumsanzeige und Helligkeitsanzeige)
 *            * Nachschaltung einstellbar via Menü
 *            * Nachschaltung abbrechbar via Mode-Taste
 *            * Nachschaltung wieder reaktivierbar via gleichzeitigem Tastendruck von Hour- und Minute-Taste
 *            * Anzeige der vergangenen Stunden und Minuten seit der letzten erfolgreichen DCF-Synchronisation (im Menü)
 * V 3.4.8b - Neue DCF-Decoding-Klasse mit automatische Driftkorrektur, die eine fehlerhafte Drift zwischen RTC und DCF auszugleichen vermag und
 *            dadurch robuster arbeitet. 
 *            - Zuverlässigkeit der Zeitsynchronisation erhöht:
 *             -- Drifts in der Phase werden durch eine automatische Offsetkorrektur bereinigt.
 *             -- Damit ist die Erkennung der Schaltsekunde nach spätestens einer Minute zuverlässig möglich.
 *                Das Fehlen der korrekten Erkennung dieser Schaltsekunde in früheren Versionen verhinderte eine zuverlässige Zeitsynchronisation.
 *             -- Deutlich exaktere Einstellung der Zeit dank Driftkorrektur möglich.
 *            Die neue Klasse benötigt Timer1. (Ab V 3.4.9b3 kein Timer mehr erforderlich!)
 * V 3.4.8b2- Anzeige der vergangenen Stunden und Minuten seit der letzten erfolgreichen DCF-Synchronisation in eigenes Menü gelegt.
 * V 3.4.9. - Stundenbegrenzung (0 <= h <= 24) auch in setHourse eingefuehrt, siehe http://diskussion.christians-bastel-laden.de/viewtopic.php?f=17&t=2028
 * V 3.4.9a - Fehler bei Stundenbegrenzung in setMinutes korrigiert und Stundenbegrenzung aus setHours wieder entfernt, siehe http://diskussion.christians-bastel-laden.de/viewtopic.php?f=17&t=2028
 *          - Große Aufräumarbeiten, Code optimiert, Basisklasse TimeStamp eingeführt und Alarm, MyDCF77, sowie MyRTC davon abgeleitet, um Speicher einzusparen.
 * V 3.4.9b - Zusätzliche Features hinzugefügt:
 *            * Für besondere Ereignisse (einzutragen in neue Datei Ereignisse.h) kann ein Symbol hinterlegt werden, das statt dem Datum in der Datumsanzeige angezeigt wird.
 *            * Countdownfunktion eingeführt (nutzt ebenfalls Ereignisse.h), die einen Countdown zu festgelegten Ereignissen und nach Ablauf des Countdowns das definierte Symbol blinkend anzeigt.
 *            Das Aussehen der großen Null in Zahlen.h wurde geändert (die Null hat keinen diagonalen Strich mehr) und passt damit besseren zur kleinen Null (wenn vier Zahlen angezeigt werden).
 * V 3.4.9b2- Kleine Codeoptimierungen
 *          - Schalter für Zusatzfunktionen nach Configuration.h verschoben.
 *          - Zusätzliches Feature:
 *            * Zusätzliche Option, für jede Eckled nur die dazugehörige Kathode und nicht alle einzuschalten. Dies Verhindert das Glimmen ausgeschalteter Eckleds. (Standard: ausgeschaltet)
 *              Hinweis: Diese Option kann nur bei richtiger Verdrahtung verwendet werden. 
 * V 3.4.9b3- Kleine Codeoptimierungen
 *          - * Automatische Driftkorrektur beim DCF77-Empfang benötigt KEINEN Timer mehr! *
 *          - Menü in drei Kategorien unterteilt: MAIN, TIME und TEST, um Navigation zu erleichtern.
 *            Mit der Mode-Taste läuft man durch alle Menüseiten (wie bisher), man kann aber eine Kategorie (MAIN, TIME oder TEST) durch Drücken der Minuten- oder Stundentaste überspringen.
 *            Kurzüberblick welche Modi sich hinter den Kategorien verbergen:
 *            MAIN: LDR, Eck-LED-Richtung, Alarm ein-/ausschalten, DCF normal/invertiert, Sprache
 *            TIME: Uhrzeiteinstellung, Zeitzone, Timeout für automatischen Rücksprung, Nachtschaltung Beginn (Mo-Fr), Nachtschaltung Ende (Mo-Fr), Nachtschaltung Beginn (Sa,So), Nachtschaltung Ende (Sa,So)
 *            TEST: Testmodus (LEDs schalten durch), letzte erfolgreiche DCF-Synchronisation, DCF-Debug (vier Eck-LEDs schalten bei gutem Empfang sekundenweise durch)        
 *            Hinweis: Möchte man durch alle Menüseiten durchblättern, einfach wie gewohnt die Mode-Taste benutzen.
 *          - Zusätzliches Feature:
 *            * Nachtschaltung kennt jetzt zwei Zeitbereiche, ist also für Mo bis Fr und Sa, So separat einstellbar.
 * V 3.4.9b4- Größere Codeoptimierung bei der Nachtschaltung (knapp 800 Byte Einsparung)
 *          - Kleinere Codeoptimierungen in AnalogButton, Button und Qlockthree:
 *            * In allen drei Dateien/Klassen wurde ein Überlauf von millis() abgefangen. Dies ist aber unnötig, wenn nur Differenzen mit einem früheren Wert von millis() betrachtet werden.
 *              Ein Überlauf ändert nämlich die Differenz nicht. Da sich alle erforderlichen Vergleiche auf Differenzen haben umschreiben lassen, ist das Abfangen des Überlaufs entfernt worden.
 *              Beispiel:   Folgende IF-Abfrage
 *                  _lastPressTime + BUTTON_TRESHOLD < millis()  // <-- Bei einem Überlauf von millis() schlägt dieser Vergleich fehl
 *                          lässt sich wie folgt umschreiben:
 *                  millis() - _lastPressTime > BUTTON_TRESHOLD  // <-- Jetzt spielt der Überlauf von millis() keine Rolle mehr, weil nur Differenz (zwischen zwei unsigned Variablen) betrachtet wird.
 *          - Fehler in der Nachtschaltung behoben, der das Aufwecken der Uhr verhindert hat.
 */
#include <Wire.h> // Wire library fuer I2C
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <SPI.h>
#include <LedControl.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include <LPD8806.h>
#include "Configuration.h"
#include "LedDriver.h"
#include "LedDriverDefault.h"
#include "LedDriverUeberPixel.h"
#include "LedDriverPowerShiftRegister.h"
#include "LedDriverNeoPixel.h"
#include "LedDriverDotStar.h"
#include "LedDriverLPD8806.h"
#include "IRTranslator.h"
#include "IRTranslatorSparkfun.h"
#include "IRTranslatorMooncandles.h"
#include "IRTranslatorLunartec.h"
#include "MyRTC.h"
#include "MyDCF77.h"
#include "Button.h"
#include "AnalogButton.h"
#include "LDR.h"
#include "DCF77Helper.h"
#include "Renderer.h"
#include "Staben.h"
#include "Alarm.h"
#include "Settings.h"
#include "Zahlen.h"
#include "ZahlenKlein.h"
#ifdef EVENTDAY
    #include "Ereignisse.h"
#endif

#define FIRMWARE_VERSION "V 3.4.9b4 vom 17.04.2016"

/*
 * Den DEBUG-Schalter gibt es in allen Bibiliotheken. Wird er eingeschaltet, werden ueber den
 * 'Serial-Monitor' der Arduino-Umgebung Informationen ausgegeben. Die Geschwindigkeit im
 * Serial-Monitor muss mit der hier angegeben uebereinstimmen.
 * Default: ausgeschaltet
 */
//#define DEBUG
#include "Debug.h"
// Die Geschwindigkeit der seriellen Schnittstelle. Default: 57600. Die Geschwindigkeit brauchen wir immer,
// da auch ohne DEBUG Meldungen ausgegeben werden!
#define SERIAL_SPEED 57600

/*
 * Die persistenten (im EEPROM gespeicherten) Einstellungen.
 */
Settings settings;

/**
 * Die Ab- und Anschaltzeiten fuer das Display werden über ein separates Menü eingestellt. Die Abschaltung des
 * Displays verbessert den Empfang des DCF77-Empfaengers. Und hilft, falls die Uhr im
 * Schlafzimmer haengt. Nach einer Minute kann man das Display manuell wieder ein- / ausschalten.
 * Sind Start- und Endzeit gleich wird das Display nie abgeschaltet. 
 * Achtung! Wenn sich die Uhr nachmittags abschaltet ist sie in der falschen Tageshaelfte!
 */
 // ENTFALLEN, WURDE DURCH ERWEITERTEN MENÜPUNKT ERSETZT
 
//// um 3 Uhr Display abschalten (Minuten, Stunden, -, -, -, -)
//TimeStamp settings.getOffTime()(0, 3, 0, 0, 0, 0);
//// um 4:30 Uhr Display wieder anschalten (Minuten, Stunden, -, -, -, -)
//TimeStamp onTime(30, 4, 0, 0, 0, 0);

/**
 * Der Renderer, der die Woerter auf die Matrix ausgibt.
 */
Renderer renderer;

/**
 * Der LED-Treiber fuer 74HC595-Shift-Register. Verwendet
 * von der Drei-Lochraster-Platinen-Version und dem
 * NachBau_V02-Board sowie dem WortWecker nach Christian.
 *
 * Data: 10; Clock: 12; Latch: 11; OutputEnable: 3
 * LinesToWrite: 10
 */
#ifdef LED_DRIVER_DEFAULT
LedDriverDefault ledDriver(10, 12, 11, 3, 10);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED 4
#define PIN_DCF77_LED 8

#define PIN_SPEAKER 13
#endif

/**
 * Der LED-Treiber fuer 4 MAX7219-Treiber wie im Ueberpixel.
 * Data: 10; Clock: 11; Load: 12
 */
#ifdef LED_DRIVER_UEBERPIXEL
LedDriverUeberPixel ledDriver(5, 6, 7);

#define PIN_MODE 8
#define PIN_M_PLUS 3
#define PIN_H_PLUS 4

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED 10
#define PIN_DCF77_LED 11

#define PIN_SPEAKER 13
#endif

/**
 * Der LED-Treiber fuer Power-Shift-Register.
 * Data: 10; Clock: 11; Load: 12
 */
#ifdef LED_DRIVER_POWER_SHIFT_REGISTER
LedDriverPowerShiftRegister ledDriver(10, 12, 11, 3);

#define PIN_MODE 7
#define PIN_M_PLUS 5
#define PIN_H_PLUS 6

#define BUTTONS_PRESSING_AGAINST HIGH

#define PIN_IR_RECEIVER A1

#define PIN_LDR A3
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 9

#define PIN_DCF77_PON A0

#define PIN_SQW_LED -1
#define PIN_DCF77_LED -1

#define PIN_SPEAKER -1
#endif

/**
 * Der LED-Treiber fuer NeoPixel-Stripes an einem BBRTCAD.
 * Data: 6
 */
#ifdef LED_DRIVER_NEOPIXEL
LedDriverNeoPixel ledDriver(6);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED true

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1
#endif

/**
 * Der LED-Treiber fuer DotStars-Stripes an einem BBRTCAD.
 * Data: 6
 * Clock: 7
 */
#ifdef LED_DRIVER_DOTSTAR
LedDriverDotStar ledDriver(6, 7);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED false

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1
#endif

/**
 * Der LED-Treiber fuer LPD8806-Stripes an einem BBRTCAD.
 * Data: 6
 * Clock: 7
 */
#ifdef LED_DRIVER_LPD8806
LedDriverLPD8806 ledDriver(6, 7);

#define PIN_MODE 11
#define PIN_M_PLUS 13
#define PIN_H_PLUS 12

#define BUTTONS_PRESSING_AGAINST LOW

#define PIN_IR_RECEIVER A1

#define PIN_LDR A0
#define IS_INVERTED false

#define PIN_SQW_SIGNAL 2
#define PIN_DCF77_SIGNAL 3

#define PIN_DCF77_PON 4

#define PIN_SQW_LED 9
#define PIN_DCF77_LED 10

#define PIN_SPEAKER -1
#endif

/**
 * Der IR-Remote-Control-Receiver.
 */
#ifndef REMOTE_NO_REMOTE
IRrecv irrecv(PIN_IR_RECEIVER);
decode_results irDecodeResults;
#endif
#ifdef REMOTE_SPARKFUN
IRTranslatorSparkfun irTranslator;
#endif
#ifdef REMOTE_MOONCANDLES
IRTranslatorMooncandles irTranslator;
#endif
#ifdef REMOTE_LUNARTEC
IRTranslatorLunartec irTranslator;
#endif

/**
 * Die Real-Time-Clock mit der Status-LED fuer das SQW-Signal.
 */
MyRTC rtc(0x68, PIN_SQW_LED);
volatile byte helperSeconds;

/**
 * Der Funkempfaenger (DCF77-Signal der PTB Braunschweig).
 */
MyDCF77 dcf77(PIN_DCF77_SIGNAL, PIN_DCF77_LED);
DCF77Helper dcf77Helper;

/**
 * Variablen fuer den Alarm.
 */
Alarm alarm(PIN_SPEAKER);

/**
 * Der Helligkeitssensor
 */
LDR ldr(PIN_LDR, IS_INVERTED);
unsigned long lastBrightnessCheck;

/**
 * Die Helligkeit zum Anzeigen mit den Balken.
 */
byte brightnessToDisplay;

/**
 * Die Tasten.
 */
Button minutesPlusButton(PIN_M_PLUS, BUTTONS_PRESSING_AGAINST);
Button hoursPlusButton(PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button extModeDoubleButton(PIN_M_PLUS, PIN_H_PLUS, BUTTONS_PRESSING_AGAINST);
Button modeChangeButton(PIN_MODE, BUTTONS_PRESSING_AGAINST);

/**
 * Die Standard-Modi.
 */
#define STD_MODE_NORMAL     0
#define STD_MODE_ALARM      1
#define STD_MODE_SECONDS    2
#define STD_MODE_COUNTDOWN  3
#define STD_MODE_DATE       4
#define STD_MODE_BRIGHTNESS 5
#define STD_MODE_BLANK      6
#define STD_MODE_COUNT      7
// nicht manuell zu erreichender Modus...
#define STD_MODE_NIGHT      8



/**
 * Die erweiterten Modi.
 */
#define EXT_MODE_START           10

#define EXT_MODE_MAIN_SETTINGS_START  10
#define EXT_MODE_LDR_MODE        11
#define EXT_MODE_CORNERS         12
#define EXT_MODE_ENABLE_ALARM    13
#define EXT_MODE_DCF_IS_INVERTED 14
#define EXT_MODE_LANGUAGE        15

#define EXT_MODE_TIME_SETTINGS_START  16
#define EXT_MODE_TIMESET         17
#define EXT_MODE_TIME_SHIFT      18
#define EXT_MODE_JUMP_TIMEOUT    19
#define EXT_MODE_OFFTIME_MOFR         20
#define EXT_MODE_ONTIME_MOFR          21
#define EXT_MODE_OFFTIME_SASO         22
#define EXT_MODE_ONTIME_SASO          23

#define EXT_MODE_TEST_DEBUG_START     24
#define EXT_MODE_TEST            25
#define EXT_MODE_DCF_SYNC        26
#define EXT_MODE_DCF_DEBUG       27
#define EXT_MODE_COUNT           28

// Startmode...
byte mode = STD_MODE_NORMAL;
// Merker fuer den Modus vor der Abschaltung...
byte lastMode = mode;

// Ueber die Wire-Library festgelegt:
// Arduino analog input 4 = I2C SDA
// Arduino analog input 5 = I2C SCL

// Die Matrix, eine Art Bildschirmspeicher
word matrix[16];

// Hilfsvariable, da I2C und Interrupts nicht zusammenspielen
volatile boolean needsUpdateFromRtc = true;

// Fuer den Bildschirm-Test
byte x, y;

// Fuer fps-Anzeige
word frames = 0;
unsigned long lastFpsCheck = 0;

// Eigene Variablendeklaration
#ifdef EVENTDAY
    // Fuer die Anzeige eines Symbols bei einem Ereignis
    char eventdaySymbol;
#endif EVENTDAY

#ifdef COUNTDOWN
    // Fuer den Ereignis-Countdown
    int countdown;
#endif

#ifdef AUTO_JUMP_TO_TIME
    // Fuer automatischen Rücksprung zur Standardanzeige
    byte jumpToTime;
#endif

/**
 * Automatisches Zurückschalten von einer definierten Anzeige auf die Zeitanzeige nach einer
 * festgelegten Zeitspanne jumpToTime. Ist dieser Wert == 0, so findet kein
 * automatischer Rücksprung statt.
 * Achtung!: Als Ursprungsmodus (von dem Zurückgesprungen werden soll) nur Standardmodi verwenden.
 */
#ifdef AUTO_JUMP_TO_TIME
    static void updateJumpToTime() {
        if (settings.getJumpToTime()) {
            switch (mode) {
                case STD_MODE_SECONDS:
                case STD_MODE_DATE:
                case STD_MODE_BRIGHTNESS:
                    jumpToTime--;
                    if (!jumpToTime) {
                        mode = STD_MODE_NORMAL;
                        lastMode = mode;
                    }
                    break;
            }
        }
    }
#endif

/* 
 *  Es wird geprüft, ob aktuell ein Countdown geplant ist
 *  und dieser berechnet.
 */
#ifdef COUNTDOWN
    void CheckCountdown() {
        countdown = -1;
        eventdaySymbol = -1;
        for (byte i = 0; i < sizeof(eventdayObject)/sizeof(EventdayObject); i++) {
            TimeStamp TSTemp( pgm_read_byte_near(&eventdayObject[i].countdownStartMinute),
                              pgm_read_byte_near(&eventdayObject[i].countdownStartHour),
                              pgm_read_byte_near(&eventdayObject[i].countdownStartDay),
                              0,
                              pgm_read_byte_near(&eventdayObject[i].countdownStartMonth),
                              rtc.getYear());
            long diff = rtc.getMinutesOfCentury() - TSTemp.getMinutesOfCentury();
            if ( (diff >= 0) && (diff <= pgm_read_byte_near(&eventdayObject[i].countdownMinutes)) ) {
                countdown = ( pgm_read_byte_near(&eventdayObject[i].countdownMinutes) - diff ) * 60 - rtc.getSeconds();
                // Countdown auf 1 Stunde begrenzen (mehr kann nicht angezeigt werden)
                countdown %= 3600;
                eventdaySymbol = i;
                break;
            }
        }
    }
    
    static void updateCountDown() {
        if (mode == STD_MODE_COUNTDOWN) {
            countdown--;
            
        }
    }
#endif

/**
 * Aenderung der Anzeige als Funktion fuer den Interrupt, der ueber das SQW-Signal
 * der Real-Time-Clock gesetzt wird. Da die Wire-Bibliothek benutzt wird, kann man
 * den Interrupt nicht direkt benutzen, sondern muss eine Hilfsvariable setzen, die
 * dann in loop() ausgewertet wird.
 */
void updateFromRtc() {
    #ifdef AUTO_JUMP_TO_TIME
        // Automatischer Rücksprung nach jumpToTime Sekunden auf Zeitanzeige
        updateJumpToTime();
    #endif
    #ifdef COUNTDOWN
        // Countdown aktualisieren
        updateCountDown();
    #endif
    needsUpdateFromRtc = true;
    // die Zeit verursacht ein kurzes Flackern. Wir muessen
    // sie aber nicht immer lesen, im Modus 'normal' alle 60 Sekunden,
    // im Modus 'seconds' alle Sekunde, sonst garnicht.
    helperSeconds++;
    if (helperSeconds >= 60) {
        helperSeconds = 0;
    }
}

#ifdef __arm__
// unistd.h inkludieren definiert auch alarm, was Problem mit Alarm Klasse macht.*/
extern "C" char* sbrk(int incr);
#endif

/**
 * Den freien Specher abschaetzen.
 * Kopiert von: http://playground.arduino.cc/Code/AvailableMemory
 */
int freeRam() {
#ifdef __arm__
    char top;
	return &top - reinterpret_cast<char*>(sbrk(0));
#else
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#endif
}


/**
 * Initialisierung. setup() wird einmal zu Beginn aufgerufen, wenn der
 * Arduino Strom bekommt.
 */
void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.println(F("Qlockthree is initializing..."));
    DEBUG_PRINTLN(F("... and starting in debug-mode..."));
    Serial.flush();

    pinMode(PIN_DCF77_PON, OUTPUT);
    enableDcf(false);

    // LED-Treiber initialisieren
    ledDriver.init();
    // Inhalt des Led-Treibers loeschen...
    ledDriver.clearData();
    // und Inhalt des Bildspeichers loeschen
    renderer.clearScreenBuffer(matrix);
    // wir brauchen nur 10 Zeilen...
    ledDriver.setLinesToWrite(10);

    // starte Wire-Library als I2C-Bus Master
    Wire.begin();

    // RTC-Interrupt-Pin konfigurieren
    pinMode(PIN_SQW_SIGNAL, INPUT);
    digitalWrite(PIN_SQW_SIGNAL, HIGH);

    // DCF77-LED drei Mal als 'Hello' blinken lassen
    // und Speaker piepsen kassen, falls ENABLE_ALARM eingeschaltet ist.
    for (byte i = 0; i < 3; i++) {
        dcf77.statusLed(true);
        if (settings.getEnableAlarm()) {
            alarm.buzz(true);
        }
        delay(100);
        dcf77.statusLed(false);
        if (settings.getEnableAlarm()) {
            alarm.buzz(false);
        }
        delay(100);
    }

    Serial.print(F("Compiled: "));
    Serial.print(F(__TIME__));
    Serial.print(F(" / "));
    Serial.println(F(__DATE__));

    /*
    // Uhrzeit nach Compile-Zeit stellen...
    rtc.set(__DATE__, __TIME__);
    rtc.writeTime();
     */

    // RTC starten...
    rtc.readTime();
    if ((rtc.getSeconds() >= 60) || (rtc.getMinutes() >= 60) || (rtc.getHours() >= 24) || (rtc.getYear() < 15)) {
        // Echtzeituhr enthaelt Schrott, neu mit erkennbaren Zahlen beschreiben...
        DEBUG_PRINT(F("Resetting RTC..."));
        rtc.set(11, 11, 1, 1, 1, 15);
        rtc.setSeconds(11);
    }

#ifdef DS1307
    Serial.println(F("Uhrentyp ist DS1307."));
    rtc.enableSQWOnDS1307();
#elif defined DS3231
    Serial.println(F("Uhrentyp ist DS3231."));
    rtc.enableSQWOnDS3231();
#else
    Definition_des_Uhrtyps_fehlt!
    In der Configuration.h muss der Uhrentyp angegeben werden!
#endif

    rtc.writeTime();
    helperSeconds = rtc.getSeconds();
    Serial.print(F("RTC-Time: "));
    Serial.print(rtc.getHours());
    Serial.print(F(":"));
    Serial.print(rtc.getMinutes());
    Serial.print(F(":"));
    Serial.print(rtc.getSeconds());
    Serial.print(F(" RTC-Date: "));
    Serial.print(rtc.getDate());
    Serial.print(F("."));
    Serial.print(rtc.getMonth());
    Serial.print(F("."));
    Serial.println(rtc.getYear());

    // Den Interrupt konfigurieren,
    // nicht mehr CHANGE, das sind 2 pro Sekunde,
    // RISING ist einer pro Sekunde, das reicht.
    // Auf FALLING geandert, das signalisiert
    // den Sekundenwechsel, Danke an Peter.
    attachInterrupt(0, updateFromRtc, FALLING);

    // Werte vom LDR einlesen und vermuellen, da die ersten nichts taugen...
    for (int i = 0; i < 1000; i++) {
        analogRead(PIN_LDR);
    }

    // rtcSQWLed-LED drei Mal als 'Hello' blinken lassen
    // und Speaker piepsen kassen, falls ENABLE_ALARM eingeschaltet ist.
    for (byte i = 0; i < 3; i++) {
        rtc.statusLed(true);
        if (settings.getEnableAlarm()) {
            alarm.buzz(true);
        }
        delay(100);
        rtc.statusLed(false);
        if (settings.getEnableAlarm()) {
            alarm.buzz(false);
        }
        delay(100);
    }

    // ein paar Infos ausgeben
    Serial.println(F("... done and ready to rock!"));

    Serial.print(F("Version: "));
    Serial.println(F(FIRMWARE_VERSION));

    Serial.print(F("Driver: "));
    ledDriver.printSignature();

#ifndef REMOTE_NO_REMOTE
    Serial.print(F("Remote: "));
    irTranslator.printSignature();
    irrecv.enableIRIn();
#else
    Serial.print(F("Remote: disabled."));
#endif

    if (settings.getEnableAlarm()) {
        Serial.println(F("Alarm is enabled"));
    }

    if (settings.getDcfSignalIsInverted()) {
        Serial.println(F("DCF77-Signal is inverted."));
    }

    Serial.print(F("Free ram: "));
    Serial.print(freeRam());
    Serial.println(F(" bytes."));

    Serial.flush();

    // DCF77-Empfaenger einschalten...
    enableDcf(true);
    // Display einschalten...
    ledDriver.wakeUp();
    ledDriver.setBrightness(settings.getBrightness());
}

/*
 * Schreibroutinen für Buchstaben und Zahlen
 */
void write1xyStab(char char1, byte posx, byte posy) {
    for (byte i = 0; i < 5; i++) {
        matrix[posy + i] |= pgm_read_byte_near(&(staben[char1 - 'A'][i])) << posx;
    }
}

void write2yStaben(char char1, char char2, byte posy) {
    write1xyStab(char1, 11, posy);
    write1xyStab(char2,  5, posy);
}

void write2Staben(char char1, char char2) {
    write2yStaben(char1, char2, 2);
}

void write4Staben(char char1, char char2, char char3, char char4) {
    write2yStaben(char1, char2, 0);
    write2yStaben(char3, char4, 5);
}

void write1xyDigit(byte number, byte posx, byte posy) {
    for (byte i = 0; i < 7; i++) {
        matrix[posy + i] |= pgm_read_byte_near(&(ziffern[number][i])) << posx;
    }
}

void write2yDigits(byte number, byte posy) {
    write1xyDigit(number / 10, 11, posy);
    write1xyDigit(number % 10,  5, posy);
}

void write4SmallDigits(byte firstNumber, byte secondNumber) {
    for (byte i = 0; i < 5; i++) {            
        matrix[0 + i] |= pgm_read_byte_near(&(ziffernKlein[firstNumber / 10][i])) << 12;
        matrix[0 + i] |= pgm_read_byte_near(&(ziffernKlein[firstNumber % 10][i])) << 7;
        matrix[5 + i] |= pgm_read_byte_near(&(ziffernKlein[secondNumber / 10][i])) << 12;
        matrix[5 + i] |= pgm_read_byte_near(&(ziffernKlein[secondNumber % 10][i])) << 7;
    }
}

#ifdef EVENTDAY
    void writeEventSymbol() {
        for (byte i = 0; i < 10; i++) { 
            matrix[i] |= pgm_read_word_near(&(eventdayObject[eventdaySymbol].symbol[i])) << 5;
        }
    }
#endif

/**
 * loop() wird endlos auf alle Ewigkeit vom Microcontroller durchlaufen
 */
void loop() {
    //
    // Variablendeklaration (temporär)
    //
    unsigned int ui_min;
    byte b_hour, b_min;
    char c_TimeShift;

    //
    // FPS
    //
#ifdef DEBUG
#ifdef FPS_SHOW_DEBUG
    frames++;
    if (millis() - lastFpsCheck > 1000) {
        DEBUG_PRINT("FPS: ");
        DEBUG_PRINTLN(frames);
        lastFpsCheck = millis();
        frames = 0;
    }
#endif
#endif
    
    //
    // Dimmung.
    //
    if (settings.getUseLdr()) {
        if (millis() < lastBrightnessCheck) {
            // wir hatten einen Ueberlauf...
            lastBrightnessCheck = millis();
        }
        if (lastBrightnessCheck + LDR_CHECK_RATE < millis()) { // langsam nachsehen...
            byte lv = ldr.value();
            if (ledDriver.getBrightness() > lv) {
                ledDriver.setBrightness(ledDriver.getBrightness() - 1);
            } else if (ledDriver.getBrightness() < lv) {
                ledDriver.setBrightness(ledDriver.getBrightness() + 1);
            }
            lastBrightnessCheck = millis();
        }
    }

    //
    // needsUpdateFromRtc wird via Interrupt gesetzt ueber fallende
    // Flanke des SQW-Signals von der RTC.
    // Oder falls eine Tasten-Aktion eine sofortige Aktualisierung des Displays braucht.
    //
    if (needsUpdateFromRtc) {
        needsUpdateFromRtc = false;

        //
        // Zeit einlesen...
        //
        switch (mode) {
            case STD_MODE_NORMAL:
            case STD_MODE_COUNTDOWN:
            case STD_MODE_DATE:
            case EXT_MODE_TIMESET:
            case STD_MODE_ALARM:
                if (alarm.isActive()) {
                    rtc.readTime();
                }
                if (helperSeconds == 0) {
                    rtc.readTime();
                    helperSeconds = rtc.getSeconds();
                }
                /*
                 * Display zeitgesteuert abschalten
                 * Das Verbessert den DCF77-Empfang bzw. ermoeglicht ein dunkles Schlafzimmer.
                 */
                // Als Wecker und wenn in Einstellungsmenü Display nicht abschalten...
                if ( (!settings.getEnableAlarm()) && (mode < EXT_MODE_START) && (!checkNight(1)) ) {
                    goToNight();
                } 
                break;
            case STD_MODE_NIGHT:
                // Wenn Nacht vorbei, wird Display wieder eingeschaltet (eine Sekunde verzögert)
                if (checkNight(0)) {
                    leaveFromNight();
                }
            case STD_MODE_SECONDS:
            case STD_MODE_BLANK:
                rtc.readTime();
                helperSeconds = rtc.getSeconds();
                break;
                // andere Modi egal...
        }

        //
        // Vorbereitende Aufgaben für Countdown erledigen
        //
        #ifdef COUNTDOWN       
            switch (mode) {
                case STD_MODE_NORMAL:
                    #ifndef AUTO_JUMP_TO_COUNTDOWN
                        break;
                    #endif
                case STD_MODE_COUNTDOWN:
                    #ifdef COUNTDOWN
                        if (helperSeconds == 0) {
                            CheckCountdown();
                            if (countdown >= 0) {
                                mode = STD_MODE_COUNTDOWN;
                            }
                        }   
                    #endif
                break;
                // andere Modi egal...
            }
        #endif  

        //
        // Bildschirmpuffer beschreiben...
        //
        renderer.clearScreenBuffer(matrix);
        switch (mode) {
            case STD_MODE_NORMAL:
            case EXT_MODE_TIMESET:
                renderer.setMinutes(rtc.getHours() + settings.getTimeShift(), rtc.getMinutes(), settings.getLanguage(), matrix);
                renderer.setCorners(rtc.getMinutes(), settings.getRenderCornersCw(), matrix);
                break;
            case EXT_MODE_TIME_SHIFT:
                c_TimeShift = settings.getTimeShift();
                if (c_TimeShift < 0) {
                    for (byte x = 0; x < 3; x++) {
                        ledDriver.setPixelInScreenBuffer(x, 1, matrix);
                    }
                } else if (c_TimeShift > 0) {
                    for (byte x = 0; x < 3; x++) {
                        ledDriver.setPixelInScreenBuffer(x, 1, matrix);
                    }
                    for (byte y = 0; y < 3; y++) {
                        ledDriver.setPixelInScreenBuffer(1, y, matrix);
                    }
                }
                c_TimeShift = abs(c_TimeShift);
                write1xyDigit(c_TimeShift % 10, 5, 3);
                if (c_TimeShift > 9) {
                    write1xyDigit(1, 10, 3);
                }
                break;
            case STD_MODE_ALARM:
                if (alarm.getShowAlarmTimeTimer() == 0) {
                    renderer.setMinutes(rtc.getHours() + settings.getTimeShift(), rtc.getMinutes(), settings.getLanguage(), matrix);
                    renderer.setCorners(rtc.getMinutes(), settings.getRenderCornersCw(), matrix);
                    renderer.activateAlarmLed(matrix);
                } else {
                    renderer.setMinutes(alarm.getHours() + settings.getTimeShift(), alarm.getMinutes(), settings.getLanguage(), matrix);
                    renderer.setCorners(alarm.getMinutes(), settings.getRenderCornersCw(), matrix);
                    renderer.cleanWordsForAlarmSettingMode(settings.getLanguage(), matrix); // ES IST weg
                    if (alarm.getShowAlarmTimeTimer() % 2 == 0) {
                        renderer.activateAlarmLed(matrix);
                    }
                    alarm.decShowAlarmTimeTimer();
                }
                break;
            case STD_MODE_DATE:
                #ifdef EVENTDAY
                    /**
                     * Es wird geprüft, ob der heutige Tag ein Ereignis ist und das entsprechend
                     * definierte Symbol dazu auf der Datumsanzeige ausgegeben.
                     * Ereignisse werden in der Ereignisse.h definiert.
                     */  
                    eventdaySymbol = -1;
                    for (byte i = 0; i < sizeof(eventdayObject)/sizeof(EventdayObject); i++) {
                        if ( (pgm_read_byte_near(&eventdayObject[i].day) == rtc.getDate()) && (pgm_read_byte_near(&eventdayObject[i].month) == rtc.getMonth()) ) {
                            eventdaySymbol = i;
                            break;
                        }
                    }      
                    if (eventdaySymbol != -1) {
                        // Anzeige des Geburtstagssymbols
                        writeEventSymbol();
                    } else
                #endif
                {
                    // Anzeige des Datums
                    write4SmallDigits(rtc.getDate(), rtc.getMonth());
                    ledDriver.setPixelInScreenBuffer(10, 4, matrix);
                    ledDriver.setPixelInScreenBuffer(10, 9, matrix);
                }
                break;
            #ifdef COUNTDOWN
                case STD_MODE_COUNTDOWN:
                    /**
                     * Dieser Modus zeigt den Countdown zu einem Ereignis an.
                     * Ereignisse werden in der Ereignisse.h definiert.
                     */ 
                    if (countdown >= 60) {
                        // Anzeige des Countdowns (Minuten und Sekunden)
                        write4SmallDigits(countdown / 60, countdown % 60);
                        ledDriver.setPixelInScreenBuffer(10, 1, matrix);
                        ledDriver.setPixelInScreenBuffer(10, 3, matrix);
                    } else if (countdown >= 0) {
                        // Anzeige des Countdowns (nur Sekunden)
                        write2yDigits(countdown, 1);
                    } else if (countdown > -COUNTDOWN_BLINK_DURATION) {
                        // Anzeige des Symbols blinkend im Sekundentakt
                        if (countdown % 2) {
                            writeEventSymbol();
                        }
                    } else {
                        // Rücksprung auf Uhrzeit
                        mode = STD_MODE_NORMAL;
                        lastMode = mode;
                    }
                    break;
            #endif
            case EXT_MODE_MAIN_SETTINGS_START:
                write4Staben('M', 'A', 'I', 'N');
                break;
            case EXT_MODE_TIME_SETTINGS_START:
                write4Staben('T', 'I', 'M', 'E');
                break;
            case EXT_MODE_TEST_DEBUG_START:
                write4Staben('T', 'E', 'S', 'T');
                break;
            case EXT_MODE_JUMP_TIMEOUT:
                write2yDigits(settings.getJumpToTime(), 3);
                break;
            case EXT_MODE_OFFTIME_MOFR:
            case EXT_MODE_OFFTIME_SASO:
                ledDriver.setPixelInScreenBuffer(10, 7, matrix);
            case EXT_MODE_ONTIME_MOFR:
            case EXT_MODE_ONTIME_SASO:
                b_hour = getNightTimeStamp(mode)->getHours();
                b_min = getNightTimeStamp(mode)->getMinutes();
                write4SmallDigits(b_hour, b_min);
                ledDriver.setPixelInScreenBuffer(10, 1, matrix);
                ledDriver.setPixelInScreenBuffer(10, 3, matrix);
                break; 
            case STD_MODE_SECONDS:
                write2yDigits(rtc.getSeconds(), 1);
                break;
            case EXT_MODE_LDR_MODE:
                if (settings.getUseLdr()) {
                    write1xyStab('A', 8, 2);
                } else {
                    write1xyStab('M', 8, 2);
                }
                break;
            case STD_MODE_BLANK:
            case STD_MODE_NIGHT:
                break;
            case STD_MODE_BRIGHTNESS:
                brightnessToDisplay = map(settings.getBrightness(), 1, 100, 0, 9);
                for (byte xb = 0; xb < brightnessToDisplay; xb++) {
                    for (byte yb = 0; yb <= xb; yb++) {
                        matrix[9 - yb] |= 1 << (14 - xb);
                    }
                }
                break;
            case EXT_MODE_CORNERS:
                if (settings.getRenderCornersCw()) {
                    write2Staben('C', 'W');
                } else {
                    write1xyStab('C', 8, 0);
                    write2yStaben('C', 'W', 5);
                }
                break;
            case EXT_MODE_ENABLE_ALARM:
                if (settings.getEnableAlarm()) {
                    write4Staben('A', 'L', 'E', 'N');
                } else {
                    write4Staben('A', 'L', 'D', 'A');
                }
                break;
            case EXT_MODE_DCF_IS_INVERTED:
                if (settings.getDcfSignalIsInverted()) {
                    write4Staben('R', 'S', 'I', 'N');
                } else {
                    write4Staben('R', 'S', 'N', 'O');
                }
                break;
            case EXT_MODE_LANGUAGE:
                for (byte i = 0; i < 5; i++) {
                    switch (settings.getLanguage()) {
                        case LANGUAGE_DE_DE:
                            write2Staben('D', 'E');
                            break;
                        case LANGUAGE_DE_SW:
                            write4Staben('D', 'E', 'S', 'W');
                            break;
                        case LANGUAGE_DE_BA:
                            write4Staben('D', 'E', 'B', 'A');
                            break;
                        case LANGUAGE_DE_SA:
                            write4Staben('D', 'E', 'S', 'A');
                            break;
                        case LANGUAGE_CH:
                            write2Staben('C', 'H');
                            break;
                        case LANGUAGE_EN:
                            write2Staben('E', 'N');
                            break;
                        case LANGUAGE_FR:
                            write2Staben('F', 'R');
                            break;
                        case LANGUAGE_IT:
                            write2Staben('I', 'T');
                            break;
                        case LANGUAGE_NL:
                            write2Staben('N', 'L');
                            break;
                        case LANGUAGE_ES:
                            write2Staben('E', 'S');
                            break;
                    }
                }
                break;
            case EXT_MODE_TEST:
                renderer.setCorners(helperSeconds % 5, settings.getRenderCornersCw(), matrix);
                if (settings.getEnableAlarm()) {
                    renderer.activateAlarmLed(matrix);
                }
                for (byte i = 0; i < 11; i++) {
                    ledDriver.setPixelInScreenBuffer(x, i, matrix);
                }
                x++;
                if (x > 10) {
                    x = 0;
                }
                break;
            #ifdef DCF77_SHOW_TIME_SINCE_LAST_SYNC
                case EXT_MODE_DCF_SYNC:
                    // Anzeige des letzten erfolgreichen DCF-Syncs (samplesOK) in Stunden:Minuten
                    ui_min = dcf77.getDcf77LastSuccessSyncMinutes();
                    write4SmallDigits(ui_min / 60, ui_min % 60);
                    ledDriver.setPixelInScreenBuffer(10, 1, matrix);
                    ledDriver.setPixelInScreenBuffer(10, 3, matrix);
                break;
            #endif
            case EXT_MODE_DCF_DEBUG:
                needsUpdateFromRtc = true;
                renderer.setCorners(dcf77.getDcf77ErrorCorner(settings.getDcfSignalIsInverted()), settings.getRenderCornersCw(), matrix);
                break;
        }

        // Update mit onChange = true, weil sich hier (aufgrund needsUpdateFromRtc) immer was geaendert hat.
        // Entweder weil wir eine Sekunde weiter sind, oder weil eine Taste gedrueckt wurde.
        ledDriver.writeScreenBufferToMatrix(matrix, true);
    }

    /*
     *
     * Tasten abfragen (Code mit 3.3.0 ausgelagert, wegen der Fernbedienung)
     *
     */
    // M+ und H+ im STD_MODE_NORMAL gedrueckt?
    if ((mode == STD_MODE_NORMAL) && extModeDoubleButton.pressed()) {
        doubleStdModeNormalPressed();
    }
     
    // M+ und H+ im STD_MODE_BLANK gedrueckt?
    if ((mode == STD_MODE_BLANK) && extModeDoubleButton.pressed()) {
        doubleExtModePressed();
    }

    // Taste Minuten++ (brighness++) gedrueckt?
    if (minutesPlusButton.pressed()) {
        minutePlusPressed();
    }

    // Taste Stunden++ (brightness--) gedrueckt?
    if (hoursPlusButton.pressed()) {
        hourPlusPressed();
    }

    // Taste Moduswechsel gedrueckt?
    if (modeChangeButton.pressed()) {
        modePressed();
    }

    /*
     *
     * Tasten der Fernbedienung abfragen...
     *
     */
#ifndef REMOTE_NO_REMOTE
    if (irrecv.decode(&irDecodeResults)) {
        DEBUG_PRINT(F("Decoded successfully as "));
        DEBUG_PRINTLN2(irDecodeResults.value, HEX);
        needsUpdateFromRtc = true;
        switch (irTranslator.buttonForCode(irDecodeResults.value)) {
            case REMOTE_BUTTON_MODE:
                modePressed();
                break;
            case REMOTE_BUTTON_MINUTE_PLUS:
                minutePlusPressed();
                break;
            case REMOTE_BUTTON_HOUR_PLUS:
                hourPlusPressed();
                break;
            case REMOTE_BUTTON_BRIGHTER:
                setDisplayBrighter();
                break;
            case REMOTE_BUTTON_DARKER:
                setDisplayDarker();
                break;
            case REMOTE_BUTTON_EXTMODE:
                doubleExtModePressed();
                break;
            case REMOTE_BUTTON_TOGGLEBLANK:
                setDisplayToToggle();
                break;
            case REMOTE_BUTTON_BLANK:
                setDisplayToBlank();
                break;
            case REMOTE_BUTTON_RESUME:
                setDisplayToResume();
                break;
            case REMOTE_BUTTON_SETCOLOR:
                ledDriver.setColor(irTranslator.getRed(), irTranslator.getGreen(), irTranslator.getBlue());
                break;
        }
        irrecv.resume();
    }
#endif

    /*
     *
     * Alarm?
     *
     */
    if ((mode == STD_MODE_ALARM) && (alarm.getShowAlarmTimeTimer() == 0) && !alarm.isActive()) {
        if (alarm.getMinutesOf12HoursDay(0) == rtc.getMinutesOf12HoursDay(0)) {
            alarm.activate();
        }
    }
    if (alarm.isActive()) {
        // Nach 10 Minuten automatisch abschalten, falls der Wecker alleine rumsteht und die Nachbarn nervt...
        if (alarm.getMinutesOf12HoursDay(MAX_BUZZ_TIME_IN_MINUTES) == rtc.getMinutesOf12HoursDay(0)) {
            alarm.deactivate();
            alarm.buzz(false);
            mode = STD_MODE_NORMAL;
        }
        // Krach machen...
        if (rtc.getSeconds() % 2 == 0) {
            alarm.buzz(true);
        } else {
            alarm.buzz(false);
        }
    }

    /*
     *
     * Die Matrix auf die LEDs multiplexen, hier 'Refresh-Zyklen'.
     *
     */
    if ((mode != STD_MODE_BLANK) && (mode != STD_MODE_NIGHT)) {
        ledDriver.writeScreenBufferToMatrix(matrix, false);
    }

    /*
     *
     * Status-LEDs ausgeben
     *
     */
#ifdef ENABLE_DCF_LED
    dcf77.statusLed(dcf77.signal(settings.getDcfSignalIsInverted()));
#endif
#ifdef ENABLE_SQW_LED
    rtc.statusLed(digitalRead(PIN_SQW_SIGNAL) == HIGH);
#endif

    /*
     *
     * DCF77-Empfaenger anticken...
     *
     */
    if (dcf77.poll(settings.getDcfSignalIsInverted()))
      manageNewDCF77Data();
}

/**
 * Was soll ausgefuehrt werden, wenn die H+ und M+ -Taste zusammen gedrueckt wird?
 */
// Im Mode STD_MODE_NORMAL
void doubleStdModeNormalPressed() {
    needsUpdateFromRtc = true;
    DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_NORMAL..."));
    DEBUG_FLUSH();
    if (!checkNight(0))
    {
        goToNight();
    }
}

// Im Mode STD_MODE_BLANK
void doubleExtModePressed() {
    needsUpdateFromRtc = true;
    DEBUG_PRINTLN(F("Minutes plus AND hours plus pressed in STD_MODE_BLANK..."));
    DEBUG_FLUSH();
    while (minutesPlusButton.pressed());
    while (hoursPlusButton.pressed());
    mode = EXT_MODE_START;
    setDisplayToOn();
    DEBUG_PRINTLN(F("Entering EXT_MODEs"));
    DEBUG_FLUSH();
}

/**
 * Was soll ausgefuehrt werden, wenn die Mode-Taste gedrueckt wird?
 */
void modePressed() {
    needsUpdateFromRtc = true;
    if (alarm.isActive()) {
        alarm.deactivate();
        mode = STD_MODE_NORMAL;
    } else {
        switch (mode) {
             // Durch Drücken der MODE-Taste den Nachtmodus verlassen
            case STD_MODE_NIGHT: 
                leaveFromNight();
                break;
            default:
                mode++;
        }
              
        
    }
    // Brightness ueberspringen, wenn LDR verwendet wird.
    if (settings.getUseLdr() && (mode == STD_MODE_BRIGHTNESS)) {
        mode++;
    }
    // Alarm ueberspringen, wenn kein Alarm enabled ist.
    if (!settings.getEnableAlarm() && (mode == STD_MODE_ALARM)) {
        mode++;
    }
    if (mode == STD_MODE_COUNTDOWN) {
        #ifdef COUNTDOWN
            rtc.readTime();
            CheckCountdown();
            if (countdown < 0)
        #endif
                mode++;
    }
    // EXT_MODE_DCF_SYNC überspringen, wenn nicht definiert
    #ifndef DCF77_SHOW_TIME_SINCE_LAST_SYNC
        if (mode == EXT_MODE_DCF_SYNC) {
            mode++;
        }
    #endif
    if (mode == STD_MODE_COUNT) {
        mode = STD_MODE_NORMAL;
    }
    if (mode == EXT_MODE_COUNT) {
        mode = STD_MODE_NORMAL;
    }
    if (mode == STD_MODE_ALARM) {
        // wenn auf Alarm gewechselt wurde, fuer 10 Sekunden die
        // Weckzeit anzeigen.
        alarm.setShowAlarmTimeTimer(10);
    }

    #ifdef AUTO_JUMP_TO_TIME
        switch (mode) {
            case STD_MODE_SECONDS:
            case STD_MODE_DATE:
            case STD_MODE_BRIGHTNESS:
                // Timeout für den automatischen Rücksprung von STD_MODE_SECONDS,
                // STD_MODE_DATE und STD_MODE_BRIGHTNESS zurücksetzen
                jumpToTime = settings.getJumpToTime();
                break;
        }
    #endif        

    DEBUG_PRINT(F("Change mode pressed, mode is now "));
    DEBUG_PRINT(mode);
    DEBUG_PRINTLN(F("..."));
    DEBUG_FLUSH();

    // Displaytreiber ausschalten, wenn BLANK
    if (mode == STD_MODE_BLANK) {
        setDisplayToOff();
    }
    // und einschalten, wenn BLANK verlassen wurde
    if (lastMode == STD_MODE_BLANK) {
        setDisplayToOn();
    }

    // Merker, damit wir nach einer automatischen Abschaltung
    // zum richtigen Mode zurueckkommen.
    lastMode = mode;

    // Werte speichern (die Funktion speichert nur bei geaenderten Werten)...
    settings.saveToEEPROM();
}

/**
 * Was soll ausgefuehrt werden, wenn die H+-Taste gedrueckt wird?
 */
void hourPlusPressed() {
    needsUpdateFromRtc = true;

    DEBUG_PRINTLN(F("Hours plus pressed..."));
    DEBUG_FLUSH();

    switch (mode) {
        case EXT_MODE_MAIN_SETTINGS_START:
            mode = EXT_MODE_TIME_SETTINGS_START;
            break;
        case EXT_MODE_TIME_SETTINGS_START:
            mode = EXT_MODE_TEST_DEBUG_START;
            break;
        case EXT_MODE_TEST_DEBUG_START:
            mode = STD_MODE_NORMAL;
            break;
        case EXT_MODE_TIMESET:
            rtc.incHours();
            rtc.setSeconds(0);
            rtc.writeTime();
            rtc.readTime();
            helperSeconds = 0;
            DEBUG_PRINT(F("H is now "));
            DEBUG_PRINTLN(rtc.getHours());
            DEBUG_FLUSH();
            break;
        case EXT_MODE_TIME_SHIFT:
            if (settings.getTimeShift() > -13) {
                settings.setTimeShift(settings.getTimeShift() - 1);
            }
            break;
        case EXT_MODE_JUMP_TIMEOUT:
            if (settings.getJumpToTime() > 0) {
                settings.setJumpToTime(settings.getJumpToTime() - 1);
            }
            break;
        case EXT_MODE_OFFTIME_MOFR:
        case EXT_MODE_ONTIME_MOFR:
        case EXT_MODE_OFFTIME_SASO:
        case EXT_MODE_ONTIME_SASO:
            getNightTimeStamp(mode)->incHours();
            break;
        case STD_MODE_ALARM:
            alarm.incHours();
            alarm.setShowAlarmTimeTimer(10);
            DEBUG_PRINT(F("A is now "));
            DEBUG_PRINTLN(alarm.asString());
            DEBUG_FLUSH();
            break;
        case STD_MODE_BRIGHTNESS:
            #ifdef AUTO_JUMP_TO_TIME
                // RESET counter
                jumpToTime = settings.getJumpToTime();
            #endif
            setDisplayDarker();
            break;
        case EXT_MODE_LDR_MODE:
            settings.setUseLdr(!settings.getUseLdr());
            if (!settings.getUseLdr()) {
                ledDriver.setBrightness(50);
            }
            DEBUG_PRINT(F("LDR is now "));
            DEBUG_PRINTLN(settings.getUseLdr());
            DEBUG_FLUSH();
            break;
        case EXT_MODE_CORNERS:
            settings.setRenderCornersCw(!settings.getRenderCornersCw());
            break;
        case EXT_MODE_ENABLE_ALARM:
            settings.setEnableAlarm(!settings.getEnableAlarm());
            break;
        case EXT_MODE_DCF_IS_INVERTED:
            settings.setDcfSignalIsInverted(!settings.getDcfSignalIsInverted());
            break;
        case EXT_MODE_LANGUAGE:
            if (settings.getLanguage() == 0) {
                settings.setLanguage(LANGUAGE_COUNT);
            } else {
                settings.setLanguage(settings.getLanguage() - 1);
            }
            break;
    }
}

/**
 * Was soll ausgefuehrt werden, wenn die M+-Taste gedrueckt wird?
 */
void minutePlusPressed() {
    needsUpdateFromRtc = true;

    DEBUG_PRINTLN(F("Minutes plus pressed..."));
    DEBUG_FLUSH();

    switch (mode) {
        case EXT_MODE_MAIN_SETTINGS_START:
            mode = EXT_MODE_TIME_SETTINGS_START;
            break;
        case EXT_MODE_TIME_SETTINGS_START:
            mode = EXT_MODE_TEST_DEBUG_START;
            break;
        case EXT_MODE_TEST_DEBUG_START:
            mode = STD_MODE_NORMAL;
            break;
        case EXT_MODE_TIMESET:
            rtc.incMinutes();
            rtc.setSeconds(0);
            rtc.writeTime();
            rtc.readTime();
            helperSeconds = 0;
            DEBUG_PRINT(F("M is now "));
            DEBUG_PRINTLN(rtc.getMinutes());
            DEBUG_FLUSH();
            break;
        case EXT_MODE_TIME_SHIFT:
            if (settings.getTimeShift() < 13) {
                settings.setTimeShift(settings.getTimeShift() + 1);
            }
            break;
        case EXT_MODE_JUMP_TIMEOUT:
            if (settings.getJumpToTime() < 99) {
                settings.setJumpToTime(settings.getJumpToTime() + 1);
            }
            break;
        case EXT_MODE_OFFTIME_MOFR:
        case EXT_MODE_ONTIME_MOFR:
        case EXT_MODE_OFFTIME_SASO:
        case EXT_MODE_ONTIME_SASO:
            getNightTimeStamp(mode)->incMinutes();
            break;
        case STD_MODE_ALARM:
            alarm.incMinutes();
            alarm.setShowAlarmTimeTimer(10);
            DEBUG_PRINT(F("A is now "));
            DEBUG_PRINTLN(alarm.asString());
            DEBUG_FLUSH();
            break;
        case STD_MODE_BRIGHTNESS:
            #ifdef AUTO_JUMP_TO_TIME
                // RESET counter
                jumpToTime = settings.getJumpToTime();
            #endif
            setDisplayBrighter();
            break;
        case EXT_MODE_LDR_MODE:
            settings.setUseLdr(!settings.getUseLdr());
            if (!settings.getUseLdr()) {
                ledDriver.setBrightness(50);
            }
            DEBUG_PRINT(F("LDR is now "));
            DEBUG_PRINTLN(settings.getUseLdr());
            DEBUG_FLUSH();
            break;
        case EXT_MODE_CORNERS:
            settings.setRenderCornersCw(!settings.getRenderCornersCw());
            break;
        case EXT_MODE_ENABLE_ALARM:
            settings.setEnableAlarm(!settings.getEnableAlarm());
            break;
        case EXT_MODE_DCF_IS_INVERTED:
            settings.setDcfSignalIsInverted(!settings.getDcfSignalIsInverted());
            break;
        case EXT_MODE_LANGUAGE:
            settings.setLanguage(settings.getLanguage() + 1);
            if (settings.getLanguage() > LANGUAGE_COUNT) {
                settings.setLanguage(0);
            }
            break;
    }
}

/**
 * Den DCF77-Empfaenger ein-/ausschalten.
 */
void enableDcf(boolean enable) {
    if (enable) {
        DEBUG_PRINTLN(F("DCF77-Empfaenger aufgeweckt."));
        DEBUG_FLUSH();
        digitalWrite(PIN_DCF77_PON, LOW);
    } else {
        DEBUG_PRINTLN(F("DCF77-Empfaenger schlafen gelegt."));
        DEBUG_FLUSH();
        digitalWrite(PIN_DCF77_PON, HIGH);
    }
}

/**
 * Korrekte Daten (auf Basis der Pruefbits) vom DCF-Empfaenger
 * bekommen. Sicherheitshalber gegen Zeitabstaende der RTC pruefen.
 */
void manageNewDCF77Data() {
    DEBUG_PRINT(F("Captured: "));
    DEBUG_PRINTLN(dcf77.asString());
    DEBUG_FLUSH();

    rtc.readTime();
    dcf77Helper.addSample(&dcf77, &rtc);
    // Stimmen die Abstaende im Array?
    // Pruefung mit Datum!
    if (dcf77Helper.samplesOk()) {
        helperSeconds = 0;
        rtc.setSeconds(0);
        rtc.set(&dcf77);
        rtc.writeTime();
        DEBUG_PRINTLN(F("DCF77-Time written to RTC."));
        DEBUG_FLUSH();
        #ifdef DCF77_SHOW_TIME_SINCE_LAST_SYNC
            dcf77.setDcf77SuccessSync();
        #endif
        #ifdef AUTO_JUMP_BLANK
            // falls im manuellen Dunkel-Modus, Display wieder einschalten... (Hilft bei der Erkennung, ob der DCF-Empfang geklappt hat).
            if (mode == STD_MODE_BLANK) {
                mode = STD_MODE_NORMAL;
                setDisplayToOn();
            }
        #endif
    } else {
        DEBUG_PRINTLN(F("DCF77-Time trashed because wrong distances between timestamps."));
        DEBUG_FLUSH();
    }
}

/*
 * Pointer auf NightTime abhängig vom vorgegebenen (Anzeige-)Modus
 */
TimeStamp* getNightTimeStamp(byte _mode) {
    return settings.getNightTimeStamp(_mode - EXT_MODE_OFFTIME_MOFR);
}

/*
 * Pointer auf NightTime abhängig vom Wochentag
 * _on == 0: offTime
 * _on == 1: onTime
 */
TimeStamp* getOffOnTimeDayOfWeek(byte _dayOfWeek, byte _on) {
    switch (_dayOfWeek) {
        case 0:
        case 6:
        case 7:   return settings.getNightTimeStamp(2 + _on);
                  break;
        default:  return settings.getNightTimeStamp(0 + _on);
    }
}

/**
 * Ist aktuell Nacht?
 * Bei glatt == 1 wird nur 0 zurückgegeben, wenn die aktuelle Uhrzeit
 * exakt einer eingestellten Ausschaltzeit entspricht.
 * Bei glatt == 0 wird auch dann 0 zurückgegeben, wenn die aktuelle
 * Uhrzeit innerhalb eines passenden Zeitbereichs liegt.
 */
byte checkNight(byte glatt) {
    byte retVal = 1; // no night, 1: Display on, 0: Display off
 
    byte dayOfWeek = rtc.getDayOfWeek();
    unsigned int currentMinutesOfWeek = rtc.getMinutesOfWeek_07();
    unsigned int maxMinutesOfWeek = 0;
    for (byte i = 0; i < 4; i++) {
        byte dayOfWeek0_1 = dayOfWeek - 1 + i/2;
        TimeStamp* TT = getOffOnTimeDayOfWeek(dayOfWeek0_1, i % 2);
        TT->setDayOfWeek(dayOfWeek0_1);
        unsigned int nightMinutes = TT->getMinutesOfWeek_07();
        if (nightMinutes > maxMinutesOfWeek) {
            if ( ((!glatt) && (currentMinutesOfWeek >= nightMinutes)) ||
                  ((glatt) && (currentMinutesOfWeek == nightMinutes)) ) {
                maxMinutesOfWeek = nightMinutes;
                retVal = i % 2; // 0: offTime, 1: onTime
            }
        }
    }
    
    return retVal;
}

/**
 * In den Nachtmodus wechseln
 */
void goToNight() {
    lastMode = mode;
    mode = STD_MODE_NIGHT;
    setDisplayToOff();
}

/**
 * Den Nachtmodus beenden
 */
void leaveFromNight() {
    mode = lastMode;
    setDisplayToOn();
}

/**
 * Das Display toggeln (aus-/einschalten).
 */
void setDisplayToToggle() {
    if (mode != STD_MODE_BLANK) {
        setDisplayToBlank();
    } else {
        setDisplayToResume();
    }
}

/**
 * Das Display ausschalten (BLANK-Modus).
 */
void setDisplayToBlank() {
    if (mode != STD_MODE_BLANK) {
        lastMode = mode;
        mode = STD_MODE_BLANK;
        setDisplayToOff();
    }
}

/**
 * Das Display zurückschalten.
 */
void setDisplayToResume() {
    if (mode == STD_MODE_BLANK) {
        mode = lastMode;
        setDisplayToOn();
    }
}

/**
 * Das Display ausschalten.
 */
void setDisplayToOff() {
    DEBUG_PRINTLN(F("LED-Driver: ShutDown"));
    DEBUG_FLUSH();
    ledDriver.shutDown();
}

/**
 * Das Display einschalten.
 */
void setDisplayToOn() {
    DEBUG_PRINTLN(F("LED-Driver: WakeUp"));
    DEBUG_FLUSH();
    ledDriver.wakeUp();
}

/**
 * Das Display manuell heller machen.
 */
void setDisplayBrighter() {
    byte b = settings.getBrightness();
    if ((!settings.getUseLdr()) && ((byte)b < 100)) {
        b += 10;
        if (b > 100) {
            b = 100;
        }
        setDisplayBrightness(b);
    }
}

/**
 * Das Display dunkler machen.
 */
void setDisplayDarker() {
    int i = settings.getBrightness();
    if ((!settings.getUseLdr()) && ((byte)i > 1)) {
        i -= 10;
        if (i < 1) {
            i = 1;
        }
        setDisplayBrightness(i);
    }
}

/**
 * Die Helligkeit des Display einstellen.
 */
void setDisplayBrightness(byte brightness) {
    settings.setBrightness(brightness);
    settings.saveToEEPROM();
    ledDriver.setBrightness(brightness);
}
