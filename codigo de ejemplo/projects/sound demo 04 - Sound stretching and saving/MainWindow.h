#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>
#include <Button.h>


#include <cstdlib>
#include <iostream>


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "LibWAV.h"
#include <game/SimpleGameSound.h>
#include <Path.h>
#include <app/Roster.h>

#include <stdio.h>
#include <stdlib.h>


void SetAppPointer(BApplication *bapp);
BApplication *GetAppPointer();



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			void 		PlaySoundWithEffect(char *fn, int effect_index);
			void 		Stretch();
			BSlider *slider;
			BButton *btn1;
			BButton *btn2;
			
			BSimpleGameSound *s;
			float *fdata;         // contains sound from original file
			int samples_cnt_out;  // number of samples for stretched sound
			int samples_cnt; // number of samples from sound file
			float *fdata_out;  // contains altered sound data
			

private:


};

#endif
