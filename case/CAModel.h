/*****************************************************************************/
/* CAModel.h: Header file for CAModel object.                                */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

/* $Id: CAModel.h,v 1.3 1996/09/05 11:29:34 mark Exp $ */

#ifndef _CAMODEL_H
#define _CAMODEL_H 1

class CAModel;

#include <CAVisual.h>
#include <CAEnviron.h>
#include <CACell.h>
#include <CASymbol.h>

/*****************************************************************************/

class CAModel

{
public:
   CAModel();
   CAModel(Widget new_parent);
   ~CAModel();
   virtual void Update()=0;
   virtual void Redraw();
   virtual void Click(double x, double y);
   virtual void RightClick(double x, double y);
   virtual void CoordinatesChanged();
   virtual void PopupEnvironRequester();
   Widget GetToplevelWidget();

   //   int NoCell = -1;

   
protected:
   virtual void Draw(int cell_index)=0;
   virtual void AllocateCells(int new_cells)=0;
   void PopupCellRequester(int cell_index);
   virtual void ReadEnvironRequester();
   
   CAEnviron *environ;
   CAVisual *visual;
   int cells;

   CACell **cell;

   Widget parent;
   Req *environ_req;
   int model_widgets;
   
private:
   Req *cell_req;
   int active_cell;
   static void environ_ok(Req *req, XtPointer data);
   static void environ_cancel(Req *req, XtPointer data);

   static void cell_ok(Req *req, XtPointer data);
   static void cell_cancel(Req *req, XtPointer data);
};

/*****************************************************************************/

class CAModelList
{
public:
   CAModelList();
   ~CAModelList();
   void Add(CAModel *new_model);
   int Remove(CAModel *dead_meat);
   void Reset();
   CAModel *Next();
protected:
   CAModel *model;
   CAModelList *current;
   CAModelList *next;
private:
};

/*****************************************************************************/


#endif // _CAMODEL_H

