/*****************************************************************************/
/*                                                                           */
/* DiffModel.h: Header file for CAModel object.                              */
/*                                                                           */
/*****************************************************************************/

// $Id: Palladium.h,v 1.2 1997/05/29 15:30:18 mark Exp $

#ifndef _PALLADIUM_H
#define _PALLADIUM_H

#include <stdio.h>
#include <CA.h>
#include <CAVisual.h>
#include <CAEnviron.h>
#include <ThermoCell.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <CAApp.h>
#include <Particle.h>
#include <ParticleModel.h>
#include <X11/Xaw/Dialog.h>
#include <conf.h>

/*****************************************************************************/

class Palladium : public ParticleModel

{
public:
   Palladium();
   Palladium(Widget new_parent, CASymmetry Symmetry);
   ~Palladium();
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
   double AdsorptionEnergy(int neigh1_alive, int neigh2_alive, int neigh3_alive, int neigh4_alive, int neigh5_alive, int currentsub_alive, int neigh1sub_alive, int neigh2sub_alive, int neigh3sub_alive, int neigh4sub_alive, int neigh5sub_alive, int fcc);
   double AdsorptionEnergySub(int alive0, int alive1, int alive2, int alive3, int alive4, int alive5, int subalive2, int subalive5);
   double BridgeEnergy(int neigh1_alive, int neigh2_alive, int neigh_neigh1_alive, int neigh_neigh2_alive);
   
   void DoFluctuations(int cell_index);
   void ThermalLatticeConduction(ThermoCell *current, index_array neighbours);

   int CountAlive(int cell_index, int level);
   int CountAliveSub(int cell_index, int level);
   
   CABackgroundedCircle *symbol;
   CABackgroundedCircle *subsymbol;

   virtual void FindPattern(void);
   virtual void MarkRegion(int cell_index, int test_period, int string_length, int stepsize);   
   virtual void SearchPeriod(int cell_index, int &test_period, int &string_length, int stepsize);
   Req     temp_req;
   Req     hydrogen_req;
   Req     symmetry_req;

private:

   void CheckSymmetries(void);
   void SaveState(void);
   void LoadState(void);
   
   int SColour(double value);
   int EColour(double value);
   int EColourSub(double value);

   static void temp_ok(Req *req, XtPointer data);
   static void temp_cancel(Req *req, XtPointer data);
   static void set_temp(Widget w, XtPointer data, XtPointer garbage);
   static void hydrogen_ok(Req *req, XtPointer data);
   static void hydrogen_cancel(Req *req, XtPointer data);
   static void set_hydrogen(Widget w, XtPointer data, XtPointer garbage);
   static void symmetry_ok(Req *req, XtPointer data);
   static void symmetry_cancel(Req *req, XtPointer data);
   static void set_symmetry(Widget w, XtPointer data, XtPointer garbage);
   
   double widgetT;                   // Widget for setting temperature
   double widgetH;                   // Widget for getting hydrogen filling factor
   int    widgetS;                   // Widget for setting an initial symmetry
							      
   double Taverage;                  // For whole lattice


   double p2_2m;                     // Kinetic energy of particles arriving			       
};

/*****************************************************************************/


#endif


