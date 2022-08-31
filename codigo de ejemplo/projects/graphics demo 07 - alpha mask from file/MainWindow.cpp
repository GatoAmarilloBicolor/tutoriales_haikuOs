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

ViewA::ViewA(int w, int h) : BView(BRect(1,1,w,h), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
	
}

void ViewA::Draw(BRect ur)
{
  if (app_loaded == false) return;
  if (quit_req == true) return;
  gfx->Draw(this);
  

}



MainWindow::MainWindow(void) : BWindow(BRect(100,100,700,700),"Graphics demo 07 - Alpha mask from another file",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	
    gfx = new Gfx("elephant.png", "mask.jpg");
	   
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
