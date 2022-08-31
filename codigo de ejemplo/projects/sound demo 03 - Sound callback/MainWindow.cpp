#include "MainWindow.h"


BApplication *bp;


bool quit_req;
float slider_value = 10.1f;

void SetAppPointer(BApplication *bapp)
{
  bp = bapp;
}

BApplication *GetAppPointer()
{
  return bp;
}

void hook(void*cookie, void*inBuffer, size_t byteCount, BStreamingGameSound *object)
{
  static int nn = 0;
  for (int n = 0; n < byteCount / sizeof(float); n++)
  {
    float *buf = (float*)inBuffer;
    float final = nn / *((float*)cookie);
    buf[n] = sinf(final);
    nn++;
  }
}
    
    
enum
{
  M_SLIDER_CHG = 'slch',
  M_BUTTON1 = 'btn0',
  M_BUTTON2 = 'btn1'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 03 - Using callback",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	bs = NULL;
	
	BRect r;
	r.Set(5,5,300,55);
    slider = new BSlider(r, "slider1", "Frequency", new BMessage(M_SLIDER_CHG), 1, 700);	
    AddChild(slider);

	BRect r2;
	r2.Set(5,60,300,80);
    btn1 = new BButton(r2, "button1", "Start", new BMessage(M_BUTTON1));	
    AddChild(btn1);
		
	BRect r3;
	r3.Set(5,60+30,300,80+30);
    btn2 = new BButton(r3, "button2", "Stop", new BMessage(M_BUTTON2));	
    AddChild(btn2);
   
}

void MainWindow::MessageReceived(BMessage *msg)
{
	  
	switch (msg->what)
	{
		case M_SLIDER_CHG:
		{
	      slider_value = (1000 - slider->Value()) * 0.01f;
	      if (slider_value < 2) slider_value = 2;
	      if (slider_value > 300) slider_value = 300;
		  break;
		}
		
		case M_BUTTON1:
		{
		  if (bs != NULL)
		  {
		    bs->StopPlaying();
		    delete bs;
		    bs = NULL;
		  }
		  
      	  fmt.frame_rate = 44100;
          fmt.channel_count = 1;
          fmt.format = gs_audio_format::B_GS_F; // we use float format
          fmt.buffer_size = 0; // auto determine = 0
          fmt.byte_order = 2;
          bs = new BStreamingGameSound(2048,&fmt);
          if (bs->InitCheck() != B_OK) printf("ERROR at InitCheck");
       
		  
       	  bs->SetStreamHook(hook, (void*)&slider_value);
	      bs->StartPlaying();		  
		  break;
		}
		  
		case M_BUTTON2:
		{
		  if (bs != NULL)
		  {
		    bs->StopPlaying();
		    delete bs;
		    bs = NULL;
		  }
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
	if (bs != NULL)
	{
	  bs->StopPlaying();
	  delete bs;
      bs = NULL;
    }
		  	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
