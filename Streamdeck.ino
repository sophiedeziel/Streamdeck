#include <Keyboard.h>
#include <Keypad.h>
#include <TM1637Display.h>
#include <TimeLib.h>

#define UPTIME_CLK_PIN 2
#define UPTIME_DIO_PIN 3
#define RAID_SWITCH_PIN 7
#define LIVE_SWITCH_PIN A0

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
long last_live_toggle;

bool live = false;
time_t live_since = now();

TM1637Display display(UPTIME_CLK_PIN, UPTIME_DIO_PIN);

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  keypad.addEventListener(keypadEvent);

  pinMode(RAID_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIVE_SWITCH_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RAID_SWITCH_PIN), triggerRaid, RISING);
  display.setBrightness(0x33);


}

void loop() {
  displayUptime();

  if (millis() - last_live_toggle > 200) {
    if (digitalRead(LIVE_SWITCH_PIN) == HIGH && live == false) {
      startLive();
    } else if (digitalRead(LIVE_SWITCH_PIN) == LOW && live == true) {
      endLive();
    }

    last_live_toggle = millis();
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

void startLive() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  keyboard.press(KEY_F19);

  live = true;
  live_since = now();
  delay(10);
  Keyboard.releaseAll();
}

void endLive() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_F20);

  live = false;
  delay(10);
  Keyboard.releaseAll();
}

void displayUptime() {
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

  if (live) {
    time_t t = now() - live_since;

    display.showNumberDecEx(hour(t) * 100 + minute(t), 0x40, true);
  } else {
    uint8_t SEG_UNDEFINED[] = {
      SEG_G,
      SEG_G,
      SEG_G,
      SEG_G,
    };
    display.setSegments(SEG_UNDEFINED);
  }

}
