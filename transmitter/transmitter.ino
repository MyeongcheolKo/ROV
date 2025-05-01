#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PID_v1.h>

const int pressurePin = A0;
const int pinDir = 2;
const int pinPwm = 3;
//motor driver shield setup: A to power, B to ground of the linear actuator
//so HIGH is extend, LOW is retract

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00011";

float total = 0;
float baselineVoltage;
float voltage;
int sensorValue;
float pressure;

double desiredDepth = 2.5; //desired depth = 2.5
double waterDepth = 0.0;
double output = 0.0;
const long PIDperiod = 1000; // need to be > 750
long lastSampleTime = 0.0;

double kp = 0;
double ki = 0;
double kd = 0;

PID myPID(&waterDepth, &output, &desiredDepth, kp, ki, kd, DIRECT); 

struct data{
  char name[20];
  long time;
  double waterDepth;
};

const long dataPeriod = 5000;
long lastStoredTime = 0.0;

data dataList[50]; // max data recording time span is 50*5 = 250s = 4 min max water time 
/*
sram is 2kb = 2000 bytes
one data struct = 20+4+4=28 bytes
not safe to use all 2000
say use only 1500
1500 / 28 = 53
be safe use 50
*/
int storedIndex = 0;

int atDesiredDepth = 0;

float alpha = 0.3;
float smoothedValue = 0.0;

bool start = false;
bool test = false;

void setup() {
  Serial.begin(9600);

  //set up transmitter 
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening(); //start as a reciever, waiting for command to start mission

  //calculate baseline voltage
  for(int i = 0; i < 50; i++){
    total += analogRead(pressurePin);
  }
  total /= 50;
  baselineVoltage = total * (5.0 / 1023.0);
  // Serial.print("Baseline voltage: ");
  // Serial.println(baselineVoltage);

  //set up PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1000); // check later

  //set up linear actuator 
  pinMode(pinPwm, OUTPUT);
  pinMode(pinDir, OUTPUT);

  myPID.SetOutputLimits(-255, 255);
}

void loop() {
  if(start){
    Serial.println("Mission started");

    readDepth();
    
    // PID control depth
    checkForSerialCommands(); //for testing

    

    long now = millis();
    if (now > lastSampleTime + PIDperiod && atDesiredDepth < 10) {
      lastSampleTime = now;
      myPID.Compute();

      int pwm = abs(output);
      if(pwm < 60){
        pwm = 60;
      }
      else{
        pwm = output;
      }

      if (output > 0){ // desired > currentDepth, need to go down, pull air in, so retract
        digitalWrite(pinDir, LOW);
        analogWrite(pinPwm, pwm); 
      }
      else{ // desired - currentDepth < 0 => desired < currentDepth, under desired depth, so extend, float up
        digitalWrite(pinDir, HIGH);
        analogWrite(pinPwm, -pwm); 
      }

      Serial.print(waterDepth);
      Serial.print(", ");
      Serial.print(desiredDepth);
      Serial.print(", ");
      Serial.println(output);
    }

    //record data every 5s
    if (now > lastStoredTime + dataPeriod && storedIndex < 20){
      lastStoredTime = now;
      data newData;
      strncpy(newData.name, "Hammerhead ROV", sizeof(newData.name)); //need to replace name with company number
      newData.time = now;
      newData.waterDepth = waterDepth;
      dataList[storedIndex] = newData;
      storedIndex++; 
      
      if (waterDepth < 3.0 && waterDepth > 2.0){
        atDesiredDepth++;

        Serial.print("atDesiredDepth: ");
        Serial.println(atDesiredDepth);
      }
    }
    
    //float up when mission completed and send data to reciever
    if(atDesiredDepth == 10){
      radio.stopListening();
      delay(200);
      radio.openWritingPipe(address);

      digitalWrite(pinDir, HIGH);
      analogWrite(pinPwm, 255); 
      delay(3000); // wait for the float to flow to the surface
      for (int i = 0; i < storedIndex; i++){
        if(radio.write(&dataList[i], sizeof(dataList[i]))){
          Serial.print("Succesfully sent: ");
          Serial.println(i);
        }
        else{
          Serial.print("Unseccesfully Sent: ");
          Serial.println(i);
        }
        delay(100); //need to match for transmitter and reciever
      } 
      start = false;     
      atDesiredDepth = 0;
      storedIndex = 0;
      radio.startListening();
      radio.openReadingPipe(0, address);
      Serial.println("Mission Completed. Waiting for command to restart...");
    }
  }

  else if(test){
    readDepth();

    data newData;
    strncpy(newData.name, "Hammerhead ROV", sizeof(newData.name));
    newData.time = millis();
    newData.waterDepth = waterDepth;
    dataList[0] = newData;

    radio.openWritingPipe(address);
    radio.stopListening(); 
    radio.write(&dataList[0], sizeof(dataList[0]));
    test = false;
  }

  else{
    // radio.startListening();
    // radio.openReadingPipe(0, address);
    if (radio.available()) {
      radio.read(&command, sizeof(command));
      if(command == 's'){
        // Serial.println("start");
        start = true;
      }
      else if(command == 't'){
        // Serial.println("test");
        test = true;
      }
    }
  }
}


void checkForSerialCommands() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'd') {
      desiredDepth = Serial.parseFloat();
      Serial.print("Desired Depth=");
      Serial.println(desiredDepth);
    }
    if (command == 'k') {
      kp = Serial.parseFloat();
      ki = Serial.parseFloat();
      kd = Serial.parseFloat();
      myPID.SetTunings(kp, ki, kd);
      Serial.print("Set Constants kp=");
      Serial.print(kp);
      Serial.print(" ki=");
      Serial.print(ki);
      Serial.print(" kd=");
      Serial.println(kd);
    }
  }
}

void readDepth(){
  //read and calculate water depth 
    sensorValue = analogRead(pressurePin);
    // Serial.println(sensorValue);
    voltage = sensorValue * (5.0 / 1023.0);
    smoothedValue = alpha * voltage + (1 - alpha) * smoothedValue;
    pressure = (voltage - baselineVoltage) * (10.0 / (4.5 - baselineVoltage)); 
    if(pressure < 0) {
      pressure = 0;
    }
    waterDepth = pressure * 0.70324961490205;
}