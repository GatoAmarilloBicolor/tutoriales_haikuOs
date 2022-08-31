#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H


void PitchShift(float *data, int samples, int amount);
void MakeEcho(float *data, int samples, int amount);
void MakeLowPass(float *data, int samples, int amount);
		

#endif
