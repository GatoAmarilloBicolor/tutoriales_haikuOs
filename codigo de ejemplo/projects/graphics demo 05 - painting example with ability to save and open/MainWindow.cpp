#include "MainWindow.h"


BApplication *bp;

bool quit_req;
bool app_loaded = false;

void SetAppPointer(BApplication *bapp)
{
  bp = bapp;
}

BApplication *GetAppPointer()
{
  return bp;
}

ViewA::~ViewA()
{
}

ViewA::ViewA(int w, int h) : BView(BRect(1,20,w,h+20), "view", B_FOLLOW_LEFT | B_FOLLOW_RIGHT, B_WILL_DRAW)
{
	
}



void ViewA::MouseMoved(BPoint pnt, uint32 transit, const BMessage *msg)
{
  if (app_loaded == false) return;	
  gfx->SprayIt(pnt.x, pnt.y); // perform spraying
  Invalidate(); // force redraw of this view
}

// called automatically by system when drawing is needed
void ViewA::Draw(BRect ur)
{
  if (app_loaded == false) return;
  if (quit_req == true) return;
  gfx->Draw(this);
}





MainWindow::MainWindow(void) : BWindow(BRect(100,100,700,700),"Graphics demo 05 - Painting example with saving and loading",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	quit_req = false;
	view_format = NULL;
	fp = NULL;
    save_fmt_offset = 0;

	gfx = new Gfx("h1.png");
   
    view = new ViewA(gfx->bw, gfx->bh);
    view->gfx = gfx;
	AddChild(view);
	
		
	// Add menus
	BRect r2(Bounds());
    r2.bottom = 20;
    BMenuBar *menubar = new BMenuBar(r2,"menubar");
    AddChild(menubar);
   
	
	BMenu* menu_sett = new BMenu("Format settings");
	menubar->AddItem(menu_sett);
				
	BMenu* menu_file = new BMenu("File");
	menubar->AddItem(menu_file);
	menu_file->AddItem(new BMenuItem("Open", new BMessage(M_IMAGE_LOAD), 0));
	
			
	BMenu *menu_save = new BMenu("Save as...");//, new BMessage(M_IMAGE_SAVE), 0);
	menu_file->AddItem(menu_save);
	menu_sett->AddItem(new BMenuItem("Close image format settings", new BMessage(M_IMAGE_FORMAT_CLOSE), 0));
			



    // scan for bitmap translators for various image files and add them menus
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	translator_id *translators;
	int32 count;

	roster->GetAllTranslators(&translators, &count);
	const translation_format *format;
	int32 format_count;
		
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
		  // add menu items for adjusting format settings and for saving 					
		  menu_sett->AddItem(new BMenuItem(format[y].name, new BMessage(M_IMAGE_FORMAT + save_fmt_offset), translators[x], format[y].type));
		  menu_save->AddItem(new BMenuItem(format[y].name, new BMessage(M_IMAGE_SAVE + save_fmt_offset), translators[x], format[y].type));
				    
		  save_image_formats[save_fmt_offset] = format[y].type; // store image format index for later use when saving to file
		  settings_image_formats[save_fmt_offset] = translators[x];  // store image format index for image's format settings
		  
		  save_fmt_offset++; 
		  break;
		}
	  }
	}
	
	delete [] translators; 
	
	app_loaded = true;
	view->Invalidate(); // force view's redraw
}
	


void MainWindow::MessageReceived(BMessage *msg)
{	
  // event responder for when we click on specific format settings
  if ((msg->what >= M_IMAGE_FORMAT) && (msg->what <= M_IMAGE_FORMAT + save_fmt_offset))
  {
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	
	BRect rect(0, 0, 239, 239);
	if (view_format != NULL)
	{
	  view_format->RemoveSelf();
	  delete view_format;
	}
	status_t err = roster->MakeConfigurationView(settings_image_formats[msg->what - M_IMAGE_FORMAT], NULL, &view_format, &rect);	  
	AddChild(view_format);
	view_format->MoveTo(0,20);
	view->Hide();
	return;
  }		
  // event responder for when we click on saving to wanted file format
  else if ((msg->what >= M_IMAGE_SAVE) && (msg->what <= M_IMAGE_SAVE + save_fmt_offset))
  {
    // if format settings are open, close them otherwise app will crash
  	if (view_format != NULL)
	{
	  view_format->RemoveSelf();
	  delete view_format;
	  view_format = NULL;
	  view->Show();
	}
	  	
    save_format_id = save_image_formats[msg->what - M_IMAGE_SAVE];
    if (fp != NULL) delete fp;		
    BMessenger bmsg = BMessenger(this);
    fp = new BFilePanel(B_SAVE_PANEL,&bmsg);
    fp->Show();
    return;       
  }	  				
  // when we click on format settings close action
  else if (msg->what ==	M_IMAGE_FORMAT_CLOSE)
  {
  	// if format settings are open, close them	
  	if (view_format != NULL)
	{
	  view_format->RemoveSelf();
	  delete view_format;
	  view_format = NULL;
	  view->Show();
	}
	return;
  }	
  // when we click on load action in menu
  else if (msg->what == M_IMAGE_LOAD)
  {	
    // if format settings are open, close them	
  	if (view_format != NULL)
	{
	  view_format->RemoveSelf();
	  delete view_format;
	  view_format = NULL;
	  view->Show();
	}
	  
    if (fp != NULL) delete fp;	
    BMessenger bmsg = BMessenger(this);
    fp = new BFilePanel(B_OPEN_PANEL, &bmsg);
    fp->Show();
    return;
  }

  			
	
  switch (msg->what)
  {
  	// when save action is performed on save dialog
  	case B_SAVE_REQUESTED:
  	{
  		printf("SAVE...\n");
     	entry_ref dir_ref;
      	const char *name;
      	
       	msg->FindString("name", &name);
     	msg->FindRef("directory", &dir_ref);
     	BDirectory dir(&dir_ref); // create BDirectory instance for use with BFile
     	     	     	     	
        BTranslatorRoster *roster = BTranslatorRoster::Default();
        printf("SAVE1...\n");
        BBitmapStream *stream = new BBitmapStream(gfx->bmp); 
   
   		printf("SAVE2...\n");
   		BPath path(&dir_ref);
   		path.Append(name);
		
		// in case you need full path based filename in char* format		   		 	    
        printf("Full filename with path: %s\n", path.Path());
        
        
        // BFile instance is needed when working with Translators 
        BFile file (&dir, name, B_WRITE_ONLY | B_CREATE_FILE);
        roster->Translate(stream, NULL, NULL, &file, save_format_id);
  	    stream->DetachBitmap(&gfx->bmp);
  	    delete roster;	
  	    delete stream;
  	    break;
  	}
  	 
  	// when open action is performed on open dialog
  	case B_REFS_RECEIVED:
  	{
  	  
  	  fp->Rewind();
      entry_ref ref;
      while (fp->GetNextSelectedRef(&ref) == B_OK) 
      {
        BPath p(&ref);
        gfx->LoadImage((char*)p.Path());
        break;
      }

	  delete fp;
	  fp = NULL;
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
