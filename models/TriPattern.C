/*****************************************************************************/
/*                                                                           */
/* Trilattice model simulation                                               */
/*                                                                           */
/*****************************************************************************/

/* $Id: TriPattern.C,v 1.6 1997/02/05 15:56:49 mark Exp $ */

#include <TriModel.h>

 // Essentially identical to TriModel. This should have been structured
 // differently.

/*****************************************************************************/

TriPattern::TriPattern()
{
}

/*****************************************************************************/

TriPattern::TriPattern(Widget new_parent): CAModel(new_parent)

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
			   VIS_TextLines, 12,
			   TAG_DONE
			   );

 srand48(lrand48());

 environ = grid = new CATriangularGrid(0.0,0.0,10.0,10.0,24,12,60);

 symbol = new CAColoredEllipseSymbol(visual, environ);

 environ->SetDrawGrid(TRUE);
 environ->SetPeriodic(TRUE);

 AllocateCells(grid->GetNumberOfCells());

 environ->SyncViewport(visual);

 visual->Popup();
 visual->SetJustification(1, CATextVisual::CENTER);
 visual->SetFont(1, "Times", 'b', 'r', 14);
 visual->SetJustification(2, CATextVisual::CENTER);
 visual->SetJustification(3, CATextVisual::CENTER);

 visual->Print(1, "Triangular lattice");
 visual->Print(2, "Oslo College");
 visual->Print(3, "Centre of Sci-Tech");
 
 // Set color palette:
 
 visual->SetPaletteRGB(0, 255, 255, 255); //White
 visual->SetPaletteRGB(1, 0, 0, 0);       //Black
// visual->SetPaletteRGB(2, 255, 0, 0);     //red
// visual->SetPaletteRGB(3, 0, 255, 0);     //green
// visual->SetPaletteRGB(4, 0, 0, 255);     //blue

 visual->SetPaletteRGB(2, 40, 0,   255);
 visual->SetPaletteRGB(3, 40, 0,   240);
 visual->SetPaletteRGB(4, 40, 80,  230);
 visual->SetPaletteRGB(5, 40, 100, 220);
 visual->SetPaletteRGB(6, 40, 120, 210);
 visual->SetPaletteRGB(7, 60, 160, 210);
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
 
 pos = 0;
 iterations = 0.0;

Redraw();
}

/*****************************************************************************/

TriPattern::~TriPattern()
{
}

/*****************************************************************************/

void TriPattern::Redraw()

{ CATriangularGrid *grid = (CATriangularGrid*) environ;

grid->CalcGeometry();

CAModel::Redraw();
}

/*****************************************************************************/

void TriPattern::Update()

{ int cell_index, i,j,r,c,delta11,delta33,n0,n1;
  TriCell *current;
  CATriangularGrid *grid = (CATriangularGrid*) environ;
  index_array neighbours;

pos++;

if (pos > cells-1)
   {
   pos = 0;
   }

iterations++;
 
visual->CurrentPageBack();

 grid->DrawGrid(visual);

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    current = (TriCell*) cell[cell_index];
    
    grid->QueryGridPosition(cell_index,r,c);
    current->charge = 0;
    
    delta11 = abs((r-c)%2);
    delta33 = (cell_index + (r%2)*3)%6;

    for (i = 0; i > 999999999; i++)   // Slow things down!
      {
	j = 20;
	delta11 = i * 4 / 3;
	for (j = 0; j < 100000; j++)
	  {
	  }
      }

    
    switch(pos % 30)
       {
       case 0:
	   n0=0;
	   // 1x1 symmetry
	   if (delta11 == n0)
	      {
	      current->charge=2;
	      Draw(cell_index);
	      }
	   visual->Print(5, "1 X 1 (n0) = (0)");
	   break;
	   
       case 1:
	   
	   n0=1;
	   // 1x1 symmetry
	   if (delta11 == n0)
	      {
	      current->charge=2;
	      Draw(cell_index);
	      }
	   visual->Print(5, "1 X 1 (n0) = (1)");
	   break; 
	   
       case 2:
	   
	   // 3x3, 1/3
	   n0 = 0;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (0)");	   
	   break;


       case 3:
	   // 3x3, 1/3
	   n0 = 1;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (0,1)");	   
	   break;


       case 4:
	   // 3x3, 1/3
	   n0 = 2;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (2)");	   
	   break;


       case 5:
	   // 3x3, 1/3
	   n0 = 3;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (3)");	   
	   break;

       case 6:
	   // 3x3, 1/3
	   n0 = 4;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (4)");	   
	   break;

       case 7:
	   // 3x3, 1/3
	   n0 = 5;
	   if (delta33 == (n0))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 1/3 (n0) = (5)");	   
	   break;

       case 8:
	   // 3x3, 2/3
	   n0 = 0;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (0)");	   
	   break;

       case 9:
	   // 3x3, 2/3
	   n0 = 1;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (0,1)");	   
	   break;

       case 10:
	   // 3x3, 2/3
	   n0 = 2;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (2)");	
	   break;

       case 11:
	   // 3x3, 2/3
	   n0 = 3;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (3)");	
	   break;

       case 12:
	   // 3x3, 2/3
	   n0 = 4;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (4)");	
	   break;

       case 13:
	   // 3x3, 2/3
	   n0 = 5;
	   if ((delta33 == n0)||(delta33 == ((n0+2)%6)))//n0=(0,1) n1=(0,2)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "3x3, 2/3 (n0) = (5)");	
	   break;
	   
       case 14:
	   // 2x2, 1/2
	   n0 = 0;
	   n1 = 0;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (0,0)");	 
	   break;
	   
       case 15:
	   // 2x2, 1/2
	   n0 = 0;
	   n1 = 1;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (0,1)");	 
	   break;

       case 16:
	   // 2x2, 1/2
	   n0 = 0;
	   n1 = 2;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (0,2)");	 
	   break;

       case 17:
	   // 2x2, 1/2
	   n0 = 0;
	   n1 = 3;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (0,3)");	 
	   break;

       case 18:
	   // 2x2, 1/2
	   n0 = 1;
	   n1 = 0;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (1,0)");	 
	   break;
	   
       case 19:
	   // 2x2, 1/2
	   n0 = 1;
	   n1 = 1;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (1,1)");	 
	   break;

       case 20:
	   // 2x2, 1/2
	   n0 = 1;
	   n1 = 2;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (1,2)");	 
	   break;

       case 21:
	   // 2x2, 1/2
	   n0 = 1;
	   n1 = 3;
	   if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
	       (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // red
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/2 (n0,n1) = (1,3)");	 
	   break;
	   
       case 22:
	   // 2x2, 1/4
	   n0 = 0;
	   n1 = 0;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (0,0)");
	   break;

       case 23:
	   // 2x2, 1/4
	   n0 = 0;
	   n1 = 1;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (0,1)");
	   break;

       case 24:
	   // 2x2, 1/4
	   n0 = 0;
	   n1 = 2;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (0,2)");
	   break;

       case 25:
	   // 2x2, 1/4
	   n0 = 0;
	   n1 = 3;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (0,3)");
	   break;

       case 26:
	   // 2x2, 1/4
	   n0 = 1;
	   n1 = 0;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (1,0)");
	   break;
	   
       case 27:
	   // 2x2, 1/4
	   n0 = 1;
	   n1 = 1;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (1,1)");
	   break;
	   
       case 28:
	   // 2x2, 1/4
	   n0 = 1;
	   n1 = 2;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (1,2)");
	   break;
       case 29:
	   // 2x2, 1/4
	   n0 = 1;
	   n1 = 3;
	   if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))//n0=(0,1) n1=(0,3)
	      {
	      current->charge=2;   // green
	      Draw(cell_index);
	      }
	   visual->Print(5, "2x2, 1/4 (n0,n1) = (1,3)");
	   break;	   
       }        
    }
 
 visual->Printf(7, "Symmetry %d",pos % 30);

 visual->CurrentPageFront();
 return;
}

/*****************************************************************************/

void TriPattern::DblClick(double x, double y)
{

}

/*****************************************************************************/

void TriPattern::Click(double x, double y)
{
}

/*****************************************************************************/

void TriPattern::Draw(int cell_index)

{ TriCell *current = (TriCell*) cell[cell_index];

if (cell_index != NoCell)
   {
   symbol->Draw(cell_index, current->charge);
   }
}

/******************************************************************************/

void TriPattern::AllocateCells(int new_cells)

{ int cell_index, x,y;
  CATriangularGrid *grid = (CATriangularGrid*) environ;


 for (cell_index=0; cell_index < cells; cell_index++)
    {
    delete cell[cell_index];
    }
 
 delete [] cell;

 cells = new_cells;
 cell  = new CACell*[cells];

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    cell[cell_index] = new TriCell;
    }
}

/*****************************************************************************/

void TriPattern::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();
  CAGrid *grid = (CAGrid*) environ;
  
 if (cells != grid->GetNumberOfCells())
    {
    AllocateCells(grid->GetNumberOfCells());
    }
 Redraw();
}

/*****************************************************************************/
