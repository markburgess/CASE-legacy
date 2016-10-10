/*********************************************************************/
/* X2Dview.c:                                                        */
/* Widget class for 2-dimensjonal viewport                           */
/* Copyright (c)1995, 1996 Thomas Engh Sevaldrud                     */  
/*                         Jon Anders Mikkelsen                      */
/*********************************************************************/

/* $Id: X2Dview.c,v 1.9 1997/06/03 13:00:18 mark Exp $ */

#include <conf.h>
#include <stdio.h>

#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/CoreP.h>

#include <X11/extensions/Xdbe.h>

#include "trans2d.h"
#include "X2DviewP.h"
#include "X2Dview.h"
#include "converters.h"
#include "widget_tools.h"

/*********************************************************************/
/* MACROS/DEFINITIONS:                                               */
/*********************************************************************/

/*short DEBUG = 0;*/

/*********************************************************************/
/* ACTIONS/TRANSLATIONS:                                             */
/*********************************************************************/

static char view2Dtranslations[] = "<Btn2Down>: zoom_start() \n\
                                    <Btn2Motion>: zoom() \n\
                                    <Btn2Up>: zoom_end() \n\
                                    <Btn3Up>: widget_rt_click() \n\
                                    <Btn1Up>(2): widget_dblclick()\n\
                                    <Btn1Down>,<Btn1Up>: widget_click()";

static XtActionsRec view2DactionsList[] = {
  { "zoom_start", (XtActionProc) start_zoom},
  { "zoom", (XtActionProc) handle_zoom},
  { "zoom_end", (XtActionProc) end_zoom},
  { "widget_rt_click", (XtActionProc) widget_rt_click},
  { "widget_click", (XtActionProc) widget_click},
  { "widget_dblclick", (XtActionProc) widget_dblclick},
};

/*********************************************************************/
/* DEFAULT RESOURCES:                                                */
/*********************************************************************/

static XtResource view2Dresources[] = {
  { XtNx_min, XtCX_min, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.x_min), XtRString, (XtPointer)"-2.0" },
  { XtNx_max, XtCX_max, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.x_max), XtRString, (XtPointer)"2.0"  },
  { XtNy_min, XtCY_min, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.y_min), XtRString, (XtPointer)"-2.0" },
  { XtNy_max, XtCY_max, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.y_max), XtRString, (XtPointer)"2.0"  },

  { XtNpixx, XtCPixx, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.pixx), XtRString, (XtPointer)"0.0" },
  { XtNpixy, XtCPixy, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.pixy), XtRString, (XtPointer)"0.0" },

  { XtNdef_x_min, XtCDef_x_min, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.def_x_min), XtRString, (XtPointer)"0" },
  { XtNdef_x_max, XtCDef_x_max, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.def_x_max), XtRString, (XtPointer)"0" },
  { XtNdef_y_min, XtCDef_y_min, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.def_y_min), XtRString, (XtPointer)"0" },
  { XtNdef_y_max, XtCDef_y_max, XtRDouble, sizeof(double),
      XtOffset(X2DViewWidget, view.def_y_max), XtRString, (XtPointer)"0"  },

  { XtNzboxLineStyle, XtCZboxLineStyle, XtRGCValue, sizeof(int),
      XtOffset(X2DViewWidget, view.zbox_line_style), XtRString, (XtPointer)"LineOnOffDash" },
  { XtNzboxLineWidth, XtCZboxLineWidth, XtRInt, sizeof(int),
      XtOffset(X2DViewWidget, view.zbox_line_width), XtRString, (XtPointer)"1"  },
  { XtNzboxForeground, XtCColor, XtRPixel, sizeof(Pixel),
      XtOffset(X2DViewWidget, view.zbox_foreground), XtRString, (XtPointer)"Green" },

  { XtNviewportChanged, XtCCallback, XtRCallback, sizeof(caddr_t),
      XtOffset(X2DViewWidget, view.viewportChanged), XtRCallback, NULL  },
  { XtNclick, XtCCallback, XtRCallback, sizeof(caddr_t),
      XtOffset(X2DViewWidget, view.click), XtRCallback, NULL  },
  { XtNdblClick, XtCCallback, XtRCallback, sizeof(caddr_t),
      XtOffset(X2DViewWidget, view.dblClick), XtRCallback, NULL  },
  { XtNrtClick, XtCCallback, XtRCallback, sizeof(caddr_t),
      XtOffset(X2DViewWidget, view.rtClick), XtRCallback, NULL  },

  { XtNncolors, XtCNcolors, XtRInt, sizeof(int),
      XtOffset(X2DViewWidget, view.ncolors), XtRString, (XtPointer)"0" },

  { XtNinstall, XtCInstall, XtRBoolean, sizeof(Boolean),
      XtOffset(X2DViewWidget, view.install_cmap), XtRString, (XtPointer)"False" },

  { XtNallocColors, XtCAllocColors, XtRInt, sizeof(int),
      XtOffset(X2DViewWidget, view.alloc_colors), XtRString, (XtPointer)"0" },
};

/*********************************************************************/
/* WIDGET CLASS RECORD:                                              */
/*********************************************************************/

X2DViewClassRec X2DviewClassRec = {
    {
        (WidgetClass) &widgetClassRec,
        "View2D",
        sizeof(X2DViewRec),
        X2DInitClass,		        /* class_initialize */
        NULL,
        FALSE,
        X2DInitialize,			/* initialize */
        NULL,
        X2Drealize,			/* realize */
        view2DactionsList,		/* actions */
        XtNumber(view2DactionsList),	/* num_actions */
        view2Dresources,		/* Resources */
        XtNumber(view2Dresources),	/* num_resources */
        NULLQUARK,			/* xrm_class */
        TRUE,				/* compress_motion */
        TRUE,				/* compress_exposure */
        TRUE,				/* compress_enterleave */
        TRUE,				/* visible_interest */
        X2Ddestroy,			/* destroy */
        X2Dresize,			/* resize */
        X2Dredisplay,			/* expose */
        X2DsetValues,			/* set_values */
        NULL,				/* set_values_hook */
        XtInheritSetValuesAlmost,	/* set_values_almost */
        NULL,				/* get_values_hook */
        NULL,				/* accept_focus */
        XtVersion,			/* version */
        NULL,				/* callback private */
        view2Dtranslations,		/* tm_table */
        NULL,
        NULL,
        NULL,
    },
    {
        0,
    }
};

WidgetClass X2DviewWidgetClass = (WidgetClass) &X2DviewClassRec;

/*********************************************************************/
/*                                                                   */
/* WIDGET CREATION/HANDLING ROUTINES:                                */
/*                                                                   */
/*********************************************************************/

static void X2DInitClass(Widget request)

/* -------------------------------------------------------------
   Registers the widgets custom converters.
   ------------------------------------------------------------- */

{
XtAddConverter(XtRString, XtRDouble, CvtStringToDouble, NULL, 0);
XtAddConverter(XtRString, XtRGCValue, CvtStringToGCvalue, NULL, 0);
}

/*********************************************************************/

static void X2DInitialize(Widget request, Widget new)

  /* -------------------------------------------------------------
     Initializes the widgets internal resources to either default
     values or given values.
     ------------------------------------------------------------- */
    
{ XGCValues values;
  XtGCMask valueMask;
  X2DViewWidget vp = (X2DViewWidget)new;

vp->view.zooming = False;
vp->view.zooming_allowed = True;
vp->view.draw_pixmap = False;	    /* Drawing in window only, not pixmap. */
/*vp->view.install_cmap = True;*/

vp->core.background_pixel = 0;

if (request->core.width == 0)
   {
   vp->core.width = 100; 
   }

if (request->core.height == 0)
   {
   vp->core.height = 100;   
   }

if (vp->view.x_min >= vp->view.x_max)
   {
   printf("Maximum x coordinate (%f given) must be greater than the minimum (%f given)!\n", vp->view.x_max, vp->view.x_min);
   vp->view.x_min = -2.0;
   vp->view.x_max = 2.0;
   }

if (vp->view.y_min >= vp->view.y_max)
   {
   printf("Maximum y coordinate (%f given) must be greater than the minimum (%f given)!\n", vp->view.y_max, vp->view.y_min);
   vp->view.y_min = -2.0;
   vp->view.y_max = 2.0;
   }

if (vp->view.def_x_min >= vp->view.def_x_max || vp->view.def_y_min >= vp->view.def_y_max)
   { 
   vp->view.def_x_min = vp->view.x_min;
   vp->view.def_x_max = vp->view.x_max;
   vp->view.def_y_min = vp->view.y_min;
   vp->view.def_y_max = vp->view.y_max;
   }

/* 
 * Set up viewport transformation matrix:
 * (transformation: logical coordinates -> pixel coordinates)
 */

make_identity (vp->view.vm);

X2DwindowToViewportTrans(vp->view.vm, 
			 vp->view.x_min, vp->view.x_max, 
			 vp->view.y_min, vp->view.y_max,
			 0, vp->core.width,
			 0, vp->core.height);

vp->view.pixx = (vp->view.x_max-vp->view.x_min)/vp->core.width;
vp->view.pixy = (vp->view.y_max-vp->view.y_min)/vp->core.height;

/*
** Set up a graphics context for the widget window:
*/

valueMask = GCLineWidth | GCBackground | GCFillStyle | GCFillRule;
values.line_width = 1;
values.background = vp->core.background_pixel;
values.fill_style = FillSolid;
values.fill_rule = EvenOddRule;
vp->view.gc = XtGetGC((Widget)vp, valueMask, &values);

/*
** ...and for the zoom box:
*/

valueMask = GCLineStyle | GCForeground | GCLineWidth;
values.foreground = vp->view.zbox_foreground;
values.line_style = vp->view.zbox_line_style;
values.line_width = vp->view.zbox_line_width;
vp->view.zbox_gc = XtGetGC((Widget)vp, valueMask, &values);

vp->view.pixmap = (Pixmap) NULL;
}

/*********************************************************************/

#define superclass (&coreClassRec)

static void X2Drealize(Widget w, XtValueMask *valueMask, 
                       XSetWindowAttributes *attributes)

/* -------------------------------------------------------------
     This routine draws the widget. First it calls the Core widgets
     realize-routine. Then it sets up the widgets pixmap and the
     double-buffering
     ------------------------------------------------------------- */
     
{ 
int num;
int numbuffers = 2;
int MBEVENT,MBERROR;
X2DViewWidget vp = (X2DViewWidget)w;

(*superclass->core_class.realize) (w,valueMask,attributes);
    
vp->view.pixmap = XCreatePixmap(XtDisplay(vp),                    
                                XtWindow(vp),                     
                                vp->core.width, vp->core.height,  
                                DefaultDepth(XtDisplay(vp),0));   
 if (! vp->view.pixmap)
    {
    printf("X2DViewWidget: Couldn't create pixmap!\n");
    }
 
 
 if (! XdbeQueryExtension(XtDisplay(w),&MBEVENT,&MBERROR))
    {
    printf ("Sorry! this machine does not have the double buffer extension!\n");
    printf ("You need X11R6.1 for this extension. If you have the older Xmbuf\n");
    printf ("multibuffer extension you can recompile the X2Dview widget with\n");
    printf ("MB_MULTIBUF defined.\n");
    exit(1);
    }
 
 vp->view.backpage = XdbeAllocateBackBufferName(XtDisplay(vp), XtWindow(vp), XdbeUndefined);
 
 vp->view.currentpage = XtWindow(vp);   /* Xdbe always regards the front buffer as the window */
 
 vp->view.swapinfo = (XdbeSwapInfo *)malloc(sizeof(XdbeSwapInfo));
 
 if(! vp->view.swapinfo)
    {
    printf("Memory allocation for swapinfo failed!\n");
    exit(1);
    }
 
 vp->view.swapinfo->swap_window = XtWindow(vp);
 vp->view.swapinfo->swap_action = XdbeBackground;
 
X2DinitPalette(w);
X2DclearAll((Widget)vp);
}
#undef superclass
     
/*********************************************************************/

static void X2Ddestroy(X2DViewWidget vp)

{
XtDestroyGC(vp->view.gc);
XtDestroyGC(vp->view.zbox_gc);

if(vp->view.pixmap != (Pixmap)NULL)
   {
   XFreePixmap(XtDisplay(vp), vp->view.pixmap);
   }

free(vp->view.swapinfo);

XtRemoveAllCallbacks((Widget)vp, XtNviewportChanged);
}

/*********************************************************************/

static void X2Dresize(Widget w)

/* -------------------------------------------------------------
     o Frees the old pixmap and creates a new one the right size. 
     o Clears the window and the pixmap. 
     o Sets up the new view matrix.
     o Runs any callback routines associated with the 
       "viewport changed" event. 
     ------------------------------------------------------------- */
{

X2DViewWidget vp = (X2DViewWidget)w;

if(vp->view.pixmap != (Pixmap)NULL)
   {
   XFreePixmap(XtDisplay(vp), vp->view.pixmap);
   }

vp->view.pixmap = XCreatePixmap(XtDisplay(vp),                     
                                XtWindow(vp),                      
                                vp->core.width, vp->core.height,   
                                DefaultDepth(XtDisplay(vp),0));    
if(! vp->view.pixmap)
   {
   printf("X2DViewWidget: X2DResize(): Couldn't create pixmap!\n");
   }

X2DclearAll((Widget)vp);

vp->view.pixx = (vp->view.x_max-vp->view.x_min)/vp->core.width;
vp->view.pixy = (vp->view.y_max-vp->view.y_min)/vp->core.height;

make_identity (vp->view.vm);

X2DwindowToViewportTrans(vp->view.vm, 
                         vp->view.x_min, vp->view.x_max, 
                         vp->view.y_min, vp->view.y_max,
                         0, vp->core.width,
                         0, vp->core.height);


XtCallCallbacks((Widget)vp, XtNviewportChanged, NULL);
}

/*********************************************************************/

static void X2Dredisplay(X2DViewWidget vp, XEvent *event, Region region)
{
 XRectangle rect;
 XClipBox(region, &rect);
 
 if(vp->core.visible)
    {
    XCopyArea(XtDisplay(vp), vp->view.pixmap, vp->view.currentpage, 
	      vp->view.gc, rect.x, rect.y, rect.width, rect.height,
	      rect.x, rect.y);
/*   X2DupdateGraphics((Widget)vp);*/
    }
}

/*********************************************************************/

void X2DupdateGraphics(Widget w)
{ 
X2DViewWidget vp = (X2DViewWidget)w;
}

/*********************************************************************/

static Boolean X2DsetValues(Widget curr, 
                            Widget request, 
                            Widget new_w)

/* -------------------------------------------------------------
     This routine is called when any of the internal values of the
     widget is changed.

     o Checks if logical coordinate boundaries are changed
       - sets up new transformation matrix if necessary.
     o Checks if widget size has changed
       - sets up new transformation matrix if necessary.
     o Checks if GC values have changed and sets up new GCs if 
       necessary
     ------------------------------------------------------------- */
{
Boolean redraw = FALSE;
X2DViewWidget current = (X2DViewWidget) curr;
X2DViewWidget new = (X2DViewWidget) new_w;
XGCValues values;
XtGCMask valueMask;

if(new->view.x_min != current->view.x_min || new->view.x_max != current->view.x_max ||
   new->view.y_min != current->view.y_min || new->view.y_max != current->view.y_max)
   {
   if(new->view.x_min >= new->view.x_max)
      {
      XtWarning("Maximum x coordinate must be greater than the minimum!");
      new->view.x_min = -2.0;
      new->view.x_max = 2.0;
      }
   
   if(new->view.y_min >= new->view.y_max)
      {
      XtWarning("Maximum y coordinate must be greater than the minimum!");
      new->view.y_min = -2.0;
      new->view.y_max = 2.0;
      }
   
   if(new->view.def_x_min >= new->view.def_x_max ||
      new->view.def_y_min >= new->view.def_y_max)
      { 
      new->view.def_x_min = new->view.x_min;
      new->view.def_x_max = new->view.x_max;
      new->view.def_y_min = new->view.y_min;
      new->view.def_y_max = new->view.y_max;
      }
   
   new->view.def_x_min = new->view.x_min;
   new->view.def_x_max = new->view.x_max;
   new->view.def_y_min = new->view.y_min;
   new->view.def_y_max = new->view.y_max;
   
   make_identity (new->view.vm);
   X2DwindowToViewportTrans(new->view.vm, 
                            new->view.x_min, new->view.x_max, 
                            new->view.y_min, new->view.y_max,
                            0, new->core.width,
                            0, new->core.height);
   
   new->view.pixx = (new->view.x_max-new->view.x_min)/new->core.width;
   new->view.pixy = (new->view.y_max-new->view.y_min)/new->core.height;
   redraw = TRUE;
   }

if(new->core.width != current->core.width || new->core.height != current->core.height)
   {
   make_identity (new->view.vm);
   X2DwindowToViewportTrans(new->view.vm, 
                            new->view.x_min, new->view.x_max, 
                            new->view.y_min, new->view.y_max,
                            0, new->core.width,
                            0, new->core.height);
   
   new->view.pixx = (new->view.x_max-new->view.x_min)/new->core.width;
   new->view.pixy = (new->view.y_max-new->view.y_min)/new->core.height;
   redraw = TRUE;
   }

if(new->core.background_pixel != current->core.background_pixel)
   {
   valueMask = GCLineWidth | GCBackground;
   values.line_width = 1;
   values.background = new->core.background_pixel;
   new->view.gc = XtGetGC((Widget)new, valueMask, &values);
   }

if(new->view.zbox_foreground != current->view.zbox_foreground ||
   new->view.zbox_line_style != current->view.zbox_line_style ||
   new->view.zbox_line_width != current->view.zbox_line_width)
   {
   valueMask = GCLineStyle | GCForeground | GCLineWidth;
   values.foreground = new->view.zbox_foreground;
   values.line_style = new->view.zbox_line_style;
   values.line_width = new->view.zbox_line_width;
   new->view.zbox_gc = XtGetGC((Widget)new, valueMask, &values);
   }

return(redraw);

}

/*********************************************************************/
/*                                                                   */
/* ZOOMING ROUTINES:                                                 */
/*                                                                   */
/*********************************************************************/

Boolean X2DisZooming(Widget w)

/* -------------------------------------------------------------
     Convenience routine for determining whether or not we are 
     zooming.
     ------------------------------------------------------------- */
{
X2DViewWidget vp = (X2DViewWidget)w;

return vp->view.zooming;
}

/*********************************************************************/

static void start_zoom(Widget w, XEvent *event)
{
/* -------------------------------------------------------------
     This routine is called when the middle mouse button i pressed

     o Determines if zooming is allowed (not turned off) exits if true.
     o Sets zoom box corner at mouse pointer position.
     o Starts timeout routine for drawing of the zoom box.
     ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;
int ztop, zleft;

if(vp->view.zooming_allowed)
   {
   vp->view.zooming = True;
   
   zleft = event->xmotion.x;
   ztop = event->xmotion.y;
   vp->view.zbox[0].x = vp->view.zbox[3].x = vp->view.zbox[4].x = zleft;            
   vp->view.zbox[0].y = vp->view.zbox[1].y = vp->view.zbox[4].y = ztop;
   
   vp->view.zbox[1].x = vp->view.zbox[2].x = zleft+1;
   vp->view.zbox[2].y = vp->view.zbox[3].y = ztop+1;
   
   vp->view.timeout = XtAppAddTimeOut(XtWidgetToApplicationContext(w),100,draw_zoom,w); 
   }
}

/*********************************************************************/

void handle_zoom(Widget w, XEvent *event)
{
/* -------------------------------------------------------------
     This routine is called as an event handler when the middle 
     mouse button is pressed while moving the mouse. It updates
     the corner coordinates of the zoom box.
     ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;

if(vp->view.zooming_allowed)
   {
   vp->view.zbox[1].x = (event->xmotion.x); 
   vp->view.zbox[2].x = (event->xmotion.x); 
   vp->view.zbox[2].y = (event->xmotion.y);
   vp->view.zbox[3].y = (event->xmotion.y);
   }
}

/*********************************************************************/

static void draw_zoom(Widget w)
{
/* -------------------------------------------------------------
     This is the timeout routine for drawing of the zoom box.
     o Switches buffer
     o Copies the contents of the pixmap to the current buffer
     o Draws the zoom box to current buffer
     o Displays the active buffer
     o registers the timout again.
     ------------------------------------------------------------- */


X2DViewWidget vp = (X2DViewWidget)w;


X2DcurrentPageBack(w);

/* if(vp->view.draw_pixmap)
   {*/
   XCopyArea(XtDisplay(vp), vp->view.pixmap, vp->view.currentpage,
	     vp->view.gc, 0, 0, vp->core.width, vp->core.height, 0, 0);
/*   }*/
XDrawLines(XtDisplay(w),vp->view.currentpage,
	   vp->view.zbox_gc, &vp->view.zbox[0],5,CoordModeOrigin);

X2DcurrentPageFront(w);

vp->view.timeout = XtAppAddTimeOut(XtWidgetToApplicationContext(w),100,
                                   (XtTimerCallbackProc)draw_zoom,w); 
}

/*********************************************************************/

static void end_zoom(Widget w, XEvent *event)
{
/* -------------------------------------------------------------
     This routine is called when the middle button is released.

     o Remove timeout
     o Sets new logical coordinate system boundaries
     ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;

if(vp->view.zooming_allowed)
   {
   vp->view.zooming = False;
   
   XtRemoveTimeOut(vp->view.timeout);
   
   if(vp->view.zbox[0].x != vp->view.zbox[2].x &&
      vp->view.zbox[0].y != vp->view.zbox[2].y)
      {
      X2DsetViewport((Widget)vp, 
                     Min(vp->view.zbox[0].x*vp->view.pixx+vp->view.x_min, 
                         vp->view.zbox[2].x*vp->view.pixx+vp->view.x_min), 
                     Max(vp->view.zbox[0].x*vp->view.pixx+vp->view.x_min, 
                         vp->view.zbox[2].x*vp->view.pixx+vp->view.x_min), 
                     Min(-vp->view.zbox[0].y*vp->view.pixy+vp->view.y_max, 
                         -vp->view.zbox[2].y*vp->view.pixy+vp->view.y_max), 
                     Max(-vp->view.zbox[0].y*vp->view.pixy+vp->view.y_max, 
                         -vp->view.zbox[2].y*vp->view.pixy+vp->view.y_max));
      
      }
   }
}

/*********************************************************************/

static void zoom_out(Widget w, XEvent *event)
{
X2DViewWidget vp = (X2DViewWidget)w;

if(vp->view.zooming_allowed)
   {
   X2DsetViewport((Widget)vp, 
                  vp->view.def_x_min, vp->view.def_x_max,
                  vp->view.def_y_min, vp->view.def_y_max); 
   
   }
}

/*********************************************************************/

void X2DdisableZooming(Widget w)
{
X2DViewWidget vp = (X2DViewWidget)w;

vp->view.zooming_allowed = False;
}

/*********************************************************************/

void X2DenableZooming(Widget w)
{
X2DViewWidget vp = (X2DViewWidget)w;

vp->view.zooming_allowed = True;
}

/*********************************************************************/
/*                                                                   */
/* COLOR FUNCTIONS:                                                  */
/*                                                                   */
/*********************************************************************/

void X2DinitPalette(Widget w)
{
 /* -------------------------------------------------------------
    THis function counts up available color cells. Then it
    places the indices of the free color cells in the pixel array,
    vp->view.palette.
    ------------------------------------------------------------- */
 X2DViewWidget vp = (X2DViewWidget)w;
 Widget top;
 Colormap def_cmap;
 XColor def_colors[MAX_COLS];
 int numcolors, i;
 /*
 ** Initialize a colormap
 */

 if(vp->view.install_cmap)
    {
    top = GetToplevelWidget((Widget)vp);
    
    /* Get the colors of the default colormap and put them in def_colors */
    numcolors = DisplayCells(XtDisplay(top), DefaultScreen(XtDisplay(top)));
    vp->view.ncolors = numcolors;
    def_cmap = DefaultColormap(XtDisplay(vp),DefaultScreen(vp));
    for(i = 0; i < numcolors; i++)
       {
       def_colors[i].pixel = i;
       def_colors[i].flags = DoRed | DoGreen | DoBlue;
       vp->view.palette[i] = def_colors[i].pixel;
       }
    XQueryColors(XtDisplay(vp), def_cmap, def_colors, numcolors);
    
    /* Create our new colormap */
    vp->view.cmap = XCreateColormap(XtDisplay(top),DefaultRootWindow(XtDisplay(top)),
				    DefaultVisual(XtDisplay(top), DefaultScreen(top)),
				    AllocAll);
    
    /* Store the default colors in the new colormap */
    XStoreColors(XtDisplay(top), vp->view.cmap, def_colors, numcolors);
    XSetWindowColormap(XtDisplay(top), XtWindow(top), vp->view.cmap);
    XInstallColormap(XtDisplay(top), vp->view.cmap);
    }
 else
    {
    vp->view.cmap = DefaultColormap(XtDisplay(vp),DefaultScreen(vp));

    /*
    for (numcolors = MAX_COLS; numcolors >= 2; numcolors-=6) 
       {
       if (XAllocColorCells(XtDisplay(w), vp->view.cmap, False , NULL,
			    0, vp->view.palette,
			    (unsigned int)numcolors) != 0) 
	  {
	  vp->view.ncolors = numcolors;
	  }
       }
       */
    vp->view.ncolors = MAX_COLS;
    }

 vp->view.palette_offset = MAX_COLS - vp->view.ncolors;
}

/*********************************************************************/

void X2DsetPaletteRGB(Widget w, int index, int red, int green, int blue)
{
 /* -------------------------------------------------------------
    This function stores the RGB values in the color cell pointed
    to by the value in index 'index' of the palette array.
    ------------------------------------------------------------- */
 XColor colour;
 X2DViewWidget vp = (X2DViewWidget)w;
 
 colour.flags = DoRed | DoGreen | DoBlue;
 colour.red = red*257;
 colour.blue = blue*257;
 colour.green = green*257;

 if(XAllocColor(XtDisplay(vp),vp->view.cmap,&colour))
    {
    XSetForeground(XtDisplay(vp),vp->view.gc,colour.pixel);
    }

 vp->view.palette[index + vp->view.palette_offset] = colour.pixel;

/* colour.pixel = vp->view.palette[index + vp->view.palette_offset]; 
 XStoreColor(XtDisplay(w), vp->view.cmap, &colour);*/
}

/*****************************************************************************/

Pixel X2DgetPaletteColor(Widget w, int col)
{
 X2DViewWidget vp = (X2DViewWidget)w;
 
if(col + vp->view.palette_offset < MAX_COLS) 
   {
   return(vp->view.palette[col + vp->view.palette_offset]);
   }
else
   {
   fprintf(stderr, "X2Dview error: Color out of range!\n");
   return 0;
   }
}

/*****************************************************************************/

void X2DsetForeground(Widget w,Pixel color)
{
X2DViewWidget vp = (X2DViewWidget)w;

if(color + vp->view.palette_offset < MAX_COLS) 
   {
   XSetForeground(XtDisplay(vp),vp->view.gc,vp->view.palette[color + vp->view.palette_offset]);
   }
 else
   {
/*   printf("Color out of range!\n");*/
   }
}

/**************************************************************************/

void X2DsetBackground(Widget w,Pixel color)
{
X2DViewWidget vp = (X2DViewWidget)w;

if(color + vp->view.palette_offset < MAX_COLS) 
   {
   XSetBackground(XtDisplay(vp), vp->view.gc,
		  vp->view.palette[color + vp->view.palette_offset]);
   }
else
   {
   printf("Color out of range!\n");
   }
}

/**************************************************************************/

void X2DsetLineWidth(Widget w, int width)

{ X2DViewWidget vp = (X2DViewWidget)w;

 XGCValues values;
 XtGCMask mask = GCLineWidth;
 
 values.line_width = width;
 X2DchangeDrawingGC(w, mask, &values);
}

/**************************************************************************/
 
void X2DsetLineStyle(Widget w, int style)
 
{
 XGCValues values;
 XtGCMask mask = GCLineStyle;

 values.line_style = style;
 X2DchangeDrawingGC(w, mask, &values);
}

/**************************************************************************/

void X2DsetForegroundRGB(Widget w, int red, int green, int blue)
{ 
XColor colour;
X2DViewWidget vp = (X2DViewWidget)w;

colour.red = red*257;
colour.blue = blue*257;
colour.green = green*257;

if(XAllocColor(XtDisplay(vp),vp->view.cmap,&colour))
   {
   XSetForeground(XtDisplay(vp),vp->view.gc,colour.pixel);
   }
else
   {
   printf("Warning: couldn't allocate color %d %d %d\n",
          colour.red, colour.green, colour.blue);
   XSetForeground(XtDisplay(vp),vp->view.gc,
                  BlackPixel(XtDisplay(vp), DefaultScreen(vp)));
   }
}

/*********************************************************************/

void X2DsetBackgroundRGB(Widget w, int red, int green, int blue)
{ 
XColor colour;
X2DViewWidget vp = (X2DViewWidget)w;

colour.red = red*257;
colour.blue = blue*257;
colour.green = green*257;

if(XAllocColor(XtDisplay(vp),vp->view.cmap,&colour))
   {
   XSetBackground(XtDisplay(vp),vp->view.gc,colour.pixel);
   }
else
   {
   printf("Warning: couldn't allocate color %d %d %d\n",
          colour.red, colour.green, colour.blue);
   XSetBackground(XtDisplay(vp),vp->view.gc,
                  BlackPixel(XtDisplay(vp), DefaultScreen(vp)));
   }
}

/**************************************************************************/

Pixel X2DgetForeground(Widget w, int index)
{
 X2DViewWidget vp = (X2DViewWidget)w;
 return(vp->view.palette[index + vp->view.palette_offset]);
}

/**************************************************************************/

int X2DloadColorMap(Widget w, char *filename)

/* -------------------------------------------------------------
   Loads a data file consisting of RGB values and sets the color-
   map.
   ------------------------------------------------------------- */
{
XColor colour[256];
Widget top;
int i;
FILE *fp;
X2DViewWidget vp = (X2DViewWidget)w;
top = GetToplevelWidget((Widget)vp);
i = 0;

fp = fopen(filename, "r");
if(fp == NULL)
   {
   printf("Error: couldn't open palette file %s\n", filename);
   return 0;
   }

while(i < vp->view.ncolors)
   {
   fscanf(fp, "%d %d %d",
   &colour[i].red,
   &colour[i].blue,
   &colour[i].green);

   colour[i].red *= 257;
   colour[i].blue *= 257;
   colour[i].green *= 257;
   colour[i].pixel = i;
   colour[i].flags = DoRed | DoGreen | DoBlue;
   i++;
   if(feof(fp))
      {
      i == vp->view.ncolors;
      }
   }
fclose(fp);

/*i = 0;*/
XFreeColormap(XtDisplay(top), vp->view.cmap);
vp->view.cmap = XCreateColormap(XtDisplay(top),DefaultRootWindow(XtDisplay(top)),
                                DefaultVisual(XtDisplay(top), DefaultScreen(top)),
                                AllocAll);

if(i > vp->view.ncolors) i = vp->view.ncolors;

XStoreColors(XtDisplay(top), vp->view.cmap, colour, i);
   
XSetWindowColormap(XtDisplay(top), XtWindow(top), vp->view.cmap);
XInstallColormap(XtDisplay(top), vp->view.cmap);

vp->view.alloc_colors = i;
return i;
}

/*********************************************************************/
/*                                                                   */
/* VIEWPORT FUNCTIONS                                                */
/*                                                                   */
/*********************************************************************/

void X2DsetViewport(Widget w,
                    double new_x_min, double new_x_max,
                    double new_y_min, double new_y_max)


/* -------------------------------------------------------------
     Makes a logical window where x is in the interval [new_x_min,
     new_x_max], and y in [new_y_min, new_x_max]. The width of the
     actual window is core.width x core.height. Then a transforma-
     tion matrix, vm is created for the transformation from logical
     to actual coordinates. At last all Callback routines associa-
     ed with the event of a changed viewport are called.

     Widget must be realized before this routine can be called.
     ------------------------------------------------------------- */
{
X2DViewWidget vp = (X2DViewWidget)w;

vp->view.x_min = new_x_min;
vp->view.x_max = new_x_max;
vp->view.y_min = new_y_min;
vp->view.y_max = new_y_max;

make_identity (vp->view.vm);

X2DwindowToViewportTrans(vp->view.vm, 
                         vp->view.x_min, vp->view.x_max, 
                         vp->view.y_min, vp->view.y_max,
                         0, vp->core.width,
                         0, vp->core.height);

/* Calculate pixels pr. x-unit and y-unit: */
vp->view.pixx = (vp->view.x_max-vp->view.x_min)/vp->core.width;
vp->view.pixy = (vp->view.y_max-vp->view.y_min)/vp->core.height;

X2DclearAll((Widget)vp);

/* Call callback routine defined by user */
XtCallCallbacks((Widget)vp, XtNviewportChanged, NULL);
}

/**************************************************************************/

void X2DsetViewportAndWindow(Widget w,
                             double new_x_min, double new_x_max,
                             double new_y_min, double new_y_max,
                             int new_widget_width, int new_widget_height)
{
/* -------------------------------------------------------------
     Same as above, only that the widget size is set to 
     new_widget_width x new_widget_height, causing X2Dresize() to
     be called automatically.
     ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;

vp->view.x_min = new_x_min;
vp->view.x_max = new_x_max;
vp->view.y_min = new_y_min;
vp->view.y_max = new_y_max;

XtVaSetValues(w,
              XtNwidth,   new_widget_width,
              XtNheight,  new_widget_height,
              NULL);
}

/*********************************************************************/

void X2DwindowToViewportTrans(matrix t, 
                              double xwmin, double xwmax,
                              double ywmin, double ywmax, 
                              int xvmin, int xvmax,
                              int yvmin, int yvmax) 
{
/* -------------------------------------------------------------
     This routine makes the transformation matrix vm, for the 
     tranformation from logical coordinates to widget coordinates. 
     ------------------------------------------------------------- */

matrix m;

/* Scaling factors:
 *             VIEWPORT          WINDOW */
double sx = (xvmax - xvmin) / fabs(xwmax - xwmin);
double sy = (yvmax - yvmin) / fabs(ywmax - ywmin);

make_identity(m);

/* This is to make the y coordinates increase from the bottom of the widget */
m[1][1]= -1;
m[1][2]= yvmax;

translate (t,-xwmin,-ywmin);

scale (t,sx,sy);
translate (t,0,yvmin);
combine_transformations(m,t);
}

/*********************************************************************/

void X2DsetOuterLimits(Widget w,
                       double new_x_min, double new_x_max,
                       double new_y_min, double new_y_max)
{
/* -------------------------------------------------------------
     This routine sets the outer bounds of the zoom box.
     ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;

vp->view.def_x_min = new_x_min;
vp->view.def_x_max = new_x_max;
vp->view.def_y_min = new_y_min;
vp->view.def_y_max = new_y_max;
}

/*********************************************************************/
/*                                                                   */
/* DRAWING FUNCTIONS                                                 */
/*                                                                   */
/*********************************************************************/

void X2DclearAll(Widget w)

/* -------------------------------------------------------------
     Clears window and pixmap.
     This seems to be the only way to clear both effectively. Using
     XClearWindow() and XClearArea() only clears the visible part
     of the window...
     ------------------------------------------------------------- */
{  

X2DViewWidget vp = (X2DViewWidget)w;
GC gc;
XGCValues vals;
unsigned long tmp_pixel;

/*
XtVaGetValues(w,XtNbackground, &vals.foreground,NULL);
*/
 
XGetGCValues(XtDisplay(w), vp->view.gc, GCBackground, &vals);
vals.foreground = vals.background;
gc = XtGetGC(w, GCForeground, &vals);

XFillRectangle(XtDisplay(vp), vp->view.pixmap,gc, 0, 0, vp->core.width, vp->core.height);

XCopyArea(XtDisplay(vp), vp->view.pixmap, vp->view.currentpage, 
          gc, 0, 0, vp->core.width, vp->core.height, 0, 0);

}

/*************************************************************************/

void X2DchangeDrawingGC(Widget w, unsigned long valuemask, XGCValues *values)
{
/* -------------------------------------------------------------
   Changes the drawing GC of the widget.
   ------------------------------------------------------------- */

X2DViewWidget vp = (X2DViewWidget)w;

XChangeGC(XtDisplay(vp), vp->view.gc, valuemask, values);

}

/*************************************************************************/

void X2DdrawPoint(Widget w, double x, double y)
{
 X2DViewWidget vp = (X2DViewWidget)w;
  
 transform_a_point (x,y,&x,&y,vp->view.vm);
 XDrawPoint(XtDisplay(vp) , vp->view.currentpage , vp->view.gc ,round(x),round(y));

 if(vp->view.draw_pixmap)
    {
    XDrawPoint(XtDisplay(vp) , vp->view.pixmap , vp->view.gc ,round(x),round(y));
    }
}

/*************************************************************************/

void X2DdrawLine(Widget w, double x1, double y1, double x2, double y2)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x1,y1,&x1,&y1,vp->view.vm);
 transform_a_point (x2,y2,&x2,&y2,vp->view.vm);
 XDrawLine(XtDisplay(vp) , vp->view.currentpage, vp->view.gc, round(x1), round(y1), round(x2), round(y2));

 if(vp->view.draw_pixmap)
    {
    XDrawLine(XtDisplay(vp) , vp->view.pixmap , vp->view.gc, round(x1), round(y1), round(x2), round(y2));
    }
}

/*************************************************************************/

void X2DdrawText(Widget w, char *text, int text_length, double x, double y)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x,y,&x,&y,vp->view.vm);
 XDrawString(XtDisplay(vp) , vp->view.currentpage , vp->view.gc, round(x), round(y), text, text_length);

 if(vp->view.draw_pixmap)
    {
    XDrawString(XtDisplay(vp) , vp->view.pixmap , vp->view.gc, round(x), round(y), text, text_length);
    }
}

/*********************************************************************/

void X2DsetFont(Widget w, char *fontname, char weight, char slant, int size)
{
 X2DViewWidget vp = (X2DViewWidget)w;
 char font_string[80];   /* That should be enough for most fonts! */
 char wt[10];
 XGCValues values;
 XtGCMask valueMask;

 if(weight == 'b')
    {
    strcpy(wt, "bold");
    }
 else
    {
    strcpy(wt, "medium");
    }
 /*
 ** Load font and Calculate its size in pixels.
 */
 sprintf(font_string, "-*-%s-%s-%c-*-*-*-%d-*-*-*-*-*-*", fontname, wt, slant, size*10);

 if((vp->view.font_info = XLoadQueryFont(XtDisplay(w), font_string))==(XFontStruct *)NULL)
    {
    printf("Couldn't open font %s, using \"fixed\" instead\n", font_string);
    vp->view.font_info = XLoadQueryFont(XtDisplay(w), "fixed");
    }

 /*
 ** Change graphics context for the widget window:
 */
 valueMask = GCFont;
 values.font = vp->view.font_info->fid;
 XChangeGC(XtDisplay(w), vp->view.gc, valueMask, &values);

}

/*********************************************************************/

void X2DdrawRectangle(Widget w, double x, double y, double width, double height)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x, y,&x,&y,vp->view.vm);
 width /= vp->view.pixx;
 height /= vp->view.pixy;

 XDrawRectangle(XtDisplay(vp) , vp->view.currentpage, vp->view.gc, round(x), round(y),
		round(width), round(height));

 if(vp->view.draw_pixmap)
    {
    XDrawRectangle(XtDisplay(vp) , vp->view.pixmap, vp->view.gc, round(x), round(y),
		   round(width), round(height));
    }
}

/*********************************************************************/

void X2DfilledRectangle(Widget w, double x, double y, double width, double height)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x, y,&x,&y,vp->view.vm);
 width /= vp->view.pixx;
 height /= vp->view.pixy;

 XFillRectangle(XtDisplay(vp) , vp->view.currentpage, vp->view.gc, round(x), round(y),
		round(width), round(height));

 if(vp->view.draw_pixmap)
    {
    XFillRectangle(XtDisplay(vp) , vp->view.pixmap, vp->view.gc, round(x), round(y),
		   round(width), round(height));
    }
}

/*********************************************************************/

void X2DdrawCircle(Widget w, double centre_x, double centre_y, double radius)
{
/* -------------------------------------------------------------
     Draws a circle with centre in (centre_x, centre_y) an radius
     radius.
     ------------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;

 radius = radius/vp->view.pixx;

 transform_a_point (centre_x,centre_y,&centre_x,&centre_y,vp->view.vm);

 XDrawArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(centre_x)-round(radius),
	  round(centre_y)-round(radius),
	  round(radius)*2, round(radius)*2, 0, 64*360);

 if(vp->view.draw_pixmap)
    {
    XDrawArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(centre_x)-round(radius),
	     round(centre_y)-round(radius),
	     round(radius)*2, round(radius)*2, 0, 64*360);
    }
}

/*********************************************************************/

void X2DfilledCircle(Widget w, double centre_x, double centre_y, double radius)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (centre_x,centre_y,&centre_x,&centre_y,vp->view.vm);

 radius = radius/vp->view.pixx;

 XFillArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(centre_x)-round(radius),
	  round(centre_y)-round(radius),
	  round(radius)*2, round(radius)*2, 0, 64*360);

 if(vp->view.draw_pixmap)
    {
    XFillArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(centre_x)-round(radius),
	     round(centre_y)-round(radius),
	     round(radius)*2, round(radius)*2, 0, 64*360);
    }
}

/*********************************************************************/

void X2DdrawEllipse(Widget w, double centre_x, double centre_y, double radius_x, double radius_y)
{
/* -------------------------------------------------------------
     Draws an ellipse with centre in (centre_x, centre_y) and radius
     radius_x, radius_y in x- and y-direction.
     ------------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;

 radius_x = radius_x/vp->view.pixx;
 radius_y = radius_y/vp->view.pixy;

 transform_a_point (centre_x,centre_y,&centre_x,&centre_y,vp->view.vm);

 XDrawArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(centre_x)-round(radius_x),
	  round(centre_y)-round(radius_y),
	  round(radius_x)*2, round(radius_y)*2, 0, 64*360);

 if(vp->view.draw_pixmap)
    {
    XDrawArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(centre_x)-round(radius_x),
	     round(centre_y)-round(radius_y),
	     round(radius_x)*2, round(radius_y)*2, 0, 64*360);
    }
}

/*********************************************************************/

void X2DfilledEllipse(Widget w, double centre_x, double centre_y, double radius_x, double radius_y)
{
 X2DViewWidget vp = (X2DViewWidget)w;

 radius_x = radius_x/vp->view.pixx;
 radius_y = radius_y/vp->view.pixy;

 transform_a_point (centre_x,centre_y,&centre_x,&centre_y,vp->view.vm);

 XFillArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(centre_x)-round(radius_x),
	  round(centre_y)-round(radius_y),
	  round(radius_x)*2, round(radius_y)*2, 0, 64*360);

 if(vp->view.draw_pixmap)
    {
    XFillArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(centre_x)-round(radius_x),
	     round(centre_y)-round(radius_y),
	     round(radius_x)*2, round(radius_y)*2, 0, 64*360);
    }
}

/*********************************************************************/

void X2DdrawArc(Widget w, double x, double y, double width, double height, int angle1, int angle2)
{
/* -------------------------------------------------------------
   Draws an arc. (x,y) is the upper left corner of the bounding 
   rectangle of the arc. Width, height is the major and minor 
   axis of the arc. angle1 specifies the start of the arc relative
   to the 3 o'clock position from the center in degrees. Angle2 
   specifies the path and extent of the arc relative to the start 
   of the arc, in units of degrees
   ------------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x, y, &x, &y, vp->view.vm);

 XDrawArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(x), round(y), round(width), round(height), 
	  angle1*64, angle2*64);

 if(vp->view.draw_pixmap)
    {
    XDrawArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(x), round(y), round(width), round(height), 
	     angle1*64, angle2*64);
    }
}

/*********************************************************************/

void X2DfilledArc(Widget w, double x, double y, double width, double height, int angle1, int angle2)
{
/* -------------------------------------------------------------
   Same as above, but filled.
   ------------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;

 transform_a_point (x, y, &x, &y, vp->view.vm);

 XFillArc(XtDisplay(vp) , vp->view.currentpage , vp->view.gc,
	  round(x), round(y), round(width), round(height), 
	  angle1*64, angle2*64);

 if(vp->view.draw_pixmap)
    {
    XFillArc(XtDisplay(vp) , vp->view.pixmap , vp->view.gc,
	     round(x), round(y), round(width), round(height), 
	     angle1*64, angle2*64);
    }
}

/*********************************************************************/

void X2DfloodFill(Widget w, double x, double y)
{
X2DViewWidget vp = (X2DViewWidget)w;

transform_a_point (x,y,&x,&y,vp->view.vm);

}

/*************************************************************************/

void X2DfilledPolygon(Widget w, int n, double *x, double *y)
    
{ double *xo;
  double *yo;
  int i;
  X2DViewWidget vp = (X2DViewWidget)w;
  XPoint *polygon;

 xo = (double *) malloc(n*sizeof(double));
 
 if (xo == NULL)
    {
    printf("malloc failure in X2DfilledPolygon 1!\n");
    }

 yo = (double *) malloc(n*sizeof(double));
 
 if (yo == NULL)
    {
    printf("malloc failure X2DfilledPolygon 2! n=%d\n",n*sizeof(double));
    }
 
 transform_points (n, x, y, xo, yo, vp->view.vm);

 polygon = (XPoint *) malloc(n*sizeof(XPoint));

 if (polygon == NULL)
    {
    printf("malloc failure X2DfilledPolygon 3!\n");    
    }

 for (i = 0; i < n; i++) 
    {
    polygon[i].x = xo[i];
    polygon[i].y = yo[i];
    }

 XFillPolygon(XtDisplay(vp),vp->view.currentpage,vp->view.gc,polygon,n,Convex,CoordModeOrigin);
 
 if(vp->view.draw_pixmap)
    {
    XFillPolygon(XtDisplay(vp),vp->view.pixmap,vp->view.gc,polygon,n,Convex,CoordModeOrigin);
    }
 
 free(xo);
 free(yo);
 free(polygon);
}

/*************************************************************************/

void X2DdrawPolygon(Widget w, int n, double *x, double *y) 

/* ---------------------------------------------------------------
   Draws a polygon with n edges (x[k], y[k]) for k=0,...,n-1
   --------------------------------------------------------------- */

{ double *xo;
  double *yo;
  int k;
  X2DViewWidget vp = (X2DViewWidget)w;

 xo = (double *)malloc(n*sizeof(double));
 
 if(xo == NULL)
    {
    printf("malloc failure!\n");
    }

 yo = (double *)malloc(n*sizeof(double));
 
 if(yo == NULL)
    {
    printf("malloc failure!\n");
    }
 
 transform_points (n, x, y, xo, yo, vp->view.vm);
 
 n--;
 
 for (k = 0; k < n; k++)
    {    
    XDrawLine(XtDisplay(vp) , vp->view.currentpage , vp->view.gc, xo[k], yo[k], xo[k+1], yo[k+1]);
    
    if(vp->view.draw_pixmap)
       {
       XDrawLine(XtDisplay(vp) , vp->view.pixmap , vp->view.gc, xo[k], yo[k], xo[k+1], yo[k+1]);
       }
    }

 XDrawLine(XtDisplay(vp) , vp->view.currentpage , vp->view.gc, xo[n], yo[n], xo[0], yo[0]);
 
 if(vp->view.draw_pixmap)
    {
    XDrawLine(XtDisplay(vp) , vp->view.pixmap , vp->view.gc, xo[n], yo[n], xo[0], yo[0]);
    }
 
 free(xo);
 free(yo);
}

/**************************************************************************/
/* POINTS TOOKIT: buffering of points                                     */
/**************************************************************************/

void X2DinitPointBuffer(Widget w)
{
X2DViewWidget vp = (X2DViewWidget)w;

int i;
for(i = 0;i < vp->view.ncolors; i++)
   {
   vp->view.points.npoints[i] = 0;
   }
}

/*********************************************************************/

void X2DbufferPoint(Widget w, double x, double y, int color)
{
 X2DViewWidget vp = (X2DViewWidget)w;


 if(vp->view.points.npoints[color] == MAX_BUF_POINTS-1)
    {
    XSetForeground(XtDisplay(w), vp->view.gc, color);
    
    XDrawPoints(XtDisplay(w), vp->view.currentpage, vp->view.gc,
		vp->view.points.data[color], vp->view.points.npoints[color],
		CoordModeOrigin);
    if(vp->view.draw_pixmap)
       {
       XDrawPoints(XtDisplay(w), vp->view.pixmap, vp->view.gc,
		   vp->view.points.data[color], vp->view.points.npoints[color],
		   CoordModeOrigin);
       }
    /* Reset buffer */
    vp->view.points.npoints[color] = 0;
    }
 
/* Store point in buffer, indexed on color */
 
 transform_a_point (x,y,&x,&y,vp->view.vm);
 vp->view.points.data[color][vp->view.points.npoints[color]].x = round(x);
 vp->view.points.data[color][vp->view.points.npoints[color]].y = round(y);
 vp->view.points.npoints[color]++;
 
}

/*********************************************************************/

void X2DflushPointBuffer(Widget w)
{
/* ---------------------------------------------------------
   Flushes any remaining points from the point buffer
   --------------------------------------------------------- */
 X2DViewWidget vp = (X2DViewWidget)w;
 int i;
 for(i = 0; i < vp->view.ncolors; i++)
    {
    if(vp->view.points.npoints[i])
       {
       XSetForeground(XtDisplay(w), vp->view.gc, i);
       
       XDrawPoints(XtDisplay(w), vp->view.currentpage, vp->view.gc,
		   vp->view.points.data[i], vp->view.points.npoints[i],
		   CoordModeOrigin);
       if(vp->view.draw_pixmap)
	  {
	  XDrawPoints(XtDisplay(w), vp->view.pixmap, vp->view.gc,
		      vp->view.points.data[i], vp->view.points.npoints[i],
		      CoordModeOrigin);
	  }
       vp->view.points.npoints[i] = 0;
       }
    }
}

/*********************************************************************/
/* MISCELLANEOUS:                                                    */
/*********************************************************************/

double Max(double a, double b)
{
if(a > b) return a;
return b;
}

/*********************************************************************/

double Min(double a, double b)
{
if(a < b) return a;
return b;
}

/*********************************************************************/

void X2DcurrentPageBack(Widget w)
{
X2DViewWidget vp = (X2DViewWidget)w;

vp->view.currentpage = vp->view.backpage;
}

/*********************************************************************/

void X2DcurrentPageFront(Widget w)
{
/* -----------------------------------------------------------
   This function sets the current drawing page to be the front page,
   and the contents of the back page is displayed. All following 
   drawing operations will go to the front page.
   ----------------------------------------------------------- */
X2DViewWidget vp = (X2DViewWidget)w;


XdbeSwapBuffers(XtDisplay(w), vp->view.swapinfo, 1);
vp->view.currentpage = XtWindow(w);
}

/*********************************************************************/

void X2DdrawPixmapOn(Widget w)
{
/* ---------------------------------------------------------
   Wrapper routine for setting flag draw_pixmap. When this
   flag is on, all drawing functions will draw in both pixmap
   and window
   --------------------------------------------------------- */
 X2DViewWidget vp = (X2DViewWidget)w;

 vp->view.draw_pixmap = True;
}

/*********************************************************************/

void X2DdrawPixmapOff(Widget w)
{
/* ---------------------------------------------------------
   Wrapper routine for clearing flag draw_pixmap.
   --------------------------------------------------------- */
 X2DViewWidget vp = (X2DViewWidget)w;

 vp->view.draw_pixmap = False;
}

/*********************************************************************/

static void widget_click(Widget w, XEvent *event)
{
/* ---------------------------------------------------------
   Calls a callback routine with the pointer x and y coordi-
   nates as parameters.
   --------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;
 X2DPoint pos;

 pos.x = event->xmotion.x*vp->view.pixx+vp->view.x_min;
 pos.y = -event->xmotion.y*vp->view.pixy+vp->view.y_max;
 
 XtCallCallbacks((Widget)vp, XtNclick, &pos);
}

/*********************************************************************/

static void widget_dblclick(Widget w, XEvent *event)
{
/* ---------------------------------------------------------
   Calls a callback routine with the pointer x and y coordi-
   nates as parameters.
   --------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;
 X2DPoint pos;

 pos.x = event->xmotion.x*vp->view.pixx+vp->view.x_min;
 pos.y = -event->xmotion.y*vp->view.pixy+vp->view.y_max;
 
 XtCallCallbacks((Widget)vp, XtNdblClick, &pos);
}

/*********************************************************************/

static void widget_rt_click(Widget w, XEvent *event)
{
/* ---------------------------------------------------------
   Calls a callback routine with the pointer x and y coordi-
   nates as parameters.
   --------------------------------------------------------- */

 X2DViewWidget vp = (X2DViewWidget)w;
 X2DPoint pos;

 pos.x = event->xmotion.x*vp->view.pixx+vp->view.x_min;
 pos.y = -event->xmotion.y*vp->view.pixy+vp->view.y_max;
 
 XtCallCallbacks((Widget)vp, XtNrtClick, &pos);
}

/*********************************************************************/












