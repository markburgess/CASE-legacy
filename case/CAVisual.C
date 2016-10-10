/*****************************************************************************/
/* CAVisual.C: CAVisual object.                                              */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

/* $Id: CAVisual.C,v 1.7 1997/05/26 12:31:33 mark Exp $ */

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <CAVisual.h>
#include <X2Dview.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <CAReq.h>
#include <taglist.h>
#include <menu.h>
#include <CA.h>

/*****************************************************************************/
/*                                                                           */
/*  CAVisual TagList object                                                  */
/*                                                                           */
/*****************************************************************************/

CAVisTagList::CAVisTagList()
{
  InitTags(CAVIS_TAGS,
	   VIS_Width,         300,
	   VIS_Height,        300,
	   VIS_Xmin,          "-2.0",
	   VIS_Xmax,          "2.0",
	   VIS_Ymin,          "-2.0",
	   VIS_Ymax,          "2.0",
	   VIS_DrawingMode,   CAVisual::PIXMAP,
	   VIS_GfxScrollbars, False,
	   VIS_TxtScrollbars, False,
	   VIS_TextLines,     12,
	   VIS_LineDist,      4,
	   VIS_LineWidth,     150,
	   VIS_Fontname,      "fixed",
	   VIS_Fontsize,      10,
	   VIS_Justification, CATextVisual::LEFT,
	   VIS_FontWeight,    0,
	   VIS_FontSlant,     'r');
}

/*****************************************************************************/
/*                                                                           */
/*  CAVisual - graphics window only                                          */
/*                                                                           */
/*****************************************************************************/

CAVisual::CAVisual()
{
/* ------------------------------------------------------------------
   Default constructor
   ------------------------------------------------------------------ */
 parent_widget = (Widget)NULL;
 model = (CAModel *)NULL;
 color_req = new ColorReq(this);
}

/*****************************************************************************/

CAVisual::CAVisual(CAModel *parent_model, ...)
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent_model);
 
 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    if(tag != TAG_DONE)
       {
       value = va_arg(parameters, ULONG);
       taglist.SetValue(tag, value);
       }
    }
 va_end(parameters);

 model = parent_model;
 parent_widget = model->GetToplevelWidget();
 color_req = new ColorReq(this);
 alloced_colors = 0;
 
 init_gfx_field();
}

/*****************************************************************************/

void CAVisual::Initialize(CAModel *parent_model, ...)
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent_model);

 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    if(tag != TAG_DONE)
       {
       value = va_arg(parameters, ULONG);
       taglist.SetValue(tag, value);
       }
    }
 va_end(parameters);

 model = parent_model;
 parent_widget = model->GetToplevelWidget();

 init_gfx_field();
}

/*****************************************************************************/

CAVisual::~CAVisual()
{
 
}

/*****************************************************************************/


void CAVisual::GetCoordinateSystem(double &get_x_min, double &get_y_min,
				   double &get_x_max, double &get_y_max)
{
 get_x_min = x_min;
 get_y_min = y_min;
 get_x_max = x_max;
 get_y_max = y_max;

}

/*****************************************************************************/

Widget CAVisual::GetGfxWidget()
{
 return gfx_field;
}

/*****************************************************************************/

menubar CAVisual::GetMenubar()
{
 return vis_menus;
}

/*****************************************************************************/

Widget CAVisual::AddControlMenuItem(char *title, XtCallbackProc function, XtPointer data)
{
 return control_menu.AddItem(title, function, data);
}

/*****************************************************************************/

Widget CAVisual::AddFileMenuItem(char *title, XtCallbackProc function, XtPointer data)
{
 return file_menu.AddItem(title, function, data);
}

/*****************************************************************************/

int CAVisual::Popup()
{
/* ------------------------------------------------------------------
   Pop up the CAVisual window. Returns 1 on failure.
   ------------------------------------------------------------------ */
 if(toplevel != NULL)
    {
    XtRealizeWidget(toplevel);
    X2DsetViewport(gfx_field, x_min, x_max, y_min, y_max);
    return(1);
    }
 printf("CAVisual hasn't got a parent_widget (poor thing!)\n");
 return(0);
}

/*****************************************************************************/

int CAVisual::Popdown()
{
/* ------------------------------------------------------------------
   Pop down the CAVisual window. Returns 1 on failure.
   ------------------------------------------------------------------ */
 
 if(toplevel != NULL)
    {
    XtUnrealizeWidget(toplevel);
    return(1);
    }
 return(0);
}

/*****************************************************************************/

void CAVisual::PopUpColorRequester()
{
 color_req->Show();
}

/*****************************************************************************/

void CAVisual::SetDrawable(int new_drawable)
{
/* ------------------------------------------------------------------
   Set the drawable type. All subsequent drawing operations will be
   made in this drawable type. Note: PIXMAP means both PIXMAP and
   WINDOW.
   ------------------------------------------------------------------ */
 
 drawable = new_drawable;
 switch(drawable)
    {
    case PIXMAP:
	X2DdrawPixmapOn(gfx_field);
	break;
    case WINDOW:
	X2DdrawPixmapOff(gfx_field);
	break;

    default:
	printf("Invalid drawable type in CAVisual.\n");
	break;
    }
}

/*****************************************************************************/

void CAVisual::DrawPoint(double x, double y)
{
 X2DdrawPoint(gfx_field, x, y);
}

/*****************************************************************************/

void CAVisual::DrawLine(double x1, double y1, double x2, double y2)
{
 X2DdrawLine(gfx_field, x1, y1, x2, y2);
}

/*****************************************************************************/

void CAVisual::DrawText(char *text, int length, double x, double y)
{
 X2DdrawText(gfx_field, text, length, x, y);
}

/*****************************************************************************/

void CAVisual::SetDrawingFont(char *fontname, char weight, char slant, int size)
{
 X2DsetFont(gfx_field, fontname, weight, slant, size);
}

/*****************************************************************************/

void CAVisual::DrawPolygon(polygon &poly)
{
 X2DdrawPolygon(gfx_field, poly.GetSize(), poly.x, poly.y);
}

/*****************************************************************************/

void CAVisual::FilledPolygon(polygon &poly)
{
 X2DfilledPolygon(gfx_field, poly.GetSize(), poly.x, poly.y);
}

/*****************************************************************************/

void CAVisual::DrawCircle(double centre_x, double centre_y, double radius)
{
 X2DdrawCircle(gfx_field, centre_x, centre_y, radius);
}

/*****************************************************************************/

void CAVisual::FilledCircle(double centre_x, double centre_y, double radius)
{
  X2DfilledCircle(gfx_field, centre_x, centre_y, radius);
}

/*****************************************************************************/

void CAVisual::DrawEllipse(double cx, double cy, double rx, double ry)
{
 X2DdrawEllipse(gfx_field, cx, cy, rx, ry);
}

/*****************************************************************************/

void CAVisual::FilledEllipse(double cx, double cy, double rx, double ry)
{
 X2DfilledEllipse(gfx_field, cx, cy, rx, ry);
}

/*****************************************************************************/

void CAVisual::DrawArc(double x, double y, double width, double height, int angle1, int angle2)
{
 X2DdrawArc(gfx_field, x, y, width, height, angle1, angle2);
}

/*****************************************************************************/

void CAVisual::FilledArc(double x, double y, double width, double height, int angle1, int angle2)
{
 X2DfilledArc(gfx_field, x, y, width, height, angle1, angle2);
}

/*****************************************************************************/

void CAVisual::DrawRectangle(double x, double y, double width, double height)
{
 X2DdrawRectangle(gfx_field, x, y, width, height);
}

/*****************************************************************************/

void CAVisual::FilledRectangle(double x, double y, double width, double height)
{
 X2DfilledRectangle(gfx_field, x, y, width, height);
}

/*****************************************************************************/

void CAVisual::ClearAll()
{
 X2DsetBackground(gfx_field,0);
 X2DclearAll(gfx_field);
}

/*****************************************************************************/

void CAVisual::SetPaletteRGB(int color, int r, int g, int b)
{
 X2DsetPaletteRGB(gfx_field, color, r, g, b);
 
 if (color >= alloced_colors)
    {
//    alloced_colors = color + 1;
    alloced_colors++;
    }
}

/*****************************************************************************/

void CAVisual::SetDrawingColor(int color)
{
X2DsetForeground(gfx_field, color);
}
 
/*****************************************************************************/

void CAVisual::SetLineStyle(int style)
{
 /* -------------------------------------------------------------
    Sets the foreground drawing line style. Values can be one of
    LineSolid, LineOnOffDash, LineDoubleDash
    ------------------------------------------------------------- */

 X2DsetLineStyle(gfx_field, style);
}

/*****************************************************************************/

void CAVisual::SetLineWidth(int width)
{
 X2DsetLineWidth(gfx_field,width);
}

/*****************************************************************************/

void CAVisual::CurrentPageFront()
{
 X2DcurrentPageFront(gfx_field);
}

/*****************************************************************************/

void CAVisual::CurrentPageBack()
{
 X2DcurrentPageBack(gfx_field);
 ClearAll();
}

/*****************************************************************************/

int CAVisual::GetAllocedColors()
{
 return alloced_colors;
}

/*****************************************************************************/
/*
int CAVisual::LoadColorMap(char *filename)
{
 if(X2DloadColorMap(gfx_field, filename))
    {
    return(1);
    }
 return(0);
}
*/
/*****************************************************************************/
/* PRIVATE FUNCTIONS:                                                        */
/*****************************************************************************/

void CAVisual::init_gfx_field()
{
/* ------------------------------------------------------------------
   Initialize the main window and the graphics field.
   ------------------------------------------------------------------ */
 Visual *xvis;

 x_min = atof((char *)taglist.GetValue(VIS_Xmin));
 y_min = atof((char *)taglist.GetValue(VIS_Ymin));
 x_max = atof((char *)taglist.GetValue(VIS_Xmax));
 y_max = atof((char *)taglist.GetValue(VIS_Ymax));

 gfx_width = (int)taglist.GetValue(VIS_Width);
 gfx_height = (int)taglist.GetValue(VIS_Height);
 
 if(! parent_widget)
    {
    printf("CAVisual has no parent_widget (poor thing)\n");
    exit(1);
    }
 toplevel = XtVaAppCreateShell(NULL, "cavisual", applicationShellWidgetClass, XtDisplay(parent_widget), 
			       NULL);
 main_form = XtVaCreateManagedWidget("main_form", formWidgetClass, toplevel, 
				     NULL);

 vis_menus.Initialize(main_form);

 file_menu = vis_menus.AddMenu("File");
 file_menu.AddItem("Close", close_vis, (XtPointer)NULL);

 control_menu = vis_menus.AddMenu("Control");
 control_menu.AddItem("Palette", palette_req, (XtPointer)this);
 control_menu.AddItem("Settings", options_req, (XtPointer)this);
 control_menu.AddItem("Zoom out", zoom_out, (XtPointer)this);

 gfx_form = XtVaCreateManagedWidget("gfx_form", formWidgetClass, main_form,
				    XtNfromVert, vis_menus.GetWidget(),
				    XtNleft, XawChainLeft,
				    XtNtop, XawChainTop,
				    XtNbottom, XawChainBottom,
				    XtNright, XawChainRight,
				    NULL);

 if(taglist.GetValue(VIS_GfxScrollbars) == (Boolean)True)
    {
    gfx_scrollbars = XtVaCreateManagedWidget("gfx_scrollbars",viewportWidgetClass, gfx_form,
					     XtNallowHoriz, True,
					     XtNallowVert,  True,
					     XtNforceBars,  True,
					     XtNuseBottom,  True,
					     XtNuseRight,   True,
					     NULL);
    
    gfx_field = XtVaCreateManagedWidget("gfx_field", X2DviewWidgetClass, gfx_scrollbars,
					XtNwidth,      gfx_width,
					XtNheight,     gfx_height,
					NULL);
    }
 else
    {
    gfx_field = XtVaCreateManagedWidget("gfx_field", X2DviewWidgetClass, gfx_form,
					XtNwidth,  gfx_width,
					XtNheight, gfx_height,
					NULL);
    }
 XtAddCallback(gfx_field, XtNclick, (XtCallbackProc)click_handler, this);
// XtAddCallback(gfx_field, XtNdblClick, dbl_click_handler, this);
 XtAddCallback(gfx_field, XtNviewportChanged, (XtCallbackProc)coordinates_changed, this);
 XtAddCallback(gfx_field, XtNrtClick, (XtCallbackProc)right_click_handler, this);

 SetDrawable((int)taglist.GetValue(VIS_DrawingMode));
}

/*****************************************************************************/

void CAVisual::coordinates_changed(Widget w, XtPointer data, XtPointer nothing)
{
 CAVisual *vis = (CAVisual *)data;

 vis->model->CoordinatesChanged();
}

/*****************************************************************************/

void CAVisual::click_handler(Widget w, XtPointer data, X2DPoint *pos)
{
 CAVisual *vis = (CAVisual *)data;
 vis->model->Click(pos->x, pos->y);
}

/*****************************************************************************/

void CAVisual::right_click_handler(Widget w, XtPointer data, X2DPoint *pos)
{
 CAVisual *vis = (CAVisual *)data;
 vis->model->RightClick(pos->x, pos->y);
}

/*****************************************************************************/

void CAVisual::color_requester_ok()
{
 X2DsetBackground(gfx_field, 0);
 model->Redraw();
}

/*****************************************************************************/

void CAVisual::color_requester_cancel()
{
}

/*****************************************************************************/

void CAVisual::close_vis(Widget w, XtPointer data, XtPointer nothing)
{
}

/*****************************************************************************/

void CAVisual::palette_req(Widget w, XtPointer data, XtPointer nothing)
{
 CAVisual *vis = (CAVisual *)data;

 vis->PopUpColorRequester();
}

/*****************************************************************************/

void CAVisual::options_req(Widget w, XtPointer data, XtPointer nothing)
{
 CAVisual *vis = (CAVisual *)data;
 vis->model->PopupEnvironRequester();
}

/*****************************************************************************/

void CAVisual::zoom_out(Widget w, XtPointer data, XtPointer nothing)
{
 CAVisual *vis = (CAVisual *)data;

 X2DsetViewport(vis->gfx_field, vis->x_min, vis->x_max, vis->y_min, vis->y_max);
}

/*****************************************************************************/
/*                                                                           */
/* CATextVisual: both text and graphics window                               */
/*                                                                           */
/*****************************************************************************/

CATextVisual::CATextVisual()
{
 parent_widget = (Widget)NULL;
 model = NULL;
}

/*****************************************************************************/

CATextVisual::CATextVisual(CAModel *parent_model, ...) 
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent_model);
 
 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    if(tag != TAG_DONE)
       {
       value = va_arg(parameters, ULONG);
       taglist.SetValue(tag, value);
       }
    }
 va_end(parameters);

 model = parent_model;
 parent_widget = model->GetToplevelWidget();

 init_gfx_field();
 init_text_field();
}

/*****************************************************************************/

CATextVisual::~CATextVisual()
{
 delete text_line;
}

/*****************************************************************************/

void CATextVisual::Initialize(CAModel *parent_model, ...)
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent_model);

 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    if(tag != TAG_DONE)
       {
       value = va_arg(parameters, ULONG);
       taglist.SetValue(tag, value);
       }
    }
 va_end(parameters);

 model = parent_model;
 parent_widget = model->GetToplevelWidget();

 init_gfx_field();
 init_text_field();
}

/*****************************************************************************/

void CATextVisual::init_text_field()
{
/* ------------------------------------------------------------------
   Initialize the text field. Calculate the widget height from the
   (font height + line distance) * number of lines.
   ------------------------------------------------------------------ */
 int i, dir, asc, desc;
 XCharStruct overall;
 int text_lines;
 int text_width, text_height;
 char font_string[80]; 
 char *fontname;
 int font_height;
 int fontsize;
 int line_dist;
 char weight[10];
 char slant;
 XtJustify just;

 if(! parent_widget)
    {
    printf("CATextVisual has no parent (poor thing)\n");
    exit(1);
    }

 //
 // Get Tag Values
 //
 text_lines = (int)taglist.GetValue(VIS_TextLines);
 line_dist = (int)taglist.GetValue(VIS_LineDist);
 text_width = (int)taglist.GetValue(VIS_LineWidth);
 switch((int)taglist.GetValue(VIS_Justification))
    {
    case LEFT:
	just = XtJustifyLeft; 
	break;

    case CENTER:
	just = XtJustifyCenter;
	break;

    case RIGHT:
	just = XtJustifyRight;
	break;

    default:
	printf("Invalid Justification!\n");
	break;
    }

 //
 // Load font and Calculate its size in pixels.
 //
 fontname = (char *)taglist.GetValue(VIS_Fontname);
 fontsize = (int)taglist.GetValue(VIS_Fontsize);
 slant = (char)taglist.GetValue(VIS_FontSlant);

 if((char)taglist.GetValue(VIS_FontWeight) == 'b')
    {
    strcpy(weight, "bold");
    }
 else
    {
    strcpy(weight, "medium");
    }

 sprintf(font_string, "-*-%s-%s-%c-*-*-*-%d-*-*-*-*-*-*",
	 fontname, weight, slant, fontsize*10);

 if((font_info = XLoadQueryFont(XtDisplay(toplevel), font_string))==(XFontStruct *)NULL)
    {
    printf("Couldn't open font %s using \"fixed\" instead\n",font_string);
    font_info = XLoadQueryFont(XtDisplay(toplevel), "fixed");
    }
 
 XTextExtents(font_info, "Ag", 2, &dir, &asc, &desc, &overall);
 font_height = asc + desc;
 text_height = (font_height + line_dist)*text_lines;
 //
 // Initialize widgets
 //
 text_form = XtVaCreateManagedWidget("text_form", formWidgetClass, main_form,
				     XtNfromVert, vis_menus.GetWidget(),
				     XtNfromHoriz, gfx_form,
				     XtNleft, XawChainRight,
				     XtNtop, XawChainTop,
				     XtNbottom, XawChainTop,
				     XtNright, XawChainRight,
				     NULL);

 if(taglist.GetValue(VIS_TxtScrollbars) == (Boolean)True)
    {
    text_scrollbars = XtVaCreateManagedWidget("text_scrollbars",viewportWidgetClass, text_form,
					      XtNallowHoriz, True,
					      XtNallowVert, True,
					      XtNforceBars, True,
					      XtNuseBottom, True,
					      XtNuseRight,  True,
					      XtNwidth, text_width,
					      XtNheight, text_height,
					      NULL);
    
    text_line_box = XtVaCreateManagedWidget("text_line_box", boxWidgetClass, text_scrollbars,
					    XtNvSpace, 0,
					    NULL);
    }
 else
    {
    text_line_box = XtVaCreateManagedWidget("text_line_box", boxWidgetClass, text_form,
					    XtNvSpace, 0,
					    NULL);
    }
 //
 // Allocate memory for all the lines, and set up the line widgets.
 //
 text_line = new Widget[text_lines];

 if(! text_line)
    {
    printf("Memory allocation failure in create_textfield!\n");
    exit(1);
    }

 for(i = 0; i < text_lines; i++)
    {
    text_line[i] = XtVaCreateManagedWidget("text_line", labelWidgetClass, text_line_box,
					   XtNwidth, text_width,
					   XtNheight, font_height + line_dist,
					   XtNlabel, "",
					   XtNfont, font_info,
					   XtNjustify, just, 
					   XtNresize, False, 
					   XtNborderWidth, 0,
					   NULL);
    }

Debug("Leaving CATextVisual::init_text_field()\n ");
}

/*****************************************************************************/

void CATextVisual::Print(int line, char *string)
{
 XtVaSetValues(text_line[line],
	       XtNlabel, string,
	       NULL);
}

/*****************************************************************************/

void CATextVisual::Printf(int line, char *format, ...)
{
 char *str;
 va_list parameters;

 if ((str = new char[256]) == NULL)
    {
    printf("Memory allocation failure in CAVisual::Printf()\n");
    }
 va_start(parameters, format);
// (void) vsnprintf(str, 256, format, parameters);
 (void) vsprintf(str, format, parameters);
 va_end(parameters);

 XtVaSetValues(text_line[line],
	       XtNlabel, str,
	       NULL);
 
}
/*****************************************************************************/

void CATextVisual::SetFont(char *fontname, char weight, char slant, int size)
{
 int i;
 
 for(i = 0; i < text_lines; i++)
    {
    SetFont(i, fontname, weight, slant, size);
    }
}

/*****************************************************************************/

void CATextVisual::SetFont(int line, char *fontname, char weight, char slant, int size)
{
 char font_string[80]; 
 char wt[10];

 if(weight == 'b')
    {
    strcpy(wt, "bold");
    }
 else
    {
    strcpy(wt, "medium");
    }

 sprintf(font_string, "-*-%s-%s-%c-*-*-*-%d-*-*-*-*-*-*", fontname, wt, slant, size*10);

 if((font_info = XLoadQueryFont(XtDisplay(toplevel), font_string))==(XFontStruct *)NULL)
    {
    printf("Couldn't open font %s, using \"fixed\" instead\n",font_string);
    font_info = XLoadQueryFont(XtDisplay(toplevel), "fixed");
    }

 XtVaSetValues(text_line[line],
	       XtNfont, font_info,
	       NULL);
}

/*****************************************************************************/

void CATextVisual::SetJustification(int just)
{
 int i;
 
 for(i = 0; i < text_lines; i++)
    {
    SetJustification(i, just);
    }
}

/*****************************************************************************/

void CATextVisual::SetJustification(int line, int new_just)
{
 XtJustify just;

 switch(new_just)
    {
    case LEFT:
	just = XtJustifyLeft; 
	break;

    case CENTER:
	just = XtJustifyCenter;
	break;

    case RIGHT:
	just = XtJustifyRight;
	break;

    default:
	printf("Invalid Justification!\n");
	break;
    }

 XtVaSetValues(text_line[line],
	       XtNjustify, just,
	       NULL);
}

/*****************************************************************************/

void CATextVisual::SetLineColor(int fgcol, int bgcol)
{
 int i;
 
 for(i = 0; i < text_lines; i++)
    {
    SetLineColor(i, fgcol, bgcol);
    }
}

/*****************************************************************************/

void CATextVisual::SetLineColor(int line, int fgcol, int bgcol)
{
XtVaSetValues(text_line[line],
	       XtNforeground, X2DgetPaletteColor(gfx_field, fgcol),
	       XtNbackground, X2DgetPaletteColor(gfx_field, bgcol),
	       NULL);
}

/*****************************************************************************/




