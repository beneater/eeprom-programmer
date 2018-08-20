


// Controll word components
#define HLT 0b10000000000000000000000  // Halt clock
#define MI  0b01000000000000000000000  // Memory address register in
#define RI  0b00100000000000000000000  // RAM data in
#define RO  0b00010000000000000000000  // RAM data out
#define IO  0b00001000000000000000000  // Instruction register out
#define II  0b00000100000000000000000  // Instruction register in
#define AI  0b00000010000000000000000  // A register in
#define AO  0b00000001000000000000000  // A register out
#define EO  0b00000000100000000000000  // ALU out
#define SU  0b00000000010000000000000  // ALU subtract
#define BI  0b00000000001000000000000  // B register in
#define BO  0b00000000000100000000000  //B register out
#define DI  0b00000000000010000000000  // Display register in
#define PCE 0b00000000000001000000000  // Program counter enable
#define PCO 0b00000000000000100000000  // Program counter out
#define J   0b00000000000000010000000  // Jump (program counter in)
#define FI  0b00000000000000001000000  // Flags in
#define BKI 0b00000000000000000100000  // BANK in
#define BKO 0b00000000000000000010000  // BANK out
#define CI  0b00000000000000000001000  // CALL registger in
#define CBO 0b00000000000000000000100  // CALL register BANK out
#define CPO 0b00000000000000000000010  // CALL register programm counter out
#define RSS 0b00000000000000000000001  // Reset programm substep

// states of the computer
byte RAM[256];               // content of RAM
byte LOCAL_RAM_ADDRESS = 0;  // current RAM address
byte BANK_ADDRESS = 0;       // current BANK address
byte RAM_OUTPUT = 0;         // output of current RAM address
byte BUS = 0;                // value of the bus
byte REG_A = 0;              // value in register A
byte REG_B = 0;              // value in register B
byte REG_SUM = 0;            // value of the addition/substraction
byte REG_INSTRUCTION = 0;    // 4 bit instruction + 4 bit value/address
byte REG_C = 0;              // CALL register, 4 bit BANK + 4 bit Programm Counter
byte OUT_DISPLAY = 0;        // value of the output on the Display
bool CLOCK = false;          // clock
bool CONTROLL_WORD[16];      // 16 bit controll word
byte PROGRAMM_COUNTER = 0;   // 4 bit
byte INSTRUCTION_CYCLE = 0;  // instruction cycle for this instruction
bool CARRY_FLAG = false;     // carry flag of last addition/substraction
bool ZERO_FLAG = false;       // zero flag of last addition/substraction
long CONTROLLWORD = 0;        // controllword
bool HALT_FLAG = false;

// Input of user
bool PROGRAMMING_MODE = false;
bool USB_MODE = false;
bool USB_PROGRAMMING_MODE = false;
bool USB_MANUAL_CLOCK = false;
bool USB_NUMBER_MODE = false;
int USB_CLOCK_SPEED = 0;
int CLOCK_SPEED = 100;
bool CLEAR_BEFOR_OUT = false;
bool SKIP_NEXT_CLEAR = false;
bool DISABLE_OUTPUT = false;

// PINS
#define PIN_OUTPUT_SET       2  // D02
#define PIN_OUTPUT_SHIFT     3  // D03
#define PIN_OUTPUT_SERIAL    4  // D04
#define PIN_INPUT_SERIAL     5  // D05
#define PIN_INPUT_ENABLE     6  // D06
#define PIN_INPUT_SHIFTMODE  7  // D07
#define PIN_PROGRAMMING_MODE 8  // D08
#define PIN_MANUAL_STEP      9  // D09
#define PIN_MANUAL_ENABLE    10 // D10
#define PIN_NUMBER_MODE      11 // D11
#define PIN_USB_MODE         12 // D12
#define PIN_CLOCK_SPEED      19 // A0




void NOP() //0000 0000
{
  // No operation => do nothing
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RSS;
  BUS = 0;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);   
}

void HALT() //0000 0001
{
  // Stops den Computer
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = HLT|RSS;
  BUS = 0;
  HALT_FLAG = true;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);  
}

void DSPA() //0000 0010
{
  // Output Register A to Display
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|DI|RSS;
  BUS = REG_A;
  OUT_DISPLAY = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false); 
}

void DSPB() //0000 0011
{
  // Output Register B to Display
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BO|DI|RSS;;
  BUS = REG_B;
  OUT_DISPLAY = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void DSPC() //0000 0100
{
  // Output Register C to Display
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CBO|CPO|DI|RSS;
  BUS = REG_C;
  OUT_DISPLAY = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPAB() //0000 0101
{
  // copy register A to register B
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|BI|RSS;
  BUS = REG_A;
  REG_B = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPAC() //0000 0110
{
  // copy register A to register C
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|CI|RSS;
  BUS = REG_A;
  REG_C = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPBA() //0000 0111
{
  // copy register B to register A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AI|BO|RSS;
  BUS = REG_B;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPBC() //0000 1000
{
  // copy register B to register C
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BO|CI|RSS;
  BUS = REG_B;
  REG_C = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPCA() //0000 1001
{
  // copy register C to register A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CBO|CPO|AI|RSS;
  BUS = REG_C;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CPCB() //0000 1010
{
  // copy register C to register B
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CBO|CPO|BI|RSS;
  BUS = REG_C;
  REG_B = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void ZEROB() //0000 1011
{
  //stores zero in register B
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BI|RSS;
  BUS = 0;
  REG_B = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void ZEROC() //0000 1100
{
  //stores zero in register C
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CI|RSS;
  BUS = 0;
  REG_C = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void ADDB() // 0000 1101
{
  // A+B stored in B
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|BI|FI|RSS;
  calcSum(true);
  BUS = REG_SUM;
  REG_B = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void SUBB() // 0000 1110
{
  // A-B stored in B
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|BI|FI|RSS;
  calcDiff(true);
  BUS = REG_SUM;
  REG_B = BUS;
  calcDiff(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void RET() //0000 1111
{
  // Return out of subroutine
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CBO|BKI;
  BUS = REG_C & 0xF0;
  BANK_ADDRESS = BUS >> 4;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);

  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CPO|J|RSS;
  BUS = REG_C & 0x0F;
  PROGRAMM_COUNTER = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void RETA() //0001 OPT
{
  // Return out of a subroutine OPT memory location is saved in A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);

  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|AI;
  BUS = RAM_OUTPUT;
  REG_A = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CBO|BKI;
  BUS = REG_C & 0xF0;
  BANK_ADDRESS = BUS >> 4;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);

  // Step 5
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = CPO|J|RSS;
  BUS = REG_C & 0x0F;
  PROGRAMM_COUNTER = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void LDA() //0010 OPT
{
  // Load from OPT-Memory Location to A Register
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);

  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|AI|RSS;
  BUS = RAM_OUTPUT;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void LDI() //0011 OPT
{
  // Load OPT emediatly into Register A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|AI|RSS;
  BUS = REG_INSTRUCTION & 0x0F;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void STA() //0100 OPT
{
  // Store Value of Register A in OPT memory location
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|RI|RSS;
  BUS = REG_A;
  RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS] = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);   
}

void SWBK() //0101 OPT
{
  // Switch to BANK OPT, Stores the OPT in the Bank Register
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|BKI|RSS;
  BUS = REG_INSTRUCTION & 0x0F;
  BANK_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void SWP() //0110 OPT
{
  // Swaps Register A with OPT location in memory
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|BI;
  BUS = RAM_OUTPUT;
  REG_B = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|RI;
  BUS = REG_A;
  RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS] = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 5
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BO|AI|RSS;
  BUS = REG_B;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void ADD() //0111 OPT
{
  // Load from memory, store in Register B, add Register B to Register A, store sum in Register A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|BI;
  BUS = RAM_OUTPUT;
  REG_B = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|AI|FI|RSS;
  calcSum(true);
  BUS = REG_SUM;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void SUB() //1000 OPT
{
  // Load from memory, store in Register B, substract Register B from Register A, store sum in Register A
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|MI;
  BUS = REG_INSTRUCTION & 0x0F;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|BI;
  BUS = RAM_OUTPUT;
  REG_B = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|AI|SU|FI|RSS;
  calcDiff(true);
  BUS = REG_SUM;
  REG_A = BUS;
  calcDiff(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void INC() //1001 OPT
{
  // Increment the Value of Register A by OPT
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|BI;
  BUS = REG_INSTRUCTION & 0x0F;
  REG_B = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
    
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|AI|FI|RSS;
  calcSum(true);
  BUS = REG_SUM;
  REG_A = BUS;
  calcSum(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void DECR() //1010 OPT
{
  // Decrement the Value of Register A by OPT
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|BI;
  BUS = REG_INSTRUCTION & 0x0F;
  REG_B = BUS;
  calcSum(false);
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  output();
  waitForNextClockTick(true);
  CONTROLLWORD = EO|AI|SU|FI|RSS;
  calcDiff(true);
  BUS = REG_SUM;
  REG_A = BUS;
  calcDiff(false);
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void JMP() //1011 OPT
{
  // Jump to Instruction = set Programmcounter to OPT
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|J|RSS;
  BUS = REG_INSTRUCTION & 0x0F;
  PROGRAMM_COUNTER = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void JC() //1100 OPT
{
  // Jump to Instruction if carry bit is set
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  if (CARRY_FLAG)
  {
    CONTROLLWORD = IO|J|RSS;
    BUS = REG_INSTRUCTION & 0x0F;
    PROGRAMM_COUNTER = BUS;
    INSTRUCTION_CYCLE = 0;
  }
  else
  {
    CONTROLLWORD = RSS;
    BUS = 0;
    INSTRUCTION_CYCLE = 0;
  }
  output();
  waitForNextClockTick(false);
}

void JZ() //1101 OPT
{
  // Jump to Instruction if zero bit is set
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  if (ZERO_FLAG)
  {
    CONTROLLWORD = IO|J|RSS;
    BUS = REG_INSTRUCTION & 0x0F;
    PROGRAMM_COUNTER = BUS;
    INSTRUCTION_CYCLE = 0;
  }
  else
  {
    CONTROLLWORD = RSS;
    BUS = 0;
    INSTRUCTION_CYCLE = 0;
  }
  output();
  waitForNextClockTick(false);
}

void CALLD() //1110 OPT
{
  // call direct: calls a subroutine anywhere in RAM
  // highest 4 bits of A are used as BANK, OPT is used as memory location
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BKO|PCO|CI;
  BUS = (BANK_ADDRESS << 4) | PROGRAMM_COUNTER;
  REG_C = BUS;
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = AO|BKI;
  BUS = REG_A;
  BANK_ADDRESS = REG_A >> 4;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|J|RSS;
  BUS = REG_INSTRUCTION & 0x0F;
  PROGRAMM_COUNTER = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}

void CALL() //1111 OPT
{
  // Starts a subrutine, current Programm Counter is stored in the CALL_REGISTOR
  // OPT overrides the BANK REGISTER and sets the Programm Counter to zero
  // Step 2
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = BKO|PCO|CI;
  BUS = (BANK_ADDRESS << 4) | PROGRAMM_COUNTER;
  REG_C = BUS;
  output();
  waitForNextClockTick(false);
  
  // Step 3
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = IO|BKI;
  BUS = REG_INSTRUCTION & 0x0F;
  BANK_ADDRESS = REG_A;
  RAM_OUTPUT = RAM[BANK_ADDRESS|LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 4
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = J|RSS;
  BUS = 0;
  PROGRAMM_COUNTER = BUS;
  INSTRUCTION_CYCLE = 0;
  output();
  waitForNextClockTick(false);
}


void calcSum(bool setFlags)
{
  // calculates A + B, Flags are set if setFlags is true
  REG_SUM = REG_A + REG_B;
  if (!setFlags) // if flags not set, then return here
    return;
  // calculate flags
  CARRY_FLAG = (REG_SUM < REG_A && REG_SUM < REG_B);
  ZERO_FLAG = (REG_SUM == 0);
}

void calcDiff(bool setFlags)
{
  // calculates A - B, Flags are set if setFlags is true
  REG_SUM = REG_A + (~REG_B + 1); // A plus double compliment of B
  if (!setFlags) // if flags not set, then return here
    return;
  // calculate flags
  CARRY_FLAG = (REG_SUM < REG_A && REG_SUM < REG_B);
  ZERO_FLAG = (REG_SUM == 0);
}

void executeNextCommand()
{
//HALT()  //0000 0001
//DSPA()  //0000 0010
//DSPB()  //0000 0011
//DSPC()  //0000 0100
//CPAB()  //0000 0101
//CPAC()  //0000 0110
//CPBA()  //0000 0111
//CPBC()  //0000 1000
//CPCA()  //0000 1001
//CPCB()  //0000 1010
//ZEROB() //0000 1011
//ZEROC() //0000 1100
//ADDB()  //0000 1101
//SUBB()  //0000 1110
//RET()   //0000 1111
//RETA()  //0001 OPT
//LDA()   //0010 OPT
//LDI()   //0011 OPT
//STA()   //0100 OPT
//SWBK()  //0101 OPT
//SWP()   //0110 OPT
//ADD()   //0111 OPT
//SUB()   //1000 OPT
//INC()   //1001 OPT
//DECR()  //1010 OPT
//JMP()   //1011 OPT
//JC()    //1100 OPT
//JZ()    //1101 OPT
//CALLD() //1110 OPT
//CALL()  //1111 OPT

  // Step 0: Programm counter out, memory in
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = PCO|MI;
  BUS = PROGRAMM_COUNTER;
  LOCAL_RAM_ADDRESS = BUS;
  RAM_OUTPUT = RAM[(BANK_ADDRESS << 4) | LOCAL_RAM_ADDRESS];
  output();
  waitForNextClockTick(false);
  
  // Step 1: ram out, instruction register in, programm counter enable => increment
  INSTRUCTION_CYCLE++;
  if (!USB_MODE)
    output();
  waitForNextClockTick(true);
  CONTROLLWORD = RO|II|PCE;
  BUS = RAM_OUTPUT;
  REG_INSTRUCTION = BUS;
  PROGRAMM_COUNTER++;
  PROGRAMM_COUNTER &= 0x0F; // clear first 4 bits, programm counter has only 4 bits
  output();
  waitForNextClockTick(false);
  
  // start command
  switch (REG_INSTRUCTION)
  {
    case 0b00000000:
      NOP();
      return;
    case 0b00000001:
      HALT();
      return;
    case 0b00000010:
      DSPA();
      return;
    case 0b00000011:
      DSPB();
      return;
    case 0b00000100:
      DSPC();
      return;
    case 0b00000101:
      CPAB();
      return;
    case 0b00000110:
      CPAC();
      return;
    case 0b00000111:
      CPBA();
      return;
    case 0b00001000:
      CPBC();
      return;
    case 0b00001001:
      CPCA();
      return;
    case 0b00001010:
      CPCB();
      return;
    case 0b00001011:
      ZEROB();
      return;
    case 0b00001100:
      ZEROC();
      return;
    case 0b00001101:
      ADDB();
      return;
    case 0b00001110:
      SUBB();
      return;
    case 0b00001111:
      RET();
      return;
  }
  switch (REG_INSTRUCTION >> 4)
  {
    case 0b00000001:
      RETA();
      return;
    case 0b00000010:
      LDA();
      return;
    case 0b00000011:
      LDI();
      return;
    case 0b00000100:
      STA();
      return;
    case 0b00000101:
      SWBK();
      return;
    case 0b00000110:
      SWP();
      return;
    case 0b00000111:
      ADD();
      return;
    case 0b00001000:
      SUB();
      return;
    case 0b00001001:
      INC();
      return;
    case 0b00001010:
      DECR();
      return;
    case 0b00001011:
      JMP();
      return;
    case 0b00001100:
      JC();
      return;
    case 0b00001101:
      JZ();
      return;
    case 0b00001110:
      CALLD();
      return;
    case 0b00001111:
      CALL();
      return;
  }
}

void clearTerminal()
{
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

void waitForNextClockTick(bool inverse)
{
  if (USB_MODE)
    usbWaitForNextClockTick(inverse);
  else
    serialWaitForNextClockTick(inverse);
}

void usbWaitForNextClockTick(bool inverse)
{
  while(HALT_FLAG) // loop here forever if HALT_FLAG is set
  {
    if (CLEAR_BEFOR_OUT)
      clearTerminal();
    if (SKIP_NEXT_CLEAR)
      SKIP_NEXT_CLEAR = false;
    else
    {
      Serial.println("###### computer halted ######");
      Serial.println("1) enter programming mode");
      Serial.println("2) dump memory");
      Serial.println("3) clear halt");
      Serial.println("4) output");;
      Serial.println("9) reset");
      Serial.println();
    }
    int userInput = readInt();
    switch (userInput)
    {
      case 1:
        input();
        return;
      case 2:
        SKIP_NEXT_CLEAR = CLEAR_BEFOR_OUT;
        dumpMemory();
        break;
      case 3:
        HALT_FLAG = false;
        return;
      case 4:
        output();
        break;
      case 9:
        reset();
        return;
      default:
        break;
    }
  }
  CLOCK = !inverse; // set CLOCK for output 
  if (inverse)
    return;
  if (USB_MANUAL_CLOCK)
  {
    Serial.println("Enter any key for next step.");
    while (Serial.available() == 0); // wait for serial input
  }
  else
    delay(CLOCK_SPEED);
}

void serialWaitForNextClockTick(bool inverse)
{
  while(HALT_FLAG) // loop here forever if HALT_FLAG is set
  {
    if (digitalRead(PIN_PROGRAMMING_MODE)) // programming mode detected
      input();
      return;
  } 
  
  bool MANUAL_CLOCK = digitalRead(PIN_MANUAL_STEP);
  if (MANUAL_CLOCK)
  {
    while (inverse ^ (!digitalRead(PIN_MANUAL_STEP)))
    { // wait for high signal (low inverse)
        if (digitalRead(PIN_PROGRAMMING_MODE)) // programming mode detected
          input();
    }
    delay(10); // delay for bouncing button
  }
  else
  {
    // read poti and adjust CLOCK_SPEED to read value
    long CLOCK_SPEED = analogRead(PIN_CLOCK_SPEED);
    CLOCK_SPEED <<= 2;
    delay(CLOCK_SPEED);
  }
  CLOCK = !inverse; // set CLOCK for output
}

void input()
{
  if (USB_MODE)
    usbInput();
  else
    serialInput();
}

void usbInput()
{
  byte address = 0;
  int value = 0;
  bool hexInputMode = true;
  bool binInputMode = false;
  bool intInputMode = false;
  
  while (true)
  {
    if (SKIP_NEXT_CLEAR)
      SKIP_NEXT_CLEAR = false;
    else
    {
      if (CLEAR_BEFOR_OUT)
        clearTerminal();
      Serial.println("###### PROGRAMMING MODE ########");
      Serial.println("1) enter memory value");
      Serial.println("2) show value at");
      Serial.println("3) dump memory");
      if (!hexInputMode)
        Serial.println("4) switch to HEX input mode");
      if (!binInputMode)
        Serial.println("5) switch to BIN input mode");
      if (!intInputMode)
        Serial.println("6) switch to DEC input mode");
      Serial.println("9) switch programming mode off");
      Serial.println("");
    }
    int userInput = readInt();
    switch (userInput)
    {
      case 1:
        Serial.print("Enter address");
        if (intInputMode)
        Serial.println(" (0...255)");
        if (hexInputMode)
        Serial.println(" (00...FF)");
        if (binInputMode)
        Serial.println(" (00000000...11111111)");
        if (intInputMode)
          address = readInt();
        if (hexInputMode)
          address = readHex();
        if (binInputMode)
          address = readBin();
        Serial.print("Enter value for address: ");
        Serial.print(address, HEX);
        if (intInputMode)
        Serial.println(" (0...255)");
        if (hexInputMode)
        Serial.println(" (00...FF)");
        if (binInputMode)
        Serial.println(" (00000000...11111111)");
        if (intInputMode)
          value = readInt();
        if (hexInputMode)
          value = readHex();
        if (binInputMode)
          value = readBin();
        RAM[address] = value;
        Serial.println();
        break;
      case 2:
        SKIP_NEXT_CLEAR = CLEAR_BEFOR_OUT;
        Serial.print("Enter address");
        if (intInputMode)
        Serial.println(" (0...255)");
        if (hexInputMode)
        Serial.println(" (00...FF)");
        if (binInputMode)
        Serial.println(" (00000000...11111111)");
        if (intInputMode)
          address = readInt();
        if (hexInputMode)
          address = readHex();
        if (binInputMode)
          address = readBin();
        if (RAM[address] < 15)
          Serial.print("0");
        Serial.println(RAM[address], HEX);
        Serial.println();
        break;
      case 3:
        SKIP_NEXT_CLEAR = CLEAR_BEFOR_OUT;
        dumpMemory();
        break;
      case 4:
        hexInputMode = true;
        binInputMode = false;
        intInputMode = false;
        break;
      case 5:
        hexInputMode = false;
        binInputMode = true;
        intInputMode = false;
        break;
      case 6:
        hexInputMode = false;
        binInputMode = false;
        intInputMode = true;
        break;  
      case 9:
        USB_PROGRAMMING_MODE = false;
        return;
      default:
        break;
    }
  }
}


void dumpMemory()
{
  Serial.println("####### MEMORY DUMP #########");
  Serial.println("         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  for (byte b=0; b<16; b++)
  {
    Serial.print("BANK ");
    Serial.print(b, HEX);
    Serial.print(": ");
    for (byte i=0; i<16; i++)
    {
      if (RAM[b*16+ i] < 16)
         Serial.print("0");
      Serial.print(RAM[b*16+ i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}

void serialInput()
{
  PROGRAMMING_MODE = digitalRead(PIN_INPUT_ENABLE);
  output();
  while (PROGRAMMING_MODE)
  {
    while (!digitalRead(PIN_INPUT_ENABLE)) // wait for userinput
    {
     if (!digitalRead(PIN_PROGRAMMING_MODE)) // user switchs programming mode off
     {
      PROGRAMMING_MODE = false;
      HALT_FLAG = false;
      return;
     }
    }
    delay(100);
    while (!digitalRead(PIN_INPUT_ENABLE))
    {} // wait for button release

    // setup for SerialRead
    digitalWrite(PIN_INPUT_SHIFTMODE, true);
    pinMode(PIN_INPUT_ENABLE, OUTPUT);
    
    // read address
    byte address = 0;
    for (byte i=0; i<8; i++)
    {
      digitalWrite(PIN_INPUT_ENABLE, true);
      address |= digitalRead(PIN_INPUT_SERIAL) >> 8-i;
      digitalWrite(PIN_INPUT_ENABLE, false);
    }
    // read value
    byte value = 0;
    for (byte i=0; i<8; i++)
    {
      digitalWrite(PIN_INPUT_ENABLE, true);
      value |= digitalRead(PIN_INPUT_SERIAL) >> 8-i;
      digitalWrite(PIN_INPUT_ENABLE, false);
    }
    // save value
    RAM[address] = value;
    BANK_ADDRESS = address >> 4;
    LOCAL_RAM_ADDRESS = address & 0x0F;
    output();
  }
}

void output()
{
  if (USB_MODE)
    usbOutput();
  else
    serialOutput();
}

bool isPresentInControllword(long element)
{
  long present = element & CONTROLLWORD;
  if (present == 0)
    return false;
  return true;
}

void printlnTwoCharHex(byte value)
{
  if (value < 16)
    Serial.print("0");
  Serial.println(value, HEX);  
}

void usbOutput()
{
  if (DISABLE_OUTPUT)
    return;
  if (SKIP_NEXT_CLEAR)
    SKIP_NEXT_CLEAR = false;
  else
  {
    if (CLEAR_BEFOR_OUT)
      clearTerminal();
    Serial.println("###### Ausgabe ######");
    Serial.print("CONTROLLWORD: ");
    if (isPresentInControllword(HLT))
      Serial.print("HLT ");  
    if (isPresentInControllword(MI))
      Serial.print("MI ");
    if (isPresentInControllword(RI))
      Serial.print("RI ");
    if (isPresentInControllword(RO))
      Serial.print("RO ");  
    if (isPresentInControllword(IO))
      Serial.print("IO ");
    if (isPresentInControllword(II))
      Serial.print("II ");
    if (isPresentInControllword(AI))
      Serial.print("AI ");  
    if (isPresentInControllword(AO))
      Serial.print("AO ");
    if (isPresentInControllword(EO))
      Serial.print("EO ");
    if (isPresentInControllword(SU))
      Serial.print("SU ");  
    if (isPresentInControllword(BI))
      Serial.print("BI ");
    if (isPresentInControllword(BO))
      Serial.print("BO ");
    if (isPresentInControllword(DI))
      Serial.print("DI ");  
    if (isPresentInControllword(PCE))
      Serial.print("PCE ");
    if (isPresentInControllword(PCO))
      Serial.print("PCO ");
    if (isPresentInControllword(J))
      Serial.print("J ");  
    if (isPresentInControllword(FI))
      Serial.print("FI ");
    if (isPresentInControllword(BKI))
      Serial.print("BKI ");
    if (isPresentInControllword(BKO))
      Serial.print("BKO ");
    if (isPresentInControllword(CI))
      Serial.print("CI ");  
    if (isPresentInControllword(CBO))
      Serial.print("CBO ");
    if (isPresentInControllword(CPO))
      Serial.print("CPO ");
    if (isPresentInControllword(RSS))
      Serial.print("RSS ");
    Serial.println();
  
    Serial.print("INSTRUCTION_CYCLE: ");
    Serial.println(INSTRUCTION_CYCLE, HEX);
    Serial.print("REG_INSTRUCTION:   ");
    Serial.println(REG_INSTRUCTION, HEX);
    Serial.print("PROGRAMM_COUNTER:  ");
    Serial.println(PROGRAMM_COUNTER, HEX);
    Serial.print("BANK ADDRESS:      ");
    Serial.println(BANK_ADDRESS, HEX);
    Serial.print("LOCAL_RAM_ADDRESS: ");
    Serial.println(LOCAL_RAM_ADDRESS, HEX);
    Serial.print("MEMORY VALUE:      ");
    printlnTwoCharHex(RAM_OUTPUT);
    Serial.print("CARRY FLAG: ");
    Serial.println(CARRY_FLAG, HEX);
    Serial.print("ZERO FLAG:  ");
    Serial.println(ZERO_FLAG, HEX);
    Serial.print("HALT FLAG:  ");
    Serial.println(HALT_FLAG, HEX);
    Serial.print("BUS:        ");
    printlnTwoCharHex(BUS);
    Serial.print("REG_A:      ");
    printlnTwoCharHex(REG_A);
    Serial.print("REG_B:      ");
    printlnTwoCharHex(REG_B);
    Serial.print("REG_SUM:    ");
    printlnTwoCharHex(REG_SUM);
    Serial.print("REG_CALL:   ");
    printlnTwoCharHex(REG_C);
    Serial.print("DISPLAY:    ");
    if (USB_NUMBER_MODE && ((OUT_DISPLAY >> 7)&0x01) == 0x01)
    {
      Serial.print("-");
      Serial.println((OUT_DISPLAY^0xFF)-0x01, DEC);
    }
    else
      Serial.println(OUT_DISPLAY, DEC);
    Serial.println();
  }
}

void serialOutput()
{
  bool output[110];
  bool disp[22];
  calcDisp(disp);
  for (byte i=0; i<22; i++)
  {
    output[i] = disp[i];
  }
  output[22] = CARRY_FLAG;
  output[23] = ZERO_FLAG;
  setValues(output, REG_SUM, 24, 0);
  setValues(output, REG_B, 32, 0);
  setValues(output, REG_A, 40, 0);
  setValues(output, BUS, 48, 0);
  setValues(output, RAM_OUTPUT, 56, 0);
  setValues(output, LOCAL_RAM_ADDRESS, 64, 4);
  setValues(output, BANK_ADDRESS, 68, 4);
  setValues(output, REG_INSTRUCTION, 72, 0);
  setValues(output, REG_C, 80, 0);
  setValues(output, INSTRUCTION_CYCLE, 88, 3);
  setValues(output, PROGRAMM_COUNTER, 91, 4);
  for (byte i=0; i<23; i++)
  {
    output[i+95] = (CONTROLLWORD >> i) & 0x00001;
  }
  output[118] = PROGRAMMING_MODE;
  output[119] = CLOCK;
  
}

void setValues(bool arrayToChange[], byte value, byte startIndex, byte ignoreUpper)
{
  for (byte i=0; i<8-ignoreUpper; i++)
  {
    arrayToChange[startIndex+i] = (value >> i) & 0x01;
  }
}

void calcDisp(bool disp[])
{ 
  bool NUMBER_MODE = digitalRead(PIN_NUMBER_MODE);
                                  // a,b,c,d,e,f,g
  bool seven_seg_digits[10][7] = { { 1,1,1,1,1,1,0 },  // = 0     --a--
                                   { 0,1,1,0,0,0,0 },  // = 1    |     |
                                   { 1,1,0,1,1,0,1 },  // = 2    f     b
                                   { 1,1,1,1,0,0,1 },  // = 3    |     |
                                   { 0,1,1,0,0,1,1 },  // = 4     --g--
                                   { 1,0,1,1,0,1,1 },  // = 5    |     |
                                   { 1,0,1,1,1,1,1 },  // = 6    e     c
                                   { 1,1,1,0,0,0,0 },  // = 7    |     |
                                   { 1,1,1,1,1,1,1 },  // = 8     --d--
                                   { 1,1,1,1,0,1,1 }   // = 9
                                 };
  byte value;
  if (NUMBER_MODE) 
  { // 2. compliment
    if (OUT_DISPLAY >> 7 == 1) // is negative
    {
      disp[0] = 1;
      value = (OUT_DISPLAY^0xFF) -1;
    }
    else
    {
      disp[0] = 0;
      value = OUT_DISPLAY;
    }
  }
  else
  { // unsigned
    disp[0] = 0;
    value = OUT_DISPLAY;
  }
  for (byte i=2; 0<=i; i--)
  { // convert digits to 7segment
    byte digit = value%10;
    for (byte j=0; j<7; j++)
    {
      disp[i*7+j+1] = seven_seg_digits[digit][j];
    }
    value /= 10;
  }
}

int charToHexValue(byte charValue)
{
  switch (charValue)
  {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
    case 'A':
      return 10;
    case 'b':
    case 'B':
      return 11;
    case 'c':
    case 'C':
      return 12;
    case 'd':
    case 'D':
      return 13;
    case 'e':
    case 'E':
      return 14;
    case 'f':
    case 'F':
      return 15;
    default:
      return 0;
  }
  
}

int readBin()
{ //  read integer from usb serial
  while (Serial.available() == 0); // wait for input
  int value = 0;
  byte charValue = 0;
  for (int i=0; i<8; i++)
  {
    if (Serial.available() > 0)
    {
      charValue = Serial.read(); // Read next bit
      if (charValue > 0)
        charValue = 1;
      value = (value<<1) | charValue;
      delay(1000); //needed so that next value will be registered
    }
    else
      return value;
  }
  while (Serial.available() > 0)
  {
    delay(10); //needed so that next value will be registered
    Serial.read(); // wait until input is clear
  }
  return value; //read int 
}

int readHex()
{ //  read integer from usb serial
  while (Serial.available() == 0); // wait for input
  byte charValue = Serial.read(); //read 1. HEX
  int value = charToHexValue(charValue)<<4;
  delay(1000); // needed that 2. HEX will be detected
  if (Serial.available() > 0)
    charValue = Serial.read(); //read 2. HEX
  else
    value >>= 4;
  value |= charToHexValue(charValue);
  while (Serial.available() > 0)
  {
    delay(10); //needed so that next value will be registered
    Serial.read(); // wait until input is clear
  }
  return value; //read int 
}

int readInt()
{ //  read integer from usb serial
  while (Serial.available() == 0); // wait for input
  int value = Serial.parseInt(); //read int 
  while (Serial.available() > 0)
  {
    delay(10); //needed so that next value will be registered
    Serial.read(); // wait until input is clear
  }
  return value;
}

void usbMenu()
{
  while(true)
  {
    if (SKIP_NEXT_CLEAR)
      SKIP_NEXT_CLEAR = false;
    else
    {
      if (CLEAR_BEFOR_OUT)
        clearTerminal();
      Serial.println("###### Main Menu ########");
      if (USB_PROGRAMMING_MODE)
        Serial.println("1) exit programming mode");
      else
        Serial.println("1) enter programming mode");  
      if (USB_MANUAL_CLOCK)
        Serial.println("2) disable manual clock");
      else
        Serial.println("2) enable manual clock");
      Serial.println("3) set clock speed");
      if (USB_NUMBER_MODE)
        Serial.println("4) switch to unsigned number mode");
      else
        Serial.println("4) switch to 2. compliment");
      Serial.println("5) dump memory");
      Serial.println("6) output signals");
      if (!USB_PROGRAMMING_MODE)
        Serial.println("7) start programm");
      if (CLEAR_BEFOR_OUT)
        Serial.println("8) disable clear before output");
      else
        Serial.println("8) enable clear before output");
      if (DISABLE_OUTPUT)
        Serial.println("9) enable output");
      else
        Serial.println("9) disable output");
      Serial.println("10) reset all");
      Serial.println();
    }

    bool mem_disable_output = DISABLE_OUTPUT;
    int userInput = readInt();
    switch (userInput)
    {
      case 1:
        USB_PROGRAMMING_MODE = !USB_PROGRAMMING_MODE;
        if (USB_PROGRAMMING_MODE)
          input();
        break;
      case 2:
        USB_MANUAL_CLOCK = !USB_MANUAL_CLOCK;
        break;
      case 3:
        Serial.println("Enter value for clockspeed in ms. (0...1023)");
        Serial.println();
        CLOCK_SPEED = readInt();
        break;
      case 4:
        USB_NUMBER_MODE = !USB_NUMBER_MODE;
        break;
      case 5:
        SKIP_NEXT_CLEAR = CLEAR_BEFOR_OUT;
        dumpMemory();
        break;
      case 6:
        SKIP_NEXT_CLEAR = CLEAR_BEFOR_OUT;
        output();
        DISABLE_OUTPUT = mem_disable_output;
        break;
      case 7:
        return;
      case 8:
        CLEAR_BEFOR_OUT = !CLEAR_BEFOR_OUT;
        break;
      case 9:
        DISABLE_OUTPUT = !DISABLE_OUTPUT;
        break;
      case 10:
        reset();
        break;
      default:
        break;
    }
  }
}

void reset()
{
  for (byte i=0; i<256; i++)
  {
    RAM[i] = 0;               // content of RAM  
  }
  LOCAL_RAM_ADDRESS = 0;  // current RAM address
  BANK_ADDRESS = 0;       // current BANK address
  RAM_OUTPUT = 0;         // output of current RAM address
  BUS = 0;                // value of the bus
  REG_A = 0;              // value in register A
  REG_B = 0;              // value in register B
  REG_SUM = 0;            // value of the addition/substraction
  REG_INSTRUCTION = 0;    // 4 bit instruction + 4 bit value/address
  REG_C = 0;              // CALL register, 4 bit BANK + 4 bit Programm Counter
  OUT_DISPLAY = 0;        // value of the output on the Display
  CLOCK = false;          // clock
  CONTROLL_WORD[16];      // 16 bit controll word
  PROGRAMM_COUNTER = 0;   // 4 bit
  INSTRUCTION_CYCLE = 0;  // instruction cycle for this instruction
  CARRY_FLAG = false;     // carry flag of last addition/substraction
  ZERO_FLAG = true;       // zero flag of last addition/substraction
  CONTROLLWORD = 0;        // controllword
  HALT_FLAG = false;

  // Input of user
  PROGRAMMING_MODE = false;
  USB_MODE = false;
  USB_PROGRAMMING_MODE = false;
  USB_MANUAL_CLOCK = false;
  USB_NUMBER_MODE = false;
  USB_CLOCK_SPEED = 0;
}


void setup() {
  // setup pins
  pinMode(PIN_OUTPUT_SET, OUTPUT);
  pinMode(PIN_OUTPUT_SHIFT, OUTPUT);
  pinMode(PIN_OUTPUT_SERIAL, OUTPUT);
  pinMode(PIN_INPUT_SERIAL, INPUT);
  pinMode(PIN_INPUT_ENABLE, INPUT);
  pinMode(PIN_INPUT_SHIFTMODE, OUTPUT);
  pinMode(PIN_PROGRAMMING_MODE, INPUT);
  pinMode(PIN_MANUAL_STEP, INPUT);
  pinMode(PIN_MANUAL_ENABLE, INPUT);
  pinMode(PIN_CLOCK_SPEED, INPUT);
  pinMode(PIN_USB_MODE, INPUT);
  
  USB_MODE = !digitalRead(PIN_USB_MODE);
  if (USB_MODE)
  {
    Serial.begin(9600);
    usbMenu();
  }
}

void loop() {
  if (digitalRead(PIN_PROGRAMMING_MODE))
    input(); // enter programming mode
  executeNextCommand();
}












