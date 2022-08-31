#include "MainWindow.h"

BApplication *bp;

bool quit_req;
bool playing = false;
unsigned int play_pos = 0;
unsigned int samples_cnt;
float *fdata;
BSlider *slider;
thread_id thread;
float play_position_relative = 0.0f;
BLocker *locker;
BWindow *win;


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
  if (playing == false)
  {
    // fill in empty frames when not playing
    for (int n = 0; n < byteCount / sizeof(float); n++)
    {
      float *buf = (float*)inBuffer;
      buf[n] = 0.0f;
    }
    return;
  }	

  if (locker->Lock())
  {
  	// play_position_relative is used later in slider event handler for displaying current position
  	play_position_relative = play_pos / (float) samples_cnt;
    locker->Unlock();
  }
   	
  for (int n = 0; n < byteCount / sizeof(float); n++)
  {
    float *buf = (float*)inBuffer;
    buf[n] = fdata[play_pos++]; // copy sound data from fdata array to output buffer
    
    if (play_pos >= samples_cnt) // when we come to end of sound, stop playing
    {
      playing = false;
      break;
    }
  }

}
   
   
   

// id's for GUI objects, used for event handling
enum
{
  M_BUTTON1 = 'btn0',
  M_SET_SLIDER = 'sld2',   // id for event, when we want to automatically update slider
  M_SLIDER_CHG = 'sldc'   // we don't use event handling when slider is changed by user
};

status_t mythread(void *data)
{
  BMessage *msg2 = new BMessage(M_SET_SLIDER);
          
  // use loop in thread, so we can constantly send messages to window      
  while (1)
  {
    if (locker->Lock())
    {		
      // here we send M_SET_SLIDER message to our window's event handler
      // where slider's position will be updated
      win->PostMessage(msg2);  
      
      locker->Unlock();
  	}
  }
}

MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 07 - Sound position display using thread",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	bs = NULL;
    win = this;

	BRect r2;
	r2.Set(5,20,300,50);
    btn1 = new BButton(r2, "button1", "Play", new BMessage(M_BUTTON1));	
    AddChild(btn1);
    
    
    BRect r;
	r.Set(5,55,300,85);
    slider = new BSlider(r, "slider1", "Position", new BMessage(M_SLIDER_CHG), 1, 1000);	
    AddChild(slider);
    
    								
    locker = new BLocker();
    thread = spawn_thread(mythread, "mythread", B_STATUS_RENDERING, NULL);
    resume_thread(thread);
   
   
    UploadSoundFromFile("test.wav"); // load sound data from file into memory
    
    StartStreaming(); // start streaming at beginning of our app
}


void MainWindow::UploadSoundFromFile(char *fn)
{
   app_info *ai = new app_info();
   bp->GetAppInfo(ai);
   BPath path(&ai->ref);
   BPath ppp;
   path.GetParent(&ppp);
   ppp.Append(fn);
   LoadWavFile((char*)ppp.Path());
   delete ai;
   
   samples_cnt = ReturnPostConvSamplesCount();
   fdata = new float[samples_cnt];
   FillWithNormalizedValues(fdata);	
   
   play_pos = 0;
   playing = false;  
}

void MainWindow::StartStreaming()
{
  if (bs != NULL)
  {
    bs->StopPlaying();
    delete bs;
    bs = NULL;
  }
  
  gs_audio_format fmt;		  
  fmt.frame_rate = 44100;   // don't change frame rate
  fmt.channel_count = 1; // mono
  fmt.format = gs_audio_format::B_GS_F; // we use float format
  fmt.buffer_size = 0; // auto determine = 0
  fmt.byte_order = 2;  // don't change this
  bs = new BStreamingGameSound(2048,&fmt);
  if (bs->InitCheck() != B_OK) printf("Error at InitCheck!");
  bs->SetStreamHook(hook, NULL);
  bs->StartPlaying();	
}


void MainWindow::MessageReceived(BMessage *msg)
{
	  
	switch (msg->what)
	{
		// this event is called automatically from thread
		case M_SET_SLIDER:

		   if (locker->Lock())		  
  		   {
		     slider->SetValue((int)(play_position_relative * 1000.0f));
		     char cstr[255];
		     float pos_in_seconds = play_pos / 44100.0f;
		     sprintf(cstr, "Position [sec]: %f", pos_in_seconds);
		     BString *lbl = new BString((char*)&cstr[0]);
		     
		     slider->SetLabel(*lbl);
		     delete lbl;
		     locker->Unlock();
		   }
		  break;
		  
		// when we click on play button
		case M_BUTTON1:
		  playing = false;
		  play_pos = 0;
		  playing = true;
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
	playing = false;
    if (bs != NULL)
    {
      bs->StopPlaying();
      delete bs;
      bs = NULL;
    }	
	kill_thread(thread);
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
