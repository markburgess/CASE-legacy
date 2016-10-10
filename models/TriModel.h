/* $Id $ */


#ifndef _TRIMODEL_H
#define _TRIMODEL_H

#include <CAVisual.h>
#include <CAEnviron.h>
#include <TriCell.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xaw/Dialog.h>
#include <CA.h>
#include <conf.h>

/*****************************************************************************/

class TriModel : public CAModel
{
public:

   TriModel();
   TriModel(Widget new_parent);
   ~TriModel();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);
   
protected:
   
   virtual void Draw(int);
   virtual void Redraw();

   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();
   
   CAColoredEllipseSymbol *symbol;

   void   InitCells();

   int     pos;

   double  beta;
   Req     temp_req;
   int     iter;
   
private:

   double iterations;

   void CheckSymmetries(void);
};

/*****************************************************************************/

class TriPattern : public CAModel
{
public:

   TriPattern();
   TriPattern(Widget new_parent);
   ~TriPattern();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);
   
protected:
   
   virtual void Draw(int);
   virtual void Redraw();

   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();
   
   CAColoredEllipseSymbol *symbol;

   void   InitCells();
   int    pos;

   double  beta;
   Req     temp_req;
   int     iter;
   
private:

   double iterations;

   void CheckSymmetries(void);
};

/*****************************************************************************/


#endif // _TRIMODEL_H



