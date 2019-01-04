#include <Keyboard.h>
#define BUTTON 2

const int debounceTime  = 100;
long last_jump;

void setup() {
  pinMode(BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), blop, RISING);
  Serial.begin(115200);
  Keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void blop() {
  if (millis() - last_jump > debounceTime) {
    last_jump = millis();
    Serial.println("Blop!");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F8);

    delay(100);
    Keyboard.releaseAll();
  }
}
