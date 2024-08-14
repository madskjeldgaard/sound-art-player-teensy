#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <TeensyVariablePlayback.h>

/*
 *
 * This is an audio player interface that is specific to the Teensy.
 *
 */
namespace sap {

/*

TODO: Set up EQ
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=106,312
AudioFilterBiquad        biquad2;        //xy=254,338
AudioFilterBiquad        biquad1;        //xy=257,290
AudioMixer4              mixer1;         //xy=445,389
AudioOutputI2S           i2s2;           //xy=580,301
AudioAnalyzePeak         peak1;          //xy=598,385
AudioConnection          patchCord1(playSdWav1, 0, biquad1, 0);
AudioConnection          patchCord2(playSdWav1, 1, biquad2, 0);
AudioConnection          patchCord3(biquad2, 0, mixer1, 1);
AudioConnection          patchCord4(biquad2, 0, i2s2, 1);
AudioConnection          patchCord5(biquad1, 0, mixer1, 0);
AudioConnection          patchCord6(biquad1, 0, i2s2, 0);
AudioConnection          patchCord7(mixer1, peak1);
AudioControlSGTL5000     sgtl5000_1;     //xy=375,541
// GUItool: end automatically generated code

*/

// The driver for the SD card player
// This is hardware specific
class TeensySDCardPlayer {
public:
  bool begin() {
    auto result = setupAudioShield();
    if (!result) {
      Serial.println("Could not setup audio shield");
      return false;
    }

    AudioNoInterrupts();
    playSd1.enableInterpolation(true);
    playSd1.setPlaybackRate(1.0f);
    setupAudioConnections();
    AudioInterrupts();

    setupEQ();

    return result;
  }

  bool playWav(String fileName) {
    AudioNoInterrupts();
    const auto ok = playSd1.playWav(fileName.c_str());
    // const auto ok = playSd1.play(fileName.c_str());
    AudioInterrupts();

    return ok;
  }

  void stop() {
    AudioNoInterrupts();
    playSd1.stop();
    AudioInterrupts();
  }

  bool isPlaying() { return playSd1.isPlaying(); }

  void setPlayrate(float newRate) {
    AudioNoInterrupts();
    playSd1.setPlaybackRate(newRate);
    AudioInterrupts();
  }

  void setVolume(float newVolume) {
    AudioNoInterrupts();
    audioShield.volume(newVolume);
    AudioInterrupts();
  }

  bool setupAudioShield() {
    auto result = audioShield.enable();

    setVolume(0.5f);
    AudioMemory(24);

    return result;
  }

  void setupAudioConnections() {
    Serial.println("Setting up audio connections without EQ");
    disconnectAll();

    // Direct output
    patchCord1.connect(playSd1, 0, i2s2, 0);
    patchCord2.connect(playSd1, 1, i2s2, 1);

    // Analysis
    patchCord3.connect(playSd1, 0, peak_left, 0);
    patchCord4.connect(playSd1, 1, peak_right, 1);
  }

  void disconnectAll() {
    patchCord1.disconnect();
    patchCord2.disconnect();
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord8.disconnect();
  }

  void setupEQ() {
    audioShield.audioPostProcessorEnable();
    // This chooses the tone control EQ in the SGTL5000
    constexpr auto toneControl = 2;
    audioShield.eqSelect(toneControl);
  }

  // Gain in DB
  void setHighShelfGain(float gain) {
    constexpr auto treble = 4;
    audioShield.eqBand(treble, gain);
  }

  // Gain in DB
  void setLowShelfGain(float gain) {
    constexpr auto bass = 0;
    audioShield.eqBand(bass, gain);
  }

  void eqActive(bool active) {
    if (active) {
      audioShield.audioPostProcessorEnable();
    } else {
      audioShield.audioProcessorDisable();
    }
  }

  auto getPeakReaderLeft() { return &peak_left; }
  auto getPeakReaderRight() { return &peak_right; }

  // AudioPlaySdWav playSd1;
  AudioControlSGTL5000 audioShield;

  AudioOutputI2S i2s2;

  AudioAnalyzePeak peak_left{}, peak_right{}; // xy=598,385
  AudioPlaySdResmp playSd1{};
  // AudioPlaySdWav playSd1;
  AudioConnection patchCord1, patchCord2, patchCord3, patchCord4, patchCord5,
      patchCord6, patchCord7, patchCord8;
};

} // namespace sap
