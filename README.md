
# Embedded Systems RC Car Controls

## About
Bare metal C project on PIC18F4515 microcontroller programmed 
via PICkit 3 debugger. Implemented real-time RC car motor control 
by reading the PIC18F4515 datasheet and programming hardware 
registers directly using bit-level instructions.

## How It Works
- Potentiometer analog input read via ADC
- ADC values converted to control motor speed via PWM output
- Hardware interrupts used for real-time motor response
- All register configuration done manually at bit level

## Technical Problem Solved
Incorrect ADC readings caused motor speed to respond 
incorrectly. Diagnosed by reviewing ADC configuration 
registers in the PIC18F4515 datasheet — identified 
misconfigured acquisition time setting and corrected 
at the register level.

## Technologies
- C (Bare Metal — no libraries)
- PIC18F4515 Microcontroller (Microchip)
- PICkit 3 Programmer/Debugger
- ADC (Analog to Digital Conversion)
- PWM (Pulse Width Modulation)
- Hardware Interrupts
- Register-Level Bit Manipulation
