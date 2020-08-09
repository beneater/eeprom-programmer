/**
 *   This sketch is specifically for programming the EEPROM used in the 8-bit
 *   decimal display decoder described in https://youtu.be/dLh1n2dErzE
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13


/*
   Output the address bits and outputEnable signal using shift registers.
*/
void setAddress(int address, bool outputEnable) {
  address = address & 0x7FFF;

  // Shift the data out, via the shift register clock
  byte msb = (address >> 8) | (outputEnable ? 0x00 : 0x80);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, msb);

  byte lsb = address & 0xFF;
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, lsb);

  // Shift-register data is stored in the storage register.
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
   Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0x00;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
   Write a byte to the EEPROM at the specified address.
*/
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/**
 * Clear out the entire EEPROM
 */
void eraseEEPROM(int address, int length)
{
  char data[80];
  sprintf(data, "Erasing EEPROM, Start 0x%04x, Length 0x%04x", address, length);
  Serial.println(data);
  for (int offset = 0x0000; offset < length; offset++) {
    writeEEPROM(address + offset, 0xff);

    if (offset % 0x40 == 0x00) {
      Serial.print(".");
    }
  }
  Serial.println();
  Serial.println("Erasing EEPROM -- Done.");
  Serial.println();
}


/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents(int address) {
  for (int base = 0x0000; base < 0x0100; base += 0x10)
  {
    char buf[20];
    sprintf(buf, "0x%04x: ", address + base);
    Serial.print(buf);
    for (int offset = 0x00; offset < 0x10; offset += 1)
    {
      byte data = readEEPROM(address + base + offset);
      sprintf(buf, "%02x ", data);
      Serial.print(buf);

      if (offset == 7)
        Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}


byte digits[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47};
byte minusSign = 0x01;
byte blank = 0x00;

/*
   Using Indeximal's idea to remove the leading zeros, and use blank space instead.
*/
void programEEPROM()
{
  // loop over all possible inputs
  for (int i = 0x0000; i < 0x0100; i++)
  {
    if (i % 0x10 == 0x00)
    {
      Serial.print((i / 256.0) * 100.0);
      Serial.println("%");
    }

    // unsigned byte as decimal
    {
      int d2 = (i / 100) % 10; // 2nd 7segment display (hundreds)
      int d3 = (i / 10) % 10; // 3rd 7segment display (tenths)
      writeEEPROM(i + 0x0300, blank); // clear first 7segment display
      writeEEPROM(i + 0x0200, (d2 == 0) ? blank : digits[d2]); // write the digit or clear if 0
      writeEEPROM(i + 0x0100, (d2 == 0 && d3 == 0) ? blank : digits[d3]); // write the digit or clear if d3 and d2 are 0
      writeEEPROM(i + 0x0000, digits[i % 10]); // write the digit even 0
    }

    // Two's complement
    if (i < 0x80)
    {
      // positive numbers, same as above
      int d2 = (i / 100) % 10; // 2nd 7segment display (hundreds)
      int d3 = (i / 10) % 10; // 3rd 7segment display (tenths)
      writeEEPROM(i + 0x0700, blank); // clear first 7segment display
      writeEEPROM(i + 0x0600, (d2 == 0) ? blank : digits[d2]); // write the digit or clear if 0
      writeEEPROM(i + 0x0500, (d2 == 0 && d3 == 0) ? blank : digits[d3]); // write the digit or clear if d3 and d2 are 0
      writeEEPROM(i + 0x0400, digits[i % 10]); // write the digit including 0
    }
    else
    {
      // Example
      // d1 d2 d3 d4
      //  -  1  2  3
      //     -  4  2
      //        -  7

      int value = i - 0x0100; //real value as two's complement
      int d2 = (abs(value) / 100) % 10; //hundreds
      int d3 = (abs(value) / 10) % 10; //tenths
      int d4 = abs(value) % 10; //ones

      //first 7segment display
      writeEEPROM(i + 0x0700, d2 == 0 ? blank : minusSign); //write a minus sign to the first 7segment display if the second 7segment display isn't clear

      //second 7segment display
      if (d2 == 0 && d3 == 0) { // if the minus sign is further right
        writeEEPROM(i + 0x0600, blank); // clear second 7segment display
      } else {
        writeEEPROM(i + 0x0600, d2 == 0 ? minusSign : digits[d2]);
      }

      // third 7segment display
      writeEEPROM(i + 0x0500, (d2 == 0 && d3 == 0) ? minusSign : digits[d3]); // write a minus sign if both the second and the third 7segment display are clear

      // last 7segment display
      writeEEPROM(i + 0x0400, digits[d4]); // always write the last digit (ones)
    }
  }
}


void setup() {
  // put your setup code here, to run once: 
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);

  digitalWrite(SHIFT_LATCH, LOW);
  pinMode(SHIFT_LATCH, OUTPUT);

  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);

  Serial.begin(57600);

  // Clear and program the EEPROM
  // eraseEEPROM(0x0600, 0x0100);
  programEEPROM();
  
  
  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  for (int address = 0x0000; address < 0x0800; address += 0x0100)
    printContents(address);
}

void loop() {
  // put your main code here, to run repeatedly:
}

