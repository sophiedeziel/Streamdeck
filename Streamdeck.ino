#include <Keyboard.h>
#include <Keypad.h>

const byte ROWS = 2; //four rows
const byte COLS = 2; //three columns
char keys[ROWS][COLS] = {
  {'1', '2'},
  {'3', '4'}
};
byte rowPins[ROWS] = {3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  keypad.addEventListener(keypadEvent);
}

void loop() {
  keypad.getKey();
}

void keypadEvent(KeypadEvent key) {
  Serial.println(key);
  switch (keypad.getState()) {
    case PRESSED:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      switch (key) {
        case '1':
          Keyboard.press(KEY_F1);
          break;
        case '2':
          Keyboard.press(KEY_F2);
          break;
        case '3':
          Keyboard.press(KEY_F3);
          break;
        case '4':
          Keyboard.press(KEY_F4);
          break;
      }
      break;
    case RELEASED:
      switch (key) {
          Keyboard.releaseAll();
          break;
      }
      break;
  }
}
