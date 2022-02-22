# AQM - Air Quality Monitor

![Slightly blurry photo of the gadget](https://raw.githubusercontent.com/n-ovesen/co2/master/static/pic.jpg)

## What/why?

Basically, this is a little thing I whacked together over a weekend, to win a discussion at the office about the HVAC not being adjusted properly.

So this little gadget spits out CO2 and TVOC on a little OLED screen, together with arrows showing the current trend compared to a running average over 10 minutes (5sec polling interval * 120 samples), together with a smiley that shows if it is over, near or well below treshold values I quickly snatched from google(this was to win an argument, not conduct science after all).

I have also soldered on a SD-card breakout, but I'm doing something stupid with that, so I'll probably(*read: never*) update the repo with that functionality later.

This is the whole platformio project, so should be a breeze for you to upload it with that, I'm sure you can wrestle a bit with the arduino IDE to get it done as well!

## Bill of materials

- Adafruit Feather M0
  (you can do wifi functionality and stuff with that too, but I have no wifi to use at the office anyways)
- Adafruit FeatherWing Doubler
- Adafruit FeatherWing OLED - 128x32 OLED
- 3.7V LiPo Cell
- IAQ Core C

## Assembly

1. Solder on headers to feather-bits
2. Solder leads to pads on IAQ
3. Solder leads to SDA, SCL, 3v3 and GND on the feather doubler. Disregard pullups, as there are some on the OLED, on the same i2c bus (*and the not science part*)
4. Whack together in the most sensible way you can think of.

## Usage:

1. Power up
2. Wait for sensor to do it's thing
3. Read data on screen, or use whatever serial thing to log to computer.
4. Win argument and get the HVAC tuned up.
