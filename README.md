# Digital Thermometer

Embedded digital thermometer based on the PIC18F4550.

## Hardware

- PIC18F4550
- LM35 temperature sensor
- ADC0801 analog-to-digital converter
- 20x4 LCD
- Proteus simulation

## Software

- MPLAB XC8 V4.00
- Visual Studio Code
- Proteus

## Project stages

### Stage 1
ADC0801 + LM35 + interrupt-based conversion detection.

### Stage 2
Temperature calculation and validation.

### Stage 3
20x4 LCD interface.

### Stage 4
Temperature alarms.

### Stage 5
System integration and final validation.

## Architecture

LM35 → ADC0801 → PIC18F4550 → LCD