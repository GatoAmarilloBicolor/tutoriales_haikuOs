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
#include "Effects.h"

void SetAppPointer(BApplication *bapp);
BApplication *GetAppPointer();



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			void 		PlaySoundWithEffect(char *fn, int effect_index);

			BSlider *slider;
			BButton *btn1;
			BButton *btn2;
			BButton *btn3;
			BButton *btn4;
			BButton *btn5;
			
			BSimpleGameSound *s;
			float *fdata;
			
			LowPassEffect* eff_lowpass;
			EchoEffect* eff_echo;
			PitchShiftEffect* eff_pitch;
			
			TremoloEffect *eff_tremolo;
			BandpassEffect *eff_bandpass;

private:


};

#endif
