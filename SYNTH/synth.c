#include "synth.h"
#include <avr/pgmspace.h>
#include "wavetable_8bit.h"

VOICE glos[MAX_VOICES];
short int ilosc_glosow;
volatile int depth = 0;
volatile int skok = 0;
volatile int freq = 0;
volatile int iteracja = 0;
volatile short int mode = 0;

void setDepth(int depthh)
{
	skok = depthh/7;
	//depth = 0;
}
void setFreq(int freqq)
{
	freq = freqq;
	//iteracja = 0;
}

void Synth_init(int chan)
{
	
	ilosc_glosow = 0;
	for(int i = 0; i < MAX_VOICES; i++)
	{
		glos[i].current = 0;
		glos[i].phase = 0;
		glos[i].depth = 0;
		glos[i].velocity = 1;
	}
	switch(chan)
	{
		case 0 :
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&SINE[i]);
			}
		break;
		case 1 :
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&PULSE[i]);
			}
		break;
		case 2 :
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&TRIANGLE[i]);
			}
		break;
		case 3 :
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&SAW[i]);
			}
		break;
		case 4 : 
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&CELLO[i]);
			}
		break;
		case 5:
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&XYZ[i]);
			}
			break;
		case 6:
		for(int i = 0; i < 256; i++)
		{
			wave[i] = pgm_read_byte_near(&ABC[i]);
		}
		break;
		case 7:
		for(int i = 0; i < 256; i++)
		{
			wave[i] = pgm_read_byte_near(&DEF[i]);
		}
		case 8:
		for(int i = 0; i < 256; i++)
		{
			wave[i] = pgm_read_byte_near(&PPP[i]);
		}
		break;
		default:
			for(int i = 0; i < 256; i++)
			{
				wave[i] = pgm_read_byte_near(&SINE[i]);
			}
		break;
	}
	
	
}

short int get_OSC_sample()
{
	short int out = 0;
	iteracja++;
	if(iteracja > (freq * 10))
	{
		if(mode == 0)
		{
			
			depth+=1;
			if(depth > skok) mode = 1;
		}
		if(mode == 1)
		{
			depth-=1;
			if(depth < (-1*skok)) mode = 2;
		}
		if(mode == 2)
		{
			depth+=1;
			if(depth == 0) mode = 0;
		}
		iteracja = 0;
	}		
	for(int i = 0; i < ilosc_glosow; i++)
	{
		glos[i].current += (glos[i].phase+depth);
		if(glos[i].current >= 25600) glos[i].current -= 25600;
		out += wave[(glos[i].current)/100];
	}
	out /= MAX_VOICES;
	return out;
}

void NoteOn(int phase,int velocity)
{
	if(ilosc_glosow < 8)
	{
		glos[ilosc_glosow].phase = phase;
		glos[ilosc_glosow].velocity = velocity;
		ilosc_glosow++;
	}
}
void NoteOffAll()
{
	ilosc_glosow = 0;
	for(int i = 0; i < MAX_VOICES; i++)
	{
		glos[i].current = 0;
		glos[i].phase = 0;
		glos[i].velocity = 1;
	}
}
void NoteOff(int phase)
{
	if(ilosc_glosow > 0)
	{
		
		VOICE tmp[MAX_VOICES];
		for(int i = 0; i < MAX_VOICES; i++)
		{
			tmp[i].current = 0;
			tmp[i].phase = 0;
		}
		int j = 0;
		for(int i = 0; i < ilosc_glosow; i++)
		{
			if(glos[i].phase != phase)
			{
				tmp[j] = glos[i];
				j++;
			}
			
		}
		
		for(int i = 0; i < MAX_VOICES; i++)
		{
			glos[i] = tmp[i];
			
		}
		ilosc_glosow--;
		
	}
	
}
