
#ifndef easyhaptic_h
#define easyhaptic_h

#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include "debug.h"

/**
 * EasyHaptic is based on DRV2605L breakout board, and provides simple setup, and simple vibration playback.
 * 
 * Default constructor takes no parameters
 * eg: EasyHaptic motor;
 * 
 * Call the begin function to initialize the driver, set the mode, select library, and set waveforms.
 * eg.motor.begin();
 * 
 * Motor can be activated 
 * the files are loaded/copied onto the SD card.
 * eg. audio.playTrack(1);
 */
class EasyHaptic
{
  private:
    Adafruit_DRV2605 _drv;
    bool _activated = false;

  public:
    EasyHaptic() {};

    void begin() {
#ifdef ENABLE_EASY_HAPTIC
      debugLog("setup haptic motor");
      _drv.begin();
      // I2C trigger by sending 'go' command 
      _drv.setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

      _drv.selectLibrary(1);
      _drv.setWaveform(0, 84); // ramp up medium 1, see datasheet part 11.2
      _drv.setWaveform(1, 14); // strong buzz 100%, see datasheet part 11.2
      _drv.setWaveform(2, 14); // strong buzz 100%, see datasheet part 11.2
      _drv.setWaveform(3, 0);  // end of waveforms
#endif
    }

    /**
     * Sets the pattern to activate the leds. Expects an instance of ezPattern.
     * See ezPattern for classes.
     */
    void activate() {
      debugLog("activating haptic motor");
      _activated = true;
    }

    /**
     * This should be called in the main program loop().
     * The call is a proxy to the ezPattern, if one has been provided.
     */
    void updateMotor() {
#ifdef ENABLE_EASY_HAPTIC
      debugLog("updating haptics");
      if (_activated) {
        _drv.go();
        _activated = !_activated;
      }
#endif
    }
};

#endif
