/* $Id: XYBModel.h,v 1.17 1996/12/18 14:23:30 mark Exp $ */


#ifndef _XYBMODEL_H
#define _XYBMODEL_H

#include <CAVisual.h>
#include <CAEnviron.h>
#include <XYBCell.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xaw/Dialog.h>
#include <CA.h>
#include <conf.h>

/*****************************************************************************/

class XYBModel : public CAModel
{
public:

   XYBModel();
   XYBModel(Widget new_parent);
   ~XYBModel();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);

   double *south;   // these are link pointers, same dimension
   double *east;    // as the cell[] array.

protected:
   
   virtual void Draw(int);
   virtual void Redraw();

   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();
   
   CAColoredSignSymbol *symbol;

   void   DrawArrow(int cell_index, double length, int XorY);
   void   InitCells();
   void   SetInitialPlaquetteState();
   void   CheckLinks(int r, int c);
   double Link(int direction,int row,int col);
   void   SetLink(int direction, int row, int col, double value);
   void   Monte_Carlo(int cell_index);
   int    Vorticity(int);
   
   double  beta;
   Req     temp_req;
   Req     vort_req;
   int     iter;
   double  *init_cell;
   double  fudge_factor;  // This is to be explained by Are.
   
   int vortices;

private:
   static void temp_ok(Req *req, XtPointer data);
   static void temp_cancel(Req *req, XtPointer data);
   static void set_temp(Widget w, XtPointer data, XtPointer garbage);
   static void vort_ok(Req *req, XtPointer data);
   static void vort_cancel(Req *req, XtPointer data);
   static void set_vort(Widget w, XtPointer data, XtPointer garbage);

};

/*****************************************************************************/

#endif // _XYBMODEL_H



