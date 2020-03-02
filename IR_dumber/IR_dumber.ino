
struct IRCode {
  unsigned long code;
  char c;
  char* buttonName;
};
IRCode cmds[] = {
  {0xFF48B7, '0', "Button0"},
  {0xFF906F, '1', "Button1"},
  {0xFFB847, '2', "Button2"},
  {0xFFF807, '3', "Button3"},
  {0xFFB04F, '4', "Button4"},
  {0xFF9867, '5', "Button5"},
  {0xFFD827, '6', "Button6"},
  {0xFF8877, '7', "Button7"},
  {0xFFA857, '8', "Button8"},
  {0xFFE817, '9', "Button9"},
  {0xFF50AF, 'l', "Vol-"},
  {0xFF7887, 'r', "Vol+"},
  {0xFF40BF, 'b', "CH-"},
  {0xFFA05F, 't', "CH+"},
  {0xFF02FD, 'c', "FULL"},
  {0xFF32CD, 'a', "Record"},
  {0xFF30CF, 'f', "TimeShift"},
  {0xFFB24D, 'p', "PWR"},
  {0xFF2AD5, 's', "SRC"},
  {0xFF6897, 'm', "Mute"},
  {0xFF38C7, 'l', "Recall"}
};
#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    bool found = false;
    for (int i = 0; i < 21; i++) {
      if (results.value == cmds[i].code) {
        Serial.print(cmds[i].c);
//        Serial.println(cmds[i].buttonName);
        found=true;
      }
    }
/*    if(!found && results.value !=0xFFFFFFFF){
      Serial.println(results.value,HEX);
    }*/
    irrecv.resume(); // Receive the next value
  }
  delay(10);
}
