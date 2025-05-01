#define JS_R_X_PIN A1 //transform left right 
#define JS_R_Y_PIN A0 //forward backward <-
#define JS_L_X_PIN A3 //rotate <-
#define JS_L_Y_PIN A2 //vertical up down <-

//joystick read values
int jsRX = 0; //transform left right
int jsRY = 0; //forward backward <-
int jsLX = 0; //rotate <-
int jsLY = 0; //vertical up down <-
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  jsRY = analogRead(JS_R_Y_PIN);
  jsLX = analogRead(JS_L_X_PIN);
  jsRX = analogRead(JS_R_X_PIN);
  jsLY = analogRead(JS_L_Y_PIN);
  int forward = map(jsRY, 250, 820, 1250 ,1750);
  // translate = map(jsRX, 240, 835, 1750 ,1250);
  int rotate = map(jsLX, 180, 820, 1660, 1340);
  int vertical = map(jsLY, 230, 825, 1340, 1660); 
  Serial.print("jsRY: ");
  Serial.print(forward);
  // Serial.print(" | jsRX: ");
  // Serial.print(jsRX);
  Serial.print(" | jsLX: ");
  Serial.print(rotate);
  Serial.print(" | jsLY: ");
  Serial.println(vertical);
}
