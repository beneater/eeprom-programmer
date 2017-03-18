/**
 * This sketch is specifically for programming the EEPROM used in the 8-bit
 * decimal display decoder described in https://youtu.be/dLh1n2dErzE
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
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

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

  byte data = 0;
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


/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void programmEEPROM() {
	byte digits[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b };
	byte minusSign = 0x01;

	int progressStep = 16;
	Serial.println("Programming EEPROM:");

	// Go over all inputs
	for (byte i = 0; i < 256; i++) {
		if (i % progressStep == 0) {
			Serial.print((i / 256) * 100);
      Serial.println("%");
		}

		// Non two's complement part
		{
			int d2 = (i / 100) % 10; // 2nd 7segment display (hundreds)
			int d3 = (i / 10) % 10; // 3rd 7segment display (tenths)
			writeEEPROM(i + 512 + 256, 0); // clear first 7segment display
			writeEEPROM(i + 512, (d2 == 0) ? 0 : digits[d2]); // write the digit or clear if 0
			writeEEPROM(i + 256, (d2 == 0 && d3 == 0) ? 0 : digits[d3]); // write the digit or clear if d3 and d2 are 0
			writeEEPROM(i, digits[i % 10]); // write the digit even 0
		}
		// two's complement
		{
			int value = i - 128; //real value as two's complement
			if (value >= 0) { // same as above
				int d2 = (value / 100) % 10; // 2nd 7segment display (hundreds)
				int d3 = (value / 10) % 10; // 3rd 7segment display (tenths)
				writeEEPROM(value + 1024 + 512 + 256, 0); // clear first 7segment display
				writeEEPROM(value + 1024 + 512, (d2 == 0) ? 0 : digits[d2]); // write the digit or clear if 0
				writeEEPROM(value + 1024 + 256, (d2 == 0 && d3 == 0) ? 0 : digits[d3]); // write the digit or clear if d3 and d2 are 0
				writeEEPROM(value + 1024, digits[i % 10]); // write the digit including 0
			} else {
				// Example
				// d1 d2 d3 d4
				//  -  1  2  3
				//     -  4  2
				//        -  7

				int d2 = (abs(value) / 100) % 10; //hundreds
				int d3 = (abs(value) / 10) % 10; //tenths
				int d4 = abs(value) % 10; //ones

				//first 7segment display
				writeEEPROM(i + 1024 + 512 + 265, d2 == 0 ? 0 : minusSign); //write a minus sign to the first 7segment display if the second 7segment display isn't clear
				
				//second 7segment display
				if (d2 == 0 && d3 == 0) { // if the minus sign is further right
					writeEEPROM(i + 1024 + 512, 0); // clear second 7segment display
				} else {
					writeEEPROM(i + 1024 + 512, d2 == 0 ? minusSign : digits[d2]);
				}

				// third 7segment display
				writeEEPROM(i + 1024 + 256, (d2 == 0 && d3 == 0) ? minusSign : digits[d3]); // write a minus sign if both the second and the third 7segment display are clear

				// last 7segment display
				writeEEPROM(i + 1024, digits[d4]); // always write the last digit (ones)

			}
		}
	}
	Serial.println("Finished");
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  programmEEPROM();

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}
