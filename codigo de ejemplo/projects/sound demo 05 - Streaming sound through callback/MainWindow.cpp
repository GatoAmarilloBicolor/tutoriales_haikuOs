#include "MainWindow.h"

BApplication *bp;


bool quit_req;
bool playing = false;
unsigned int play_pos = 0;
unsigned int samples_cnt;
float *fdata;

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
  M_BUTTON1 = 'btn0'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 05 - Streaming sound through callback",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	bs = NULL;


	BRect r2;
	r2.Set(5,20,300,50);
    btn1 = new BButton(r2, "button1", "Play", new BMessage(M_BUTTON1));	
    AddChild(btn1);
    
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
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
