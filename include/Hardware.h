#pragma once

#include "ArduinoButton.hpp"
#include "ArduinoVoltage.hpp"
#include "AudioFilePlayer.h"
#include "LED.h"
#include "Pins.h"
#include <Arduino.h>
#include <memory>

namespace sap {

/**
 * @class Hardware
 * @brief Set up pins and hardware components like buttons, potentiometers and
 * LEDs and register observer callbacks for them when things happen.
 *
 */
class Hardware {
public:
  explicit Hardware(AudioFilePlayer &audioPlayer) : mAudioPlayer(audioPlayer) {}

  void begin() {
    setupButtons();
    setupPotentiometers();
    mLED.begin();
    mLED.setLEDColor(SAPColor::ORANGE);
  }

  void update() {

    mButton1.update();
    mButton2.update();
    mButton3.update();
    mButton4.update();
    mButton5.update();

    mPot1.update();
    mPot2.update();
    mPot3.update();
    mPot4.update();

    mLED.update();
  }

  auto getLED() { return &mLED; }

private:
  void setupButtons() {

    mButton1.subscribe(mButton1Poster);
    mButton2.subscribe(mButton2Poster);
    mButton3.subscribe(mButton3Poster);
    mButton4.subscribe(mButton4Poster);
    mButton5.subscribe(mButton5Poster);

    mButton1.subscribe(mPlayToggleObs);
    mButton2.subscribe(mNextObs);
    mButton3.subscribe(mPrevObs);
    mButton4.subscribe(mShuffleObs);
    mButton5.subscribe(mEqToggleObs);
  }

  void setupPotentiometers() {
    mFlipPot1 = true;
    mFlipPot2 = true;
    mFlipPot3 = true;
    mFlipPot4 = true;

    mPot1.subscribe(mPot1PlaybackRate);
    mPot2.subscribe(mPot2Volume);
    mPot3.subscribe(mPot3LowShelf);
    mPot4.subscribe(mPot4HighShelf);

    mPot1.subscribe(mPot1Poster);
    mPot2.subscribe(mPot2Poster);
    mPot3.subscribe(mPot3Poster);
    mPot4.subscribe(mPot4Poster);
  }

  // Potentiometers
  obs::ArduinoVoltage mPot1{pot1Pin}, mPot2{pot2Pin}, mPot3{pot3Pin},
      mPot4{pot4Pin};

  // Potentiometer observers
  obs::VoltageObserver mPot1Poster = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          Serial.println("Pot 1 value: " + String(voltage.getSmoothedValue()));
        }
      });

  obs::VoltageObserver mPot2Poster = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          Serial.println("Pot 2 value: " + String(voltage.getSmoothedValue()));
        }
      });

  obs::VoltageObserver mPot3Poster = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          Serial.println("Pot 3 value: " + String(voltage.getSmoothedValue()));
        }
      });

  obs::VoltageObserver mPot4Poster = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          Serial.println("Pot 4 value: " + String(voltage.getSmoothedValue()));
        }
      });

  // Set playback rate with potentiometer
  obs::VoltageObserver mPot1PlaybackRate = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          auto rate = voltage.getNormalizedValue();

          if (mFlipPot1) {
            rate = 1.0f - rate;
          };

          mAudioPlayer.getAudioDriver()->setPlayrate(rate);
        }
      });

  // Set volume with potentiometer
  // TODO:
  obs::VoltageObserver mPot2Volume = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          auto volume = voltage.getNormalizedValue();

          if (mFlipPot2) {
            volume = 1.0f - volume;
          };

          // Scale volume to a reasonable range
          volume *= 0.75f;

          mAudioPlayer.getAudioDriver()->setVolume(volume);
        }
      });

  // Set low shelf amount
  obs::VoltageObserver mPot3LowShelf = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          auto amount = voltage.getNormalizedValue();

          if (mFlipPot3) {
            amount = 1.0f - amount;
          };

          const auto minLowShelfGain = -1.0f;
          const auto maxLowShelfGain = 1.0f;
          auto gain = map(amount, 0.0f, 1.0f, minLowShelfGain, maxLowShelfGain);
          Serial.println("Setting low shelf gain to " + String(gain));
          mAudioPlayer.setLowShelfGain(gain);
        }
      });

  // Set high shelf amount
  obs::VoltageObserver mPot4HighShelf = obs::VoltageObserver(
      [&](obs::ArduinoVoltage &voltage, const String &field_name) {
        if (field_name == "value") {
          auto amount = voltage.getNormalizedValue();
          if (mFlipPot4) {
            amount = 1.0f - amount;
          };

          const auto minHighShelfGain = -1.0f;
          const auto maxHighShelfGain = 1.0f;
          auto gain =
              map(amount, 0.0f, 1.0f, minHighShelfGain, maxHighShelfGain);
          Serial.println("Setting high shelf gain to " + String(gain));
          mAudioPlayer.setHighShelfGain(gain);
        }
      });

  // Buttons
  obs::ArduinoButton mButton1{button1Pin}, mButton2{button2Pin},
      mButton3{button3Pin}, mButton4{button4Pin}, mButton5{button5Pin};

  // Serial communication observers
  obs::ButtonObserver mButton1Poster = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("Button 1 pressed");
        }
      });

  obs::ButtonObserver mButton2Poster = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("Button 2 pressed");
        }
      });

  obs::ButtonObserver mButton3Poster = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("Button 3 pressed");
        }
      });

  obs::ButtonObserver mButton4Poster = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("Button 4 up");
        } else if (field_name == "rise") {
          Serial.println("Button 4 down");
        }
      });

  obs::ButtonObserver mButton5Poster = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "rise") {
          Serial.println("Button 5 down");
        } else if (field_name == "fall") {
          Serial.println("Button 5 up");
        }
      });

  // AudioPlayer state
  obs::ButtonObserver mPlayToggleObs = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "rise") {
          Serial.println("Stop button pressed");
          mAudioPlayer.stop();
        } else if (field_name == "fall") {
          Serial.println("Play button pressed");
          mAudioPlayer.play();
        }
      });

  obs::ButtonObserver mNextObs = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          mAudioPlayer.next();
        }
      });

  obs::ButtonObserver mPrevObs = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          mAudioPlayer.prev();
        }
      });

  obs::ButtonObserver mShuffleObs = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("Shuffle button pressed high");
          mAudioPlayer.shuffle(true);
        } else if (field_name == "rise") {
          Serial.println("Shuffle button pressed low");
          mAudioPlayer.shuffle(false);
        }
      });

  obs::ButtonObserver mEqToggleObs = obs::ButtonObserver(
      [&](obs::ArduinoButton &button, const String &field_name) {
        if (field_name == "fall") {
          Serial.println("EQ button pressed HIGh");
          mAudioPlayer.eqActive(true);
        } else if (field_name == "rise") {
          Serial.println("EQ button pressed LOW");
          mAudioPlayer.eqActive(false);
        }
      });

  AudioFilePlayer &mAudioPlayer;

  LED mLED{};

  bool mFlipPot1{true}, mFlipPot2{true}, mFlipPot3{true}, mFlipPot4{true};
};

} // namespace sap
