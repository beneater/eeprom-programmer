# Wiring reference

Copyright 2024, Geoffrey Trueman Falk (under MIT license)

## EEPROM connection

Install two sockets: a 24-pin socket for programming the AT28C16, and a 28-pin socket for the AT28C64 and AT28C256.

Only program one EEPROM at a time.

 We can use the same socket for both the AT28C64 and AT28C256. The wiring differences are minor (see **Note 1** below.)

The following applies to PDIP package. All have the same width of 600 mils (6 * 2.54 mm).

| EEPROM part | pin count | address bus line count | storage capacity (octets) |
|-------------|-----------|-------------------|-------------------|
| [AT28C16](/doc/AT28C16.pdf) | 24       | 11 | 2048 |
| [AT28C64](/doc/AT28C64.pdf) | 28      | 13  | 8192 |
| [AT28C256](/doc/AT28C256.pdf) | 28    | 15  | 32768 |

EEPROM pin label is the reference in the datasheet.

There are two Shift Registers (74HC595), SR1 and SR2. (In the schematic they are called U2 and U3)

D*n* are Arduino pins.

| EEPROM pin label | function | AT28C16 pin | AT28C64 pin | AT28C256 pin | wire to |
|--------|-------------|---------|--------------|--------------|---------------|
| A0     | address bus | 8 | 10 | 10 | SR1 15 | 
| A1   | address bus | 7 | 9 | 9 | SR1 1 | 
| A2   | address bus | 6 | 8 | 8 | SR1 2 | 
| A3    | address bus | 5 | 7 | 7| SR1 3 | 
| A4   | address bus | 4 | 6 | 6 | SR1 4 | 
| A5    | address bus | 3 | 5 | 5 | SR1 5 | 
| A6    | address bus | 2 | 4 | 4 | SR1 6 | 
| A7    | address bus | 1 | 3 | 3 | SR1 7 | 
| A8    | address bus | 23 | 25 | 25 | SR2 15  | 
| A9    | address bus | 22 | 24 | 24 | SR2 1 | 
| A10   | address bus | 19 | 21 | 21 | SR2 2 | 
| A11   | address bus | - | 23 | 23 | SR2 3 |
| A12   | address bus | - | 2 | 2 | SR2 4 |
| A13 | address bus | - | NC (**Note 2**) | 26 | SR2 5 |
| A14 | address bus | - | - | 1 | SR2 6 (**Note 1**) |
| CE_ | chip enable | 18 | 20 | 20 | GND | 
| OE_ | output enable | 20 | 22 | 22 | SR2 7|
| WE_ | write enable | 21 | 27 | 27| D13 |
| I/O0 | data bus | 9 | 11 | 11 | D5 | 
| I/O1 | data bus | 10 | 12 | 12 | D6 | 
| I/O2 | data bus | 11 | 13 | 13 | D7 | 
| I/O3 | data bus | 13 | 15 | 15 | D8 | 
| I/O4 | data bus | 14 | 16 | 16 | D9 | 
| I/O5 | data bus | 15 | 17 | 17 | D10 | 
| I/O6 | data bus | 16 | 18 | 18 | D11 | 
| I/O7 | data bus | 17 | 19 | 19 | D12 | 
| GND | power | 12 | 14 | 14 | GND | 
| Vcc | power | 24 | 28 | 28 |  +5V | 

**Note 1:** Our design shares a 28-pin socket for an AT28C64 or an AT28C256 to be programmed. On the AT28C64, pin 1 is an open-drain output (READY/BUSY_) that we do not use. On the AT28C256, pin 1 is an input (A14). In this situation, we need to prevent the possibility of a dead short when using the AT28C64. Although SR2 6 is normally low in its address range, it is prudent to place a 4.7K current-limiting resistor between SR2 pin 6 and the socket pin 1.

**Note 2:** Pin 26 of the shared socket is internally non-connected on the AT28C64. We connect it anyways, although it is recommended to leave it unconnected. 
