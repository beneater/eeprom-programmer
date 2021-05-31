# eeprom-programmer-WITHOUT-eeprom
I have forked the original repository to make modifications so that I can see the input going to the EEPROM without having the EEPROM itself.
Helpful for simulation.

The .ino files can be used if any one has Arduino hardware lying and can get the code by uploading the program and seeing the serial output and there are also .pdsprj files which are proteus design simulation files which can simulate the entire code along with the hardware and any one with Proteus Simulation software can run them without any extra component and still get the EEPROM contents to be programmed.

However, I have given the serial dump also if anyone needs the original ROM contents, and the .ino files and .pdsprj files will help one to write their own microcode and get the ROM contents for simulation.

### Here are some screenshots of the properly formatted one as shown in Ben-Eater's video - 44:Conditional jump instructions (https://www.youtube.com/watch?v=Zg1NdPKoosU&list=PLowKtXNTBypGqImE405J2565dvjafglHU&index=44) : 

![1.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/1.png?raw=true)

![2.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/2.png?raw=true)

![3.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/3.png?raw=true)

![4.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/4.png?raw=true)

![5.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/5.png?raw=true)

![6.png](https://github.com/Subhankar2000/eeprom-programmer-WITHOUT-eeprom/blob/master/blob/6.png?raw=true)

```diff
+ completed
```

```diff
- CORRECTED output for Logisim
Programming EEPROM................ done
Reading EEPROM
000: 40 14 00 00 00 00 00 00   40 14 48 12 00 00 00 00   
010: 40 14 48 10 02 00 00 00   40 14 48 10 02 00 00 00   
020: 40 14 48 21 00 00 00 00   40 14 0a 00 00 00 00 00   
030: 40 14 08 00 00 00 00 00   40 14 00 00 00 00 00 00   
040: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
050: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
060: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
070: 40 14 01 00 00 00 00 00   40 14 80 00 00 00 00 00   
080: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
090: 04 08 00 20 81 00 00 00   04 08 00 20 c1 00 00 00   
0a0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
0b0: 04 08 02 00 00 00 00 00   04 08 00 00 00 00 00 00   
0c0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
0d0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
0e0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
0f0: 04 08 10 00 00 00 00 00   04 08 00 00 00 00 00 00   
100: 40 14 00 00 00 00 00 00   40 14 48 12 00 00 00 00   
110: 40 14 48 10 02 00 00 00   40 14 48 10 02 00 00 00   
120: 40 14 48 21 00 00 00 00   40 14 0a 00 00 00 00 00   
130: 40 14 08 00 00 00 00 00   40 14 08 00 00 00 00 00   
140: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
150: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
160: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
170: 40 14 01 00 00 00 00 00   40 14 80 00 00 00 00 00   
180: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
190: 04 08 00 20 81 00 00 00   04 08 00 20 c1 00 00 00   
1a0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
1b0: 04 08 02 00 00 00 00 00   04 08 02 00 00 00 00 00   
1c0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
1d0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
1e0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
1f0: 04 08 10 00 00 00 00 00   04 08 00 00 00 00 00 00   
200: 40 14 00 00 00 00 00 00   40 14 48 12 00 00 00 00   
210: 40 14 48 10 02 00 00 00   40 14 48 10 02 00 00 00   
220: 40 14 48 21 00 00 00 00   40 14 0a 00 00 00 00 00   
230: 40 14 08 00 00 00 00 00   40 14 00 00 00 00 00 00   
240: 40 14 08 00 00 00 00 00   40 14 00 00 00 00 00 00   
250: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
260: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
270: 40 14 01 00 00 00 00 00   40 14 80 00 00 00 00 00   
280: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
290: 04 08 00 20 81 00 00 00   04 08 00 20 c1 00 00 00   
2a0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
2b0: 04 08 02 00 00 00 00 00   04 08 00 00 00 00 00 00   
2c0: 04 08 02 00 00 00 00 00   04 08 00 00 00 00 00 00   
2d0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
2e0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
2f0: 04 08 10 00 00 00 00 00   04 08 00 00 00 00 00 00   
300: 40 14 00 00 00 00 00 00   40 14 48 12 00 00 00 00   
310: 40 14 48 10 02 00 00 00   40 14 48 10 02 00 00 00   
320: 40 14 48 21 00 00 00 00   40 14 0a 00 00 00 00 00   
330: 40 14 08 00 00 00 00 00   40 14 08 00 00 00 00 00   
340: 40 14 08 00 00 00 00 00   40 14 00 00 00 00 00 00   
350: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
360: 40 14 00 00 00 00 00 00   40 14 00 00 00 00 00 00   
370: 40 14 01 00 00 00 00 00   40 14 80 00 00 00 00 00   
380: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
390: 04 08 00 20 81 00 00 00   04 08 00 20 c1 00 00 00   
3a0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
3b0: 04 08 02 00 00 00 00 00   04 08 02 00 00 00 00 00   
3c0: 04 08 02 00 00 00 00 00   04 08 00 00 00 00 00 00   
3d0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
3e0: 04 08 00 00 00 00 00 00   04 08 00 00 00 00 00 00   
3f0: 04 08 10 00 00 00 00 00   04 08 00 00 00 00 00 00   
```