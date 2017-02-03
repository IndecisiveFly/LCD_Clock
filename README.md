#LCD Clock for Raspberry Pi using WiringPi Library

## To Run

 /$gpio load i2c
 /$gpio clock 7 1000
 /$sudo ./clock

## Wiring
 * 5v to buttons, output of buttons to resistors and into appropriate pin number
 * 5v to LCD 9th pin from left
 * ground to LCD, 8th pin from left
 * SDA to LCD, 7th pin from left
 * SCL to LCD, 6th pin from left
 
 * If backlight is wanted:
 * 3.3v to LCD, 13th pin from left (1st pin from right)
 * ground to LCD, 10th pin from left

 refer to wiringpi.com/pins/ for more reference
 
## Usage
 * mode button toggles between timer and clock
 * set button to adjust clock. Press mode to change between hour and min adjustments
 * in timer mode, select to start/stop and set to reset


