#include "SoundArtPlayer.h"
#include <Arduino.h>
#include <TaskScheduler.h>

constexpr auto controlRateFrequency = 20.f; // hz

// #define DEBUG
sap::SoundArtPlayer soundartplayer;

constexpr auto postResourceUsage = true;

void printUsage() {
  Serial.print("Proc = ");
  Serial.print(AudioProcessorUsage());
  Serial.print(" (");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("),  Mem = ");
  Serial.print(AudioMemoryUsage());
  Serial.print(" (");
  Serial.print(AudioMemoryUsageMax());
  Serial.println(")");
}

Scheduler runner;
Task t1(1000, TASK_FOREVER, &printUsage, &runner, true);

const auto controlRatePeriod = static_cast<int>(1000.f / controlRateFrequency);
Task controlRateTask(
    controlRatePeriod, TASK_FOREVER, []() { soundartplayer.processControls(); },
    &runner, true);

void setup() {
  Serial.begin(115200);

#ifdef DEBUG
  while (!Serial || millis() < 3000)
    ;
#endif
  soundartplayer.begin();

  runner.startNow();
  runner.addTask(controlRateTask);

  if (postResourceUsage) {
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();

    runner.addTask(t1);
  }
}

void loop() {
  soundartplayer.update();

  if (postResourceUsage)
    runner.execute();
}
