#ifndef _Req_H
#define _Req_H 1
class Req;
class ColorReq;

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <CAVisual.h>
/*****************************************************************************/

class Req
{

public:
   Req(Widget new_parent, char *new_name);
   Req();
   ~Req();

   void SetCallbacks(void (*new_ok_callback)(Req*,XtPointer),
		void (*new_cancel_callback)(Req*,XtPointer), XtPointer data); 

   void Initialize(Widget new_parent, char *new_name);

   Widget AddGroup(char *new_group,
		   WidgetClass new_widget_class = boxWidgetClass);
   Widget AddWidget(char *new_name, char *reference_name,
		    WidgetClass new_widget_class);
   Widget AddControlWidget(char *new_name, WidgetClass new_widget_class);

   Widget GetWidget(char *name, char *reference_name = NULL);
   
   void Popup();
   void Popdown();
   Boolean PoppedUp();


protected:
   Widget toplevel, form, main_area, control_area, ok_button, cancel_button;

   void (*ok_callback)(Req*,XtPointer); 
   void (*cancel_callback)(Req*,XtPointer); 
   
   static void OK(Widget w, XtPointer data, XtPointer garbage);
   static void Cancel(Widget w, XtPointer data, XtPointer garbage);

private:
   void DestroyWidgets();
   XtPointer client_data;
};

/*****************************************************************************/
class ColorReq
{
  public:
   ColorReq(CAVisual *new_visual);
   void Show();
   
   typedef struct
   {
      ColorReq *req;
      int i;
   }cl_data;
   
   
protected:
private:
   static void OK(Widget w, XtPointer data, XtPointer garbage);
   static void Blend(Widget w, XtPointer data, XtPointer garbage);

   static void Click(Widget w, XtPointer data, XtPointer garbage);
   static void Click2(Widget w, XtPointer data, XtPointer garbage);
   static void Scroll(Widget w, XtPointer data, XtPointer garbage);

   CAVisual *visual;
   Widget toplevel, owner;
   int number,selected, multiplier, mode, divider;
   Widget *w,form, box;
   Pixel *pallette, bordercolor;
   Widget scrollred, scrollgreen, scrollblue;
   enum {NormalClick, BlendClick};
   cl_data *data_array;
};

#endif // _Req_H

