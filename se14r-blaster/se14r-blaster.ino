/**
 * This is the main file for the SE-14R blaster setup.
 * 
 * Hardware setup:
 *  - 40mm speaker in rear cap
 *  - arduino nano in the receiver
 *  - DF mini player and amp in handle
 *  - 7mm momentary switch in the handle
 *  - sliding on/aff switch in front of receiver
 *  - 7.4v lipo battery and buck convertere in the mag
 *  - 7LED Array in front, need lens
 *  Optional hardware considerations:
 *  - Lever switch reload in the front barrel
 *  - Haptic vibration motors in grip
 * 
 * All components settings can be found in "config.h". Before running, 
 * review and change all configurations based on your setup.
 * 
 * There's are number of libraries that you will need to install using the 
 * Library Manager:
 * 1. DFPlayerMini_Fast
 * 2. FastLED
 * 4. FireTimer
 * 5. ezButton
 * 6. Adafruit_DRV2605
 * 
 * The main loop:
 * 1. Check audio queue for playback
 * 2. Check if the haptic motors have been activated
 *   a. play the script
 * 3. Check if the leds have been activated
 *   a. update the leds following the pattern that has been set
 * 4. Check the fire trigger momentary switch.
 * 5. Send a blaster pulse, if fire trigger is a short press
 *   a. Toggles a clip counter
 *   b. Checks for an empty clip 
 *   c. If clip is not empty
 *      i. play alternating audio tracks for different sounds 
 *     ii. activate front led with blaster shot
 *         1. Flash white
 *         2. Burn Red to orange, then fade as it cools
 *   d. Otherwise play empty clip track
 * 6. Changes mode, if fire trigger is a long press
 *     i. playback change mode track
 *    ii. toggle fire mode
 * 
 */
 
