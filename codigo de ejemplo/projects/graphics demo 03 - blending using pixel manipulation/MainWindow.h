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

void SetAppPointer(BApplication *bapp);
BApplication *GetAppPointer();



class ViewA: public BView
{
public: 
  ViewA(int w, int h);
  ~ViewA();
  void Draw(BRect ur);
  Gfx *gfx;
};



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			Gfx *gfx;        // pointer to graphics processing object
			BSlider *slider; // pointer to slider GUI boject
			ViewA *view;     // pointer to view, which we use for drawing 
private: 


};

#endif
