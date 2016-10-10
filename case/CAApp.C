/*****************************************************************************/
/* CAApp.h: Header file for toplevel application object.                     */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

#include <conf.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Scrollbar.h>
#include <toolkit.h>
#include <CAApp.h>
#include "play.xbm"
#include "pause.xbm"
#include "back.xbm"
#include "menu.h"
#include <stdlib.h>

/*****************************************************************************/

CAApp::CAApp()
{
model_list = new CAModelList;
interval = 300;
timeout_id = 0;
}

/*****************************************************************************/

CAApp::~CAApp()
{
 delete model_list;
}
/*****************************************************************************/

void CAApp::Initialize(int argc, char **argv)

{
toplevel = XtVaOpenApplication(&ac,"CAApp", NULL, 0, &argc,
				argv, NULL, sessionShellWidgetClass,
				NULL);

form = XtVaCreateManagedWidget("form", formWidgetClass, toplevel,
				NULL);
menus.Initialize(form);
file_menu = menus.AddMenu("File");
options_menu = menus.AddMenu("Options");
model_menu = menus.AddMenu("Model");

// file_menu.AddItem("Open", OpenFile, (XtPointer)this);
// file_menu.AddItem("Save", SaveFile, (XtPointer)this);
// file_menu.AddItem("About", About, (XtPointer)this);

file_menu.AddItem("Exit", Exit, (XtPointer)this);

options_menu.AddItem("Simulation", SimulationOptions, (XtPointer)this);

back = XtVaCreateManagedWidget("back", commandWidgetClass, form,
				XtNfromVert, menus.GetWidget(),
				NULL);

XtAddCallback(back, XtNcallback, Back, (XtPointer) this); 
 
play = XtVaCreateManagedWidget("play", commandWidgetClass, form,
				XtNfromVert, menus.GetWidget(),
				XtNfromHoriz, back,
				NULL);
XtAddCallback(play, XtNcallback, Play, this); 

pause = XtVaCreateManagedWidget("pause", commandWidgetClass, form,
				 XtNfromVert, menus.GetWidget(),
				 XtNfromHoriz, play,
				 NULL);
XtAddCallback(pause, XtNcallback, Pause, this); 

interval_bar = XtVaCreateManagedWidget("interval", scrollbarWidgetClass, form,
					XtNfromVert, play,
					XtNorientation, XtorientHorizontal,
					XtNlength, 200,
					XtNwidth, 200,
					NULL);
XtAddCallback(interval_bar, XtNjumpProc, ChangeInterval, this);
}

/*****************************************************************************/

void CAApp::Popup()
{
XtRealizeWidget(toplevel);
AddBitmapToWidget(back, back_width, back_height, (char *) back_bits);
AddBitmapToWidget(play, play_width, play_height, (char *)play_bits);
AddBitmapToWidget(pause, pause_width, pause_height, (char *)pause_bits);
}

/*****************************************************************************/

void CAApp::MainLoop()
{
XtAppMainLoop(ac);
}

/*****************************************************************************/

void CAApp::Update(XtPointer data, XtIntervalId *id)

{
CAApp *app = (CAApp*) data;
CAModel *current;
 
app->model_list->Reset();
while (current = app->model_list->Next())
   {
   current->Update();
   }

app->timeout_id = XtAppAddTimeOut(app->ac, app->interval, CAApp::Update, app);
}

/*****************************************************************************/

void CAApp::Play(Widget w, XtPointer data, XtPointer garbage)

{ CAApp *app = (CAApp*) data;

if (!app->timeout_id)
   {
   app->timeout_id = XtAppAddTimeOut(app->ac, app->interval, CAApp::Update, app);
   }
}

/*****************************************************************************/

void CAApp::Pause(Widget w, XtPointer data, XtPointer garbage)

{ CAApp *app = (CAApp*) data;

if (app->timeout_id)
   {
   XtRemoveTimeOut(app->timeout_id);
   app->timeout_id = 0;
   }
}

/*****************************************************************************/

void CAApp::Back(Widget w, XtPointer data, XtPointer garbage)
{ 
}

/*****************************************************************************/

void CAApp::ChangeInterval(Widget w, XtPointer data, XtPointer garbage)

{ float val;
  CAApp *app = (CAApp*) data;

XtVaGetValues(app->interval_bar,
	      XtNtopOfThumb, &val,
	      NULL);

app->interval = (unsigned long) (val * 1000);

if (app->interval<5)
   {
   app->interval = 5;
   }
}

/*****************************************************************************/

void CAApp::OpenFile(Widget w, XtPointer data, XtPointer garbage)
{
 
}

/*****************************************************************************/

void CAApp::SaveFile(Widget w, XtPointer data, XtPointer garbage)
{
 
}

/*****************************************************************************/

void CAApp::About(Widget w, XtPointer data, XtPointer garbage)
{
 
}

/*****************************************************************************/

void CAApp::Exit(Widget w, XtPointer data, XtPointer garbage)
{
 CAApp *app = (CAApp*) data;
 XtDestroyWidget(app->toplevel);
 exit(0);
}

/*****************************************************************************/

void CAApp::SimulationOptions(Widget w, XtPointer data, XtPointer garbage)
{
 
}

/*****************************************************************************/







