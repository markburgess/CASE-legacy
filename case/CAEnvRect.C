/*****************************************************************************/
/* CAEnviron.C: Header file for CAEnviron object                             */
/*              All coordinate systems have positive direction from left to  */
/*              right in x-direction and from top downwards in y-direction.  */
/*                                                                           */
/*****************************************************************************/

/* $Id: CAEnvRect.C,v 1.4 1997/05/30 13:34:43 mark Exp $ */

#include <CAEnviron.h>
#include <toolkit.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Toggle.h>
#include <CA.h>

/*****************************************************************************/
/* CARectangularGrid                                                         */
/*****************************************************************************/

CARectangularGrid::CARectangularGrid(double new_x_min, double new_y_min,
				     double new_x_max, double new_y_max,
				     int new_cols, int new_rows)
	:CAGrid(new_x_min, new_y_min, new_x_max, new_y_max, new_cols, new_rows)
{
}

/*****************************************************************************/

CARectangularGrid::~CARectangularGrid()
{
}

/*****************************************************************************/

void CARectangularGrid::QueryPosition(int cell_index, double &x, double &y)
{
/* ----------------------------------------------------------
   This function returns the x and y coordinates of the centre
   of a cell.
   ---------------------------------------------------------- */
 double dx, dy;

 dx = (x_max - x_min)/ (double) cols;
 dy = (y_max - y_min)/ (double) rows;

 x = x_min + (cell_index % cols)*dx + 0.5*dx;
 y = y_min + (cell_index / cols)*dy + 0.5*dy;
}


/*****************************************************************************/

void CARectangularGrid::QueryGridPosition(int cell_index, int &row, int &col)

{
row = cell_index / cols;
col = cell_index % cols;
}

/*****************************************************************************/

int CARectangularGrid::QueryCell(double x, double y)
    
{ double dx, dy;
  int col, row, cell_index;
 
 dx = (x_max - x_min)/ (double) cols;
 dy = (y_max - y_min)/ (double) rows;

 col = (int) ((x-x_min)/dx);
 row = (int) ((y-y_min)/dy);
 cell_index = RowCol2Index(row,col);        // Takes account of boundary cond
 return cell_index;
}

/*****************************************************************************/

int CARectangularGrid::CountNeighbours(int cell_index, int level)
{
 return 8*level;
}

/*****************************************************************************/

void CARectangularGrid::QueryNeighbours(int cell_index, int level,index_array &neighbour)

  //   Neighbour levels:
  //
  // |2|2|2|2|2|    X = cell[cell_index]
  // |2|1|1|1|2|    
  // |2|1|X|1|2|
  // |2|1|1|1|2|
  // |2|2|2|2|2|
  //


{ neighbour.SetSize(8*level);
  int n, n_row, n_col, row, col;

//
// Let's pretend we have a 2-d array...
//
 
 row = cell_index/cols;
 col = cell_index%cols;

 n = 0;
 n_row = row - level;

 for(n_col = col - level; n_col <= col + level; n_col++)
    {
    neighbour.array[n] = RowCol2Index(n_row, n_col);
    n++;
    }
 n_col--;

 for(n_row = n_row + 1; n_row <= row + level; n_row++)
    {
    neighbour.array[n] = RowCol2Index(n_row, n_col);
    n++;
    }
 n_row--;
 
 for(n_col = n_col-1; n_col >= col - level; n_col--)
    {
    neighbour.array[n] = RowCol2Index(n_row, n_col);
    n++;
    }
 n_col++;

 for(n_row = n_row - 1; n_row > row - level; n_row--)
    {
    neighbour.array[n] = RowCol2Index(n_row, n_col);
    n++;
    }
}

/*****************************************************************************/

void CARectangularGrid::QueryPolygon(int cell_index, polygon &poly)

 /* -----------------------------------------------------------------
   This function returns two arrays, one for the x-coordinates, and
   one for the y-coordinates of the polygon. The polygon describes
   one cell.
   ----------------------------------------------------------------- */

{ double centre_x, centre_y;
  double dx, dy;
  
 dx = (x_max - x_min)/ (double) cols;
 dy = (y_max - y_min)/ (double) rows;

 QueryPosition(cell_index, centre_x, centre_y);

 if(! poly.SetSize(4))
    {
    printf("Memory allocation failure in QueryPolygon()\n");
    exit(1);
    }

 poly.x[0] = centre_x - 0.5*dx;
 poly.y[0] = centre_y - 0.5*dy;
 poly.x[1] = centre_x + 0.5*dx;
 poly.y[1] = centre_y - 0.5*dy;
 poly.x[2] = centre_x + 0.5*dx;
 poly.y[2] = centre_y + 0.5*dy;
 poly.x[3] = centre_x - 0.5*dx;
 poly.y[3] = centre_y + 0.5*dy;

}

/*****************************************************************************/

void CARectangularGrid::QueryScaleFactors(double &sx, double &sy)
{
 sx = (x_max - x_min)/ (double) cols / 2.0;
 sy = (y_max - y_min)/ (double) rows / 2.0;
}

/*****************************************************************************/

void CARectangularGrid::QueryCornerXY(int cell_index, double &corner_x, double &corner_y)
{
/* ------------------------------------------------------------------
   This function returns the coordinates of the upper left corner of
   a cell.
   ------------------------------------------------------------------ */
 double dx, dy;
 
 dx = (x_max - x_min)/ (double) cols;
 dy = (y_max - y_min)/ (double) rows;

 QueryPosition(cell_index, corner_x, corner_y);

 corner_x -= dx/2;
 corner_y -= dy/2;
 
}

/*****************************************************************************/

void CARectangularGrid::QueryCornerCells(int cell_index,
  					index_array &corner_cell)


 /* ------------------------------------------------------------------
   This function returns an array of cell indices. The cells in this
   array are the ones surrounding the lower left corner of a cell
   like this:

   |2|1|
   |3|x|

   The cell itself (x) is put in position 0 in the array.
   The array corner_cell must be allocated before this function is
   called.
   ------------------------------------------------------------------ */
    
{ int x, y;
  int row, col;
 
 x = cell_index%cols;
 y = cell_index/cols; 
 row = cell_index/cols;
 col = cell_index%cols; 
 
 corner_cell.SetSize(4);
 
 corner_cell.array[0] = RowCol2Index(row, col);
 corner_cell.array[1] = RowCol2Index(row - 1, col);
 corner_cell.array[2] = RowCol2Index(row - 1, col - 1);
 corner_cell.array[3] = RowCol2Index(row, col - 1);
}

/*****************************************************************************/

void CARectangularGrid::DrawGrid(CAVisual *vis)

{ int i;
  double dx,dy;

 if (!draw_grid)
    {
    return;
    }

 dx = (x_max - x_min)/ (double) cols;
 dy = (y_max - y_min)/ (double) rows;

 vis->SetDrawingColor(1);
 
 for (i=1; i<cols; i++)
    {
    vis->DrawLine(x_min + i*dx, y_min, x_min + i*dx, y_max);
    }

 for (i = 1; i < rows; i++)
    {
    vis->DrawLine(x_min, y_min + i*dy, x_max, y_min + i*dy);
    }
}


/*****************************************************************************/
/* CARectGrid Each cell has only 4 neighbours                                */
/*****************************************************************************/

CARectGrid::CARectGrid(double new_x_min, double new_y_min,
				       double new_x_max,
				       double new_y_max,
				       int new_cols, int new_rows)
	:CARectangularGrid(new_x_min, new_y_min,
			   new_x_max, new_y_max,
			   new_cols, new_rows)
{
}

/*****************************************************************************/

CARectGrid::~CARectGrid()
{
}

/*****************************************************************************/

void CARectGrid::QueryNeighbours(int cell_index, int level,
					 index_array &neighbour)
{ neighbour.SetSize(4);
  int n, n_row, n_col, row, col;

row = cell_index/cols;
col = cell_index%cols;
 
neighbour.array[0] = RowCol2Index(row - level, col);
neighbour.array[1] = RowCol2Index(row, col + level);
neighbour.array[2] = RowCol2Index(row + level, col);
neighbour.array[3] = RowCol2Index(row, col - level);
}

