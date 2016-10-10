/*****************************************************************************/
/*                                                                           */
/* DiffModel.h: Header file for CAModel object.                              */
/*                                                                           */
/*****************************************************************************/

// $Id: DiffModel.h,v 1.2 1997/05/29 15:30:18 mark Exp $

#ifndef _PERIODIC_H
#define _PERIODIC_H

#include <CA.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <CAApp.h>
#include <ParticleModel.h>
#include <conf.h>

/*****************************************************************************/

class Periodic: public ParticleModel

{
public:
   Periodic();
   Periodic(Widget new_parent, CASymmetry Symmetry);
   ~Periodic();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);

   CACell **subcell;

protected:
   virtual void Draw(int cell_index);
   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();
   
   CABackgroundedCircle *symbol;
   CABackgroundedCircle *subsymbol;
   
private:

   unsigned char *Symmetries;

   void GeneratePattern(void);
   
   int SColour(int cell_index);
   int EColour(double value);
   int EColourSub(double value);
};

/*****************************************************************************/

   const int symmetries = 30;

#endif













