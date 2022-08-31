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


// id's for GUI objects, used for event handling
enum
{
  M_SLIDER_CHG = 'slch',
  M_BUTTON1 = 'btn0',
  M_BUTTON2 = 'btn1',
  M_BUTTON3 = 'btn2',
  M_BUTTON4 = 'btn3',
  M_BUTTON5 = 'btn4'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,380),"Sound demo 01 - Apply effect before playing",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	s = NULL;
	fdata = NULL;
	
	BRect r;
	r.Set(5,5,300,55);
    slider = new BSlider(r, "slider1", "Effect parameter", new BMessage(M_SLIDER_CHG), 1, 1000);	
    AddChild(slider);

	BRect r2;
	r2.Set(5,60,300,80);
    btn1 = new BButton(r2, "button1", "Echo", new BMessage(M_BUTTON1));	
    AddChild(btn1);
		
	BRect r3;
	r3.Set(5,60+30,300,80+30);
    btn2 = new BButton(r3, "button2", "Pitch shift (slow)", new BMessage(M_BUTTON2));	
    AddChild(btn2);
    
	BRect r4;
	r4.Set(5,60+60,300,80+60);
    btn3 = new BButton(r4, "button3", "Low pass", new BMessage(M_BUTTON3));	
    AddChild(btn3);
    
    BRect r5;
	r5.Set(5,60+60+30,300,80+60+30);
    btn4 = new BButton(r5, "button4", "Tremolo", new BMessage(M_BUTTON4));	
    AddChild(btn4);
    
    BRect r6;
	r6.Set(5,60+60+60,300,80+60+60);
    btn5 = new BButton(r6, "button5", "Bandpass", new BMessage(M_BUTTON5));	
    AddChild(btn5);
    
	eff_lowpass = new LowPassEffect();
	eff_echo = new EchoEffect();
	eff_pitch = new PitchShiftEffect();
	
	eff_tremolo = new TremoloEffect();
	eff_bandpass = new BandpassEffect();
}


void MainWindow::PlaySoundWithEffect(char *fn, int effect_index)
{
   app_info *ai = new app_info();
   bp->GetAppInfo(ai);
   BPath path(&ai->ref);
   BPath ppp;
   path.GetParent(&ppp);
   ppp.Append(fn);
   LoadWavFile((char*)ppp.Path());
   delete ai;
   
   if (fdata != NULL) delete fdata;
   int samples_cnt = ReturnPostConvSamplesCount();
   fdata = new float[samples_cnt];
   FillWithNormalizedValues(fdata);	
   
   gs_audio_format fmt;
   fmt.format = gs_audio_format::B_GS_F; // use float format
   fmt.frame_rate = 44100;  
   fmt.channel_count = 1;  // mono
   fmt.byte_order = 2;     // don't change this
   fmt.buffer_size = 255;
   
   if (s != NULL)
   {
     s->StopPlaying();
     delete s;
   }
   
   
   if (effect_index == 0)
   {
     eff_echo->SetEchoAmount(slider->Value());
     eff_echo->ApplyToSound(fdata, samples_cnt);
   }
   else if (effect_index == 1)
   {		
     eff_pitch->SetPitch(slider->Value());
     eff_pitch->ApplyToSound(fdata, samples_cnt);
   }
   else if (effect_index == 2)
   {
     eff_lowpass->SetCutoffFrequency(slider->Value());
     eff_lowpass->ApplyToSound(fdata, samples_cnt);
   }
   else if (effect_index == 3)
   {
     eff_tremolo->SetFrequency(1 + slider->Value() / 200.0f);
     eff_tremolo->ApplyToSound(fdata, samples_cnt);
   }
   else if (effect_index == 4)
   {
   	 eff_bandpass->SetFrequency(10 + slider->Value() * 4);
     eff_bandpass->ApplyToSound(fdata, samples_cnt);
   }
   
   s = new BSimpleGameSound((const void*)fdata,samples_cnt, &fmt);
   s->InitCheck();
   s->StartPlaying();
   
}


void MainWindow::MessageReceived(BMessage *msg)
{
	  
	switch (msg->what)
	{
		case M_BUTTON1:
		  PlaySoundWithEffect("test.wav",0);
		  break;
		  
		case M_BUTTON2:
		  PlaySoundWithEffect("test.wav",1);
		  break;
		  
		case M_BUTTON3:
		  PlaySoundWithEffect("test.wav",2);
		  break;
		  
		case M_BUTTON4:
		  PlaySoundWithEffect("test.wav",3);
		  break;

		case M_BUTTON5:
		  PlaySoundWithEffect("test.wav",4);
		  break;
		
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
    }	
   
	be_app->PostMessage(B_QUIT_REQUESTED);
    return true;
}
