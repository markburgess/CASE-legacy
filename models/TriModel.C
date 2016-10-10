/*****************************************************************************/
/*                                                                           */
/* Trilattice model simulation                                               */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

/* $Id: TriModel.C,v 1.6 1997/02/05 15:56:49 mark Exp $ */

#include <TriModel.h>

/*****************************************************************************/

TriModel::TriModel()
{
}

/*****************************************************************************/

TriModel::TriModel(Widget new_parent): CAModel(new_parent)

{ int i;
  long ltime;
  Widget temperature;
  CATriangularGrid *grid;
  char beta_string[40];
  visual = new CATextVisual(this,
			   VIS_Height, 500,
			   VIS_Width, 500,
			   VIS_Fontname, "Times",
			   VIS_Fontsize, 12,
			   VIS_TextLines, 10,
			   TAG_DONE
			   );
 srand48(lrand48());

 environ = grid = new CATriangularGrid(0.0,0.0,10.0,10.0,18,10,60);

 symbol = new CAColoredEllipseSymbol(visual, environ);

 environ->SetPeriodic(TRUE);

 AllocateCells(grid->GetNumberOfCells());

 environ->SyncViewport(visual);

 visual->Popup();
 visual->SetJustification(1, CATextVisual::CENTER);
 visual->SetFont(1, "Times", 'b', 'r', 14);
 visual->SetJustification(2, CATextVisual::CENTER);
 visual->SetJustification(3, CATextVisual::CENTER);

 visual->Print(1, "Triangular neighbours");
 visual->Print(2, "Oslo College");
 visual->Print(3, "Centre of Sci-Tech");
 visual->Print(4,"Illustrates how a triangular lattice");
 visual->Print(5,"is composed to two interlocking");
 visual->Print(6,"lattices. Edges cells are blue.");
 // Set color palette:
 
 visual->SetPaletteRGB(0, 255, 255, 255); //White
 visual->SetPaletteRGB(1, 0, 0, 0);       //Black
 visual->SetPaletteRGB(2, 55, 0, 0);     //red
 visual->SetPaletteRGB(3, 105, 0, 0);     //red
 visual->SetPaletteRGB(4, 155, 0, 0);     //red
 visual->SetPaletteRGB(5, 255, 0, 0);     //red
 visual->SetPaletteRGB(6, 0, 55, 0);     //green
 visual->SetPaletteRGB(7, 0, 105, 0);     //green
 visual->SetPaletteRGB(8, 0, 155, 0);     //green
 visual->SetPaletteRGB(9, 0, 205, 0);     //green
 visual->SetPaletteRGB(10, 0, 225, 0);     //green
 visual->SetPaletteRGB(11, 0, 255, 0);     //green
 visual->SetPaletteRGB(12, 0, 0, 55);     //blue
 visual->SetPaletteRGB(13, 0, 0, 105);     //blue
 visual->SetPaletteRGB(14, 0, 0, 155);     //blue
 visual->SetPaletteRGB(15, 0, 0, 255);     //blue

 pos = 45;

Redraw();
}

/*****************************************************************************/

TriModel::~TriModel()
{
}

/*****************************************************************************/

void TriModel::Redraw()

{ CATriangularGrid *grid = (CATriangularGrid*) environ;

grid->CalcGeometry();

CAModel::Redraw();
}

/*****************************************************************************/

void TriModel::Update()

{ int cell_index, i;
  TriCell *current;
  CATriangularGrid *grid = (CATriangularGrid*) environ;
  index_array neighbours;

pos++;

if (pos > cells-1)
   {
   pos = 0;
   }

visual->CurrentPageBack();

grid->DrawGrid(visual);

for (cell_index = 0; cell_index < cells; cell_index++)
  {
  current = (TriCell*) cell[cell_index];

  if (grid->CAGrid::IsEdgeCell(cell_index))
    {
    current->charge = 4;
    Draw(cell_index);
    }
  else
    {
    current->charge=0;
    Draw(cell_index);
    }
  }

cell_index = pos;

current = (TriCell*) cell[cell_index];
current->charge=3;
Draw(cell_index);

grid->QueryNeighbours(cell_index,1,neighbours);

for (i = 0; i < neighbours.GetSize(); i++)
   {
   current = (TriCell*) cell[neighbours.array[i]];
   current->charge=i+2;
   Draw(neighbours.array[i]);
   }

grid->QueryNeighbours(cell_index,2,neighbours);

for (i = 0; i < neighbours.GetSize(); i++)
   {
   current = (TriCell*) cell[neighbours.array[i]];
   current->charge=i+6;
   Draw(neighbours.array[i]);
   }

grid->QueryNeighbours(cell_index,3,neighbours);

for (i = 0; i < neighbours.GetSize(); i++)
   {
   current = (TriCell*) cell[neighbours.array[i]];
   current->charge=i+12;
   Draw(neighbours.array[i]);
   }

visual->CurrentPageFront();
}

/*****************************************************************************/

void TriModel::DblClick(double x, double y)
{

}

/*****************************************************************************/

void TriModel::Click(double x, double y)
{
}

/*****************************************************************************/

void TriModel::Draw(int cell_index)

{ TriCell *current = (TriCell*) cell[cell_index];

if (cell_index != NoCell)
   {
   symbol->Draw(cell_index, current->charge);
   }
}

/******************************************************************************/

void TriModel::AllocateCells(int new_cells)

{ int cell_index, x,y;
  CATriangularGrid *grid = (CATriangularGrid*) environ;


 for (cell_index=0; cell_index < cells; cell_index++)
    {
    delete cell[cell_index];
    }
 
 delete [] cell;

 cells = new_cells;
 cell  = new CACell* [cells];

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    cell[cell_index] = new TriCell;
    }
}

/*****************************************************************************/

void TriModel::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();
  CAGrid *grid = (CAGrid*) environ;
  
 if (cells != grid->GetNumberOfCells())
    {
    AllocateCells(grid->GetNumberOfCells());
    }
 Redraw();
}

