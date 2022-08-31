#include "Gfx.h"


// linear intepolation function
int lerp(int value1, int value2, float proc)
{
  return (int)(value1 + (value2 - value1) * proc);
}


void Gfx::LoadImage(char *fn1)
{
  BBitmap *bmp_h1;	// temporary store for loaded bitmap
  if (bmp != NULL) delete bmp;
  if (data != NULL) delete [] data;
  
  bmp_h1 = BTranslationUtils::GetBitmapFile(fn1); // load picture

  // store it's dimensions         
  bw = bmp_h1->Bounds().right;
  bh = bmp_h1->Bounds().bottom;
  
  bmp = new BBitmap(BRect(1,1,bw,bh), B_RGBA32);
  data = new unsigned char[4*bw*bh];
    
  
  // Next region of code copies existing loaded bitmap to new one with
  // predefined (fixed) format to avoid errors which can be result of original
  // image format
  unsigned char* data_h1 = (unsigned char*) bmp_h1->Bits();
  int br;
  br = bmp_h1->BytesPerRow();
  
  for (int x = 0; x < bw; x++)
  for (int y = 0; y < bh; y++)
  {  
    int r,g,b;
    
    r = data_h1[y*br+x*4+0];
    g = data_h1[y*br+x*4+1];
    b = data_h1[y*br+x*4+2];
			
    data[y*bw*4+x*4+0] = r;
    data[y*bw*4+x*4+1] = g;
    data[y*bw*4+x*4+2] = b;
    data[y*bw*4+x*4+3] = 255; // Set alpha component to maximum value (255)
  }
  
  bmp->SetBits((void*)data,bh*bw*4,0,B_RGBA32); // copy raw data from data array to bmp BBitmap object
  delete bmp_h1; //  delete original picture, we don't need it anymore
}



Gfx::Gfx(char *fn1)
{
  bmp = NULL;
  data = NULL;
  LoadImage(fn1);
  spray_size = 20;
  
}


void Gfx::SetSpraySize(int size)
{
  this->spray_size = size;
}


void Gfx::SprayIt(int ix, int iy)
{ 
  int cx = ix + spray_size / 2;
  int cy = iy + spray_size / 2;
  
  for (int x = ix; x < ix + spray_size; x++)
  for (int y = iy; y < iy + spray_size; y++)
  {  
    int r,g,b;
    
    // prevent drawing outside of image region
    if (x >= bw) continue;
    if (y >= bh) continue;
    if (x < 0) continue;
    if (y < 0) continue;
        
    r = data[y*bw*4+x*4+0];
    g = data[y*bw*4+x*4+1];
    b = data[y*bw*4+x*4+2];
    				
    				
    // color at center of spray must be very solid (in this example black color),
    // but at distance from center it must be non solid (original color)    					
    float dist = fabs(sqrtf((cx - x)*(cx-x) + (cy - y)*(cy - y)));
    dist = dist / (spray_size / 2);
    if (dist > 1.0f) dist = 1.0f;
    if (dist < 0.0f) dist = 0.0f;  
    
    
    float amount = 1.0f - dist;  			    			
    r = lerp(r, 0, amount);
    g = lerp(g, 0, amount);
    b = lerp(b, 0, amount);
			
    data[y*bw*4+x*4+0] = r;
    data[y*bw*4+x*4+1] = g;
    data[y*bw*4+x*4+2] = b;
    data[y*bw*4+x*4+3] = 255; // Set alpha component to maximum value (255)
  }
		  
  bmp->SetBits((void*)data,bw*bh*4,0,B_RGBA32); // copy raw data from "data" array to bmp, which will be drawn to view later
  
}


// called by BView when needed
void Gfx::Draw(BView *view)
{
  view->DrawBitmap(bmp);
}
