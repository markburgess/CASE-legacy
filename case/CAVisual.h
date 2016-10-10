/*****************************************************************************/
/* CAVisual.h: Header file for CAVisual object.                              */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

#ifndef _CAVISUAL_H
#define _CAVISUAL_H 1

class CAVisual;

#include <X2Dview.h>
#include <toolkit.h>
#include <taglist.h>
#include <menu.h>
#include <CAModel.h>
#include <CAReq.h>


/*****************************************************************************/
/* CAVisual Tags:                                                            */
/*****************************************************************************/

#ifndef TAG_DONE 
#define TAG_DONE 0
#endif

enum
{
    VIS_Width,
    VIS_Height,
    VIS_Xmin,
    VIS_Xmax,
    VIS_Ymin,
    VIS_Ymax,
    VIS_DrawingMode,
    VIS_GfxScrollbars,
    VIS_TxtScrollbars,
    VIS_TextLines,
    VIS_LineDist,
    VIS_LineWidth,
    VIS_Fontname,
    VIS_Fontsize,
    VIS_Justification,
    VIS_FontWeight,
    VIS_FontSlant,
    CAVIS_TAGS,
};

/*****************************************************************************/

class CAVisTagList : public TagList
{
public:
   CAVisTagList();
};

/*****************************************************************************/

class CAVisual
{
public:
   CAVisual();
   CAVisual(CAModel *parent_model, ...);
   ~CAVisual();
   virtual void Initialize(CAModel *parent_model, ...);
   void GetCoordinateSystem(double &get_x_min, double &get_y_min,
		   double &get_x_max, double &get_y_max);
   Widget GetGfxWidget();
   int Popup();
   int Popdown();
   menubar GetMenubar();
   Widget AddControlMenuItem(char *title, XtCallbackProc function, XtPointer data);
   Widget AddFileMenuItem(char *title, XtCallbackProc function, XtPointer data);


// DRAWING FUNCTIONS: 
   void ClearAll();
   void SetDrawable(int new_drawable);
   void DrawPoint(double x, double y);
   void DrawLine(double x1, double y1, double x2, double y2);
   void DrawPolygon(polygon &poly);
   void FilledPolygon(polygon &poly);
   void SetLineWidth(int width);
   void SetLineStyle(int style);
   void SetDrawingFont(char *fontname, char weight, char slant, int size);
   void DrawText(char *text, int length, double x, double y);
   void DrawCircle(double centre_x, double centre_y, double radius);
   void FilledCircle(double centre_x, double centre_y, double radius);
   void DrawEllipse(double cx, double cy, double rx, double ry);
   void FilledEllipse(double cx, double cy, double rx, double ry);
   void DrawArc(double x, double y, double width, double height, int angle1, int angle2);
   void FilledArc(double x, double y, double width, double height, int angle1, int angle2);
   void DrawRectangle(double x, double y, double width, double height);
   void FilledRectangle(double x, double y, double width, double height);
   void SetPaletteRGB(int color, int r, int g, int b);
   void SetDrawingColor(int color);
   void CurrentPageFront();
   void CurrentPageBack();
   int GetAllocedColors();

   void PopUpColorRequester();
   enum {POSTSCRIPT, PIXMAP, WINDOW}; // Drawable types

   // WRITING FUNCTIONS (Abstract):
       
   virtual void Print(int line, char *string)=0;
   virtual void Printf(int line, char *format, ...)=0;
   virtual void SetFont(char *fontname, char weight, char slant, int size)=0;
   virtual void SetFont(int line, char *fontname, char weight, char slant, int size)=0;
   virtual void SetJustification(int just)=0;
   virtual void SetJustification(int line, int just)=0;

   virtual void SetLineColor(int fgcol, int bgcol)=0;
   virtual void SetLineColor(int line, int fgcol, int bgcol)=0;
					     
   virtual void color_requester_ok();
   virtual void color_requester_cancel();

   
   
protected:

   int drawable;
   int gfx_width, gfx_height;
   int alloced_colors;
   double x_min, x_max, y_min, y_max;
   Widget parent_widget, toplevel, main_form, gfx_scrollbars, gfx_form, gfx_field;
   CAModel *model;
   ColorReq *color_req;
   CAVisTagList taglist;

   menubar vis_menus;
   menu file_menu;
   menu control_menu;
   
   virtual void init_gfx_field();
   virtual void init_text_field()=0;

   static void close_vis(Widget w, XtPointer data, XtPointer nothing);
   static void palette_req(Widget w, XtPointer data, XtPointer nothing);
   static void options_req(Widget w, XtPointer data, XtPointer nothing);
   static void zoom_out(Widget w, XtPointer data, XtPointer nothing);
   
private:

   static void coordinates_changed(Widget w, XtPointer data, XtPointer nothing);
   static void click_handler(Widget w, XtPointer data, X2DPoint *pos);
   static void right_click_handler(Widget w, XtPointer data, X2DPoint *pos);

};

/*****************************************************************************/

class CATextVisual : public CAVisual
{
public:
   CATextVisual();
   CATextVisual(CAModel *parent_model, ...);
   ~CATextVisual();

   virtual void Initialize(CAModel *parent_model, ...);

   // WRITING FUNCTIONS:
   virtual void Print(int line, char *string);
   virtual void Printf(int line, char *format, ...);
   virtual void SetFont(char *fontname, char weight, char slant, int size);
   virtual void SetFont(int line, char *fontname, char weight, char slant, int size);
   virtual void SetJustification(int just);
   virtual void SetJustification(int line, int just);

   virtual void SetLineColor(int fgcol, int bgcol);
   virtual void SetLineColor(int line, int fgcol, int bgcol);

   enum {LEFT, CENTER, RIGHT};	      // Text Justification

protected:
   int text_width, text_lines, text_line_dist;
   Widget text_scrollbars, text_line_box, text_form, *text_line;
   XFontStruct *font_info;
   virtual void init_text_field();

private:
};


/*****************************************************************************/

const int CA_DEFAULT_LINEWIDTH = 1;
const int CA_DOUBLE_LINEWIDTH = 2;

/*****************************************************************************/

#endif // _CAVISUAL_H





