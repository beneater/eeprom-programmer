/**
 * This sketch programs the microcode EEPROMs for the 8-bit breadboard computer
 * See this video for more: https://youtu.be/JUVt_KYAp-I
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#define HLT 0b1000000000000000  // Halt clock
#define MI  0b0100000000000000  // Memory address register in
#define RI  0b0010000000000000  // RAM data in
#define RO  0b0001000000000000  // RAM data out
#define IO  0b0000100000000000  // Instruction register out
#define II  0b0000010000000000  // Instruction register in
#define AI  0b0000001000000000  // A register in
#define AO  0b0000000100000000  // A register out
#define EO  0b0000000010000000  // ALU out
#define SU  0b0000000001000000  // ALU subtract
#define BI  0b0000000000100000  // B register in
#define OI  0b0000000000010000  // Output register in
#define CE  0b0000000000001000  // Program counter enable
#define CO  0b0000000000000100  // Program counter out
#define J   0b0000000000000010  // Jump (program counter in)

uint16_t data[] = {
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  RO|AI,  0,         0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  RO|BI,  EO|AI,     0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  RO|BI,  EO|AI|SU,  0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  AO|RI,  0,         0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  0,      0,         0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   0,      0,         0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 0111
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1000
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1011
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1100
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  0,      0,         0, 0, 0,   // 1110 - OUT
  MI|CO,  RO|II|CE,  HLT,    0,      0,         0, 0, 0,   // 1111 - HLT
};


/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  address = address & 0x7FFF;
  
  byte msb = (address >> 8) | (outputEnable ? 0x00 : 0x80);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, msb);

  byte lsb = address & 0xFF;
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, lsb);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
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
 * Write a byte to the EEPROM at the specified address.
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
void eraseEEPROM()
{
  Serial.println("Erasing EEPROM");
  for (int address = 0x0000; address < 0x0800; address++) {
    writeEEPROM(address, 0xff);

    if (address % 0x40 == 0x00) {
      Serial.print(".");
    }
  }
  Serial.println("Erasing EEPROM -- Done.");
  Serial.println();
}


void programEEPROM(byte data[], int length)
{
  Serial.println("Programming EEPROM");
  for (int address = 0x0000; address < length; address++) {
    writeEEPROM(address, data[address]);    

    if (address % 0x40 == 0x00) {
      Serial.print(".");
    }
  }
  Serial.println(" Done.");
  Serial.println();
}


void programEEPROM(uint16_t data[], int offset, int length, bool highByte)
{
  Serial.println("Programming EEPROM");
  for (int address = 0x0000; address < length; address++){
    byte value = data[address] >> (highByte ? 8 : 0);
    writeEEPROM(address + offset, value);

    if (address % 0x40 == 0x00) {
      Serial.print(".");
    }
  }
  Serial.println(" Done.");
  Serial.println();
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents(int address) {
  for (int base = 0x0000; base < 0x0100; base += 0x10) {
    char buf[20];
    sprintf(buf, "0x%04x: e", address + base);
    Serial.print(buf);
    for (int offset = 0; offset < 0x10; offset += 0x01) {
      byte data = readEEPROM(address + base + offset);
      sprintf(buf, "%02x ", data);
      Serial.print(buf);

      if (offset == 0x07) {
        Serial.print(" ");
      }
    }

    Serial.println();
  }

  Serial.println();
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  // eraseEEPROM();

  // Program data bytes
  // Program the 8 high-order bits of microcode into the first 0x80 bytes of EEPROM
  programEEPROM(data, 0x00, sizeof data / sizeof data[0], true);

  // Program the 8 low-order bits of microcode into the second 0x80 bytes of EEPROM
  programEEPROM(data, 0x80, sizeof data / sizeof data[0], false);  

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents(0x0000);
}


void loop() {
  // put your main code here, to run repeatedly:

}

