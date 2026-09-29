# ShiftRegisterKeypad

Eigene Arduino-Bibliothek für ein 4×4-Keypad mit Zeilen an Q0–Q3 eines 74HC595
und vier direkt angeschlossenen Spalten. Entwickelt und kompiliert für Arduino Uno.

Die Bibliothek liegt im `src`-Verzeichnis des Sketches und wird automatisch
mitkompiliert ([Arduino-Sketch-Spezifikation](https://docs.arduino.cc/arduino-cli/sketch-specification/)).
Keine zusätzliche Installation nötig. Zum Wiederverwenden den gesamten Ordner
`ShiftRegisterKeypad` in den Bibliotheksordner eines anderen Arduino-Projekts
kopieren und dort `#include <ShiftRegisterKeypad.h>` verwenden.

```cpp
#include "src/ShiftRegisterKeypad/src/ShiftRegisterKeypad.h"

const uint8_t columns[4] = {6, 7, 8, 9};
// Reihenfolge: DATA, CLOCK, LATCH, Spaltenpins.
ShiftRegisterKeypad keypad(12, 11, 10, columns);

void setup() {
  keypad.begin();
}

void loop() {
  char key = keypad.getKey();
  if (key != 0) {
    // Tastendruck verarbeiten.
  }
}
```

Tastenbelegung zeilenweise: `123A`, `456B`, `789C`, `*0#D`.
Die Spalten verwenden interne Pull-ups; eine LOW-Zeile aktiviert die Abfrage.
Q4–Q7 bleiben HIGH. Der 74HC595 muss ausschließlich für das Keypad verwendet
werden; OE liegt LOW und MR HIGH.

`getKey()` regelmäßig in `loop()` aufrufen. Es scannt höchstens alle 5 ms,
entprellt Tastendrücke und Loslassen über 30 ms und liefert ansonsten `0`.
Gedrückthalten erzeugt keine Wiederholung. Ein stabiler Wechsel auf eine andere
Taste erzeugt ein neues Ereignis. Mehrere gleichzeitige Tasten werden nicht
unterstützt; die erste erkannte Taste gewinnt, ohne Ghosting-Erkennung.
Pro gescannter Zeile sind 50 µs zum Einschwingen nötig; es gibt keine
Wartepause für die Entprellung. Töne und Spiellogik gehören zum aufrufenden Sketch.

Host-Tests aus dem Repository-Hauptverzeichnis:

```sh
c++ -std=c++11 -Wall -Wextra -Werror \
  -I ReactorCoolerCode/tests/keypad \
  -I ReactorCoolerCode/ReactorCoolingGameCode/src/ShiftRegisterKeypad/src \
  ReactorCoolerCode/tests/keypad/test_keypad.cpp \
  ReactorCoolerCode/ReactorCoolingGameCode/src/ShiftRegisterKeypad/src/ShiftRegisterKeypad.cpp \
  -o /tmp/lf07-keypad-test
/tmp/lf07-keypad-test
```
