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

// custom id's for tracking events of GUI objects
enum
{
  M_SLIDER_CHG = 'slch',
  M_BUTTON1 = 'btn0',
  M_BUTTON2 = 'btn1'
};



MainWindow::MainWindow(void) : BWindow(BRect(100,100,400,260),"Sound demo 04 - Sound stretching",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	s = NULL;
	fdata = NULL;
	fdata_out = NULL;
	
	BRect r;
	r.Set(5,5,300,55);
    slider = new BSlider(r, "slider1", "Time stretch", new BMessage(M_SLIDER_CHG), 1, 100);	
    AddChild(slider);
	slider->SetValue(30);
	
	BRect r2;
	r2.Set(5,60,300,80);
    btn1 = new BButton(r2, "button1", "Play", new BMessage(M_BUTTON1));	
    AddChild(btn1);
		
	BRect r3;
	r3.Set(5,60+30,300,80+30);
    btn2 = new BButton(r3, "button2", "Save to output.wav", new BMessage(M_BUTTON2));	
    AddChild(btn2);
    
    
}

// linear interpolation function
float lerp(float i1, float i2, float i3)
{
  return i1 + (i2 - i1) * i3;
}

void MainWindow::Stretch()
{
  int samples_cnt_in = samples_cnt;
  samples_cnt_out = samples_cnt / (float)((8+slider->Value()) / 50.0f);
  if (fdata_out != NULL) delete [] fdata_out;
  fdata_out = new float[samples_cnt_out];
  
  for (int n = 0; n < samples_cnt_out; n++)
  {		
    float pos_proc = n / (float) samples_cnt_out;
	float pos_proc_from_in = pos_proc * samples_cnt_in;
	float lerp_amount = ((int)pos_proc_from_in)+1  - pos_proc_from_in;
    float final_data = lerp(fdata[(int)pos_proc_from_in], fdata[((int)pos_proc_from_in)+1], lerp_amount);
    fdata_out[n] = final_data;
  }

}
void MainWindow::PlaySoundWithEffect(char *fn, int effect_index)
{
   // load given filename from current app path
   app_info *ai = new app_info();
   bp->GetAppInfo(ai);
   BPath path(&ai->ref);
   BPath ppp;
   path.GetParent(&ppp);
   ppp.Append(fn);
   LoadWavFile((char*)ppp.Path());
   delete ai;
   
   
   samples_cnt = ReturnPostConvSamplesCount(); // return number of float samples
   if (fdata != NULL) delete [] fdata;
   fdata = new float[samples_cnt];
   FillWithNormalizedValues(fdata);	
   
   gs_audio_format fmt;
   fmt.format = gs_audio_format::B_GS_F;
   fmt.frame_rate = 44100;
   fmt.channel_count = 1;
   fmt.byte_order = 2;
   fmt.buffer_size = 255;
   
   if (s != NULL)
   {
     s->StopPlaying();
     delete s;
   }
   
   if (effect_index == 0)
   {   
     Stretch();   
   }
   else
   {
     // save
     Stretch();
     
     // load reference wave from current path to set output format
     app_info *ai = new app_info();
     bp->GetAppInfo(ai);
     BPath path(&ai->ref);
     BPath ppp;
     path.GetParent(&ppp);
     ppp.Append("ref.wav");
     LoadWavFile((char*)ppp.Path());
     delete ai;     
     
     SetSamples((byte*) fdata_out, samples_cnt_out * 4);
     
     // save sound to current path (as output.wav)
     ai = new app_info();
     bp->GetAppInfo(ai);
     BPath path2(&ai->ref);
     BPath ppp2;
     path2.GetParent(&ppp2);
     ppp2.Append("output.wav");
     SaveWavFile((char*)ppp2.Path());
     delete ai;     
     return;
   }


   s = new BSimpleGameSound((const void*)fdata_out,samples_cnt_out, &fmt);
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
