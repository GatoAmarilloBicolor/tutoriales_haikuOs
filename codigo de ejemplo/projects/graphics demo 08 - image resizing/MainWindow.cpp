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


ViewA::ViewA(int w, int h) : BView(BRect(1,1,2000,2000), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
	
}

// this method is automatically called everytime system thinks it must draw or redraw it
void ViewA::Draw(BRect ur)
{
  if (app_loaded == false) return;	
  if (quit_req == true) return;
  gfx->Draw(this);
}


MainWindow::MainWindow(void) : BWindow(BRect(100,100,700,700),"Graphics demo 08 - Image resizing",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	
	gfx = new Gfx("031.jpg");
	gfx->win = this;
	       
    view = new ViewA(gfx->bw, gfx->bh);
    view->gfx = gfx;
	AddChild(view);
	
	app_loaded = true;
	view->Invalidate(); // force view to redraw itself
}

// in our class MainWindow we override parent's method, called FrameResized.
// It's being called everytime the window is resized so we can stretch picture accordingly
void MainWindow::FrameResized(float w, float h)
{
    view->Invalidate(); // force to redraw
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
