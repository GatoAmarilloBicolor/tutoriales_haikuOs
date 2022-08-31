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

#include <game/StreamingGameSound.h>
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

			BSlider *slider;
			BButton *btn1;
			BButton *btn2;
			
		    BStreamingGameSound *bs;
    		gs_audio_format fmt;
			
private:


};

#endif
