#pragma once
#include <Arduino.h>

// Teensy audio
#include <Audio.h>

namespace sap {

constexpr auto neopixel_pin = 39;

// Pin connections for Teensy + Teensy audio shield
constexpr int button1Pin = 35;
constexpr int button2Pin = 34;
constexpr int button3Pin = 29;
constexpr int button4Pin = 33;
constexpr int button5Pin = 28;

constexpr int pot1Pin = 31;
constexpr int pot2Pin = 32;
constexpr int pot3Pin = A10;
constexpr int pot4Pin = 37;

#define TEENSY_USE_BUILTIN_SDCARD 1

// Teensy audio shield
constexpr int mosiPin = 7;
constexpr int sckPin = 14;
constexpr int misoPin = 12;

#ifdef TEENSY_USE_BUILTIN_SDCARD
constexpr int sdCSPin = BUILTIN_SDCARD;
#else
// Teensy audio shield
constexpr int csPin = 10;
#endif

} // namespace sap
