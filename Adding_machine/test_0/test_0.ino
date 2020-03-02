#define optoPin 10
#define motorPin 15
#define colorPin 16

//  1 wheel  0123456789#-
// 10 wheels 0123456789,.
//  2 wheels 0123456789E.
//  1 wheel  R+-KGMΔ €N C
//  1 wheel  =+-x÷↕%#ST◊*

const int lineNB = 2;

volatile int turnState = 0;
volatile int linesInBuf = lineNB;
uint8_t lineBuffer[15][lineNB];
volatile long dt = 0;

void setup() {
  pinMode(optoPin, INPUT);
  for (int i = 15; i < 32; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i = 0; i <= 14; i++) {
    for (int ln = 0; ln < lineNB; ln++) {
      lineBuffer[i][ln] = ln;
    }
  }
  attachInterrupt(digitalPinToInterrupt(optoPin), photoISR, CHANGE);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started printer!");
  digitalWrite(motorPin, HIGH);
  digitalWrite(colorPin, LOW);
  dt = millis();

}

void loop() {
  if (Serial.available() > 0) {
    byte b = Serial.read();
  }


}

void photoISR(void) {
  if (digitalRead(optoPin) == LOW) { //falling edge, opaque
    if (turnState == 14)turnState = 0;
    Serial.printf("INT : %d @ %d\r\n", turnState, millis() - dt);
    turnState++;
    dt = millis();
  
  } else {//rising edge, transparent
   if (turnState == 14) {
      if (linesInBuf > 1) {
        linesInBuf--;
      } else {
        digitalWrite(motorPin, LOW);
        Serial.println("Stopping");
      }
    }
  }


}
uint8_t getPin(uint8_t index) {//solenoid index to pin mapping
  if (index < 7)return index + 17;
  return 38 - index;
}

