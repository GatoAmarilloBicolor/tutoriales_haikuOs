#include "SoundEffects.h"
#include "PitchShifter.h"

void PitchShift(float *data, int samples, int amount)
{
  if (amount < 10) amount = 10;
  float *data_out = new float[samples];
  smbPitchShift(amount / 700.0f, samples, 1024, 32, 44100, data, data_out);
  for (int n = 0; n < samples; n++) data[n] = data_out[n];
  delete [] data_out;
}
		
void MakeLowPass(float *data, int samples, int amount)
{
  float n3 = 0;
  float n4 = 0;
  float cut_lp = 0.05 + amount / 5000.0f;
  float res_lp = 0.5f; // resonance factor - you can change this value
  float fb_lp = res_lp + res_lp / (1 - cut_lp);
  
  for (int n = 0; n < samples; n++)
  {
    n3 = n3 + cut_lp * (data[n] - n3 + fb_lp * (n3 - n4));
    n4 = n4 + cut_lp * (n3 - n4);
    data[n] = n4;
  }	
}

void MakeEcho(float *data, int samples, int amount)
{
  int k = 10 * amount;
  float verz[19000];
  for (int n = 0; n < 19000; n++) verz[n] = 0;
  int zzz = 0;
  
  for (int n = 0; n < samples; n++)
  {
    float inp = data[n];
    int vi = (zzz+600) % k;
    float echo = verz[vi];
    verz[zzz] = inp + 0.7 * echo;
    float out = inp + 0.8 * echo;
    data[n] = out;
    zzz++;
    if (zzz == k) zzz = 0;
  }
}

