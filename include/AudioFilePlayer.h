#pragma once

#include "AudioFileManager.h"
#include "TeensySDCardPlayer.h"
#include <Arduino.h>
#include <memory>

namespace sap {

/**
 * @class AudioFilePlayer
 * @brief This class plays audio files and keeps track of currently playing
 * index. It's also responsible of managing an audio driver to play the files
 * with.
 *
 */
class AudioFilePlayer {
public:
  explicit AudioFilePlayer(AudioFileManager &manager)
      : mAudioDriver(std::make_shared<TeensySDCardPlayer>()),
        mAudioFileManager(manager) {}

  bool begin() { return mAudioDriver->begin(); }

  void update() {
    // Update the audio player
    // TODO:
    // - Gap between audio files

    if (mIsPlaying) {
      const auto fileFinished = !mAudioDriver->isPlaying();

      // TODO: Use for adding gaps between files
      const auto timeOfFileFinished = millis();

      // If file finished, do something
      if (fileFinished) {
        // Play next
        if (mShuffle) {
          randomize();
        } else {
          next();
        }
      }
    }
  }

  void play() {
    const auto path = mAudioFileManager.getFilePath(mCurrentPlayingFileIndex);

    auto ok = mAudioDriver->playWav(path);

    mIsPlaying = ok;
    Serial.println(ok ? "Playing audio file " + path
                      : "Could not play " + path);
  }

  void stop() {
    mIsPlaying = false;
    // Stop the audio

    mAudioDriver->stop();
  }

  void togglePlay() {
    // Toggle the play state
    if (mIsPlaying) {
      stop();
    } else {
      play();
    }
  }

  void next() {
    // Play the next audio
    // TODO
    Serial.println("Playing next file");
    mCurrentPlayingFileIndex =
        (mCurrentPlayingFileIndex + 1) % mAudioFileManager.numAudioFiles();

    if (mShuffle) {
      randomize();
    } else {
      if (mIsPlaying) {
        stop();
        play();
      }
    }
  }

  void prev() {
    // Wrap around
    if (mCurrentPlayingFileIndex <= 0) {
      mCurrentPlayingFileIndex = mAudioFileManager.numAudioFiles() - 1;
    } else {
      mCurrentPlayingFileIndex =
          (mCurrentPlayingFileIndex - 1) % mAudioFileManager.numAudioFiles();
    }

    if (mShuffle) {
      randomize();
    } else {
      if (mIsPlaying) {
        stop();
        play();
      }
    }
  }

  void randomize() {

    auto newIndex = random(0, mAudioFileManager.numAudioFiles());

    while (newIndex == mCurrentPlayingFileIndex) {
      newIndex = random(0, mAudioFileManager.numAudioFiles());
    }

    Serial.println("Randomizing to " + String(newIndex));
    mCurrentPlayingFileIndex = newIndex;

    if (mIsPlaying) {
      stop();
      play();
    }
  }

  bool isPlaying() { return mIsPlaying; }

  // Enable or disable shuffle mode
  void shuffle(bool enable) { mShuffle = enable; }

  void toggleShuffle() { mShuffle = !mShuffle; }

  auto getAudioDriver() { return mAudioDriver; }

  void eqActive(bool active) { mAudioDriver->eqActive(active); }

  // Amount is 0.0f to 1.0f
  void setLowShelfGain(float amount) { mAudioDriver->setLowShelfGain(amount); }

  void setHighShelfGain(float amount) {
    mAudioDriver->setHighShelfGain(amount);
  }

protected:
  bool mShuffle = false;
  bool mIsPlaying = false;

  int mCurrentPlayingFileIndex = 0;

  std::shared_ptr<TeensySDCardPlayer> mAudioDriver;

  AudioFileManager &mAudioFileManager;
};

} // namespace sap
