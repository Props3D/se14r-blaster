
#ifndef easyhaptic_h
#define easyhaptic_h

#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include "debug.h"

/**
 * EasyHaptic is based on DRV2605L breakout board, and provides simple setup, and simple vibration playback.
 * 
 * Constructor takes rx and tx pins as inputs, but will default to 0 and 1.
 * eg: EasyAudio audio(0, 1);
 * 
 * Call the begin function to initialize the serial comms, and set the volume.
 * Volume range is 0 - 30, default is 25.
 * eg.audio.begin(15);
 * 
 * Playback is by track index. The track index is determined by the order in which 
 * the files are loaded/copied onto the SD card.
 * eg. audio.playTrack(1);
 */
class EasyHaptic
{
  private:
    Adafruit_DRV2605 drv;
    bool activated = false;

  public:
    EasyHaptic() {};

    void begin() {
#ifdef ENABLE_EASY_HAPTIC
      debugLog("setup haptic motor");
      drv.begin();
      // I2C trigger by sending 'go' command 
      drv.setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

      drv.selectLibrary(1);
      drv.setWaveform(0, 84); // ramp up medium 1, see datasheet part 11.2
      drv.setWaveform(1, 1);  // strong click 100%, see datasheet part 11.2
      drv.setWaveform(2, 0);  // end of waveforms
#endif
    }

    /**
     * Sets the pattern to activate the leds. Expects an instance of ezPattern.
     * See ezPattern for classes.
     */
    void activate() {
      debugLog("activating haptic motor");
      activated = true;
    }

    /**
     * This should be called in the main program loop().
     * The call is a proxy to the ezPattern, if one has been provided.
     */
    void updateMotor() {
#ifdef ENABLE_EASY_HAPTIC
      debugLog("updating haptics");
      if (activated) {
        drv.go();
        activated = !activated;
      }
#endif
    }
};

#endif
