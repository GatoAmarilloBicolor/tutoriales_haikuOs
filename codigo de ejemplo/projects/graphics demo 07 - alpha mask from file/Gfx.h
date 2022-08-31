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

  Gfx(char *fn1, char *fn_mask);
  void Draw(BView *view); 
  
  BBitmap *bmp; // represents final bitmap, that is being sent to view 
  BBitmap *bmp_h1;   // represents elephant's picture
  BBitmap *bmp_mask; // represents mask image for determining alpha of bmp_h1
   
  // dimensions of pictures
  int bw, bw_mask, bw_largest;
  int bh, bh_mask, bh_largest;
  
  unsigned char *data; // here raw bitmap data is stored to be later copied to bmp BBitmap object
 
};


#endif
