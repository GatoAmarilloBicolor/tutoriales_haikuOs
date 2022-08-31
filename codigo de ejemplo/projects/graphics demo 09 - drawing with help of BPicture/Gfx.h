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
const int BLEND_COPY = 4;


// for actually drawing
class Gfx
{
public:

  Gfx(char *fn1);
  void Draw(BView *view);
  
  BView *view_offscr;
  BBitmap *bmp_offscr;
  BBitmap *bmp_h1;

  int bw;
  int bh;
  unsigned char *data;
  unsigned char *data_h1;
  

  int blend_mode;

};


#endif
