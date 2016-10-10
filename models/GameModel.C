/*****************************************************************************/
/* GameModel.C: Header file for CAModel object.                               */
/*                                                                           */
/*****************************************************************************/

/* $Id: GameModel.C,v 1.5 1997/02/05 15:56:38 mark Exp $ */

#include <GameModel.h>
#include <X11/Xaw/Toggle.h>

/*****************************************************************************/
/* Politics object:                                                        */
/*****************************************************************************/

Politics::Politics(Widget new_parent, CASymmetry Environment):ParticleModel(new_parent,Environment)
    
{ int i;
  
visual = new CATextVisual
                (
                this,
   		VIS_Height, 500,
		VIS_Width, 500,
		VIS_Fontname, "Times",
		VIS_Fontsize, 12,
		VIS_TextLines, 10,
		TAG_DONE
		);


symbol = new CAColoredEllipseSymbol(visual, environ);

AllocateCells(grid->GetNumberOfCells());
 
environ->SyncViewport(visual);
environ->SetDrawGrid(TRUE);
visual->Popup();

visual->SetJustification(1, CATextVisual::CENTER);
visual->SetFont(1, "Times", 'b', 'r', 14);

visual->Print(1,"Game of Life");

//visual->Print(2, "Line number 2");
//visual->SetFont(4, "Courier", 'b', 'i', 12);
//visual->SetJustification(4, CATextVisual::RIGHT);
//visual->Print(4, "Right");
//visual->Print(5, "And left again"); 
//visual->Printf(6, "Int %d og float %f", 8, 2.5); 

// Set color palette:
visual->SetPaletteRGB(0, 255, 255, 255); //White
visual->SetPaletteRGB(1, 0, 0, 0);       //Black
visual->SetPaletteRGB(2, 255, 0, 0);     //Red
 
Redraw();
}

/*****************************************************************************/

Politics::~Politics()

{
}

/*****************************************************************************/

void Politics::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();
  CAGrid *grid = (CAGrid*) environ;

if (cells != grid->GetNumberOfCells())
   {
   AllocateCells(grid->GetNumberOfCells());
   }

Redraw();
}


/*****************************************************************************/

void Politics::Draw(int cell_index)
    
{ GameCell *current = (GameCell*) cell[cell_index];
 
 if(current->Alive())
    {
    symbol->Draw(cell_index, 2);
    }
 else
    {
    symbol->Draw(cell_index, 0);
    }
}

/*****************************************************************************/

void Politics::Update()
    
{ int i, nalive=0;
  GameCell *current;
  int count[cells];

 CountNeighbours(count);
 BuildNextGeneration(count);
 
 for (i = 0; i < cells; i++)
    {
    current = (GameCell*) cell[i];
    if (current->Alive())
       {
       nalive++;
       }
    }

 visual->Printf(1, "%d cells alive", nalive);
 environ->DrawGrid(visual);
}

/*****************************************************************************/

void Politics::DblClick(double x, double y)
{
// PopupEnvironRequester();
// visual->PopUpColorRequester();
/* int cell_index;

 cell_index = environ->QueryCell(x, y);
 PopupCellRequester(cell_index);
*/
}

/*****************************************************************************/

void Politics::Click(double x, double y)

{ int cell_index;
  CAGrid *grid = (CAGrid*) environ;

cell_index = grid->QueryCell(x, y);

GameCell *current = (GameCell*) cell[cell_index];

if (current->Alive())
   {
   current->SetAlive(False);
   }
else
   {
   current->SetAlive(True);
   }

Draw(cell_index);
}

/*****************************************************************************/

void Politics::AllocateCells(int new_cells)

{ int i;

 for (i = 0; i < cells; i++)
    {
    delete cell[i];
    }
 
 delete [] cell;

 cells = new_cells;
 cell = new CACell* [cells];

 for (i = 0;i < cells; i++)
    {
    cell[i]=new GameCell;
    }
}

/*****************************************************************************/

void Politics::CountNeighbours(int *count)

{ int cell_index, i, census;
  CAGrid *grid;
  GameCell *current;
 
 grid = (CAGrid*) environ;
 
 for(cell_index=0; cell_index<cells; cell_index++)
     
    { index_array neighbour;

    census = 0;

    grid->QueryNeighbours(cell_index, 1, neighbour);
  
    for(i = 0; i < neighbour.GetSize(); i++)
       {
       if (neighbour.array[i] != NoCell)
	  {
	  current = (GameCell*) cell[neighbour.array[i]]; 

	  if (current->Alive())
	     {
	     census++;
	     }
	  }
       }

    count[cell_index] = census;
    }
}

/*****************************************************************************/

void Politics::BuildNextGeneration(int *count)
{
 int i;
 GameCell *current;

 for (i = 0; i < cells;i++)
    {
    current = (GameCell*) cell[i];
    if(current->Alive())
       {
       switch(count[i])
	  {
	  case 2:
	   
	  case 3: 
	      break;
	   
	  default: 
	      current->SetAlive(False);
	      Draw(i);
	      break;
	  }
       }
    else
       {
       switch(count[i])
	  {
	  case 3:
	      current->SetAlive(True);
	      Draw(i);
	      break;
	      
	  default: 
	      break;
	  }
       }
    }
}










