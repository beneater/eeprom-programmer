# Arduino EEPROM programmer

Copyright 2017 Ben Eater

This code and schematic are [MIT licensed](http://en.wikipedia.org/wiki/MIT_License).

## Circuit

This is a simple circuit for programming the 28C16, 28C64, 28C256, and similar parallel EEPROMs using an Arduino. Since the Arduino doesn’t have enough pins to directly control all of the address, data, and control lines of the EEPROM, two 74HC595 shift registers are used for the 11 address lines (15 for the 28C256) and the output enable control line.

![Schematic of EEPROM programmer](https://raw.githubusercontent.com/beneater/eeprom-programmer/master/schematic.png)


## What’s here?

There are four different Arduino sketches that correspond to several YouTube videos. A lot of the code is duplicated since each sketch built on the previous ones. But I’ve kept them separate to make it easier to find the exact code that goes with a particular video:

### 1. Basic programmer

The code in [`/eeprom-programmer`](/eeprom-programmer) is the basic programmer that programs a few bytes into the EEPROM and dumps the contents.

That software, along with the EEPROM programmer’s hardware are described in detail in the following video. This is a good place to start if you’re looking for the fastest way to make sense of this repo:
- [Build an Arduino EEPROM programmer](https://youtu.be/K88pgWhEb1M).

### 2. 8-bit decimal display

The code in [`/multiplexed-display`](/multiplexed-display) is for programming an EEPROM to be used to decode 8-bit values and drive a 4-digit 7-segment display. Check out this video for more:
- [Build an 8-bit decimal display for our 8-bit computer](https://youtu.be/dLh1n2dErzE).

### 3. 8-bit computer microcode

The code in [`/microcode-eeprom-programmer`](/microcode-eeprom-programmer) is for programming a pair of EEPROMs to serve as an instruction decoder for an 8-bit breadboard computer. You’ll probably want to watch the whole 8-bit computer playlist (see below) for this to really make sense, but the specific videos describing the code here are:
- [Reprogramming CPU microcode with an Arduino](https://youtu.be/JUVt_KYAp-I).
- [Adding more machine language instructions to the CPU](https://youtu.be/FCscQGBIL-Y).

### 4. 8-bit computer microcode with flags register

The code in [`/microcode-eeprom-with-flags`](/microcode-eeprom-with-flags) adds functionality for a flags register to the microcode above to support conditional instructions. Again, you’ll likely want more context from the full series of videos, but here’s the video describing the code:
- [Conditional jump instructions](https://youtu.be/Zg1NdPKoosU).

## More information

This EEPROM programmer was designed as part of a larger project to build an 8-bit computer from scratch. There’s a much larger [series of videos about this project](https://www.youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU) on YouTube as well. In all likelihood, if this repo interests you, you want to binge that whole playlist.
