#include "Gfx.h"
#include <interface/Picture.h>


Gfx::Gfx(char *fn1)
{
	blend_mode = BLEND_ADD;
	bmp_h1 = BTranslationUtils::GetBitmapFile(fn1);
          
    bw = bmp_h1->Bounds().right;
    bh = bmp_h1->Bounds().bottom;

	bmp_offscr = new BBitmap(BRect(1,1,bw,bh), B_RGBA32,true);
	view_offscr = new BView(BRect(1,1,bw,bh),"offs",0,0);
	bmp_offscr->AddChild(view_offscr);
}

void Gfx::Draw(BView *view)
{
   BPicture *pic;
   
   view->BeginPicture(new BPicture());
   view->SetDrawingMode(B_OP_COPY);
   view->MovePenTo(0,0);					
   view->DrawBitmap(bmp_h1);
   
   
   view->MovePenTo(0,50);					
   view->SetHighColor(255,0,0);
   view->SetLowColor(255,0,0);
   
   if (blend_mode == BLEND_ADD) view->SetDrawingMode(B_OP_ADD);
   else if (blend_mode == BLEND_SUB) view->SetDrawingMode(B_OP_SUBTRACT);
   else if (blend_mode == BLEND_MIN) view->SetDrawingMode(B_OP_MIN);
   else if (blend_mode == BLEND_MAX) view->SetDrawingMode(B_OP_MAX);
   else if (blend_mode == BLEND_COPY) view->SetDrawingMode(B_OP_COPY);


   view->SetFontSize(18);
   view->DrawString("This is example string");
   view->MovePenTo(0,23);					
   view->SetFontSize(11);
   view->DrawString("This is smaller string");
   
   
   view->MovePenTo(0,50);					

   view->FillEllipse(BPoint(200,200),150,150);
   view->StrokeEllipse(BPoint(200,200),160,160);
 
   view->SetHighColor(255,0,255);
   view->SetLowColor(255,0,255); 
   BPoint points[4];
   points[0].x = 340; points[0].y = 240;
   points[1].x = 360; points[1].y = 260;
   points[2].x = 440; points[2].y = 390;
   points[3].x = 490; points[3].y = 340;
   							
   view->FillBezier(&points[0]);
   view->FillArc(BPoint(350,400),100,70,67,235);
   
   view->SetHighColor(0,0,255);
   view->SetLowColor(0,255,255);    
   view->FillRect(BRect(450,400,550,600));
   view->FillRoundRect(BRect(150,100,250,300),20,20);
   
   view->MovePenTo(0,0); // before ending we return pen position to zero 
   pic = view->EndPicture();
   view->DrawPicture(pic);
   
   // draw pic BPicture to view_offscr BView
   // (view_offscr is attached on bmp_offscr)
   // we can use later bmp_offscr for saving to file
   bmp_offscr->Lock();
   view_offscr->SetDrawingMode(B_OP_COPY);
   view_offscr->DrawPicture(pic);
   view_offscr->Flush();
   bmp_offscr->Unlock();
   
   delete pic; // delete it as we don't need it anymore
}
