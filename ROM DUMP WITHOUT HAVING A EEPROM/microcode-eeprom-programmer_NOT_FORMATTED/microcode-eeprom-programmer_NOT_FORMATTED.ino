#define EEPROM_D0 5
#define EEPROM_D7 12

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

 char bufd[2];
char bufa[8];
int line_count = 0;
int oneTime = -1;
int addr = 0;
void writeEEPROM(int address, byte data) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    if (oneTime == -1) {
      sprintf(bufd, "%02x ", data&1);
      Serial.print(bufd);
    }
    oneTime++;
    data = data >> 1;
  }
  oneTime = -1;

  line_count++;
  if (line_count % 8 == 0)
    Serial.print("  ");
  if (line_count % 16 == 0) {
    Serial.println("");
    addr = addr + 16;
    //    Serial.print(addr,HEX);
    //    Serial.print(":  ");
    sprintf(bufa, "%03x: ", addr);
    Serial.print(bufa);
  }
}

void setup() {

  Serial.begin(57600);


  int zero = 0;
  char buff[3];
  sprintf(buff, "000: ", zero);
  Serial.print(buff);

  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
    writeEEPROM(address, data[address] >> 8);


  }

  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
    writeEEPROM(address + 128, data[address]);
  }

}


void loop() {


}
