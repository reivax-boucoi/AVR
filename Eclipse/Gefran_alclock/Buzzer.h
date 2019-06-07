#ifndef BUZZER_H_
#define BUZZER_H_
#include <avr/io.h>

#define PP (1<<7)
#define PN (1<<6)
#define BPORT PORTD
#define BREG DDRD
#define BPSCL (1<<CS10)
class Buzzer {
public:
	Buzzer();
	void start(void);
	void stop(void);
	void toggle(void);
	void buzz_ISR(void);
	~Buzzer();
private :
	bool started;
};

#endif /* BUZZER_H_ */
