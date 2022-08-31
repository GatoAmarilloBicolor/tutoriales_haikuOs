#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "Gfx.h"

#include <Directory.h>
#include <Entry.h>
#include <Node.h>
#include <Path.h>
#include <File.h>
#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include <translation/TranslatorRoster.h>
#include <translation/TranslatorAddOn.h>
#include <translation/TranslatorFormats.h>
#include <storage/FilePanel.h>
#include <translation/BitmapStream.h>


// used for drawing onto window
class ViewA : public BView
{
public:  
  ViewA(int bw_in, int bh_in);
  ~ViewA();
  
  void Draw(BRect br);
  Gfx *gfx; // pointer to our core graphic's drawer
};



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			Gfx *gfx;
			ViewA *view;    // used to draw content 
			BFilePanel *fp; // pointer to file dialog panel
						
			int save_image_formats[1024]; // stores Translator's image format indices
			int save_format_id; // used when saving
			int save_fmt_offset; // stores last possible index of picture file format 
			
private:
};

#endif
