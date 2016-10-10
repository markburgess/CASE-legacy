#include <CAReq.h>
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Toggle.h>
#include <X2Dview.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <color.h>

Req::Req(Widget new_parent, char *new_name)
{
 toplevel = NULL;
 ok_callback = NULL;
 cancel_callback = NULL;
 Initialize(new_parent, new_name);
}

/*****************************************************************************/

Req::Req()
{
 toplevel = NULL;
 ok_callback = NULL;
 cancel_callback = NULL;
}

/*****************************************************************************/

Req::~Req()
{
 if (toplevel!=NULL)
    {
    XtDestroyWidget(toplevel);
    }
}

/*****************************************************************************/

void Req::SetCallbacks(void (*new_ok_callback)(Req*,XtPointer),
	     void (*new_cancel_callback)(Req*,XtPointer), XtPointer data)
{
 ok_callback = new_ok_callback;
 cancel_callback = new_cancel_callback;
 client_data = data;
}

/*****************************************************************************/

void Req::Initialize(Widget new_parent, char *new_name)
{
 if (toplevel!=NULL)
    {
    DestroyWidgets();
    }
 
 toplevel = XtVaAppCreateShell(NULL, new_name, applicationShellWidgetClass,
			       XtDisplay(new_parent),
			       XtNtitle, new_name,
			       NULL);

 form = XtVaCreateManagedWidget("form", formWidgetClass, toplevel, 
				NULL);
 main_area = XtVaCreateManagedWidget("main_area", boxWidgetClass, form, 
				     XtNbottom, XtChainBottom,
				     XtNright, XtChainRight,
				     XtNtop, XtChainTop,
				     XtNleft, XtChainLeft,
				     NULL);
 control_area = XtVaCreateManagedWidget("control_area", boxWidgetClass, form, 
					XtNfromVert, main_area,
					XtNvertDistance, 20,
					XtNwidth, 600,
					XtNborderWidth, 0,
//					XtNorientation, XtorientVertical,
//					XtNheight, 10,
					XtNbottom, XtChainBottom,
					XtNright, XtChainLeft,
					XtNtop, XtChainBottom,
					XtNleft, XtChainLeft,
					NULL);
 ok_button = XtVaCreateManagedWidget("ok", commandWidgetClass, control_area, NULL);
 cancel_button = XtVaCreateManagedWidget("cancel", commandWidgetClass,
					 control_area, NULL);

 XtAddCallback(ok_button, XtNcallback, &Req::OK, this);
 XtAddCallback(cancel_button, XtNcallback, &Req::Cancel, this);
}

/*****************************************************************************/

Widget Req::AddGroup(char *new_group, WidgetClass new_widget_class)
{
 Widget current;
 current = XtVaCreateManagedWidget(new_group, new_widget_class, main_area, 
				   NULL);
 return current;
}

/*****************************************************************************/

Widget Req::AddWidget(char *new_name, char *reference_name,
			WidgetClass new_widget_class)
{
 Widget current, reference;

 if (reference_name == NULL)
    {
    reference = main_area;
    }
 else
    {
    reference = XtNameToWidget(main_area, reference_name);
    }
 
 current = XtVaCreateManagedWidget(new_name, new_widget_class, reference, 
				   NULL);
 return current;
}

/*****************************************************************************/

Widget Req::AddControlWidget(char *new_name, WidgetClass new_widget_class)
{
 Widget current;

 current = XtVaCreateManagedWidget(new_name, new_widget_class, control_area, 
				   NULL);
 return current;
}

/*****************************************************************************/

Widget Req::GetWidget(char *name, char *reference_name)
{
 Widget current, reference;
 if (reference_name == NULL)
    {
    reference = main_area;
    }
 else
    {
    reference = XtNameToWidget(main_area, reference_name);
    }
 
 current = XtNameToWidget(reference, name);
 return(current);
}

/*****************************************************************************/

void Req::Popup()
{
 if (toplevel!=NULL)
    {
    XtRealizeWidget(toplevel);
    }
}

/*****************************************************************************/

void Req::Popdown()
{
 if (toplevel!=NULL)
    {
    XtUnrealizeWidget(toplevel);
    }
}

/*****************************************************************************/

Boolean Req::PoppedUp()
{
 return XtIsRealized(toplevel);
}

/*****************************************************************************/

void Req::DestroyWidgets()
{
}

/*****************************************************************************/

void Req::OK(Widget w, XtPointer data, XtPointer garbage)
{
 Req *req = (Req *) data;
 req->ok_callback(req,req->client_data); 
 XtUnrealizeWidget(req->toplevel);
}

/*****************************************************************************/

void Req::Cancel(Widget w, XtPointer data, XtPointer garbage)
{
 Req *req = (Req *) data;
 req->cancel_callback(req,req->client_data); 
 XtUnrealizeWidget(req->toplevel);
}

/*****************************************************************************/
/* COLOR REQUESTER object                                                    */
/*****************************************************************************/

ColorReq::ColorReq(CAVisual *new_visual)
{
 visual = new_visual;
 multiplier=256;
 divider=1024;
 mode = NormalClick;
} 

/****************************************************************************/

void ColorReq::Show()
{
 int i;
 number = visual->GetAllocedColors();
 
 if (number == 0)
    {
    return;
    }
 
 owner = visual->GetGfxWidget();
 Widget blend_button, ok;

 data_array = new cl_data[number]; 
 w = new Widget[number];

 toplevel = XtVaAppCreateShell(NULL, "color_popup",
			       applicationShellWidgetClass,
			       XtDisplay(owner), NULL);
			      
 form = XtVaCreateManagedWidget("form", formWidgetClass, toplevel, 
				NULL);

 box = XtVaCreateManagedWidget("box", boxWidgetClass, form,
			       XtNwidth, 200,
			       NULL);


 for(i=0; i<number; i++)
    {
    w[i] = XtVaCreateManagedWidget("color", commandWidgetClass,
				   box, NULL);

    data_array[i].req = this; 
    data_array[i].i = i;
    XtVaSetValues(w[i],
		  XtNbackground, (Pixel) X2DgetForeground(owner, i),
		  XtNlabel, "",
		  XtNwidth, 10,
		  XtNheight, 10,
		  NULL);

    XtAddCallback(w[i], XtNcallback, Click, &data_array[i]);
    XtAddCallback(w[i], XtNcallback, Click2, &data_array[i]);

    }
 XtVaGetValues(w[0],
	       XtNborder, &bordercolor,
	       NULL);
 
 scrollred = XtVaCreateManagedWidget("scrollred", scrollbarWidgetClass,
				     form,
				     XtNfromVert, box,
				     XtNorientation, XtorientHorizontal,
				     XtNlength, 200,
				     XtNwidth, 200,
				     XtVaTypedArg, XtNforeground, XtRString,
				     "red",sizeof("red")+1,
				     NULL);


 scrollgreen = XtVaCreateManagedWidget("scrollgreen", scrollbarWidgetClass,
				       form,
				       XtNfromVert, scrollred,
				       XtNorientation, XtorientHorizontal,
				       XtNlength, 200,
				       XtNwidth, 200,
				       XtVaTypedArg, XtNforeground, XtRString,
				       "green", sizeof("green")+1,
				       NULL);

 scrollblue = XtVaCreateManagedWidget("scrollblue", scrollbarWidgetClass,
				      form,
				      XtNfromVert, scrollgreen,
				      XtNorientation, XtorientHorizontal,
				      XtNlength, 200,
				      XtNwidth, 200,
				      XtVaTypedArg, XtNforeground, XtRString,
				      "blue", sizeof("blue")+1,
				      NULL);

 ok = XtVaCreateManagedWidget("ok", commandWidgetClass,
			      form,
			      XtNfromVert, scrollblue,
			      NULL);

 blend_button = XtVaCreateManagedWidget("blend", commandWidgetClass,
				form,
				XtNfromVert, scrollblue,
				XtNfromHoriz, ok,
				NULL);

 XtAddCallback(scrollred, XtNjumpProc, Scroll, this);
 XtAddCallback(scrollgreen, XtNjumpProc, Scroll, this);
 XtAddCallback(scrollblue, XtNjumpProc, Scroll, this);

 XtAddCallback(ok, XtNcallback, OK, this);
 XtAddCallback(blend_button, XtNcallback, &ColorReq::Blend, (XtPointer) this);

 XtRealizeWidget(toplevel);

 selected = -1;
}

/************************************************************************/

void ColorReq::OK(Widget w, XtPointer data, XtPointer garbage)
{
 ColorReq *req = (ColorReq *)data;
 XtUnrealizeWidget(req->toplevel);
 XtDestroyWidget(req->toplevel);
 req->visual->color_requester_ok();
 delete req->data_array;
 delete req->w;
}

/************************************************************************/

void ColorReq::Blend(Widget w, XtPointer data, XtPointer garbage)
{
 ColorReq *req = (ColorReq *)data;
 req->mode=BlendClick;
}

/************************************************************************/

void ColorReq::Click(Widget w, XtPointer data, XtPointer garbage)
{
 XColor col;
 cl_data *client_data = (cl_data*) data;
 ColorReq *req = client_data->req;
 char str[40];
 float val;
 XtArgVal *l_val;
    
 if (req->mode!=NormalClick)
    {
    return;
    }
 if (req->selected>=0)
    {
    XtVaSetValues(req->w[req->selected],
		  XtNborderColor, req->bordercolor,
		  NULL);
    }

 req->selected = client_data->i;
 XtVaSetValues(req->w[req->selected],
	       XtVaTypedArg, XtNborderColor, XtRString, "red", sizeof("red"+1),
	       NULL);


 col.pixel=X2DgetForeground(req->owner, client_data->i);
 XQueryColor(
     XtDisplay(req->owner),
     DefaultColormap(XtDisplay(req->owner),
		     DefaultScreen(XtDisplay(req->owner))),
     &col);

 val = col.red/65536.0;
 l_val = (XtArgVal *) &val;

 if (sizeof(float) > sizeof(XtArgVal))
    {
    XtVaSetValues(req->scrollred,
		  XtNtopOfThumb, &val,
		  NULL);
    }
 else
    {
    XtVaSetValues(req->scrollred,
		  XtNtopOfThumb, *l_val,
		  NULL);
    }

 val = col.green/65536.0;
 l_val = (XtArgVal *) &val;

 if (sizeof(float) > sizeof(XtArgVal))
    {
    XtVaSetValues(req->scrollgreen,
		  XtNtopOfThumb, &val,
		  NULL);
    }
 else
    {
    XtVaSetValues(req->scrollgreen,
		  XtNtopOfThumb, *l_val,
		  NULL);
    }

 val = col.blue/65536.0;
 l_val = (XtArgVal *) &val;
 if (sizeof(float) > sizeof(XtArgVal))
    {
    XtVaSetValues(req->scrollblue,
		  XtNtopOfThumb, &val,
		  NULL);
    }
 else
    {
    XtVaSetValues(req->scrollblue,
		  XtNtopOfThumb, *l_val,
		  NULL);
    }
   
}    

/************************************************************************/

void ColorReq::Click2(Widget w, XtPointer data, XtPointer garbage)
{
 XColor col, from_col, to_col;
 cl_data *client_data = (cl_data*) data;
 ColorReq *req = client_data->req;
 float val;
 XtArgVal *l_val;
 int to, from,i;
 
 if (req->mode!=BlendClick)
    {
    return;
    }

 if (req->selected < 0)
    {
    return;
    }
 
 if (client_data->i > req->selected)
    {
     to = client_data->i;
     from = req->selected;
    }
 else if (client_data->i < req->selected)
    {
     to = req->selected;
     from = client_data->i;
    }
 else
    {
    return;
    }

 from_col.pixel = X2DgetForeground(req->owner, from);
 
 XQueryColor(
     XtDisplay(req->owner),
     DefaultColormap(XtDisplay(req->owner),
		     DefaultScreen(XtDisplay(req->owner))),
     &from_col);

 to_col.pixel = X2DgetForeground(req->owner, to);
 XQueryColor(
     XtDisplay(req->owner),
     DefaultColormap(XtDisplay(req->owner),
		     DefaultScreen(XtDisplay(req->owner))),
     &to_col);
 
 blend(req->owner, from, from_col.red/65536.0,
       from_col.green/65536.0, from_col.blue/65536.0,
       to, to_col.red/65536.0, to_col.green/65536.0,
       to_col.blue/65536.0);

 for (i=from; i<to; i++)
    {
    XtVaSetValues(req->w[i],
		  XtNbackground, (Pixel) X2DgetForeground(req->owner, i),
		  NULL);
    }

 req->mode=NormalClick;
}

/************************************************************************/

void ColorReq::Scroll(Widget w, XtPointer data, XtPointer percent_ptr)
{
 ColorReq *req = (ColorReq *)data;
 XColor col;
 float val;
 if (req->selected < 0)
    {
    return;
    }
 
 XtVaGetValues(req->scrollred,
	       XtNtopOfThumb, &val,
	       NULL);

 col.red= (unsigned int) (val*req->multiplier);

 XtVaGetValues(req->scrollgreen,
	       XtNtopOfThumb, &val,
	       NULL);

 col.green= (unsigned int) (val*req->multiplier);

 XtVaGetValues(req->scrollblue,
	       XtNtopOfThumb, &val,
	       NULL);

 col.blue= (unsigned int) (val*req->multiplier);

 X2DsetPaletteRGB(req->owner, req->selected, col.red, col.green, col.blue);

 XtVaSetValues(req->w[req->selected],
	       XtNbackground, (Pixel) X2DgetForeground(req->owner,
						       req->selected),
	       NULL);
}






