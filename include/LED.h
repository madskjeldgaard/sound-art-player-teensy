/*
 * TODO:
 * - Add blinking method functionality
 *
 */
#pragma once
#include "Pins.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <TaskScheduler.h>

namespace sap {

enum class SAPColor {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE,
  PINK,
  PURPLE,
  ORANGE,
  RANDOM
};

/**
 * @class LED
 * @brief A wrapper around a WS2812 RGB LED aka neopixel
 *
 */
class LED {
public:
  explicit LED() : rgbLED(1, neopixel_pin, NEO_GRB + NEO_KHZ800) {}

  void begin() {
    rgbLED.begin();
    const auto brightness = 255;
    rgbLED.setBrightness(brightness);
    rgbLED.show();

    // auto taskCallback = TaskCallback([this]() { blink(); });
    // blinkTask.set(100, TASK_FOREVER, taskCallback);

    // scheduler.addTask(blinkTask);
    // blinkTask.enable();
  }

  void setBrightness(uint brightness) {
    rgbLED.setBrightness(brightness);
    rgbLED.show();
  }


  void setLEDColor(uint r, uint g, uint b) {
    rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
    rgbLED.show();
  }

  void setLEDColor(SAPColor color) {
    switch (color) {
    case SAPColor::RED:
      setLEDColor(255, 0, 0);
      break;
    case SAPColor::GREEN:
      setLEDColor(0, 255, 0);
      break;
    case SAPColor::BLUE:
      setLEDColor(0, 0, 255);
      break;
    case SAPColor::YELLOW:
      setLEDColor(255, 255, 0);
      break;
    case SAPColor::CYAN:
      setLEDColor(0, 255, 255);
      break;
    case SAPColor::MAGENTA:
      setLEDColor(255, 0, 255);
      break;
    case SAPColor::WHITE:
      setLEDColor(255, 255, 255);
      break;
    case SAPColor::PINK:
      setLEDColor(255, 105, 180);
      break;
    case SAPColor::PURPLE:
      setLEDColor(128, 0, 128);
      break;
    case SAPColor::ORANGE:
      setLEDColor(255, 165, 0);
      break;
    case SAPColor::RANDOM:
      setLEDColor(random(0, 255), random(0, 255), random(0, 255));
      break;
    }
  }

  void update() {
    // scheduler.execute();
  }

private:
  // void blink() {
  //   phase += phaseIncrement;
  //   if (phase >= TWO_PI) {
  //     phase -= TWO_PI;
  //   }

  //   float sineValue =
  //       (sin(phase) + 1.f) / 2.f; // Normalize sine wave to range [0, 1]

  //   // TODO
  // }

  // NeoPixel object
  Adafruit_NeoPixel rgbLED;

  // float phase = 0.0;
  // float phaseIncrement = 0.05;
  // Scheduler scheduler;
  // Task blinkTask;
};
} // namespace sap
