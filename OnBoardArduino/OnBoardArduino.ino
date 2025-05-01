#include <SPI.h>
#include <Ethernet.h> 
#include <Servo.h>
#include <EthernetUdp.h>

#define thrusterPinR 5 //left - forward-backward
#define thrusterPinL 6 //right - forward-backward
#define thrusterPinT 8 //transform
#define thrusterPinV 7 //vertical(up-down)

Servo thrusterL; 
Servo thrusterR;
Servo thrusterV;
Servo thrusterT;

//thruster output values
int outputL = 0;
int outputR = 0;
int outputV = 0;
int outputT = 0;


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };  
IPAddress ip(192, 168, 0, 101);
EthernetUDP Udp;
int port = 8888;


int forward = 0;
int transform = 0;
int rotate = 0;
int vertical = 0;

int16_t receivedData[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  Udp.begin(port);

  thrusterL.attach(thrusterPinR);
  thrusterR.attach(thrusterPinL);
  thrusterV.attach(thrusterPinV);
  thrusterT.attach(thrusterPinT);


  thrusterL.writeMicroseconds(1500);
  thrusterR.writeMicroseconds(1500);
  thrusterV.writeMicroseconds(1500);
  thrusterT.writeMicroseconds(1500);


}

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize == sizeof(receivedData)) {
    Udp.read((uint8_t*)receivedData, sizeof(receivedData));

    outputL = receivedData[0];
    outputR = receivedData[1];
    outputV = receivedData[2];
    outputT = receivedData[3];

    Serial.print("Received outputL: ");
    Serial.print(outputL);
    Serial.print(" | Received outputR: ");
    Serial.print(outputR);
    // Serial.print(" | Received outputV: ");
    // Serial.print(outputV);
    // Serial.print(" | Received outputT: ");
    // Serial.println(outputT);
    Serial.print(" | Received outputV: ");
    Serial.println(outputV);
  } 
  else {
    Udp.flush();
  }
//   if(packetSize)
//   {
//     // Udp.read((byte*)&outputL, sizeof(outputL));
//     // Udp.read((byte*)&outputR, sizeof(outputR));
//     // Udp.read((byte*)&outputV, sizeof(outputV));
//     // Udp.read((byte*)&outputT, sizeof(outputT));


//     Serial.print("Received outputL: ");
//     Serial.print(outputL);
//     Serial.print(" | Received outputR: ");
//     Serial.print(outputR);
//     Serial.print(" | Received outputV: ");
//     Serial.print(outputV);
//     Serial.print(" | Received outputT: ");
//     Serial.println(outputT);
//   }
//   else{
//     Serial.println("No Packed Recieved");
//   }
  thrusterL.writeMicroseconds(outputL);
  thrusterR.writeMicroseconds(outputR);
  thrusterV.writeMicroseconds(outputV);
  thrusterT.writeMicroseconds(outputT);

}

