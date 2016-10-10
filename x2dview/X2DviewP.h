/*********************************************************************/
/* X2DviewP.h: Private header file for X2Dview widget                */
/*********************************************************************/
/* copyright (c)1995,1996 Thomas Engh Sevaldrud                      */  
/*                        Jon Anders Mikkelsen                       */
/*********************************************************************/

#ifndef _X2DVIEWP_H
#define _X2DVIEWP_H 1

/*********************************************************************/

#define MAX_BUF_POINTS 500
#define MAX_COLS 256

/*********************************************************************/

typedef struct _X2DViewClassPart
{
  int ignore;
} X2DViewClassPart;

/*********************************************************************/

typedef struct _X2DViewClassRec
{
  CoreClassPart    core_class;
  X2DViewClassPart view_class;
} X2DViewClassRec;

extern X2DViewClassRec X2DviewClassRec;

/*********************************************************************/

typedef struct 
{
  XPoint data[MAX_COLS][MAX_BUF_POINTS];
  int    npoints[MAX_COLS];
} point_buffer;

/*********************************************************************/
/* RESOURCES:                                                        */
/*********************************************************************/

typedef struct _X2DViewPart
{
   double       x_min, x_max;
   double       y_min, y_max;	      /* Boundaries for logical coordinate system */
   double       def_x_min, def_x_max; 
   double       def_y_min, def_y_max; /* Default boundaries */
   GC           gc;		      /* Graphics context for drawing operations */
   XPoint       zbox[5];	      /* Point coordinates for zoom box */
   GC           zbox_gc;	      /* Graphics context for zoom box */
   int          zbox_line_style;      /* Line style for zoom box */
   int          zbox_line_width;      /* Line width for zoom box */
   Pixel        zbox_foreground;      /* Foreground color for zoom box */
   Boolean      zooming;	      /* True if zooming*/
   Boolean      zooming_allowed;      
   Boolean      draw_pixmap;	      /* Drawing in both window and pixmap */
   int          ncolors;              /* Number of available colors */
   int          alloc_colors;         /* Number of alloced colors in colormap */
   Colormap     cmap;
   Pixel        palette[MAX_COLS];
   int          palette_offset;	      /* offset to the firs color in palette */
   Boolean      install_cmap;         /* Install private colormap or not */
   XFontStruct  *font_info;           /* Drawing font */
   XtIntervalId timeout;
   point_buffer points;		      /* Point buffer for XDrawPoints() */
   Pixmap       pixmap;
   double       pixx, pixy;	      /* Number of pixels pr. logical unit */
   matrix       vm;		      /* Transformation matrix for the transformation*/
				      /* pixels <=> logical coordinates */
   XtCallbackList viewportChanged;    /* Callback function for redrawing graphics */
   XtCallbackList click;	      /* Callback function for left mouse button press*/
   XtCallbackList dblClick;	      /* Callback function for left mouse button double click*/
   XtCallbackList rtClick;	      /* Callback function for right mouse button press*/
/* New (X11R6.1 Double buffer extension (Xdbe): */

   XdbeSwapInfo   *swapinfo;
   XdbeBackBuffer backpage;
   Drawable       currentpage;

} X2DViewPart;

/*********************************************************************/

typedef struct _X2DViewRec
{
  CorePart    core;
  X2DViewPart view;
} X2DViewRec;

/*********************************************************************/
#endif /* _X2DVIEWP_H */





