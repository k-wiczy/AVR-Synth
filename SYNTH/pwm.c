#include "pwm.h"






void Audio_init()
{
	cli();
	//PWM TIMER
	
	DDRB = (1 << 3);
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A = (1<<COM2A1) | (1<<COM2B1) | (1<<WGM20);
	TCCR2B = (1<<CS20);
	OCR2A = 255;
	
	//SAMPLING TIMER
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;

	TCCR1B |= (1 << CS10) | (1 << WGM12); //clk divider (1/1) -> f = 16000000 / 2000 = 8000 = fPWM
	OCR1A = 2000; //as above
	TIMSK1 |= (1 << OCIE1A); //interrupt mode flag
	update = 0;
	sei();
} 

ISR(TIMER1_COMPA_vect)
{
	
	OCR2A = get_OSC_sample();
}

