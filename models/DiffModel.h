/*****************************************************************************/
/*                                                                           */
/* DiffModel.h: Header file for CAModel object.                              */
/*                                                                           */
/*****************************************************************************/

// $Id: DiffModel.h,v 1.2 1997/05/29 15:30:18 mark Exp $

#ifndef _DIFFMODEL_H
#define _DIFFMODEL_H

#include <CA.h>
#include <CAVisual.h>
#include <CAEnviron.h>
#include <ThermoCell.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <CAApp.h>
#include <Particle.h>
#include <ParticleModel.h>
#include <conf.h>

/*****************************************************************************/

class Diffuse : public ParticleModel

{
public:
   Diffuse();
   Diffuse(Widget new_parent, CASymmetry Symmetry);
   ~Diffuse();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);

   CACell **subcell;
   CAEnviron *subenviron;

   CAGrid *subgrid;

protected:
   virtual void Draw(int cell_index);
   virtual void AllocateCells(int new_cells);
   virtual void ReadEnvironRequester();

   double WorkFunction(ThermoCell *x, ThermoCell *y, int i, int j);
   
   void DoFluctuations(int cell_index);
   void ThermalLatticeConduction(ThermoCell *current, index_array neighbours);

   int CountAlive(int cell_index, int level);
   int CountAliveSub(int cell_index, int level);
   
   CABackgroundedCircle *symbol;
   CABackgroundedCircle *subsymbol;
   
private:

   void CheckSymmetries(void);
   
   int TColour(double value);
   int EColour(double value);
   int EColourSub(double value);

   int nescaped;
   double iterations;
   double entropy;

   double Taverage;                 // For whole lattice


   double p2_2m;                    // Kinetic energy of particles arriving			       
};

/*****************************************************************************/
#endif













