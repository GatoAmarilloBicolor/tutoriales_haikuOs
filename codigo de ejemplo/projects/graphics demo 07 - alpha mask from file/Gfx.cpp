#include "Gfx.h"


// linear interpolation function
int lerp(int value1, int value2, float proc)
{
  return (int)(value1 + (value2 - value1) * proc);
}



Gfx::Gfx(char *fn1, char *fn_mask)
{
  bmp_h1 = BTranslationUtils::GetBitmapFile(fn1); // load elephant picture
  bmp_mask = BTranslationUtils::GetBitmapFile(fn_mask); // load masking picture
		         
  // calculate dimensions of pictures		         
  bw_largest = bw = bmp_h1->Bounds().right;
  bh_largest = bh = bmp_h1->Bounds().bottom;
	
  bw_mask = bmp_mask->Bounds().right;
  bh_mask = bmp_mask->Bounds().bottom;
  if (bw_mask > bw_largest) bw_largest = bw_mask;
  if (bh_mask > bh_largest) bh_largest = bh_mask;
		  
  									
  bmp = new BBitmap(BRect(1,1,bw_largest,bh_largest), B_RGBA32); // allocate bmp object for drawing onto view
  data = new unsigned char[4*bw_largest*bh_largest]; // allocate space for raw data that will be copied to bmp object
}



// this is being called from BView's Draw method when needed 
void Gfx::Draw(BView *view)
{
  unsigned char* data_h1 = (unsigned char*) bmp_h1->Bits(); // get raw data from elephant's bmp_h1
  int br;
  br = bmp_h1->BytesPerRow();
	
  
  // we draw background as square patterns
  bool reverse = false;
  for (int x = 0; x < bw_largest; x+= 20)
  {
  	reverse = !reverse;
    for (int y = 0; y < bh_largest; y+= 20)
    {
      reverse = !reverse;
      for (int stepx = 0; stepx < 20; stepx++)
      for (int stepy = 0; stepy < 20; stepy++)
      {				
        int nx = x + stepx;
        int ny = y + stepy;
        data[ny*bw_largest*4+nx*4+0] = 150+100 * reverse;
        data[ny*bw_largest*4+nx*4+1] = 150+100 * reverse;
        data[ny*bw_largest*4+nx*4+2] = 150+100 * reverse;
        data[ny*bw_largest*4+nx*4+3] = 255;    
      }
    }
  }
  
  
  unsigned char* data_mask = (unsigned char*) bmp_mask->Bits(); // get raw data from mask's picture bitmap
  int br2;
  br2 = bmp_mask->BytesPerRow();
  
  int w_smallest = bw;
  int h_smallest = bh;
  if (bw_mask < w_smallest) w_smallest = bw_mask;
  if (bh_mask < h_smallest) h_smallest = bh_mask;
	  
  // next we draw loaded PNG (elephant) with encoded alpha channel			
  for (int x = 0; x < w_smallest; x++)
  for (int y = 0; y < h_smallest; y++)
  {  
    int elephant_r, elephant_g, elephant_b, elephant_a, final_a;

    // retrieve elephant PNG's color channels and alpha
    elephant_r = data_h1[y*br+x*4+0];
    elephant_g = data_h1[y*br+x*4+1];
    elephant_b = data_h1[y*br+x*4+2];
    elephant_a = data_h1[y*br+x*4+3];				
							
	// retrieve alpha value from first (red) channel of alpha mask picture
	final_a = 255 - data_mask[y*br2+x*4+0];
					 
    // when elephant's alpha is zero, force set destination alpha to zero also
    if (elephant_a == 0) final_a = 0;
    		    			    

    // linearly interpolate between previously set background image and colors from loaded PNG
    // (as interpolating channel we use mask picture's alpha)	    
    			    
    data[y*bw_largest*4+x*4+0] = lerp(data[y*bw_largest*4+x*4+0], elephant_r, final_a / 255.0f); 
    data[y*bw_largest*4+x*4+1] = lerp(data[y*bw_largest*4+x*4+1], elephant_g, final_a / 255.0f);
    data[y*bw_largest*4+x*4+2] = lerp(data[y*bw_largest*4+x*4+2], elephant_b, final_a / 255.0f);
    data[y*bw_largest*4+x*4+3] = 255; // Set final alpha component to maximum value (255)
    
  }
  
  // copy raw data from "data" array to bmp BBitmap
  bmp->SetBits((void*)data,bw_largest*bh_largest*4,0,B_RGBA32);
  view->DrawBitmap(bmp); // finally, render bitmap bmp to view
}
