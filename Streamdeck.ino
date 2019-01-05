#include <Keyboard.h>
#include <Keypad.h>

const byte ROWS = 3; 
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', '0'},
  {'a', 'b', 'c', 'd', 'e'},
};
byte rowPins[ROWS] = {9,8,7}; 
byte colPins[COLS] = {2,3,4,5,6}; 

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
  if (keypad.getState() == PRESSED) {
    Serial.println(key);
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
  }
  delay(100);
  Keyboard.releaseAll();
}
