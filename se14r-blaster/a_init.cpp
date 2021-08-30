/*
 * name:  SE-14R
 * Author:  FriskyDingo
 * Date:  2021-08-28
 * Description: Code for SE-14r blaster setup
 */
#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "easybutton.h"
#include "easycounter.h"
#include "easyaudio.h"
#include "easyledv3.h"
#include "easyhaptic.h"

/**
 * All components are controlled or enabled by "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's no need to change any of the following code or functions. 
 */
EasyAudio audio(AUDIO_RX_PIN, AUDIO_TX_PIN);
EasyButton trigger(TRIGGER_PIN);

EasyLedv3<FIRE_LED_CNT, FIRE_LED_PIN> fireLed;
ezBlasterShot blasterShot(fireLed.RED, fireLed.ORANGE, 3);  // initialize colors to starting fire mode

EasyCounter fireCounter("fire", TRACK_FIRE_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);
EasyCounter stunCounter("stun", TRACK_STUN_ARR, TRACK_CLIP_EMPTY, TRACK_CLIP_RELOAD);

EasyHaptic haptic;

// trigger mode status
uint8_t selectedTriggerMode   = SELECTOR_FIRE_MODE;   // sets the fire mode to blaster to start

// function declarations
void runAudioPlayback();
void runHapticPlayback();
void runLedDisplay();
void handleFireTrigger();
void handleSelectorMode();
void sendBlasterPulse(EasyCounter &counter);
EasyCounter& getTriggerCounter();

void setup () {
  Serial.begin (9600);
  debugLog("Starting setup");
  // set up the fire trigger and the debounce threshold
  trigger.begin(25);

  //initializes the audio player and sets the volume
  audio.begin(20);

  // initialize all the leds
  // initialize the trigger led and set brightness
  fireLed.begin(75);

  // Initialize the clip counters for different modes
  fireCounter.begin(0, 10, COUNTER_MODE_DOWN);
  stunCounter.begin(0, 10, COUNTER_MODE_DOWN);

  // init the haptic ERM motors
  haptic.begin();

  // queue the first track
  debugLog("Powering up");
  audio.queuePlayback(TRACK_START_UP);
}

/**
 * The main loop.
 */
void loop () {
  // always handle the activated components first, before processing new inputs
  runHapticPlayback();
  runAudioPlayback();
  runLedDisplay();
  // check for new inputs
  handleFireTrigger();
}

/**
 * Run the haptic motor pattern
 */
void runHapticPlayback() {
  haptic.playWaveforms();
}
/**
 * Playback the next queued track
 */
void runAudioPlayback() {
  audio.playQueuedTrack();
}
/**
 * Run the led pattern
 */
void runLedDisplay() {
  fireLed.updateDisplay();
}

/**
 * Checks the fire trigger momentary switch.
 * Short press should send an alternating blaster pulse
 * Long press should change modes between A/B and C/D
 */
void handleFireTrigger() {
  // check trigger button
  int buttonStateFire = trigger.checkState();
  // check if a trigger is pressed.
  if (buttonStateFire == BUTTON_SHORT_PRESS) {
      sendBlasterPulse(getTriggerCounter());
  }
  if (buttonStateFire == BUTTON_LONG_PRESS) {
    handleSelectorMode();
  }
}

/**
 * Sends a blaster pulse.
 *   1. Toggles a clip counter
 *   2. Checks for an empty clip 
 *   3. If clip is not empty
 *      a. play audio track
 *      b. update led strip
 *      c. update led counter to the current clip counter
 *   4. Otherwise play empty clip track
 *
 */
void sendBlasterPulse(EasyCounter &counter) {
  debugLog("send alternating blaster pulse");
  bool emptyClip = counter.isEmpty();
  bool fullClip = counter.isFull();
  // check the track number before ticking, in case it's the last round
  // alternate between two firing blasts
  uint8_t idx = counter.getCount() % 2;
  int track = counter.getTrackNumber(idx); // returns the approapriate track, or empty clip
  counter.tick();
  if (fullClip)
    track = counter.getTrackNumber(idx);   // We redo this when the clip is full to get the right track
  audio.queuePlayback(track);
  if (emptyClip == false) {
    fireLed.activate(blasterShot);
    haptic.activate();
  }
}

EasyCounter& getTriggerCounter() {
  if (selectedTriggerMode == SELECTOR_FIRE_MODE)
      return fireCounter;
  return stunCounter;
}

/**
 * Checks the selector mode momentary switch.
 * 1. if a long preess plackback the theme track
 * 2. If a short press
 *   a. playback changee mode track
 *   b. toggle fire mode
 */
void handleSelectorMode() {
  // Check for Switching modes
  if (selectedTriggerMode == SELECTOR_FIRE_MODE) {
    selectedTriggerMode = SELECTOR_STUN_MODE;
    blasterShot.initialize(fireLed.YELLOW, fireLed.WHITE);  // shot - flash with color fade
    debugLog("Stun Mode selected");
  } else {
    selectedTriggerMode = SELECTOR_FIRE_MODE;
    blasterShot.initialize(fireLed.RED, fireLed.ORANGE);  // shot - flash with color fade
    debugLog("Fire Mode selected");
  }
  audio.queuePlayback(TRACK_CHANGE_MODE);
}
