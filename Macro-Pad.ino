#include <BleKeyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//NOTE: These pins wont align with ones on PCB, I dont have the physical hardware, so Im
// using a simulation to make it, once I get the parts and the PCB, Ill update the pins as per required;

const int8_t buttons[14] = {
  14, 32, 14, 12,
  4, 5, 18, 15,
  -1, -1, -1, -1, // unsassigned for now, will assign later
  19, 23, // a, b
};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Fuctions: EACH button is tied to a function, one can modify the function as their will, the default ones are given below:
// 4 buttons: arrow keys, helpful for most
// Space, MEDIA_NEXT_TRACK, MEDIA_PREV_TRACK and MEDIA_PAUSE - pretty useful at times
// Remaining 4: left empty for customization, can include ESP NOW commads, smart home, anything!
//
// Rotary Encoder: Volume
// OLED Display: Signal Stats
//

BleKeyboard bleKeyboard("BLE Macro PAD", "theNeonTerminal", 100); // keyboard object

void b1();  void b2();  void b3();  void b4();
void b5();  void b6();  void b7();  void b8();
void b9();  void b10(); void b11(); void b12();

// Rotary Encoder
int8_t encoderSteps = 0;
uint8_t encoderPrevious = 0;

const int8_t encoderTransitions[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

void updateEncoder();
void encoderClockwise();
void encoderCounterClockwise();

void setup(){
  Serial.begin(115200);
  for (int i = 0; i < 12; i++) {
    if (buttons[i] != -1) pinMode(buttons[i], INPUT_PULLUP);
  }

  // Initialize rotary encoder
  encoderPrevious = (digitalRead(buttons[10]) << 1) | digitalRead(buttons[11]);

  if(!display.begin(SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  bleKeyboard.begin();
  Serial.print("Setup Complete");
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Setup Complete");
  display.display();
  delay(3000);
}

bool detectButtonPress(uint8_t pin) { // Simple function to detect keystrokes
    static unsigned long lastTrigger[40] = {0};
    static bool wasPressed[40] = {false};

    const unsigned long repeatInterval = 300;

    if (pin >= 40) return false;

    bool pressed = (digitalRead(pin) == LOW);

    if (!pressed) {
        wasPressed[pin] = false;
        return false;
    }

    // First press
    if (!wasPressed[pin]) {
        wasPressed[pin] = true;
        lastTrigger[pin] = millis();
        return true;
    }

    // Repeated triggers while held
    if (millis() - lastTrigger[pin] >= repeatInterval) {
        lastTrigger[pin] = millis();
        return true;
    }

    return false;
}

void loop(){
  updateEncoder();

  if(bleKeyboard.isConnected()) {
    for (int i = 0; i < 12; i++) {
      if (i == 10 || i == 11) continue;
      if (buttons[i] == -1 || !detectButtonPress(buttons[i])) continue;

      switch (i) {
        case 0: b1(); break;
        case 1: b2(); break;
        case 2: b3(); break;
        case 3: b4(); break;
        case 4: b5(); break;
        case 5: b6(); break;
        case 6: b7(); break;
        case 7: b8(); break;
        case 8: b9(); break;
        case 9: b10(); break;
        case 10: b11(); break;
        case 11: b12(); break;
      }
    }
  }
}

// Rotary Encoder Functions - can be modified
void updateEncoder() {
  uint8_t current = (digitalRead(buttons[10]) << 1) | digitalRead(buttons[11]);

  if (current == encoderPrevious) return;

  encoderSteps += encoderTransitions[(encoderPrevious << 2) | current];
  encoderPrevious = current;

  if (encoderSteps >= 4) {
    encoderSteps = 0;
    if (bleKeyboard.isConnected()) encoderClockwise();
  }
  else if (encoderSteps <= -4) {
    encoderSteps = 0;
    if (bleKeyboard.isConnected()) encoderCounterClockwise();
  }
}
void encoderClockwise() { bleKeyboard.write(KEY_MEDIA_VOLUME_UP); printAction("VOLUME UP"); }
void encoderCounterClockwise() { bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);  printAction("VOLUME DOWN"); }

void printAction(const char* action) {
  Serial.println(action);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Button Press:");
  display.println(action);
  display.display();
}

void b1()  { bleKeyboard.write(KEY_UP_ARROW);              printAction("UP"); }
void b2()  { bleKeyboard.write(KEY_DOWN_ARROW);            printAction("DOWN"); }
void b3()  { bleKeyboard.write(KEY_LEFT_ARROW);            printAction("LEFT"); }
void b4()  { bleKeyboard.write(KEY_RIGHT_ARROW);           printAction("RIGHT"); }

void b5()  { bleKeyboard.press(0x20);                     printAction("SPACE"); }
void b6()  { bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);     printAction("NEXT TRACK"); }
void b7()  { bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK); printAction("PREV TRACK"); }
void b8()  { bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);     printAction("PLAY/PAUSE"); }

void b9()  { printAction("BUTTON 9"); }
void b10() { printAction("BUTTON 10"); }
void b11() { printAction("BUTTON 11"); }
void b12() { printAction("BUTTON 12"); }
