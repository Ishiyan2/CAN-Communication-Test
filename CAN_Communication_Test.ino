//MCP2515
#include <SPI.h>
#include <mcp_can.h>
#define CAN0_INT 2                                                    // Set INT to pin 2
MCP_CAN CAN0(10);                                                     // Set CS to pin 10


int rev = 0;
unsigned long timeA = 0;
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];                                               // Array to store serial string
byte data1[8] = {0x02, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00};     // Request ENGINE Speed

void setup(){
  Serial.begin(9600);
  delay(1000);
  pinMode(CAN0_INT, INPUT);                        // Configuring pin for /INT input
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("!!!!!!!!!!!!!!!!Conection done!!!!!!!!!!!!!!!!!");
  }
  CAN0.init_Mask(0,0,0x1F800000);                  // Init first mask
  CAN0.init_Mask(1,0,0x1F800000);                  // Init second mask
  for ( int i=0 ; i < 6 ; i++ ) {
    CAN0.init_Filt(i,0,0x1FA00000);                // Init filters
  }
  CAN0.setMode(MCP_NORMAL);                        // Set operation mode to normal so the MCP2515 sends acks to received data.
}

void loop() {
  while(millis() < timeA + 1000) { }
  timeA=millis();
  byte sndStat = CAN0.sendMsgBuf(0x7E0, 0, 8, data1);
  while (millis() <= timeA+50 ) {
    if(!digitalRead(CAN0_INT)) {
      CAN0.readMsgBuf(&rxId, &len, rxBuf);   
      if ( rxId==0x7E8 && rxBuf[2]==0x0C ) {
        rev=int((rxBuf[3]*256+rxBuf[4])/4);
      }
    }
  }
  Serial.print("Engine Speed:  ");
  Serial.println(rev);
}
