#include <avr/io.h>
#include "synth.h"
#include "pwm.h"
#include "phase.h"


void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = (unsigned char)(31);
	/* Enable receiver and transmitter */
	//UCSR0A = 0;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}
int main(void)
{
	short int velocity_tab[128] =
	{
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 4, 4, 4, 
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	
	unsigned char msg,note,vel;
	int prechan, chan;
	prechan = 0;
	chan = 0;
	Audio_init();
	Synth_init(chan);
	USART_Init(31);
	
	while (1)
	{
		msg = USART_Receive();
		note = USART_Receive();
		vel = USART_Receive();
		if((msg >> 4) == 9)
		{
			chan = msg & 0x0F;
			if(chan != prechan)
			{
				Synth_init(chan);
			}
			prechan = chan;
			if(vel == 0)
			{
				NoteOff(phase[note]);
			}
			else
			{
				NoteOn(phase[note],velocity_tab[vel]);
			}
		}
		else if((msg >> 4) == 11)
		{
			if(note == 0x15)
			{
				setFreq(vel+1);
			}
			else if(note == 0x16)
			{
				setDepth(vel+1);
			}
			else
			{
				NoteOffAll();
				setDepth(0);
				setFreq(8000);
			}
		}
		else
		{
			NoteOffAll();
			setDepth(0);
			setFreq(8000);
		}
		
		
	}
}


