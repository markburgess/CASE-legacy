/*===========================================================================*/
/*                                                                           */
/* File: ParticleModel.C                                                     */
/*                                                                           */
/* Revision: $Id$                                                            */
/*                                                                           */
/*===========================================================================*/

#include <CAModel.h>
#include <Particle.h>
#include <ParticleModel.h>

/*****************************************************************************/

ParticleModel::ParticleModel(Widget new_parent,CASymmetry Environment):CAModel(new_parent)

{
switch (Environment)
   {
   case CATriangular:  environ    = grid    = new CATriangularGrid(0,0,8,8,24,24,60);
                       break;

   case CARectangular: environ    = grid    = new CARectangularGrid;
   }
}

/*****************************************************************************/

ParticleModel::~ParticleModel()

{
}

/*****************************************************************************/

void ParticleModel::FindPattern()

    // Look for periodical structures in the lattice
    // The main problem here is that overlapping symmetries are possible.
    // One might imagine that the best way to look for symmetries on the
    // lattice would be to search along independent basis vectors for
    // periodicity, but this will not work, since it will simply overwrite
    // the reicgnized regions along linearly independent basis vectors.
    // Instead we have to search on multiple hierachies, but still
    // linearly. I.e. we think of the 2d lattice as structures within
    // structures and parse accordingly, knowing the lattice size.
    // Thus all syymetric structures in the lattice are one dimensional
    // periodic structures also, but those which are separated by  more
    // than the cutoff should not be marked contiguously because they belong
    // to an orthogonal direction.
    
{ int cell_index, i, test_period, string_length, delta, stepsize;
  Particle *current, *next;

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (Particle *) cell[cell_index];
   current->state = 0.0;
   }
 
for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (Particle *) cell[cell_index];

   if (current->Alive())
      {
      SearchPeriod(cell_index,test_period,string_length,1);

      if (test_period > 0 && string_length > 2)
	 {
	 // Colour the string as far as it goes. The formula is
	 // from cell_index to cell_index+(string_length-1)*test_period+1
	 // in order to not colour empty spaces at the end

	 MarkRegion(cell_index,test_period,string_length,1);
	 }

      // Now get sliced periods of order of lattice size within +/- delta/3
      // grid->cols is the lattice width. Delta finds diagonal strings
      // grid->cols+delta is now the test period and we can add to this
      // multiples of the lattice period.

      for (delta = 0; delta < 3; delta++)
	 {
	 stepsize = grid->cols+delta;
      
	 SearchPeriod(cell_index,test_period,string_length,stepsize);
	 
	 if (string_length > 2)
	    {
	    MarkRegion(cell_index,test_period,string_length,stepsize);
	    }
	 
	 stepsize = grid->cols-delta;
	 
	 SearchPeriod(cell_index,test_period,string_length,stepsize);
	 
	 if (string_length > 2)
	    {
	    MarkRegion(cell_index,test_period,string_length,stepsize);
	    }
	 }	    	    
      }
   }
}


/*****************************************************************************/

void ParticleModel::SearchPeriod(int cell_index, int &test_period, int &string_length, int stepsize)

{ unsigned int i;
  Particle *next;
  const int cutoff = 7; // Arbitrary, this chosen for Palladium
  
 // Look for next cell to determine a possible period length

 test_period = 0;
 string_length = 0;
 
 for (i = cell_index+stepsize; (i < cells); i+=stepsize)
    {
    next = (Particle *) cell[i];
    if (next->Alive())
       {
       test_period = (i - cell_index)/stepsize;
       break;
       }
    }
 
 if (test_period == 0)
    {
    return;
    }

 if (test_period > cutoff)
    {
    test_period = 0;
    return;
    }
 
 // See how far the period ansatz works
 
 string_length = 0;
 
 for (i = cell_index; i < cells; i+= test_period*stepsize)
    {
    next = (Particle *) cell[i];
    if (!next->Alive())
       {
       break;
       }
    string_length++;
    } 
}

/*****************************************************************************/

void ParticleModel::MarkRegion(int cell_index, int test_period, int string_length, int step)

{ int i;
  Particle *current;

 for (i = cell_index; i <= cell_index+((string_length-1)*test_period); i+=step)
    {
    if (i >= cells)
       {
       return;
       }

    current = (Particle *) cell[i];
    
    if (current->state == 0.0)  // Don't overwrite
       {
       current->state = (double) test_period;
       }
    } 
}





