/* $Id: XYModel.h,v 1.6 1996/11/25 16:53:45 mark Exp $ */


#ifndef _XYMODEL_H
#define _XYMODEL_H

#include <CAVisual.h>
#include <CAEnviron.h>
#include <XYCell.h>
#include <CASymbol.h>
#include <CAModel.h>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <X11/Xaw/Dialog.h>
#include <conf.h>

/*****************************************************************************/

class XYModel : public CAModel
{
public:

   XYModel();
   XYModel(Widget new_parent);
   ~XYModel();
   virtual void Update();
   virtual void RightClick(double x, double y);
   
protected:
   
   virtual void Draw(int cell_index);
   virtual void Redraw();

   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();
   
   CAArrowSymbol *symbol;
   
   void Init_spinconfig();
   int Monte_Carlo(int cell_index);
   double Trial_angle(XYCell *current);
   double New_energy(int cell_index, double proeve_vinkel);
   double Old_energy(int cell_index);
   int CheckVortex(int cell_index);
   void  CheckVortices();
   double beta;
   Req temp_req;
   int iter;

private:
   static void temp_ok(Req *req, XtPointer data);
   static void temp_cancel(Req *req, XtPointer data);
   static void set_temp(Widget w, XtPointer data, XtPointer garbage);

};

/*****************************************************************************/
   

#endif // _XYMODEL_H



