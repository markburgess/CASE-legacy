/*****************************************************************************/
/*                                                                           */
/* PeriodModel.C: Program file for CAModel object.                           */
/*                                                                           */
/*****************************************************************************/

/* $Id: $ */

#include <CA.h>
#include <Particle.h>
#include <Periodic.h>
#include <time.h>

/*****************************************************************************/
/* Periodic object:                                                            */
/*****************************************************************************/

Periodic::Periodic(Widget new_parent, CASymmetry Environment):ParticleModel(new_parent,Environment)
    
{ int i;
  long ltime;

visual = new CATextVisual
                (
                this,
   		VIS_Height, 500,
		VIS_Width, 500,
		VIS_Fontname, "Times",
		VIS_Fontsize, 12,
		VIS_TextLines, 20,
		TAG_DONE
		);

symbol = new CABackgroundedCircle(visual, environ);
subsymbol = new CABackgroundedCircle(visual, environ);

subcell=NULL; 

AllocateCells(grid->GetNumberOfCells());

srand48(lrand48());
 
environ->SyncViewport(visual);
environ->SetDrawGrid(FALSE);
environ->SetPeriodic(TRUE);
visual->Popup();

visual->SetJustification(1, CATextVisual::CENTER);
visual->SetFont(1, "Times", 'b', 'r', 14);
visual->SetJustification(2, CATextVisual::CENTER); 
visual->SetJustification(3, CATextVisual::CENTER);
visual->SetFont(3, "Times", 'b', 'r', 12);
visual->SetJustification(4, CATextVisual::CENTER);
visual->SetFont(4, "Times", 'b', 'r', 12);
visual->SetJustification(5, CATextVisual::CENTER);
visual->SetFont(5, "Times", 'b', 'r', 12);
visual->SetJustification(6, CATextVisual::CENTER);
visual->SetFont(6, "Times", 'b', 'r', 12);

visual->Print(1,"Pattern Detection");
visual->Print(2,"On a tri-lattice"); 

// Set color palette:
visual->SetPaletteRGB(0, 255, 255, 255); /* white */
visual->SetPaletteRGB(1, 0, 0, 0);       /* black */

/* background colours - blue shades */

visual->SetPaletteRGB(2, 40, 0,   190);
visual->SetPaletteRGB(3, 60, 40,  230);
visual->SetPaletteRGB(4, 60, 80,  220);
visual->SetPaletteRGB(5, 60, 100, 210);
visual->SetPaletteRGB(6, 40, 120, 200);
visual->SetPaletteRGB(7, 60, 160, 200);
visual->SetPaletteRGB(8, 80, 210, 130);
visual->SetPaletteRGB(9, 100, 250, 125);
visual->SetPaletteRGB(10,120, 255, 100);


/* foreground colours - red shades */

visual->SetPaletteRGB(11, 250, 20, 40);
visual->SetPaletteRGB(12, 230, 0, 40);
visual->SetPaletteRGB(13, 220, 0, 40);
visual->SetPaletteRGB(14, 200, 100, 70);
visual->SetPaletteRGB(15, 210, 150, 100);
visual->SetPaletteRGB(16, 220, 200, 120);
visual->SetPaletteRGB(17, 230, 230, 150);
visual->SetPaletteRGB(18, 240, 240, 200);
visual->SetPaletteRGB(19, 240, 250, 220);
 
/* foreground subcolours - dark red shades */

visual->SetPaletteRGB(21, 150, 10, 20);
visual->SetPaletteRGB(22, 130, 0, 20);
visual->SetPaletteRGB(23, 120, 0, 20);
visual->SetPaletteRGB(24, 110, 30, 35);
visual->SetPaletteRGB(25, 110, 70, 50);
visual->SetPaletteRGB(26, 120, 100, 60);
visual->SetPaletteRGB(27, 130, 110, 70);
visual->SetPaletteRGB(28, 140, 110, 100);
visual->SetPaletteRGB(29, 140, 120, 110);

Redraw();
}

/*****************************************************************************/

Periodic::~Periodic()

{
}

/*****************************************************************************/

void Periodic::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();

if (cells != grid->GetNumberOfCells())
   {
   AllocateCells(grid->GetNumberOfCells());
   }

Redraw();
}


/*****************************************************************************/

void Periodic::Draw(int cell_index)
    
{ Particle *current_cell = (Particle*) cell[cell_index];

if (current_cell->Alive())
   {
   symbol->Draw(cell_index,SColour(cell_index),EColour(current_cell->state));
   }
else
   {
   symbol->Draw(cell_index,SColour(cell_index),CANoSymbol);   
   }
}

/*****************************************************************************/

void Periodic::Update()
    
{ int i;
 
 visual->CurrentPageBack();

 GeneratePattern();
 FindPattern();

 for (i = 0; i < cells; i++)
    {
    Draw(i);
    } 

 environ->DrawGrid(visual);
 visual->CurrentPageFront(); 
}

/*****************************************************************************/

void Periodic::DblClick(double x, double y)

{
}

/*****************************************************************************/

void Periodic::Click(double x, double y)

{ int cell_index;

cell_index = grid->QueryCell(x, y);

Particle *current_cell = (Particle*) cell[cell_index];

if (current_cell->Alive())
   {
   current_cell->SetAlive(False);
   }
else
   {
   current_cell->SetAlive(True);
   }

Draw(cell_index);
}

/*****************************************************************************/

void Periodic::AllocateCells(int new_cells)

{ int i, row, col;
  Particle *current_cell;
  Particle *currentsub_cell;

 for (i = 0; i < cells; i++)
    {
    delete cell[i];
    }

 if (cell != NULL)
    {
    delete [] cell;
    }

 cells = new_cells;
 
 cell = new CACell* [cells];

 for (i = 0; i < cells; i++)
    {
    cell[i] = new Particle;
    current_cell = (Particle *)cell[i];
    current_cell->state = 0;
    current_cell->SetAlive(False);
    }
}

/*****************************************************************************/
// Level 2
/*****************************************************************************/

void Periodic::GeneratePattern()

{ int i,j, cell_i;
  Particle *current;
  
for (i = 0; (i < cells); i++)
   {
   current = (Particle*) cell[i];
   current->SetAlive(False);
   }

 cell_i = (int )((drand48()+0.5)*cells);

for (i = cell_i; (i < cell_i+6) && (i < cells); i++)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }

cell_i = (int )((drand48()+0.1)*cells);

for (i = cell_i; (i < cell_i+12) && (i < cells); i+=2)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }

cell_i = (int )((drand48()+0.1)*cells); 
 
for (i = cell_i; (i < cell_i+18) && (i < cells); i+=3)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }

cell_i = (int )((drand48()+0.1)*cells); 

for (i = cell_i; (i < cell_i+24) && (i < cells); i+=4)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }

cell_i = (int )((drand48()+0.1)*cells); 

for (i = cell_i; i < cells; i+=grid->cols)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }
 
 cell_i = (int )((drand48()+0.5)*cells);
for (i = cell_i; i < cells; i+=2*grid->cols)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }
 
 cell_i = (int )((drand48()+0.5)*cells);
 for (i = cell_i; i < cells; i+=grid->cols+1)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }
 

cell_i = (int )((drand48()+0.1)*cells); 

for (i = cell_i; i < cells; i+=grid->cols-1)
   {
   current = (Particle*) cell[i];
   current->SetAlive(True);
   Draw(i);
   }

for (i = 0; i < cells; i++)
   {
   cell_i = (int )((drand48()+0.1)*cells/2);
   current = (Particle*) cell[cell_i];
   current->SetAlive(True);
   Draw(i);   
   }
}


/*****************************************************************************/

int Periodic::SColour(int cell_index)

// Colours 2 to 10 are lattice temp
    
{ const double no_of_colours = 8.0;
  Particle *current = (Particle *) cell[cell_index];

 return (3+(int)(0.5*(current->state)+1.5));
}

/*****************************************************************************/

int Periodic::EColour(double value)

// Colours 11 to 19
    
{ const double no_of_colours = 8.0;

return 0;
}

