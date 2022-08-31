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
#include <game/StreamingGameSound.h>
#include <Slider.h>
#include <kernel/OS.h>
#include <kernel/scheduler.h>
#include <support/Autolock.h>
#include <Locker.h>
#include <String.h>

void SetAppPointer(BApplication *bapp);
BApplication *GetAppPointer();



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			void		UploadSoundFromFile(char *fn);
			void 	    StartStreaming();
			
			
			BButton *btn1;
			BStreamingGameSound *bs;
			

private:


};

#endif
