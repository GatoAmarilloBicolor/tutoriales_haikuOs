#include "Gfx.h"


// linear interpolation function
int lerp(int value1, int value2, float proc)
{
  return (int)(value1 + (value2 - value1) * proc);
}



Gfx::Gfx(char *fn1, char *fn2)
{
  slider = NULL;
  bmp_h1 = BTranslationUtils::GetBitmapFile(fn1);
  bmp_h2 = BTranslationUtils::GetBitmapFile(fn2);
         
  bw_largest = bw = bmp_h1->Bounds().right;
  bh_largest = bh = bmp_h1->Bounds().bottom;
  bw2 = bmp_h2->Bounds().right;
  bh2 = bmp_h2->Bounds().bottom;
		
  if (bw2 > bw_largest) bw_largest = bw2;
  if (bh2 > bh_largest) bh_largest = bh2;
	  
  bmp = new BBitmap(BRect(1,1,bw_largest,bh_largest), B_RGBA32);
  data = new unsigned char[4*bw_largest*bh_largest];
}



void Gfx::Draw(BView *view)
{
  if (slider == NULL) return; // if slider is not yet added, skip drawing
  
  unsigned char* data_h1 = (unsigned char*) bmp_h1->Bits();  // get raw bitmap data from first pic
  int br;
  br = bmp_h1->BytesPerRow();
		  
  unsigned char* data_h2 = (unsigned char*) bmp_h2->Bits(); // get raw bitmap data from second pic
  int br2;
  br2 = bmp_h2->BytesPerRow();


  // first we paint entire data array with white color
  for (int x = 0; x < bw_largest; x++)
  for (int y = 0; y < bh_largest; y++)
  {
    data[y*bw_largest*4+x*4+0] = 255;
    data[y*bw_largest*4+x*4+1] = 255;
    data[y*bw_largest*4+x*4+2] = 255;
    data[y*bw_largest*4+x*4+3] = 255;   	
  }

	
  int w_smallest = bw;
  int h_smallest = bh;
  if (bw2 < w_smallest) w_smallest = bw2;
  if (bh2 < h_smallest) h_smallest = bh2;

				  
  // perform blending by calculating blend for every pixel				  
  for (int x = 0; x < w_smallest; x++)
  for (int y = 0; y < h_smallest; y++)
  { 
	     
    int r,g,b;
    
    // for every channel user choosen amount of second pic's channel value is added to first pic
    r = data_h1[y*br+x*4+0] + data_h2[y*br2+x*4+0] * slider->Value() / 255.0f;
    g = data_h1[y*br+x*4+1] + data_h2[y*br2+x*4+1] * slider->Value() / 255.0f;
    b = data_h1[y*br+x*4+2] + data_h2[y*br2+x*4+2] * slider->Value() / 255.0f;
			
	// clamp each channel to it's maximum value (255) to avoid artifacts
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
  
    // store computed color to raw data array
    data[y*bw_largest*4+x*4+0] = r;
    data[y*bw_largest*4+x*4+1] = g;
    data[y*bw_largest*4+x*4+2] = b;
    data[y*bw_largest*4+x*4+3] = 255; // Set alpha component to maximum value (255)
    
  }
		  
  bmp->SetBits((void*)data,bw_largest*bh_largest*4,0,B_RGBA32); // transfer raw data from data array to BBitmap obj
  view->DrawBitmap(bmp); // finally draw bmp to given BView view object
}
