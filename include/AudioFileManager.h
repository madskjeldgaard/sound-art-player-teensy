#pragma once

#include "Pins.h"
#include <Arduino.h>
#include <SD.h>
#include <string>
#include <vector>

namespace sap {

class AudioFileManager {
public:
  AudioFileManager() = default;

  void begin(String directory) {

    initSDCard();

    mDirectory = directory;
    populateFilenames();

    Serial.println("AudioFileManager initialized");

    Serial.println("Audio files found:");
    for (const auto &filename : mFilenames) {
      Serial.println(filename);
    }

    Serial.println("Total: " + String(mFilenames.size()));
  }

  const auto &getFilepaths() const { return mFilenames; }

  int numAudioFiles() const { return mFilenames.size(); }

  auto getFilePath(int index) const { return mFilenames.at(index); }

private:
  auto isWavFile(const String &filename) {
    int m = filename.lastIndexOf(".WAV");
    int a = filename.lastIndexOf(".wav");

    // Filter these out
    int underscore = filename.indexOf("_");
    int weirdUnderscoreDot = filename.startsWith("._");

    return (m > 0 || a > 0) && (underscore != 0) && !weirdUnderscoreDot;
  }

  void populateFilenames() {
    File dir = SD.open(mDirectory.c_str());
    while (true) {
      File file = dir.openNextFile();
      if (!file) {
        break;
      }

      // If mDirectory does not end with a slash, add one
      if (mDirectory[mDirectory.length() - 1] != '/') {
        mDirectory += "/";
      }
      String curfile = file.name();

      if (isWavFile(curfile)) {
        mFilenames.push_back((mDirectory + curfile).c_str());
      }

      file.close();
    }
    dir.close();
  }

  void initSDCard() {
    //    // See this for info on teensy sd card:
    // https://github.com/PaulStoffregen/SD/blob/Juse_Use_SdFat/examples/SdFat_Usage/SdFat_Usage.ino

    // if (!(SD.begin(AUDIO_SHIELD_CHIP_SELECT))) {
    if (!SD.begin(sdCSPin)) {
      // stop here if no SD card, but print a message
      while (1) {
        Serial.println("Unable to access the SD card");
        delay(500);
      }
    }
  }

  String mDirectory;
  std::vector<String> mFilenames;
};

} // namespace sap
