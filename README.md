# Arduino EEPROM programmer

Copyright 2017 Ben Eater

This code and schematic are [MIT licensed](http://en.wikipedia.org/wiki/MIT_License).

## Circuit

This is a simple circuit for programming the 28C16, 28C64, 28C256, and similar parallel EEPROMs using an Arduino. Since the Arduino doesn't have enough pins to directly control all of the address, data, and control lines of the EEPROM, two 74HC595 shift registers are used for the 11 address lines (15 for the 28C256) and the output enable control line.

![Schematic of EEPROM programmer](https://raw.githubusercontent.com/beneater/eeprom-programmer/master/schematic.png)

## More information

For much more, check out the [complete video](https://www.youtube.com/watch?v=K88pgWhEb1M) describing the hardware and software for programmer in detail.

This EEPROM programmer was designed as part of a larger project to build an 8-bit computer from scratch. There's a whole [series of videos about this project](https://www.youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU) on YouTube as well.
