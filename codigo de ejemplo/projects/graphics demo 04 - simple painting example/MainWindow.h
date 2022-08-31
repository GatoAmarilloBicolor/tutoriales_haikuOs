#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <Bitmap.h>
#include <Slider.h>
#include <translation/TranslationUtils.h>
#include <Application.h>
#include <Screen.h>
#include <Alert.h>
#include <Gfx.h>

#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>

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


class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
			Gfx *gfx;
			BSlider *slider;  // slider GUI object
			ViewA *view;      // used for drawing to window

};

#endif
