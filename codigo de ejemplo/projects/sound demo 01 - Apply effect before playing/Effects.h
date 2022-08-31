#ifndef _seff_
#define _seff_

#include "PitchShifter.h"


class SoundEffect
{
public:

  // supply float data array and it's size (number of samples)
  virtual void ApplyToSound(float* data, int samples)
  {
  }
  
  virtual void SetParameter(float value, int param_index)
  {
  }
  
  // useful when we switch effect off and later on so we can get rid previous sound 
  // data stored in buffers
  virtual void ClearBuffers()
  {
  }
 
};





class LowPassEffect : public SoundEffect
{
private:
  float n3;
  float n4;
  float cut_lp;
  float res_lp;
  float fb_lp;

  void CalculateRest();

  
public:

  LowPassEffect();
  void SetCutoffFrequency(float value);
  void SetResonanceFactor(float value);
  void SetParameter(float value, int param_index);
  void ApplyToSound(float *data, int samples);
  void ClearBuffers();
};





class EchoEffect : public SoundEffect
{
private:

  int k;
  float *verz;
  int zzz;

  
public:

  EchoEffect();
  ~EchoEffect();
  void SetEchoAmount(float value);
  void SetParameter(float value, int param_index);
  void ApplyToSound(float *data, int samples);
  void ClearBuffers();
};






class PitchShiftEffect : public SoundEffect
{
private:

  float shift_factor;
  
public:

  PitchShiftEffect();
  void SetPitch(float value);
  void SetParameter(float value, int param_index);
  void ApplyToSound(float *data, int samples);
  void ClearBuffers();
};










class BandpassEffect : public SoundEffect
{
private:

  
  void CalculateRest();  
  float fd2_l, fd1_l, fa0, fa1, fk, wet, dry, freq;
  float bandwidth;
  
public:

  BandpassEffect();
  ~BandpassEffect();
  
  void SetBandwidth(float value);
  void SetFrequency(float value);
  void SetParameter(float value, int param_index);
  void ApplyToSound(float *data, int samples);
  void ClearBuffers();
};









class TremoloEffect : public SoundEffect
{
private:

  void CalculateRest();  
  float adv, freq, amount, sc, pos;
  
public:

  TremoloEffect();
  ~TremoloEffect();
  void SetParameter(float value, int param_index);
  void SetFrequency(float freq);
  void ApplyToSound(float *data, int samples);
  void ClearBuffers();
};
#endif
