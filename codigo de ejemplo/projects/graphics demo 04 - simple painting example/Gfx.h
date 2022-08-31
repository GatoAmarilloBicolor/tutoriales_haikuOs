#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>

class Gfx
{
public:

  Gfx(char *fn1);
  void Draw(BView *view);
  void SprayIt(int ix, int iy);
  void LoadImage(char *fn1);
  
  BBitmap *bmp;
  
  // dimensions of picture
  int bw;
  int bh;
  
  
  unsigned char *data; // raw bitmap data
  int spray_size; // spray size
  
  void SetSpraySize(int size);
  
 
};


#endif
