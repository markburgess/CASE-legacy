/*********************************************************************/
/*                                                                   */
/* X2Dview.h:                                                        */
/* Public header file for 2-dimensional graphics widget for X11.     */
/*                                                                   */
/*-------------------------------------------------------------------*/
/* Copyright (c)1995, 1996 Thomas Engh Sevaldrud                     */  
/*                         Jon Anders Mikkelsen                      */
/*********************************************************************/

#ifndef _X2DVIEW_H
#define _X2DVIEW_H

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#ifdef HAVE_FEATURES_H
# include <features.h>
#endif

#include "trans2d.h"

/***********************************************************************/
/* MACROS/DEFINITIONS                                                  */
/***********************************************************************/

/***********************************************************************/
/* RESOURCE STRINGS AND CLASSES                                        */
/***********************************************************************/

#define XtNx_min           "x_min"
#define XtNx_max           "x_max"
#define XtNy_min           "y_min"
#define XtNy_max           "y_max"
#define XtNdef_x_min       "def_x_min"
#define XtNdef_x_max       "def_x_max"
#define XtNdef_y_min       "def_y_min"
#define XtNdef_y_max       "def_y_max"
#define XtNpixx            "pixx"
#define XtNpixy            "pixy"
#define XtNzboxLineStyle   "zboxLineStyle"
#define XtNzboxLineWidth   "zboxLineWidth"
#define XtNzboxForeground  "zboxForeground"
#define XtNviewportChanged "viewportChanged"
#define XtNclick           "click"
#define XtNdblClick        "dblClick"
#define XtNrtClick        "rtClick"
#define XtNncolors         "ncolors"
#define XtNallocColors     "alloc_colors"
#define XtNinstall         "install"

#define XtCX_min           "X_min"
#define XtCX_max           "X_max"
#define XtCY_min           "Y_min"
#define XtCY_max           "Y_max"
#define XtCPixx            "Pixx"
#define XtCPixy            "Pixy"
#define XtCDef_x_min       "Def_x_min"
#define XtCDef_x_max       "Def_x_max"
#define XtCDef_y_min       "Def_y_min"
#define XtCDef_y_max       "Def_y_max"
#define XtCZboxLineStyle   "ZboxLineStyle"
#define XtCZboxLineWidth   "ZboxLineWidth"
#define XtCZboxForeground  "ZboxForeground"
#define XtCNcolors         "Ncolors"
#define XtCAllocColors     "Alloc_Colors"
#define XtCInstall         "Install"

/***********************************************************************/
/* DATATYPES.                                                          */
/***********************************************************************/

extern WidgetClass X2DviewWidgetClass;

typedef struct _X2DViewClassRec * X2DViewWidgetClass;
typedef struct _X2DViewRec      * X2DViewWidget;

typedef struct 
{
   double x;
   double y;
}X2DPoint;

/***********************************************************************/

#ifdef __cplusplus
extern "C" 
{
#endif

/***********************************************************************/
/* VIEWPORT FUNCTIONS:                                                 */
/***********************************************************************/

void X2DsetViewport(Widget w, 
		    double xwmin, double xwmax,
		    double ywmin, double ywmax);

void X2DwindowToViewportTrans(matrix t, double xwmin, double xwmax,
			      double ywmin, double ywmax, int xvmin,
			      int xvmax, int yvmin, int yvmax); 


void X2DsetViewportAndWindow(Widget w,
			     double new_x_min, double new_x_max,
			     double new_y_min, double new_y_max,
			     int new_widget_width, int new_widget_height);

void X2DsetOuterLimits(Widget w,
			     double new_x_min, double new_x_max,
			     double new_y_min, double new_y_max);

/***********************************************************************/
/* DRAWING FUNCTIONS                                                   */
/***********************************************************************/

void X2DsetFont(Widget vp, char *fontname, char weight, char slant, int size);
void X2DdrawText(Widget w, char *text, int length, double x, double y);
void X2DfloodFill(Widget w, double x, double y);
void X2DfilledPolygon(Widget w, int n, double *x, double *y);
void X2DdrawPoint(Widget w,double x, double y);
void X2DdrawLine(Widget w, double x1, double y1, double x2, double y2);
void X2DdrawPolygon(Widget w, int n, double *x, double *y);
void X2DdrawCircle(Widget vp, double centre_x, double centre_y, double radius);
void X2DfilledCircle(Widget vp, double centre_x, double centre_y, double radius);
void X2DdrawEllipse(Widget vp, double centre_x, double centre_y, double radius_x, double radius_y);
void X2DfilledEllipse(Widget vp, double centre_x, double centre_y, double radius_x, double radius_y);
void X2DdrawArc(Widget vp, double x, double y, double width, double height, int angle1, int angle2);
void X2DfilledArc(Widget vp, double x, double y, double width, double height, int angle1, int angle2);
void X2DdrawRectangle(Widget vp, double x, double y, double width, double height);
void X2DfilledRectangle(Widget vp, double x, double y, double width, double height);
void X2DclearAll(Widget w);

void X2DsetPaletteRGB(Widget w, int index, int red, int green, int blue);
void X2DsetForeground(Widget w, Pixel col);
void X2DsetBackground(Widget w, Pixel col);
void X2DsetForegroundRGB(Widget w, int red, int green, int blue);
void X2DsetBackgroundRGB(Widget w, int red, int green, int blue);
void X2DsetLineStyle(Widget w, int style);
void X2DsetLineWidth(Widget w, int width);
Pixel X2DgetForeground(Widget w, int index);
Pixel X2DgetPaletteColor(Widget w, int col);
int X2DloadColorMap(Widget w, char *filename);
void X2DinitPalette(Widget w);

/***********************************************************************/
/* WIDGET FUNCTIONS                                                    */
/***********************************************************************/

static void X2Drealize();
static void X2DInitClass();
static void X2DInitialize();
static void X2Ddestroy();
static void X2Dresize();
static void X2Dredisplay();
static Boolean X2DsetValues();

/***********************************************************************/
/* UTILITY FUNCTIONS                                                   */
/***********************************************************************/

void X2DupdateGraphics(Widget w);

void X2DcurrentPageBack(Widget w);
void X2DcurrentPageFront(Widget w);

void X2DinitPointBuffer(Widget w);
void X2DflushPointBuffer(Widget w);
void X2DbufferPoint(Widget w, double x, double y, int color);

static void widget_click(Widget w, XEvent *event);
static void widget_dblclick(Widget w, XEvent *event);
static void widget_rt_click(Widget w, XEvent *event);
void X2DchangeDrawingGC(Widget w, unsigned long valuemask, XGCValues *values);

void X2DdrawPixmapOn(Widget w);
void X2DdrawPixmapOff(Widget w);

Boolean X2DisZooming(Widget w);
void X2DdisableZooming(Widget w);
void X2DenableZooming(Widget w);
static void start_zoom(Widget vp, XEvent *event);
void handle_zoom();
static void draw_zoom();
static void zoom_out(Widget w, XEvent *event);
static void end_zoom(Widget w, XEvent *event);

double Max(double a, double b);
double Min(double a, double b);

/***********************************************************************/

#ifdef __cplusplus
}
#endif

#endif  















