#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>


// here image processing is done
class Gfx
{
public:

  Gfx(char *fn1, char *fn2);
  void Draw(BView *view); // we call this method from BView's draw function
  
  BBitmap *bmp;      // used in final stage of rendering to view
  
  // used to store pictures from files
  BBitmap *bmp_h1;
  BBitmap *bmp_h2;
  
  // dimensions of loaded pictures
  int bw, bw2, bw_largest;
  int bh, bh2, bh_largest;

  
  // pointers to data 
  unsigned char *data;     // used for applying final data before rendering to bmp object 


  BSlider *slider;      // pointer to slider GUI object
  
};


#endif
