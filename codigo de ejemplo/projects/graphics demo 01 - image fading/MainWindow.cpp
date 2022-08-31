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

// this method is called by system as neccessary (but we can force to do that by calling Invalidate on view)
void ViewA::Draw(BRect ur)
{
  if (app_loaded == false) return;
  if (quit_req == true) return;
  gfx->Draw(this);
}


enum
{
  M_SLIDER_CHG = 'slch' // our id for slider, so we can track it's event
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,700,700),"Graphics demo 01 - Image fader",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	
	gfx = new Gfx("h1.png", "h2.png");
	
	BRect r;
	r.Set(5,5,255,55);
    slider = new BSlider(r, "slider1",
                     "Interpolate between two images", new BMessage(M_SLIDER_CHG),
                     0, 255);	
                     
    AddChild(slider);
    gfx->slider = slider; 
    
        
    view = new ViewA(gfx->bw, gfx->bh);
    view->gfx = gfx;
	AddChild(view);
	
	app_loaded = true;
	view->Invalidate(); // force redraw of view
}


void MainWindow::MessageReceived(BMessage *msg)
{
	  
	switch (msg->what)
	{
		case M_SLIDER_CHG:
		  // force to refresh view
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
