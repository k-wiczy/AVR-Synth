#ifndef SYNTH_H_
#define SYNTH_H_
#define MAX_VOICES 8

#define WFM_SINE 0x00
#define WFM_PULSE 0x01
#define WFM_TRIANGLE 0x02
#define WFM_SAW 0x03


void setDepth(int depth);
void setFreq(int freq);
typedef struct
{
	int phase;
	int current;
	int velocity;
	int depth;
} VOICE;
short int wave[256];

void Synth_init();
short int get_OSC_sample();
void NoteOn(int phase,int velocity);
void NoteOff(int phase);
void NoteOffAll();

void Buffer_Thread();


#endif /* SYNTH_H_ */