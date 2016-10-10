/*****************************************************************************/
/* CAModel.h: Header file for CAModel object.                                */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/


#ifndef _GoLMODEL_H
#define _GoLMODEL_H

#include <CAVisual.h>
#include <CAEnviron.h>
#include <GoLCell.h>
#include <CASymbol.h>
#include <CAModel.h>
#include <ParticleModel.h>
#include <CAApp.h>
#include <conf.h>

/*****************************************************************************/

class GameOfLife : public ParticleModel

{
public:
   GameOfLife(Widget new_parent, CASymmetry Symmetry);
   ~GameOfLife();
   virtual void Update();
   virtual void Click(double x, double y);
   virtual void DblClick(double x, double y);
   
protected:
   virtual void Draw(int cell_index);
   virtual void AllocateCells(int new_cells);
   void CountNeighbours(int *count);
   void BuildNextGeneration(int *count);
   virtual void ReadEnvironRequester();
   
   CAColoredEllipseSymbol *symbol;
   
private:
};

/*****************************************************************************/

#endif // _CAMODEL_H









