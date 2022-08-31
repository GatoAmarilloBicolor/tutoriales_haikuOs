#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>

// used for image processing
class Gfx
{
public:

  Gfx(char *fn1, char *fn_bg);
  void Draw(BView *view); // called from BView's draw method
  
  BBitmap *bmp;    // bitmap, that represents final picture that will be drawn onto view
  BBitmap *bmp_h1; // picture of elephant with transparent background encoded with it
  BBitmap *bmp_bg; // background picture
  
  // store dimensions of pictures
  int bw, bw_bg, bw_largest;
  int bh, bh_bg, bh_largest;
  
  unsigned char *data; // stores raw bitmap data, used before rendering to final bitmap (bmp)
  
  BSlider *slider; // pointer to slider GUI object
  
};


#endif
