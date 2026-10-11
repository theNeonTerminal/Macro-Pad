<div align="center">

# 🎛️ BLE Macro Pad

### ESP32 wireless macro pad with 12 buttons, a rotary encoder, and an OLED

**12 configurable macro actions · Rotary volume control · OLED feedback · Bluetooth LE**

![Platform](https://img.shields.io/badge/Platform-ESP32-087F8C?style=flat-square) ![Connectivity](https://img.shields.io/badge/Connectivity-BLE-4967C6?style=flat-square) ![Firmware](https://img.shields.io/badge/Firmware-Arduino_C%2B%2B-00979D?style=flat-square)

*A small DIY shortcut pad for media controls and everyday keyboard macros.*

</div>

---

## About

This is a BLE macro pad built around an **ESP32**, a **128 × 64 SSD1306 OLED**, 12 pushbuttons, and a bare rotary encoder. The idea is simple: keep the shortcuts and media controls I use most on a small wireless device.

Every macro has its own function (`b1()` through `b12()`), so changing a shortcut is straightforward.

> [!IMPORTANT]
> **Arduino IDE ESP32 board package: version `2.0.17`.** Use **ESP32 by Espressif Systems v2.0.17** for this project. The current firmware does **not compile with newer core versions**.
>
> The schematic shows the planned PCB connections. The firmware GPIO array still needs to be updated to match the board before flashing.

## 🖼️ Hardware Design

### 01 / Circuit Schematic

<div align="center">

![Circuit schematic](./schematic.png)

*Electrical connections between the ESP32, buttons, OLED, and rotary encoder.*

</div>

### 02 / PCB Layout

<div align="center">

![PCB layout](./footprint.png)

*Board routing, component placement, and copper traces.*

</div>

### 03 / PCB 3D View

<div align="center">

![3D PCB view](./3d.png)

*Preview of the assembled board before fabrication.*

</div>

## 🚀 Features

- **Bluetooth LE keyboard:** Send keyboard shortcuts and media-control commands wirelessly.
- **Customizable macros:** Dedicated `b1()`–`b12()` functions make actions easy to edit.
- **Rotary encoder:** Turn to adjust system volume up or down.
- **OLED feedback:** Show the last triggered action on a 128 × 64 monochrome display.
- **Press-and-hold repeat:** Configurable repeat interval for assigned button inputs (currently 300 ms).
- **Expandable firmware:** Reserve unused macros for application shortcuts, home automation, or future ESP-NOW integrations.

## 🧩 Components

| Component | Purpose |
| --- | --- |
| ESP32 development board | Main controller + BLE |
| SSD1306 OLED, 128 × 64, I²C | On-device feedback |
| Bare EC11-style rotary encoder | Volume control |
| Momentary pushbuttons | Custom macros (up to 12 planned) |
| Custom PCB | Final electrical assembly |

## 🎮 Button Layout

The schematic uses 12 independent pushbuttons. Each button connects between its GPIO and GND and is read with `INPUT_PULLUP`.

| Button | GPIO (PCB schematic) | Default firmware action |
| :--- | :--- | :--- |
| SW1 | GPIO 13 | Up arrow |
| SW2 | GPIO 25 | Down arrow |
| SW3 | GPIO 36 (VP) | Left arrow |
| SW4 | GPIO 12 | Right arrow |
| SW5 | GPIO 39 (VN) | Space |
| SW6 | GPIO 26 | Next track |
| SW7 | GPIO 34 | Previous track |
| SW8 | GPIO 14 | Play / pause |
| SW9 | GPIO 33 | Custom |
| SW10 | GPIO 35 | Custom |
| SW11 | GPIO 15 | Custom |
| SW12 | GPIO 18 | Custom |

**Other connections:** The OLED uses I²C; the rotary encoder is intended for volume control. Refer to the schematic for their PCB connections.

> [!NOTE]
> These GPIOs are taken from the **PCB schematic**, not the prototype firmware's older `buttons[]` array. Update that array before flashing. GPIO 34–39 are input-only and don't have internal pull-ups on the ESP32, so buttons on GPIO 34, 35, 36 and 39 will need **external pull-up resistors**. Double-check the final netlist before ordering the board.

## 🛠️ Firmware Setup

1. In Arduino IDE Boards Manager, install **ESP32 by Espressif Systems — `2.0.17`**. Do not upgrade the board package for this firmware; newer versions do not compile it.
2. Install these libraries:
   - `BleKeyboard` (ESP32 BLE Keyboard)
   - `Adafruit GFX Library`
   - `Adafruit SSD1306`
   - `Wire` (included with the ESP32 Arduino core)
3. Open the project sketch and select the correct ESP32 board and port.
4. Update the sketch GPIO assignments to match the schematic, and confirm the OLED I²C address (`0x3C`).
5. Upload the sketch, then pair the device named **BLE Macro PAD** with a BLE-capable host.

> [!CAUTION]
> The hardware and firmware have not yet been tested together on the finished PCB. Check the schematic, power connections and encoder direction before regular use.

## 🧠 Customizing Macros

Each button has a small dedicated function. For example:

```cpp
void b6() { bleKeyboard.write(KEY_MEDIA_NEXT_TRACK); printAction("NEXT TRACK"); }
```

To change a macro, edit the corresponding `bN()` function. `printAction()` sends a message to both the Serial Monitor and the OLED.

For the encoder, customize:

```cpp
void encoderClockwise() {
  bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  printAction("VOLUME UP");
}

void encoderCounterClockwise() {
  bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  printAction("VOLUME DOWN");
}
```

## Notes

This is a work in progress. Once the PCB is assembled, I’ll update the pin mapping and add photos and test results.

---

<div align="center">

Built with ESP32 and Arduino C++.

</div>
