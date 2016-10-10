/*****************************************************************************/
/* CAApp.h: Header file for toplevel application object.                     */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

#ifndef _CAAPP_H
#define _CAAPP_H 1

#include <menu.h>

#include <CAModel.h>


enum CASymmetry
   {
   CARectangular,
   CATriangular
   };
   
/*****************************************************************************/

class CAApp
{
public:
   CAApp();
   ~CAApp();

   virtual void Initialize(int argc, char **argv);
   virtual void Popup();
   virtual void MainLoop();
   static void Update(XtPointer data, XtIntervalId *id);
   
   static void Play(Widget w, XtPointer data, XtPointer garbage);
   static void Pause(Widget w, XtPointer data, XtPointer garbage);
   static void Back(Widget w, XtPointer data, XtPointer garbage);
   static void ChangeInterval(Widget w, XtPointer data, XtPointer garbage);
   
   static void OpenFile(Widget w, XtPointer data, XtPointer garbage);
   static void SaveFile(Widget w, XtPointer data, XtPointer garbage);
   static void About(Widget w, XtPointer data, XtPointer garbage);
   static void Exit(Widget w, XtPointer data, XtPointer garbage);

   static void SimulationOptions(Widget w, XtPointer data, XtPointer garbage);

protected:

   Widget toplevel, form, play, pause, back, interval_bar;
   CAModelList *model_list;
   XtAppContext ac;
   unsigned long  interval;
   XtIntervalId timeout_id;
   menubar menus;
   menu file_menu, options_menu, model_menu;

private:
};

/*****************************************************************************/

#endif // _CAAPP_H

