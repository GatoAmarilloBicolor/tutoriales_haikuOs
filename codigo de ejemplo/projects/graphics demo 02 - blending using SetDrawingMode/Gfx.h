#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>



const int BLEND_ADD = 0;
const int BLEND_SUB = 1;
const int BLEND_MIN = 2;
const int BLEND_MAX = 3;


// for doing image processing
class Gfx
{
public:

  Gfx(char *fn1, char *fn2);
  void Draw(BView *view);
  
  BView *view_offscr;
  BBitmap *bmp_offscr;   
  
  // BBitmap pointers to loaded pictures
  BBitmap *bmp_h1;
  BBitmap *bmp_h2;
  
  // dimensions of both pics
  int bw, bw2, bw_largest;
  int bh, bh2, bh_largest;
  
  int blend_mode; // store here blending mode, choosen in menu

};


#endif
