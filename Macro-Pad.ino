#include <BleKeyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//NOTE: These pins wont align with ones on PCB, I dont have the physical hardware, so Im
// using a simulation to make it, once I get the parts and the PCB, Ill update the pins as per required;

const int8_t buttons[12] = {
  4, 5, 18, 15,
  -1, -1, -1, -1,
  -1, -1, -1, -1
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

BleKeyboard bleKeyboard("BLE Macro PAD", "theNeonTerminal", 50); // keyboard object

void b1();  void b2();  void b3();  void b4();
void b5();  void b6();  void b7();  void b8();
void b9();  void b10(); void b11(); void b12();

void setup(){
  for (int i = 0; i < 12; i++) {
    if (buttons[i] != -1) pinMode(buttons[i], INPUT_PULLUP);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  Serial.begin(115200);
  bleKeyboard.begin();
  Serial.print("Setup Complete");
  display.clearDisplay();
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
  if(bleKeyboard.isConnected()) {
    for (int i = 0; i < 12; i++) {
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

void b5()  { bleKeyboard.write(' ');                       printAction("SPACE"); }
void b6()  { bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);     printAction("NEXT TRACK"); }
void b7()  { bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK); printAction("PREV TRACK"); }
void b8()  { bleKeyboard.press(KEY_MEDIA_PLAY_PAUSE);     printAction("PLAY/PAUSE"); }

void b9()  { printAction("BUTTON 9"); }
void b10() { printAction("BUTTON 10"); }
void b11() { printAction("BUTTON 11"); }
void b12() { printAction("BUTTON 12"); }
