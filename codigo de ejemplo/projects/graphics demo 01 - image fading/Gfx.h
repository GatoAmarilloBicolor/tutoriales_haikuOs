#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>


// for image processing
class Gfx
{
public:

  Gfx(char *fn1, char *fn2);
  void Draw(BView *view);
  
  BBitmap *bmp;    // we copy data here in final stage to display it on BView object
  BBitmap *bmp_h1; // store here first picture
  BBitmap *bmp_h2; // store here second picture
  
  int bw, bw2, bw_largest;
  int bh, bh2, bh_largest;
  
  // pointers to data 
  unsigned char *data;     // used for applying final data before rendering to bmp object 

  BSlider *slider;         // pointer to our slider GUI object
  
};


#endif
