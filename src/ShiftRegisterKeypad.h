#pragma once

#include <Arduino.h>

class ShiftRegisterKeypad {
 public:
  ShiftRegisterKeypad(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin,
                      const uint8_t columnPins[4]);
  void begin();
  char getKey();

 private:
  void writeRows(uint8_t pattern);
  char scan();

  uint8_t dataPin_;
  uint8_t clockPin_;
  uint8_t latchPin_;
  uint8_t columnPins_[4];
  char candidate_ = 0;
  char stable_ = 0;
  uint32_t candidateSince_ = 0;
  uint32_t lastScan_ = 0;
};
