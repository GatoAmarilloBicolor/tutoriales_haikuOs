#include "Gfx.h"


// linear interpolation function
int lerp(int value1, int value2, float proc)
{
  return (int)(value1 + (value2 - value1) * proc);
}



Gfx::Gfx(char *fn1, char *fn_bg)
{
  // load pictures from file
  bmp_h1 = BTranslationUtils::GetBitmapFile(fn1);
  bmp_bg = BTranslationUtils::GetBitmapFile(fn_bg);
	
  // calculate dimensions of pictures	         
  bw_largest = bw = bmp_h1->Bounds().right;
  bh_largest = bh = bmp_h1->Bounds().bottom;
	
  bw_bg = bmp_bg->Bounds().right;
  bh_bg = bmp_bg->Bounds().bottom;
  if (bw_bg > bw_largest) bw_largest = bw_bg;
  if (bh_bg > bh_largest) bh_largest = bh_bg;
		  
  // allocate BBitmap object, which is used to draw onto our view  									
  bmp = new BBitmap(BRect(1,1,bw_largest,bh_largest), B_RGBA32);
  data = new unsigned char[4*bw_largest*bh_largest]; // used with conjuction with bmp
}

void Gfx::Draw(BView *view)
{
  unsigned char* data_h1 = (unsigned char*) bmp_h1->Bits(); // get raw bitmap data from elephant's PNG
  int br;
  br = bmp_h1->BytesPerRow();
  
  /*
  // we draw background as square patterns
  bool reverse = false;
  for (int x = 0; x < bw_largest; x+= 20)
  for (int y = 0; y < bh_largest; y+= 20)
  {
    reverse = !reverse;
    for (int stepx = 0; stepx < 20; stepx++)
    for (int stepy = 0; stepy < 20; stepy++)
    {				
      int nx = x + stepx;
      int ny = y + stepy;
      data[ny*bw_largest*4+nx*4+0] = 255 * reverse;
      data[ny*bw_largest*4+nx*4+1] = 255 * reverse;
      data[ny*bw_largest*4+nx*4+2] = 255 * reverse;
      data[ny*bw_largest*4+nx*4+3] = 255;    
    }
  }
  */
  
  // draw background first
  unsigned char* data_bg = (unsigned char*) bmp_bg->Bits(); // get raw bitmap data from background picture
  int br2;
  br2 = bmp_bg->BytesPerRow();

  // fill entire data array with white color to avoid seeing artifacts when skipping positions at next stage
  for (int x = 0; x < bw_largest; x++)
  for (int y = 0; y < bh_largest; y++)
  {
    data[y*bw_largest*4+x*4+0] = 255;
    data[y*bw_largest*4+x*4+1] = 255;
    data[y*bw_largest*4+x*4+2] = 255;
    data[y*bw_largest*4+x*4+3] = 255;     
  }  
  
  // we draw background up to size of background image  
  for (int x = 0; x < bw_bg; x++)
  for (int y = 0; y < bh_bg; y++)
  {
    data[y*bw_largest*4+x*4+0] = data_bg[y*br2+x*4+0];
    data[y*bw_largest*4+x*4+1] = data_bg[y*br2+x*4+1];
    data[y*bw_largest*4+x*4+2] = data_bg[y*br2+x*4+2];
    data[y*bw_largest*4+x*4+3] = 255;    
  }
  
  
  // next we draw loaded PNG with encoded alpha channel			
  // (we draw up to size of elephant PNG)
  for (int x = 0; x < bw; x++)
  for (int y = 0; y < bh; y++)
  {  
    int r,g,b,a;
    // retrieve PNG's color channels
    r = data_h1[y*br+x*4+0];
    g = data_h1[y*br+x*4+1];
    b = data_h1[y*br+x*4+2];
	a = data_h1[y*br+x*4+3]; // here is encoded original transparency value
					 
    a *= (slider->Value() / 255.0f); // modify alpha by takin into account alpha value of slider GUI object
    		    			    
    // linearly interpolate between previously set background image and colors from loaded PNG
    // (as interpolating channel we use PNG's alpha)	    
    			    
    data[y*bw_largest*4+x*4+0] = lerp(data[y*bw_largest*4+x*4+0], r, a / 255.0f); 
    data[y*bw_largest*4+x*4+1] = lerp(data[y*bw_largest*4+x*4+1], g, a / 255.0f);
    data[y*bw_largest*4+x*4+2] = lerp(data[y*bw_largest*4+x*4+2], b, a / 255.0f);
    data[y*bw_largest*4+x*4+3] = 255; // Set final alpha component to maximum value (255)
    
  }
	  
  bmp->SetBits((void*)data,bw_largest*bh_largest*4,0,B_RGBA32); // copy raw data from "data" array into bmp
  view->DrawBitmap(bmp); // finally draw bitmap from bmp onto view
}
