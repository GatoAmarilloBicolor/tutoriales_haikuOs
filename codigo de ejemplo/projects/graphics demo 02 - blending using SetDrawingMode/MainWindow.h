#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include "Gfx.h"

// We use inherited class from BView to be able to use our own drawing method
class ViewA : public BView
{
public:  
  ViewA(int bw_in, int bh_in);
  ~ViewA();
  									
  void Draw(BRect br); // this method which we override from BView will be called automatically
  Gfx *gfx;
};

class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			Gfx *gfx;
			ViewA *view;
	
			
private:
};

#endif
