#include <Keyboard.h>
#include <Keypad.h>

const byte ROWS = 3;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '4', '5'},
  {'6', '7', '8', '9', '0'},
  {'a', 'b', 'c', 'd', 'e'},
};
byte rowPins[ROWS] = {14, 15, 9};
byte colPins[COLS] = {4, 5, 6, 10, 8};

long last_raid;
long last_live;
bool live = false;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  keypad.addEventListener(keypadEvent);

  pinMode(7, INPUT_PULLUP);
  pinMode(A0, INPUT);
  attachInterrupt(digitalPinToInterrupt(7), triggerRaid, RISING);
}

void loop() {
  if (millis() - last_live > 200) {
    if (digitalRead(A0) == HIGH && live == false) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F19);

      live = true;
    } else if (digitalRead(A0) == LOW && live == true) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_F20);
      live = false;
    }
    delay(10);
    Keyboard.releaseAll();
    last_live = millis();
  }
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
  delay(10);
  Keyboard.releaseAll();
}

void triggerRaid() {
  if (millis() - last_raid > 200) {
    Serial.println("Le raid est partiiiiiiiiiii!");
    last_raid = millis();
  }
}
