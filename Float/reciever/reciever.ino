#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "00011";

struct data{
  char name[20];
  long time;
  double waterDepth;
};

data recievedData[50];
int recievedIndex = 0;
bool recieved = false;
bool startMission = false;

void setup() {
  Serial.begin(9600);
  radio.begin();
}


void loop() {
  if (startMission){
    if (!recieved){
      radio.openReadingPipe(0, address);
      radio.startListening();
    }
  
    if (radio.available()) {
      radio.read(&recievedData[recievedIndex], sizeof(recievedData[0]));
      // Serial.print("Received : ");
      // Serial.println(recievedIndex);
      recievedIndex++;
      if (!recieved){
        recieved = true;
      }
    } 
    else if(recieved){
      Serial.println("Name,Time,Depth");
      for (int i = 0; i < recievedIndex; i++){
        Serial.print(recievedData[i].name);
        Serial.print(",");
        Serial.print(recievedData[i].time);
        Serial.print(",");
        Serial.println(recievedData[i].waterDepth);
      }
      Serial.println("Data received");
      recieved = false;
      startMission = false;
    }
    delay(100); // need to match with transmitter
  }
  else{
    waitToStart(); //wait for command to start mission
  }
}

void waitToStart() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's' || command == 't') {
      radio.stopListening();
      delay(200);
      radio.openWritingPipe(address);
      
      if(radio.write(&command, sizeof(command))){
        startMission = true;
        // Serial.print("Successully sent: ");
        // Serial.println(command);
      }
      // else{
      //   Serial.print("Unsuccessully snet: ");
      //   Serial.println(command);
      // }
    }

  }
}