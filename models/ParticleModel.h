/*****************************************************************************/
/*                                                                           */
/* File: ParticleModel.h                                                     */
/*                                                                           */
/* Revision: $Id$                                                            */
/*                                                                           */
/*****************************************************************************/

#ifndef PARTICLEMODEL_H
#define  PARTICLEMODEL_H

#include <CAApp.h>
#include <CAModel.h>
#include <CAVisual.h>
#include <CAEnviron.h>
#include <CACell.h>
#include <CASymbol.h>

/*****************************************************************************/

class ParticleModel: public CAModel

{
  public:
    ParticleModel(Widget new_parent, CASymmetry Symmetry);
   ~ParticleModel();

    CAGrid *grid;
   
  protected:
 
   void FindPattern(void);
   void MarkRegion(int cell_index, int test_period, int string_length, int stepsize);   
   void SearchPeriod(int cell_index, int &test_period, int &string_length, int stepsize);
};

#endif
