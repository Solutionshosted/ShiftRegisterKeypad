#include "ShiftRegisterKeypad.h"

// ShiftRegisterKeypad()
// Stores the shift register pins and copies the four column pins without accessing hardware.
ShiftRegisterKeypad::ShiftRegisterKeypad(uint8_t dataPin, uint8_t clockPin,
                                       uint8_t latchPin, const uint8_t columnPins[4])
    : dataPin_(dataPin), clockPin_(clockPin), latchPin_(latchPin) {
  for (uint8_t i = 0; i < 4; ++i) columnPins_[i] = columnPins[i];
}

// begin()
// Initializes the row outputs and column pull-ups, deselects all rows, and resets debounce state.
void ShiftRegisterKeypad::begin() {
  pinMode(dataPin_, OUTPUT);
  pinMode(clockPin_, OUTPUT);
  pinMode(latchPin_, OUTPUT);
  for (uint8_t i = 0; i < 4; ++i) pinMode(columnPins_[i], INPUT_PULLUP);
  writeRows(0xFF);
  candidate_ = stable_ = 0;
  candidateSince_ = lastScan_ = millis();
}

// writeRows()
// Sends a row pattern to the 74HC595 and latches it, with LOW selecting a keypad row.
void ShiftRegisterKeypad::writeRows(uint8_t pattern) {
  digitalWrite(latchPin_, LOW);
  shiftOut(dataPin_, clockPin_, MSBFIRST, pattern);
  digitalWrite(latchPin_, HIGH);
}

// scan()
// Reads rows Q0 through Q3 and returns the first pressed key, leaving all rows inactive afterward.
char ShiftRegisterKeypad::scan() {
  static const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };
  for (uint8_t row = 0; row < 4; ++row) {
    writeRows(static_cast<uint8_t>(~(1U << row)));
    delayMicroseconds(50); // Allow the electrical signals to settle.
    for (uint8_t column = 0; column < 4; ++column) {
      if (digitalRead(columnPins_[column]) == LOW) {
        writeRows(0xFF);
        return keys[row][column];
      }
    }
  }
  writeRows(0xFF);
  return 0;
}

// getKey()
// Scans at most every 5 ms and returns a new key after 30 ms of stable input, without repeating held keys.
char ShiftRegisterKeypad::getKey() {
  const uint32_t now = millis();
  if (static_cast<uint32_t>(now - lastScan_) < 5) return 0;
  lastScan_ = now;

  const char raw = scan();
  if (raw != candidate_) {
    candidate_ = raw;
    candidateSince_ = now;
  }
  if (raw != stable_ && static_cast<uint32_t>(now - candidateSince_) >= 30) {
    stable_ = raw;
    return stable_;
  }
  return 0;
}
