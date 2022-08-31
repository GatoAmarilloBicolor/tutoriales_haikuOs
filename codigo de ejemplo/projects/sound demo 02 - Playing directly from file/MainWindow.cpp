#include "MainWindow.h"

BApplication *bp;


bool quit_req;


void SetAppPointer(BApplication *bapp)
{
  bp = bapp;
}

BApplication *GetAppPointer()
{
  return bp;
}


enum
{
  M_BUTTON1 = 'btn0',
  M_BUTTON2 = 'btn1'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 02 - Playing directly from file",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	s = NULL;
	
	BRect r1;
	r1.Set(5,60,300,80);
    btn1 = new BButton(r1, "button1", "Play", new BMessage(M_BUTTON1));	
    AddChild(btn1);

	BRect r2;
	r2.Set(5,60+30,300,80+30);
    btn2 = new BButton(r2, "button2", "Stop", new BMessage(M_BUTTON2));	
    AddChild(btn2);

}


void MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_BUTTON2:
		{
		  if (s != NULL)
   		  {
     	    s->StopPlaying();
     		delete s;
     		s = NULL;
   		  }
   		  break;
		}
		
		case M_BUTTON1:
		{
   		  app_info *ai = new app_info();
   		  bp->GetAppInfo(ai);
   		  BPath path(&ai->ref);
   		  BPath ppp;
   		  path.GetParent(&ppp);
		  ppp.Append("test.wav");
   			
   		  if (s != NULL)
   		  {
     	    s->StopPlaying();
     		delete s;
     		s = NULL;
   		  }
   
	      s = new BFileGameSound((char*)ppp.Path(), true);
	      s->InitCheck();
		  s->StartPlaying();
		  break;
		}
		
		default:
		{
		  BWindow::MessageReceived(msg);
		  break;
		}
	}
}


// called when quiting
bool MainWindow::QuitRequested(void)
{
	if (s != NULL)
   	{
      s->StopPlaying();
      delete s;
      s = NULL;
    }	
 		  
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
