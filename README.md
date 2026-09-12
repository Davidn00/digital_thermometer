# Digital Thermometer

Embedded digital thermometer based on PIC18F4550.

## Features

- LM35 temperature sensing
- External ADC0801
- Interrupt-driven ADC acquisition
- 128-sample digital filtering
- Internal PIC ADC
- Battery monitoring
- 20x4 LCD
- 4-digit multiplexed 7-segment display
- Temperature alarms
- I2C communication
- 24LC256 historical data logging
- MCP4725 DAC
- Button-controlled user interface
- Finite state machine
- Proteus simulation

## Hardware

- PIC18F4550
- LM35
- ADC0801
- PCF8574
- 20x4 LCD
- 24LC256
- MCP4725
- 4-digit 7-segment display
- LEDs
- Push buttons

## Software

- MPLAB XC8 v4.00
- Visual Studio Code
- Proteus
- Git / GitHub

## Architecture

LM35
  ↓
ADC0801
  ↓
PIC18F4550
  ├── LCD
  ├── 7-segment display
  ├── Alarm LEDs
  ├── Battery ADC
  ├── 24LC256
  └── MCP4725

## Project stages

1. ADC0801 + LM35 + interrupts
2. Digital filtering
3. Battery monitoring
4. LCD interface
5. Statistics
6. Alarm system
7. 7-segment display
8. Multiplexing
9. I2C
10. EEPROM history
11. MCP4725
12. User buttons
13. State machine
14. System integration
15. Validation
16. Documentation
17. GitHub portfolio

## Status

Final integration and validation.

## Version

v1.0.0