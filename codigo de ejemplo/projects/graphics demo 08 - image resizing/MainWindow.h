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


// use this class for drawing
class ViewA: public BView
{
public: 
  ViewA(int w, int h);
  ~ViewA();
  void Draw(BRect ur);
  Gfx *gfx; // pointer to graphic's effect "factory"
};



class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			void 	    FrameResized(float w, float h);
			Gfx *gfx;
			BSlider *slider; // pointer to slider GUI object
			ViewA *view;     // pointer to view object 
private:


};

#endif
