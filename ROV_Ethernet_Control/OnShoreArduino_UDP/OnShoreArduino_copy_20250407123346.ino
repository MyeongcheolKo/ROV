#include <SPI.h>
#include <Ethernet.h> 
#include <EthernetUDP.h> 


#define JS_R_X_PIN A1 //translate left right 
#define JS_R_Y_PIN A0 //forward backward <-
#define JS_L_X_PIN A3 //rotate <-
#define JS_L_Y_PIN A2 //vertical up down <-

//joystick read values
int jsRX = 0; //translate left right
int jsRY = 0; //forward backward <-
int jsLX = 0; //rotate <-
int jsLY = 0; //vertical up down <-

//values after mapped from joystick to thruster output
int forward = 0;
int translate = 0;
int rotate = 0;
int vertical = 0;

//final thruster output values sent to onboard arduino
int outputL = 0;
int outputR = 0;

int rotateL = 0;
int rotateR = 0;

// this is the client 
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
int port = 8888;

EthernetUDP Udp;

IPAddress ip(192, 168, 0, 100);
IPAddress serverIP(192, 168, 0, 101); 

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 

int16_t dataToSend[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Ethernet.begin(mac, ip);

  Udp.begin(port);

}

void loop() {

  //read joystick values
  jsRY = analogRead(JS_R_Y_PIN);
  jsLX = analogRead(JS_L_X_PIN);
  jsRX = analogRead(JS_R_X_PIN);
  jsLY = analogRead(JS_L_Y_PIN);

  //map joystick values to servo/thruster values
  forward = map(jsRY, 250, 820, 1250 ,1750);
  translate = map(jsRX, 240, 835, 1750 ,1250);
  rotate = map(jsLX, 180, 820, 1660, 1340);
  vertical = map(jsLY, 230, 825, 1340, 1660); 

  //check deadzones
  checkDeadzone(forward);
  checkDeadzone(rotate); 
  checkDeadzone(vertical);
  checkDeadzone(translate);

  //get rotation values
  rotateL = 0;
  rotateR = 0;
  if (rotate != 1500){
    rotateL = rotate - 1500;
    rotateR = 1500 - rotate;
  }

  outputL = forward + rotateL;
  outputR = forward + rotateR;

  //send data 
  // Udp.beginPacket(serverIP, port);
  // Udp.write((byte*)&outputL, sizeof(outputL));
  // Udp.write((byte*)&outputR, sizeof(outputR));
  // Udp.write((byte*)&vertical, sizeof(vertical));
  // Udp.write((byte*)&translate, sizeof(translate));
  // Udp.endPacket();

  dataToSend[0] = outputL;
  dataToSend[1] = outputR;
  dataToSend[2] = vertical;
  dataToSend[3] = translate;
  Udp.beginPacket(serverIP, port);
  Udp.write((uint8_t*)dataToSend, sizeof(dataToSend));
  Udp.endPacket();

  // Serial.print("Sent outputL: ");
  // Serial.print(outputL);
  // Serial.print(" | Sent outputR: ");
  // Serial.print(outputR);
  Serial.print(" | Sent outputV: ");
  Serial.println(vertical);
  // Serial.print(" | Sent outputT: ");
  // Serial.println(translate);
  delay(50);
}

void checkDeadzone(int& thruster){
  if(thruster > 1420 && thruster < 1580){
    thruster = 1500;
  }
}
