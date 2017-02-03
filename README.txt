README.txt for clock.c

Compile using make command

(pin numbers can be chnaged except for clock which has to be on pin 7)
gpio pin(wiringPi)    | Usage
0			MODE
1			SELECT
2			SET
3			CLOCK_INPUT
7			CLOCK


TO RUN:
$gpio load i2c 
make sure the i2c library is loaded first. follow the onscreen instructions to set up i2c if it is not setup.
$gpio clock 7 1000
#sets the gpio pin 7 clock frequency to 1000
$sudo ./clock


WIRING:
wire 5v to buttons, output of buttons to resistor and into apporiate pin number
5v to lcd 9th pin from left
ground to lcd 8th pin from left
SDA to lcd 7th pin from left
SCL to lcd 6th pin from left

if backlight is wanted:
3.3v to lcd 13th pin from left (1st pin from right)
ground to lcd 10th pin from left


USAGE:
mode button toggles between timer and clock
set button to adjust clock. press mode to change between hour and min adjustment
in timer mode, select to start/stop and set to reset
