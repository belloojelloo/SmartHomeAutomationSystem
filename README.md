# 🏠 Home Automation System using Dual Microcontrollers

This project is a smart home automation system built using two ATmega32 microcontrollers.

- The first MCU (called HMI_ECU) handles user input using an LCD and keypad.
- The second MCU (called CONTROL_ECU) controls the door, motor, buzzer, and other hardware.

The two MCUs communicate with each other using UART.

---

## 📁 Folder Structure

| Folder        | Description                                        |
|---------------|----------------------------------------------------|
| HMI_ECU/      | Code for the interface microcontroller (LCD, keypad) |
| CONTROL_ECU/  | Code for the control microcontroller (motor, EEPROM, PIR, buzzer) |

Each folder has its own main.c file because each runs on a separate microcontroller.

---

How to Run It

You’ll need:

- Two ATmega32 microcontrollers
- External EEPROM
- LCD + Keypad
- Motor (via H-bridge)
- PIR sensor
- Buzzer

Use Atmel Studio or AVR-GCC to compile each folder’s main.c separately, then upload one to each microcontroller.

Make sure to wire the UART pins between the two MCUs correctly:
- HMI_ECU TX → CONTROL_ECU RX
- HMI_ECU RX ← CONTROL_ECU TX

---

## ❓ FAQ

❓ Why are there two main.c files?

Because this project uses two separate microcontrollers. Each one has its own program and runs independently but they talk to each other using UART.

❓ Why are there two folders?

To keep the two systems organized. You compile and flash each folder’s main.c to a different chip.

---

## 👤 Author

Belal Ramadan – [GitHub Profile](https://github.com/belloojelloo)
