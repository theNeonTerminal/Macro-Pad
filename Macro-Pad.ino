#include <BleKeyboard.h>

const int up = 4, down = 5, left = 18, right = 15;
//NOTE: These pins wont align with ones on PCB, I dont have the physical hardware, so Im
// using a simulation to make it, once I get the parts and the PCB, Ill update the pins as per required;

BleKeyboard bleKeyboard("BLE Macro PAD", "theNeonTerminal", 50); // keyboard object

void setup(){
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);

  Serial.begin(115200);
  bleKeyboard.begin();
  Serial.print("Setup Complete");
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
    if (detectButtonPress(up)) bleKeyboard.write(KEY_UP_ARROW);
    if (detectButtonPress(down)) bleKeyboard.write(KEY_DOWN_ARROW);
    if (detectButtonPress(left)) bleKeyboard.write(KEY_LEFT_ARROW);
    if (detectButtonPress(right)) bleKeyboard.write(KEY_RIGHT_ARROW);
  }
}
