//#define DEBUG true

#include <Keyboard.h>
#include <Keypad.h>
#include <TM1637Display.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#define CLOCK_CLK_PIN 8
#define CLOCK_DIO_PIN 9
#define UPTIME_CLK_PIN 2
#define UPTIME_DIO_PIN 3
#define RAID_SWITCH_PIN 7
#define LIVE_SWITCH_PIN A0
#define RTC_DIO_PIN A2
#define RTC_CLK_PIN A3
#define RTC_CE_PIN A1

#define DOTS 0x40
#define NO_DOTS 0x00

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
RtcDateTime live_since;

TM1637Display display_live(UPTIME_CLK_PIN, UPTIME_DIO_PIN);
TM1637Display display_clock(CLOCK_CLK_PIN, CLOCK_DIO_PIN);

// Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

ThreeWire myWire(RTC_DIO_PIN, RTC_CLK_PIN, RTC_CE_PIN);
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  Keyboard.begin();
  Rtc.Begin();
  // keypad.addEventListener(keypadEvent);

  pinMode(RAID_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIVE_SWITCH_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RAID_SWITCH_PIN), triggerRaid, RISING);
  display_live.setBrightness(0x34);
  display_clock.setBrightness(0x33);

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);

  if (Rtc.GetIsWriteProtected())
  {
#ifdef DEBUG
    Serial.println("RTC was write protected, enabling writing now");
#endif
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
#ifdef DEBUG
    Serial.println("RTC was not actively running, starting now");
#endif
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
#ifdef DEBUG
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
#endif
    Rtc.SetDateTime(compiled);
  }

}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  display_liveUptime(now);

  if (millis() - last_live_toggle > 200) {
    if (digitalRead(LIVE_SWITCH_PIN) == HIGH && live == false) {
      startLive(now);
    } else if (digitalRead(LIVE_SWITCH_PIN) == LOW && live == true) {
      endLive();
    }

    last_live_toggle = millis();
  }
  delay(100);

  // keypad.getKey();
}

// void keypadEvent(KeypadEvent key) {
//  if (keypad.getState() == PRESSED) {
// Serial.println(key);
//    Keyboard.press(KEY_LEFT_CTRL);
//    Keyboard.press(KEY_LEFT_SHIFT);
//    switch (key) {
//      case '1':
//        Keyboard.press(KEY_F1);
//        break;
//      case '2':
//        Keyboard.press(KEY_F2);
//        break;
//      case '3':
//        Keyboard.press(KEY_F3);
//        break;
//      case '4':
//        Keyboard.press(KEY_F4);
//        break;
//    }
//  }
// delay(10);
// Keyboard.releaseAll();
//}

void triggerRaid() {
  if (millis() - last_raid > 200) {
    Serial.println("Le raid est partiiiiiiiiiii!");
    last_raid = millis();
  }
}

void startLive(const RtcDateTime& dt) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_F19);

  live = true;
  live_since = dt;
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

void display_liveUptime(const RtcDateTime& dt) {
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

  if (live) {
    RtcDateTime t = dt - live_since;

    display_live.showNumberDecEx(t.Hour() * 100 + t.Minute(), dt.Second() & 1 ? DOTS : NO_DOTS, true);
  } else {
    uint8_t SEG_UNDEFINED[] = {
      SEG_G,
      SEG_G,
      SEG_G,
      SEG_G,
    };
    display_live.setSegments(SEG_UNDEFINED);
  }
}

void printDateTime(const RtcDateTime& dt)
{
  display_clock.showNumberDecEx(dt.Hour() * 100 + dt.Minute(), dt.Second() & 1 ? DOTS : NO_DOTS, true);
#ifdef DEBUG
  Serial.println("Time updated");
#endif
}
