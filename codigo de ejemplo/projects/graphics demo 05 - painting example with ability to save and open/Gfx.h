#ifndef GFX_H
#define GFX_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>


// image processing class (spraying)
class Gfx
{
public:

  Gfx(char *fn1);
  void Draw(BView *view);
  void SprayIt(int ix, int iy);
  void LoadImage(char *fn1);
  
  BBitmap *bmp; // sprayed "data" is copied here when spraying and then *bmp is drawn onto view
  
  // dimensions of picture
  int bw;
  int bh;

  unsigned char *data; // used to store raw bitmap data before drawing it to bmp and later to view

  int spray_size;

  void SetSpraySize(int size);
  
 
};


#endif
