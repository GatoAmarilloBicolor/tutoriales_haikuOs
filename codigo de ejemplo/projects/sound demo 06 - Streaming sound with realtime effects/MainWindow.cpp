#include "MainWindow.h"


BApplication *bp;


bool quit_req;
bool playing = false;
unsigned int play_pos = 0;
unsigned int samples_cnt;
float *fdata;
BSlider *slider_bandpass = NULL;
BSlider *slider_tremolo = NULL;
BandpassEffect *eff_bandpass = NULL;
TremoloEffect *eff_tremolo = NULL;
BCheckBox *chk_enable_second_effect = NULL;

	
				
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

  //int ppp = slider->Value();
  int float_frames_in_current_buffer = byteCount / sizeof(float);

  for (int n = 0; n < float_frames_in_current_buffer; n++)
  {
    float *buf = (float*)inBuffer;
    buf[n] = fdata[play_pos++]; // copy sound data from fdata array to output buffer
    
    if (play_pos >= samples_cnt) // when we come to end of sound, stop playing
    {
      // we change number of float frames to maximum limit of our sound 
      // (used after this loop in effect processing chain)
	  float_frames_in_current_buffer = n - 1;   
      playing = false;
      break;
    }
  }
		  
  eff_bandpass->ApplyToSound((float*) inBuffer, float_frames_in_current_buffer); // apply first effect
  
  if (chk_enable_second_effect->Value() == 1)
    eff_tremolo->ApplyToSound((float*) inBuffer, float_frames_in_current_buffer);     // apply second effect
}
   
   
   

// id's for GUI objects, used for event handling
enum
{
  M_SLIDER_BANDPASS_CHG = 'sld1',
  M_SLIDER_TREMOLO_CHG = 'sld2',
  M_BUTTON1 = 'btn0',
  M_CHECKBOX = 'chk1'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 06 - Sound streaming with realtime effects",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	bs = NULL;
	
	BRect r;
	r.Set(5,5,300,55);
    slider_bandpass = new BSlider(r, "slider1", "Bandpass frequency", new BMessage(M_SLIDER_BANDPASS_CHG), 1, 1000);	
    AddChild(slider_bandpass);
	slider_bandpass->SetValue(500);

	BRect r2;
	r2.Set(5,5+40,300,55+40);
    slider_tremolo = new BSlider(r2, "slider2", "Tremolo frequency", new BMessage(M_SLIDER_TREMOLO_CHG), 10, 1000);	
    AddChild(slider_tremolo);
    slider_tremolo->SetValue(500);

	BRect r3;
	r3.Set(5,60+60,300,80+60);
    btn1 = new BButton(r3, "button1", "Play", new BMessage(M_BUTTON1));	
    AddChild(btn1);
    
    
	chk_enable_second_effect = new BCheckBox(BRect(5, 55+30, 300, 55+40+10),"chk1","Enable second effect", new BMessage(M_CHECKBOX));
	AddChild(chk_enable_second_effect);
    
    
    eff_bandpass = new BandpassEffect();
	eff_tremolo = new TremoloEffect();
    

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
		 
		case M_SLIDER_BANDPASS_CHG:
		  eff_bandpass->SetFrequency(slider_bandpass->Value() * 5);
		  break;		
	
		case M_SLIDER_TREMOLO_CHG:
		  eff_tremolo->SetFrequency(2 + slider_tremolo->Value() / 99.0f);
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
	playing =  false;
	if (bs != NULL)
	{
	  bs->StopPlaying();
	  delete bs;
      bs = NULL;
    }
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
