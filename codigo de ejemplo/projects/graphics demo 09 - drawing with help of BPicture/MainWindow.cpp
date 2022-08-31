#include "MainWindow.h"
#include <Application.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>

bool app_loaded = false;

ViewA::ViewA(int bw_in, int bh_in) : BView(BRect(1,20,bw_in,bh_in+20), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
 
}

ViewA::~ViewA(void)
{
 
}


void ViewA::Draw(BRect r)
{
  if (app_loaded == false) return;	
  gfx->Draw(this);
}

// define id's for menu event handling
const int M_SET_ADD = 111;
const int M_SET_SUB = 112;
const int M_SET_MIN = 113;
const int M_SET_MAX = 114;
const int M_SET_COPY = 115;
				
							
// define the first id of all image formats that are displayed as children of "Save as..." menu
const int M_IMAGE_SAVE = 555; 


MainWindow::MainWindow(void): BWindow(BRect(100,100,700,700),"Graphics demo 09 - Drawing with help of BPicture",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
    fp =  NULL;
    gfx = new Gfx("h1.png"); // load background picture
    
    view = new ViewA(gfx->bw, gfx->bh);
	AddChild(view);
	view->gfx = gfx;
	
	
	// Add menus
    BRect r2(Bounds());
    r2.bottom = 20;
    BMenuBar *menuBar = new BMenuBar(r2,"menubar");
    AddChild(menuBar);
    
    BMenu *menu = new BMenu("Blending mode");
    menu->AddItem(new BMenuItem("Add", new BMessage(M_SET_ADD),'A'));
    menu->AddItem(new BMenuItem("Subtract", new BMessage(M_SET_SUB),'S'));
    menu->AddItem(new BMenuItem("Min", new BMessage(M_SET_MIN),'M'));
    menu->AddItem(new BMenuItem("Max", new BMessage(M_SET_MAX),'X'));
    menu->AddItem(new BMenuItem("Copy", new BMessage(M_SET_COPY),'C'));
    
    menuBar->AddItem(menu);
    
    BMenu *menuFile = new BMenu("File");
	BMenu *menu_save = new BMenu("Save as...");
	menuFile->AddItem(menu_save);
    menuBar->AddItem(menuFile);


    // scan for bitmap translators for various image files and add them menus
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	translator_id *translators;
	int32 count;

	roster->GetAllTranslators(&translators, &count);
	const translation_format *format;
	int32 format_count;
	
	save_fmt_offset = 0;
	for (int x = 0; x < count; x++) 
	{
	  // Determine which formats this one can write
	  roster->GetOutputFormats(translators[x], &format, &format_count);
	  for (int y = 0; y < format_count; y++) 
	  {
	    // Check if this is an image translator
		if (format[y].group == B_TRANSLATOR_BITMAP) 
		{
          // If this format saves to some native format build a menu item for it
		  if (format[y].type == B_TRANSLATOR_BITMAP) 
		  {
		  	continue;
		  }
					
		  menu_save->AddItem(new BMenuItem(format[y].name, new BMessage(M_IMAGE_SAVE + save_fmt_offset), translators[x], format[y].type));
				    
		  save_image_formats[save_fmt_offset] = format[y].type; 
		  save_fmt_offset++; 
		  
		  break;
		}
	  }
	}
	
	delete [] translators;

    app_loaded = true;			    
    view->Invalidate(); // force redraw of view
}




void MainWindow::MessageReceived(BMessage *msg)
{
  // show file dialog form when clicking on one of the file formats in Save submenu
  if ((msg->what >= M_IMAGE_SAVE) && (msg->what <= M_IMAGE_SAVE + save_fmt_offset))
  {
    save_format_id = save_image_formats[msg->what - M_IMAGE_SAVE];
    
    if (fp != NULL) delete fp; // if panel was created before, delete it
    
    BMessenger bmsg = BMessenger(this);
    fp = new BFilePanel(B_SAVE_PANEL, &bmsg);
    fp->Show(); // show saving panel
  }	  
  
  switch (msg->what)
  {
  	 // M_SET_*  are used when clicking on menu, choosing blending modes
	 case M_SET_ADD: 
		 gfx->blend_mode = BLEND_ADD;
		 view->Invalidate(); // force view to redraw
		 break;
	  
	case M_SET_SUB:
		 gfx->blend_mode = BLEND_SUB;
		 view->Invalidate();
		 break;

	case M_SET_MIN:
		 gfx->blend_mode = BLEND_MIN;
		 view->Invalidate();
		 break;
		  
	case M_SET_MAX:
	    gfx->blend_mode = BLEND_MAX;
		view->Invalidate();
		break;
		  
	case M_SET_COPY:
	 
		gfx->blend_mode = BLEND_COPY;
		view->Invalidate();
	    break;
		  
  	// when save action is performed on save dialog
  	case B_SAVE_REQUESTED:
  	{
     	entry_ref dir_ref;
      	const char *name;
      	
       	msg->FindString("name", &name);
     	msg->FindRef("directory", &dir_ref);
     	BDirectory dir(&dir_ref); // create BDirectory instance for use with BFile
     	     	     	     	
        BTranslatorRoster *roster = BTranslatorRoster::Default();
        BBitmapStream *stream = new BBitmapStream(gfx->bmp_offscr); 
   
   		
   		BPath path(&dir_ref);
   		path.Append(name);
		
		// in case you need full path based filename in char* format		   		 	    
        printf("Full filename with path: %s\n", path.Path());
        
        
        // BFile instance is needed when working with Translators 
        BFile file (&dir, name, B_WRITE_ONLY | B_CREATE_FILE);
        roster->Translate(stream, NULL, NULL, &file, save_format_id);
  	    stream->DetachBitmap(&gfx->bmp_offscr);
  	    delete roster;	
  	    delete stream;
  	    
  	    break;
  	}		  		  		
  	  
	default:
	{
	    BWindow::MessageReceived(msg);
	    break;
	}
  }
}



bool MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
