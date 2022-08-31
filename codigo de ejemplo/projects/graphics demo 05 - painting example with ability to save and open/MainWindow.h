#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <translation/TranslatorRoster.h>
#include <translation/TranslatorAddOn.h>
#include <translation/TranslatorFormats.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>
#include <Gfx.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <FilePanel.h>
#include <storage/FilePanel.h>
#include <translation/BitmapStream.h>
#include <app/Messenger.h>
#include <Directory.h>
#include <Entry.h>
#include <Node.h>
#include <Path.h>
#include <File.h>
#include <String.h>
#include <stdio.h>
#include <stdlib.h>
		
void SetAppPointer(BApplication *bapp);
BApplication *GetAppPointer();

class ViewA: public BView
{
public: 
  ViewA(int w, int h);
  ~ViewA();
  void Draw(BRect ur);
  void MouseMoved(BPoint pnt, uint32 transit, const BMessage *msg);
  Gfx *gfx;
};





// id's for our menu actions (used in event responder)	
const int M_IMAGE_FORMAT = 111;
const int M_IMAGE_SAVE = 211;
const int M_IMAGE_LOAD = 311;
const int M_IMAGE_FORMAT_CLOSE = 411;


class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			Gfx *gfx;     
			BSlider *slider;
			ViewA *view;
			BView *view_format;
			BFilePanel *fp;
			
private:

  int save_format_id; // used when choosing format to save picture
  int save_fmt_offset; // maximum number of formats that can be saved
  int settings_image_formats[255];  // indices for image formats that have settings
  int save_image_formats[255];      // indices for image formats that can be saved
};

#endif
