#ifndef PWM_H_
#define PWM_H_
#include "synth.h"
#include <avr/interrupt.h>
#define PWM_PIN 3



void Audio_init();
volatile short int update;
#endif /* PWM_H_ */