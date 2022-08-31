#include "MainWindow.h"

BApplication *bp;


bool quit_req;
bool app_loaded = false;

void SetAppPointer(BApplication *bapp)
{
  bp = bapp;
}

BApplication *GetAppPointer()
{
  return bp;
}





ViewA::~ViewA()
{
}

ViewA::ViewA(int w, int h) : BView(BRect(1,50,w,h+50), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
	
}

void ViewA::Draw(BRect ur)
{
  if (quit_req == true) return;
  if (app_loaded == false) return;
  gfx->Draw(this);
  

}


enum
{
  M_SLIDER_CHG = 'slch' // id for slider, that is used in event responder when we change slider's value
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,700,700),"Graphics demo 06 - PNG transparency demo",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	

	
	
		
	BRect r;
	r.Set(5,5,255,55);
    slider = new BSlider(r, "slider1",
                     "Elephant's transparency factor", new BMessage(M_SLIDER_CHG),
                     0, 255);	
                     
    AddChild(slider);
    slider->SetValue(255);
    
    
	gfx = new Gfx("elephant.png", "6.jpg");
	gfx->slider = slider; 
   
    view = new ViewA(gfx->bw_largest, gfx->bh_largest);
    view->gfx = gfx;
	AddChild(view);

    app_loaded = true;
    view->Invalidate(); // force view to redraw  
}


void MainWindow::MessageReceived(BMessage *msg)
{
	  
	switch (msg->what)
	{
		case M_SLIDER_CHG:
		  // refresh view
		  view->Invalidate();
		  break;
		
		default:
		{
		  BWindow::MessageReceived(msg);
		  break;
		}
	}
}


bool MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
