#include "MainWindow.h"
#include <Application.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>

bool app_loaded = false;


ViewA::ViewA(int bw_in, int bh_in) : BView(BRect(1,20,bw_in,bh_in+20), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
 
}

ViewA::~ViewA(void)
{
 
}


	

void ViewA::Draw(BRect r)
{
  if (app_loaded == false) return;	
  gfx->Draw(this);
}


// id's for tracking down menu click events
const int M_SET_ADD = 111;
const int M_SET_SUB = 112;
const int M_SET_MIN = 113;
const int M_SET_MAX = 114;


MainWindow::MainWindow(void): BWindow(BRect(100,100,700,700),"Graphics demo 02 - Blending images with SetDrawingMode method of BView",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
    
    gfx = new Gfx("h1.png", "h2.png");
    
    
	view = new ViewA(gfx->bw, gfx->bh);
	AddChild(view);
	view->gfx = gfx;
	
    
    BRect r2(Bounds());
    r2.bottom = 20;
    BMenuBar *menuBar = new BMenuBar(r2,"menubar");
    AddChild(menuBar);
    
    BMenu *menu = new BMenu("Blending mode");
    menu->AddItem(new BMenuItem("Add", new BMessage(M_SET_ADD),'A'));
    menu->AddItem(new BMenuItem("Subtract", new BMessage(M_SET_SUB),'S'));
    menu->AddItem(new BMenuItem("Min", new BMessage(M_SET_MIN),'M'));
    menu->AddItem(new BMenuItem("Max", new BMessage(M_SET_MAX),'X'));

    
    menuBar->AddItem(menu);
    app_loaded = true;
    view->Invalidate(); // force redraw of view
}



void MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		// event catching for clicking menu items
		case M_SET_ADD:
		  gfx->blend_mode = BLEND_ADD;
		  view->Invalidate();           // by calling this redraw of view instance is forced
		  break;
		  
		case M_SET_SUB:
		  gfx->blend_mode = BLEND_SUB;
		  view->Invalidate();
		  break;

		case M_SET_MIN:
		  gfx->blend_mode = BLEND_MIN;
		  view->Invalidate();
		  break;
		  
		case M_SET_MAX:
		  gfx->blend_mode = BLEND_MAX;
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
