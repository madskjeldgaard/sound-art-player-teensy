#pragma once

#include "AudioFileManager.h"
#include "AudioFilePlayer.h"
#include "Hardware.h"
#include <Arduino.h>
#include <TaskScheduler.h>

namespace sap {

// The main class of SoundArtPlayer
class SoundArtPlayer {
public:
  explicit SoundArtPlayer()
      : mAudioPlayer(mAudioFileManager), mHardware(mAudioPlayer){};

  void begin(bool playImmediately = true) {

    constexpr auto audioFilesPath = "/audio";
    mAudioFileManager.begin(audioFilesPath);

    // This is done in hardware
    // if (playImmediately && audioPlayerLoaded) {
    //   mAudioPlayer.play();
    // }

    mHardware.begin();
    const bool audioPlayerLoaded = mAudioPlayer.begin();

    bool audioFilesExist = mAudioFileManager.numAudioFiles() > 0;

    // Show status of initialization with LED
    if ((!audioFilesExist || !audioPlayerLoaded)) {
      mHardware.getLED()->setLEDColor(SAPColor::RED);
      Serial.println("ERROR: SoundArtPlayer could not be initialized");
    } else {
      mHardware.getLED()->setLEDColor(SAPColor::GREEN);
      Serial.println("SoundArtPlayer initialized");
    }

    processControls();
  }

  void update() { mAudioPlayer.update(); }

  void processControls() {
    auto led = mHardware.getLED();
    auto audioDriver = mAudioPlayer.getAudioDriver();
    auto peakLeft = audioDriver->getPeakReaderLeft();
    auto peakRight = audioDriver->getPeakReaderRight();
    auto isPlaying = mAudioPlayer.isPlaying();

    if (peakLeft->available()) {

      auto peak = peakLeft->read();
      auto red = map(peak, 0.0f, 1.0f, 0.f, 205.f);
      // auto green = map(peak, 0.0f, 1.0f, 20.f, 255.f);
      auto blue = map(peak, 0.0f, 1.0f, 135.f, 1.f);

      // Green is set using a sine, if it is playing
      auto green = 100.0f;
      if (isPlaying) {
        auto sineFreq = 0.001f;
        green = 127.5f + 127.5f * sin(sineFreq * millis());
      }

      led->setLEDColor(static_cast<int>(red), static_cast<int>(green),
                       static_cast<int>(blue));
    }

    mHardware.update();
  }

  auto getAudioPlayer() { return &mAudioPlayer; }
  auto getAudioFileManager() { return &mAudioFileManager; }

private:
  AudioFilePlayer mAudioPlayer;
  AudioFileManager mAudioFileManager;
  Hardware mHardware;
};

} // namespace sap
