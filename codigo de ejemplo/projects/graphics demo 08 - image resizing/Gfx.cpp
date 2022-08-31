#include "Gfx.h"


// mathematical function used in stretching algorhythm
float KernelBSpline(const float x)
{
	if (x>2.0f) return 0.0f;
						
	float a, b, c, d;		
	float xm1 = x - 1.0f; 
	float xp1 = x + 1.0f;
	float xp2 = x + 2.0f;
													
	if ((xp2) <= 0.0f) a = 0.0f; else a = xp2*xp2*xp2; 
	if ((xp1) <= 0.0f) b = 0.0f; else b = xp1*xp1*xp1;
	if (x <= 0) c = 0.0f; else c = x*x*x;  
	if ((xm1) <= 0.0f) d = 0.0f; else d = xm1*xm1*xm1;

	return (0.16666666666666666667f * (a - (4.0f * b) + (6.0f * c) - (4.0f * d)));
}


const int MAX_PICTURE_RESOLUTION = 4024; // you can change this if you want to stretch larger images


// those two used in Resample method
unsigned char *data_stretching = new unsigned char[4 * MAX_PICTURE_RESOLUTION * MAX_PICTURE_RESOLUTION];
unsigned char *data_stretching_out = new unsigned char[4 * MAX_PICTURE_RESOLUTION * MAX_PICTURE_RESOLUTION];

// [ Does the stretching of picture data ] 
// w and h  represents current picture's dimensions 
// newx and newy represents new stretched dimensions
void Resample(int w, int h, long newx, long newy, bool simple_method)
{
	float xScale, yScale, fX, fY;
	xScale = (float)w / (float)newx;
	yScale = (float)h / (float)newy;

    if (simple_method)
    {
		for(long y = 0; y < newy; y++)
		{
			fY = y * yScale;
			for(long x = 0; x < newx; x++)
			{
				fX = x * xScale;
				int fY2 = (int) fY;
				int fX2 = (int) fX;
									
                int r = data_stretching[fY2*w*3+fX2*3+0];
                int g = data_stretching[fY2*w*3+fX2*3+1];
                int b = data_stretching[fY2*w*3+fX2*3+2];
				
				data_stretching_out[y*newx*3+x*3+0] = r;
				data_stretching_out[y*newx*3+x*3+1] = g;
				data_stretching_out[y*newx*3+x*3+2] = b;
			}
		}
		return;
    }
    
	float f_x, f_y, a, b, rr, gg, bb, r1, r2;
	int   i_x, i_y, xx, yy;
		
	unsigned char* iDst;
	for (long y = 0; y < newy; y++)
	{
		f_y = (float) y * yScale - 0.5f;
		i_y = (int) floor(f_y);
		a   = f_y - (float)floor(f_y);
		for (long x = 0; x < newx; x++)
		{
			f_x = (float) x * xScale - 0.5f;
			i_x = (int) floor(f_x);
			b   = f_x - (float)floor(f_x);
			rr = gg = bb = 0.0f;
			for (int m= -1; m < 3; m++) 
			{
				r1 = KernelBSpline((float) m - a);
				yy = i_y + m;
				if (yy < 0) yy = 0;
				if (yy >= h) yy = h-1;
				for (int n= -1; n < 3; n++) 
				{
					r2 = r1 * KernelBSpline(b - (float)n);
					xx = i_x+n;
					if (xx < 0) xx= 0;
					if (xx >= w) xx= w - 1;
					iDst  = data_stretching + yy*w*3+ xx*3;
					int rgbBlue = *iDst++;
					int rgbGreen= *iDst++;
					int rgbRed  = *iDst;
					rr += rgbRed * r2;
					gg += rgbGreen * r2;
					bb += rgbBlue * r2;
				}
			}
			iDst = data_stretching_out + y*newx*3+ x*3;
			*iDst++ = (unsigned char)bb;
			*iDst++ = (unsigned char)gg;
			*iDst   = (unsigned char)rr;
		}
	}

}	
	
	
Gfx::Gfx(char *fn1)
{
  bmp_h1 = BTranslationUtils::GetBitmapFile(fn1); // load picture from file
  
  // store dimensions of it
  bw = bmp_h1->Bounds().right;
  bh = bmp_h1->Bounds().bottom;
  
  // allocate raw data array used to store picture data, before being sent to bmp BBitmap object
  data = new unsigned char[4 * MAX_PICTURE_RESOLUTION * MAX_PICTURE_RESOLUTION];
}


// this is called from BView's view method when needed (+when resize window action happens)
void Gfx::Draw(BView *view)
{
  int winw, winh;
  
  // Get our window's size
  winw = win->Bounds().right;
  winh = win->Bounds().bottom;
    
  unsigned char* data_h1 = (unsigned char*) bmp_h1->Bits(); // load raw data from picture
  int br;
  br = bmp_h1->BytesPerRow();

  // fill data from loaded picture into data_stretching array, which doesn't have alpha and
  // is used in Resample algorhythm  	  
  for (int x = 0; x < bw-1; x++)
  for (int y = 0; y < bh-1; y++)
  {
    data_stretching[y*bw*3+x*3+0] = data_h1[y*br+x*4+0];
    data_stretching[y*bw*3+x*3+1] = data_h1[y*br+x*4+1];
    data_stretching[y*bw*3+x*3+2] = data_h1[y*br+x*4+2];
  }
  
  // perform stretching to window's width and height
  Resample(bw, bh, winw, winh, false); // last parameter specify whether we should use basic algorhythm

  BBitmap *bmp;     // bitmap used for final rendering to view
  bmp = new BBitmap(BRect(1,1,winw,winh), B_RGBA32); // allocate it to size of window
  
  // copy resulting resampled data to our data array 
  for (int x = 0; x < winw-1; x++)
  for (int y = 0; y < winh-1; y++)
  {  
    data[y*winw*4+x*4+0] = data_stretching_out[y*winw*3+x*3+0];
    data[y*winw*4+x*4+1] = data_stretching_out[y*winw*3+x*3+1];
    data[y*winw*4+x*4+2] = data_stretching_out[y*winw*3+x*3+2];
    data[y*winw*4+x*4+3] = 255; // Set alpha component to maximum value (255)
  }
		  
  bmp->SetBits((void*)data,winw*winh*4,0,B_RGBA32); // copy "data" array to bmp
  view->DrawBitmap(bmp); // and then bmp to view
  delete bmp; // we currently don't need bmp anymore, so delete it
}
