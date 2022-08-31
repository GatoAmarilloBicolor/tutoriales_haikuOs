#include "Gfx.h"


Gfx::Gfx(char *fn1, char *fn2)
{
	blend_mode = BLEND_ADD;   // set default blending mode
	
	
	// load pictures
	bmp_h1 = BTranslationUtils::GetBitmapFile(fn1);
    bmp_h2 = BTranslationUtils::GetBitmapFile(fn2);
   
    // store pic's dimensions and compute largest dimensions       
    bw_largest = bw = bmp_h1->Bounds().right;
    bh_largest = bh = bmp_h1->Bounds().bottom;
    bw2 = bmp_h2->Bounds().right;
    bh2 = bmp_h2->Bounds().bottom;
    
    if (bw2 > bw_largest) bw_largest = bw2;
    if (bh2 > bh_largest) bh_largest = bh2;
		
	// create offscreen BBitmap object and offscreen BView object, through which we will draw to offscreen bitmap
	bmp_offscr = new BBitmap(BRect(1,1,bw_largest,bh_largest), B_RGBA32,true);
	view_offscr = new BView(BRect(1,1,bw_largest,bh_largest),"ofvc", 0, 0);  // (BView type (view_offscr) has many useful method for drawing 
	bmp_offscr->AddChild(view_offscr); // tell bmp_offscr (BBitmap type), that what we draw to view_offscr, everyhing will be stored to bmp_offscr
}



// called from BView when drawing is needed
void Gfx::Draw(BView *view)
{
   bmp_offscr->Lock();   // before drawing to view, that is attached to bitmap we must lock bitmap
   view_offscr->SetDrawingMode(B_OP_COPY);    // before drawing first picture (bmp_h1) use copy mode
   view_offscr->DrawBitmap(bmp_h1);

     
   if (blend_mode == BLEND_ADD) view_offscr->SetDrawingMode(B_OP_ADD);
   else if (blend_mode == BLEND_SUB) view_offscr->SetDrawingMode(B_OP_SUBTRACT);
   else if (blend_mode == BLEND_MIN) view_offscr->SetDrawingMode(B_OP_MIN);
   else if (blend_mode == BLEND_MAX) view_offscr->SetDrawingMode(B_OP_MAX);
  
   view_offscr->DrawBitmap(bmp_h2); // draw second picture with user choosen blending mode
   bmp_offscr->Unlock();
		  
   view->DrawBitmap(bmp_offscr); // finally draw bmp_offscr (offscreen bitmap) to view, which is supplied as parameter of this function
}
