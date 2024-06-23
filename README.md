# Open the door

[中文](./README_CN.md)

`open_the_door` is a project developed by me and a roomate during our free time, aiming to provide a more convenient way to open the dormitory door, so that we don't have to worry about our keys anymore. We have discussed and tested various solutions, such as remote door opening via the network, password input door opening, etc., but we ultimately chose to open the door by swiping the school card, considering energy consumption, security, and other factors. However, you can easily extend this project to other door opening methods.

This project is written in C++ and based on the Arduino development platform and ESP32 development board. If you are using similar hardwares, you can refer to the **Quick Start** section for quick deployment. If you want to modify the code to adapt to your hardwares, please refer to the **Development** section. The **Usage** section introduces the usage and specific functions.

This project uses some of [these libraries](https://github.com/illustager/MyArduino).

## Features

- Written in Arduino C++ and uses ESP32 development board
- Open the door by swiping the school card
- Management system with login password supports adding, viewing and deleting users at any time
- Uses sleep mode to save energy
- Plays audio to welcome visitors when opening the door
- Easy to port (see **Development** section)

## Quick Start

1. Prepare an ESP32-WROOM-32 or compatible MCU. Prepare a servo (for opening the door), a card reader (I choose RFID-RC522), a 4x4 keypad, an OLED screen, an audio amplifier module and a speaker. If you don't want to use these hardwares to achieve your goal, please refer to the **Development** section.
2. Install hardwares in a suitable location and connect them to the ESP32. Note that the keypad and screen are used for user management, so please install them on the inside of the door.
3. Bring out one of the touch pins of the ESP32 to the outside of the door for touch wake-up from sleep.
4. Modify the `config` section at the beginning of each .cpp file in the project according to the hardware connections. You may need to modify the pin connected to a certain hardware component, or the time and angle required for the servo to open the door, or the password for entering the management system.
5. Build, upload the program, and test it.

## Usage

Under normal circumstances, the power indicator LED is always on, and the onboard blue LED is off, indicating that the ESP32 is in sleep mode.

When you need to open the door, touch the touch pin that is brought out, and the onboard blue LED will be slightly on, indicating that it wakes up.

Swipe the school card. If the detection is successful, the onboard blue LED of the ESP32 will be on, the servo will pull the door lock to open, and audio will be played to welcome the visitor.

When the ESP32 is awakened, press the 'D' button to enter the management system. After entering the password, press 'D' to confirm, 'C' to exit, '#'/'*' to clear the input. After entering the editing interface, press '#' to delete, 'A'/'B' to move the cursor, swipe the card directly to add a user, and press 'D' to save and exit. User data will be saved even after a restart, so there is no need to re-enter it every time.

## Development

This project was originally developed for the author's specific hardware, but later the author tried to move the hardware-related code to separate files to improve its portability.

1. **door.h**/**door.cpp**

	If you want to design different door opening methods for your door, please rewrite `door::open()` and `door::close()` according to hardwares you are using.

2. **lowpower.h**/**lowpower.cpp**

	If you are not using ESP32, please modify the relevant functions in this file to enable low power mode for the MCU you choose. If your MCU does not support low power mode, leave these functions empty.

3. **play.h**/**play.cpp**

	The `play::task(void*)` function will be executed in the second thread while opening the door. If you want to light up LED lights or perform other actions instead of just playing audio during door opening, you can modify it. If your MCU does not support multithreading, you can remove this function or call it in `door::open()`.

	**sound_data.h**

	It contains the audio data to be played. If you want to play other audio, convert your audio to WAV format, obtain its data using a hexadecimal editor, and replace the array in this file. Note that **play.cpp** also needs to be modified accordingly.

4. **statusinfo.h**/**statusinfo.cpp**

	Controls the LED to show the current working status of the system. You can rewrite the functions in it to suit your needs.

5. **manage.h**/**manage.cpp**

	User management system.

	If you want to use a different type of card reader, keypad or OLED screen, modify the content related to them in this file - this is a bit cumbersome because I didn't bother to decouple it from the hardware.

	If you are using a different model of MCU, you only need to modify the `nvsSave` and `nvsLoad` functions. These functions are used to write user data to reliable storage (such as flash) and load it.
