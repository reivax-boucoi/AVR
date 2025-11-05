#define F_SAMPLE_US 5
unsigned long t=0;
void setup(){
	Serial.begin(115200);
	 while (!Serial);
    
}

void loop(){

		Serial.println(analogRead(A0));
     delay(5);
}
