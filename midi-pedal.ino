#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const byte CHANNEL = 1;
const int MIDI_LOW_VOLUME = 0;
const int MIDI_MAX_VOLUME = 127;
const int ANALOG_MIN_VALUE = 0;
const int ANALOG_MAX_VALUE = 1023;

// const int KEYS_COUNT = 3;
// const int KEYS_PINS[KEYS_COUNT] = {2, 3, 4};
// const int KEYS_NOTES[KEYS_COUNT] = {41, 42, 43};
// bool keys_flag[KEYS_COUNT];

const int POTENTIOMETER_COUNT = 3;
const int POTENTIOMETER_PINS[POTENTIOMETER_COUNT] = {A0, A1, A2};
const int POTENTIOMETER_NOTES[POTENTIOMETER_COUNT] = {0, 1, 2};
const int POTENTIOMETERS_DELTA = 2;
int potentiometer_values[POTENTIOMETER_COUNT];

const int SWITCH_BUTTON_COUNT=1;
const int SWITCH_BUTTON_PINS[SWITCH_BUTTON_COUNT] = {5};
const int SWITCH_BUTTON_LEDS[SWITCH_BUTTON_COUNT] = {8};
const int SWITCH_BUTTON_NOTES[SWITCH_BUTTON_COUNT] = {5};
bool switch_button_states[SWITCH_BUTTON_COUNT];
bool switch_button_flags[SWITCH_BUTTON_COUNT];

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  // for(int i = 0; i < KEYS_COUNT; i++) {
  //   pinMode(KEYS_PINS[i], INPUT_PULLUP);
  // }
  for(int i = 0; i < SWITCH_BUTTON_COUNT; i++) {
    pinMode(SWITCH_BUTTON_PINS[i], INPUT_PULLUP);
    if (SWITCH_BUTTON_LEDS[i] != 0) {
      pinMode(SWITCH_BUTTON_LEDS[i], OUTPUT);
    }
  }
}

void loop() {
  // readKeys();
  readPotentiometers();
  readSwitchButton();
}

// void readKeys() {
//   for(int i = 0; i < KEYS_COUNT; i++) {
//     bool btnState = !digitalRead(KEYS_PINS[i]);
//     if (btnState && !keys_flag[i]) {
//       keys_flag[i] = true;
//       MIDI.sendNoteOn(KEYS_NOTES[i], MIDI_MAX_VOLUME, CHANNEL);
//     }
//     if (!btnState && keys_flag[i]) {
//       keys_flag[i] = false;
//       MIDI.sendNoteOff(KEYS_NOTES[i], MIDI_LOW_VOLUME, CHANNEL);
//     }
//   }
// }

void readSwitchButton() {
  for(int i = 0; i < SWITCH_BUTTON_COUNT; i++) {
    bool btnState = !digitalRead(SWITCH_BUTTON_PINS[i]);
    if (btnState && !switch_button_flags[i]) {
      switch_button_flags[i] = true;

      switch_button_states[i] = !switch_button_states[i];
      if (SWITCH_BUTTON_LEDS[i] != 0) {
        digitalWrite(SWITCH_BUTTON_LEDS[i], switch_button_states[i]);
      }
      MIDI.sendControlChange(SWITCH_BUTTON_NOTES[i], ((int) switch_button_states[i]) * MIDI_MAX_VOLUME, CHANNEL);
    }
    if (!btnState && switch_button_flags[i]) {
      switch_button_flags[i] = false;
    }
  }
}

void readPotentiometers() {
  for(int i = 0; i < POTENTIOMETER_COUNT; i++) {
    int value = analogRead(POTENTIOMETER_PINS[i]);
    value = map(value, ANALOG_MIN_VALUE, ANALOG_MAX_VALUE, MIDI_LOW_VOLUME, MIDI_MAX_VOLUME);
    value = constrain(value, MIDI_LOW_VOLUME, MIDI_MAX_VOLUME);
    value = MIDI_MAX_VOLUME - value;

    if (abs(value - potentiometer_values[i]) > POTENTIOMETERS_DELTA) {
      potentiometer_values[i] = value;
      MIDI.sendControlChange(POTENTIOMETER_NOTES[i], value, CHANNEL); 
    }
  }
}