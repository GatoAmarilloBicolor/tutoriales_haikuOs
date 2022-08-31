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
	
  BWindow *win; // we need pointer to window, so we can get size of window at time of rendering
  Gfx(char *fn1);
  void Draw(BView *view);
  
  BBitmap *bmp_h1;  // original picture loaded from file
  
  // dimensions of picture, loaded from file
  int bw;
  int bh;
  
  unsigned char *data;
};


#endif
