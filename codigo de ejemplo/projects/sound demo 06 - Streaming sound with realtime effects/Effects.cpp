#include "Effects.h"
#include <math.h>


  float max(float v1, float v2)
  {
    if (v1 > v2) return v1; else return v2;
  }
			
  float min(float v1, float v2)
  {
    if (v1 < v2) return v1; else return v2;
  }
  

  void LowPassEffect::CalculateRest()
  {
    fb_lp = res_lp + res_lp / (1 - cut_lp);
  }


  LowPassEffect::LowPassEffect()
  { 
    n3 = 0;
    n4 = 0;
    SetCutoffFrequency(10.0f);
    SetResonanceFactor(0.5f);
  }

  void LowPassEffect::SetCutoffFrequency(float value)
  {
    cut_lp = 0.05 + value / 4000.0f;
    CalculateRest();
  }

  void LowPassEffect::SetResonanceFactor(float value)
  {
    res_lp = value;
    CalculateRest();
  }

  void LowPassEffect::SetParameter(float value, int param_index)
  {
    if (param_index == 0) SetCutoffFrequency(value);
    else if (param_index == 1) SetResonanceFactor(value);
  }
  
  void LowPassEffect::ApplyToSound(float *data, int samples)
  {
    for (int n = 0; n < samples; n++)
    {
      n3 = n3 + cut_lp * (data[n] - n3 + fb_lp * (n3 - n4));
      n4 = n4 + cut_lp * (n3 - n4);
      data[n] = n4;
    }	
  }

  void LowPassEffect::ClearBuffers()
  {
  }












  EchoEffect::EchoEffect() 
  { 
    verz = new float[60000];
    for (int n = 0; n < 60000; n++) verz[n] = 0;
    zzz = 0;
    SetEchoAmount(25.0f);
  }
  
  EchoEffect::~EchoEffect()
  {
    delete [] verz;
  }
  
  void EchoEffect::ClearBuffers()
  {
    for (int n = 0; n < 60000; n++) verz[n] = 0;
    zzz = 0;
  }

  void EchoEffect::SetEchoAmount(float value)
  {
  	ClearBuffers();
    
    
    k = 10 * value;
  }

  void EchoEffect::SetParameter(float value, int param_index)
  {
 	
    SetEchoAmount(value);
  }
  

  void EchoEffect::ApplyToSound(float *data, int samples)
  {
    for (int n = 0; n < samples; n++)
    {
      float inp = data[n];
      int vi = (zzz + 600) % k;
      if (vi > 60000) vi = 60000; // limit to "verz" array size
      float echo = verz[vi];
      int zzz2 = zzz;
      if (zzz2 > 60000) zzz2 = 60000; // limit also here to prevent crashing
      verz[zzz2] = inp + 0.7 * echo;
      float out = inp + 0.8 * echo;
      data[n] = out;
      zzz++;
      if (zzz == k) zzz = 0;
    }
  }









  BandpassEffect::BandpassEffect()
  {
    SetBandwidth(0.92f);
  	SetFrequency(2000);				
  }
  
  BandpassEffect::~BandpassEffect()
  {
    
  }
  

  void BandpassEffect::CalculateRest()
  {
    
    float slider3 = 0.0f;
    float slider4 = -18;
    float slider5 = 0.0f;
    
    float dampening = bandwidth * 0.999f + 0.001f;
    float c = (1 / tanf(3.1434f * freq / 44100.0f));
    float a2 = 1 + c * (c + dampening);
    fa1 = 2 * (1 - c*c) / a2;
    fa0 = (1 + c * (c-dampening)) / a2;
    fk = c * dampening / a2;
    
    float wetsign = 1;
    dry = powf(2, slider4 / 6.0f);
    wet = powf(2, slider3 / 6.0f) * wetsign;
    float lastdamp = dampening;     
  }		
  
  void BandpassEffect::SetFrequency(float value)
  {
    freq = value;
    CalculateRest();
  }
  
  // value must have range [0, 1]
  void BandpassEffect::SetBandwidth(float value)
  {
    bandwidth = value;
    CalculateRest();
  }


  void BandpassEffect::SetParameter(float value, int param_index)
  {
    if (param_index == 0) SetFrequency(value);
    else if (param_index == 1) SetBandwidth(value);
  }
  

  
  void BandpassEffect::ApplyToSound(float *data, int samples)
  {
    for (int n = 0; n < samples; n++)
    {
	  float d0_l = fk*data[n] - (fa1*fd1_l + fa0*fd2_l);
 	  data[n] = wet*(d0_l - fd2_l) + dry*data[n];
      fd2_l = fd1_l;
      fd1_l = d0_l; 
    }
  }
  
  void BandpassEffect::ClearBuffers()
  {
    CalculateRest();
  }
  
  
  
  
  
   
  
  
  
  
  
  TremoloEffect::TremoloEffect()
  {
    ClearBuffers();
 	SetFrequency(9);
 	
  }
  
  TremoloEffect::~TremoloEffect()
  {
  }
  
  void TremoloEffect::CalculateRest()
  {
    adv = 3.1434f * 2 * freq / 44100.0f;
    
    amount = powf(2.0f, 1 / 6.0f);
    sc = 0.5f * amount;
    amount = 1 - amount;
  }		
  
  // given in Hz
  void TremoloEffect::SetFrequency(float value)
  {
    freq = value;
    CalculateRest();
  }

  
  void TremoloEffect::SetParameter(float value, int param_index)
  {
    SetFrequency(value);
  }
  

  void TremoloEffect::ApplyToSound(float *data, int samples)
  {
    for (int n = 0; n < samples; n++)
    {
      data[n] = data[n] * ((cosf(pos) + 1) * sc + amount);
      pos += adv;
    }
  }
  
  void TremoloEffect::ClearBuffers()
  {
    pos = 0.0f;
  }

