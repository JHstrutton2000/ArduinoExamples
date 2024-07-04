
#define MACADDRESS "a8:79:8d:b6:ab:5b"

#define AXIS_MAX_VALUE    1
#define AXIS_CENTER_VALUE 0x80

#define AXIS_CALC(value)  (uint8_t)(((value/AXIS_MAX_VALUE) * AXIS_CENTER_VALUE) + AXIS_CENTER_VALUE)

#define LEFT_DATA     0b1111111111111110 // L
#define DOWN_DATA     0b1111111111111101 // D
#define RIGHT_DATA    0b1111111111111011 // R
#define UP_DATA       0b1111111111110111 // U 
#define START_DATA    0b1111111111101111 // St
#define R3_DATA       0b1111111111011111 // R3
#define L3_DATA       0b1111111110111111 // L3
#define SELECT_DATA   0b1111111101111111 // Se
#define SQUARE_DATA   0b1111111011111111 // □
#define CROSS_DATA    0b1111110111111111 // X
#define CIRCLE_DATA   0b1111101111111111 // O
#define TRIANGLE_DATA 0b1111011111111111 // △
#define R1_DATA       0b1110111111111111 // R1
#define L1_DATA       0b1101111111111111 // L1
#define R2_DATA       0b1011111111111111 // R2
#define L2_DATA       0b0111111111111111 // L2

#define CONTROLLER_ADDRESS 0x01
#define POLLING_CMD        0x42

#define RX  27
#define TX  26
#define SCK 25

#define INT 4
#define DSR 5


#define RXBufferSize 8
#define HEADER_SIZE  3

#include <PS4Controller.h>


struct digitalInputs{
  uint8_t filler = 0xFF;
  uint8_t id = 0x41;
  uint8_t responseCode = 0x5A;
  uint16_t Buttons = 0xFFFF;
};

struct analogInputs{
  uint8_t filler = 0xFF;
  uint8_t id = 0x53;
  uint8_t ResponseCode = 0x5A;
  uint16_t Buttons = 0xFFFF;
  uint8_t RX_Axis = 0x80;
  uint8_t RY_Axis = 0x80;
  uint8_t LX_Axis = 0x80;
  uint8_t LY_Axis = 0x80;
};

struct analogInputs* analogData;

void applyButton(boolean PS4Button, uint16_t* Buttons, uint16_t value) {
  if(PS4Button){
    *Buttons &= value;
  }
  else{
    *Buttons |= ~value;
  }
}

void ParsePS4Controller() {
  applyButton(PS4.Left(),     &analogData->Buttons, LEFT_DATA);
  applyButton(PS4.Down(),     &analogData->Buttons, DOWN_DATA);
  applyButton(PS4.Right(),    &analogData->Buttons, RIGHT_DATA);
  applyButton(PS4.Up(),       &analogData->Buttons, UP_DATA);
  applyButton(PS4.Square(),   &analogData->Buttons, SQUARE_DATA);
  applyButton(PS4.Cross(),    &analogData->Buttons, CROSS_DATA);
  applyButton(PS4.Circle(),   &analogData->Buttons, CIRCLE_DATA);
  applyButton(PS4.Triangle(), &analogData->Buttons, TRIANGLE_DATA);
  applyButton(PS4.L1(),       &analogData->Buttons, L1_DATA);
  applyButton(PS4.L2(),       &analogData->Buttons, L2_DATA);
  applyButton(PS4.L3(),       &analogData->Buttons, L3_DATA);
  applyButton(PS4.R1(),       &analogData->Buttons, R1_DATA);
  applyButton(PS4.R2(),       &analogData->Buttons, R2_DATA);
  applyButton(PS4.R3(),       &analogData->Buttons, R3_DATA);
  applyButton(PS4.Share(),    &analogData->Buttons, START_DATA);
  applyButton(PS4.Options(),  &analogData->Buttons, SELECT_DATA);
  
  analogData->RX_Axis = AXIS_CALC(PS4.RStickX());
  analogData->RY_Axis = AXIS_CALC(PS4.RStickY()); 
  analogData->LX_Axis = AXIS_CALC(PS4.LStickX());
  analogData->LY_Axis = AXIS_CALC(PS4.LStickY());
}

uint8_t* rxBuff    = nullptr;
uint8_t* currentRX = nullptr;

void IRAM_ATTR clk_pulsed(){
  if(currentRX == nullptr || currentRX == nullptr){
    return;
  }

  int pinState = digitalRead(RX);
  if(currentRX == rxBuff + RXBufferSize){
    currentRX = rxBuff;

    memset(rxBuff, 0xFF, RXBufferSize);
  }

  *currentRX = (uint8_t)pinState;
  
  if(currentRX == rxBuff + RXBufferSize){
    parseRXBuff();
  }

  currentRX++;
}

void parseRXBuff(){
  if(*rxBuff == CONTROLLER_ADDRESS && *(rxBuff + 1) == POLLING_CMD){
    //sendCurrent
  }
}

void onConnect()
{
  rxBuff = (uint8_t*)malloc(RXBufferSize);
  currentRX = rxBuff;

  Serial.println("Controller connected!");
}

void onDisConnect()
{
  Serial.println("Controller disconnected!");    
}

void setup() 
{
  Serial.begin(115200);
  PS4.attach(ParsePS4Controller);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin(MACADDRESS);

  pinMode(SCK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SCK), clk_pulsed, RISING);
}

void loop() 
{

}