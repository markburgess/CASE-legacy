/*****************************************************************************/
/* CAEnviron.C: Header file for CAEnviron object                             */
/*              All coordinate systems have positive direction from left to  */
/*              right in x-direction and from top downwards in y-direction.  */
/*                                                                           */
/*****************************************************************************/

/* $Id: CAEnvTri.C,v 1.10 1997/05/30 13:34:44 mark Exp $ */

#include <CAEnviron.h>
#include <toolkit.h>
#include <CA.h>

/*****************************************************************************/
/* CATriangularGrid                                                          */
/*****************************************************************************/

// NOTE: for rows and cols must be even


CATriangularGrid::CATriangularGrid(double new_x_min, double new_y_min,
				   double new_x_max, double new_y_max,
                                   int new_cols=17,
				   int new_rows=9, 
                                   double angle=60)
	: CAGrid(new_x_min, new_y_min, new_x_max, new_y_max, new_cols, new_rows)

  // angle in degrees, converted to radians

{
if (new_rows % 2 != 0)
   {
   printf("Number of rows must be EVEN in a triangular grid\n");
   }

if (new_cols % 2 != 0)
   {
   printf("Number of columns must be EVEN in a triangular grid.");
   }

theta = angle/180.0 * M_PI;

if (tan(theta) > 2.0)
   {
   printf("Theta is too large. This lattice is equivalent to one with\n");
   printf("smaller theta, rotated by 90 degrees...\n");
   exit(1);
   }

CalcGeometry();
}

/*****************************************************************************/

void CATriangularGrid::CalcGeometry()

  // Note that we should strictly take dy = dx * tan(theta) but
  // because of the limitations of the screen geometry, this looks
  // silly, so it is best to fill the window regardless of the angle

{ double fudge = (double)cols/(double)rows/1.7;

dx = (x_max - x_min) / (double) cols;
dy = (y_max - y_min) / (double) rows;

dx_offset_odd = dx/2.0;
dy_offset_odd = dx * fudge * sin (theta/2);       // Offset to centroid of triangle

dx_offset_even = dx/2.0;
dy_offset_even = dy - dx * fudge * sin (theta/2); // Offset to centroid of triangle
}

/*****************************************************************************/

CATriangularGrid::~CATriangularGrid()
{
}

/*****************************************************************************/

void CATriangularGrid::QueryPosition(int cell_index, double &x, double &y)

//  This function returns the x and y coordinates of the centre
//  of a cell.

{ int c,r;
  Boolean alternate;

QueryGridPosition(cell_index,r,c);

if (cell_index % 2 != 0)
   {
   alternate = True;
   }
else
   {
   alternate = False;
   }

if (r % 2 == 0)
   {
   alternate = !alternate;
   }

if (alternate)
    {
    x = x_min + dx_offset_odd + (double) c * dx;
    y = y_min + dy_offset_odd + (double) r * dy;
    }
else
    {
    x = x_min + dx_offset_even + (double) c * dx;
    y = y_min + dy_offset_even + (double) r * dy;
    }
}

/*****************************************************************************/

void CATriangularGrid::QueryGridPosition(int cell_index, int &row, int &col)

{
if (cell_index == NoCell)
   {
   row = NoCell;
   col = NoCell;
   }
else
   {
   row = cell_index / cols;
   col = cell_index % cols;
   }
}

/*****************************************************************************/

int CATriangularGrid::QueryCell(double x, double y)

{ int col, row, cell_index;
 
 col = (int) ((x-x_min)/dx);
 row = (int) ((y-y_min)/dy);

 cell_index = RowCol2Index(row,col);        // Takes account of boundary cond
 return cell_index;
 }

/*****************************************************************************/

void CATriangularGrid::QueryScaleFactors(double &sx, double &sy)

 // sx and sy must scale similarly to give the appearance of
 // a true circle or square

{
 sx = sy = dx / 3.0;
}


/*****************************************************************************/

void CATriangularGrid::DrawGrid(CAVisual *vis)

{ int r,c;
  double xf,yf;

 if (!draw_grid)
    {
    return;
    }

 CalcGeometry();

 vis->SetDrawingColor(1);
 
 for (r = 0; r < rows; r++)
    {
    vis->DrawLine(x_min, y_min + (double)r*dy, x_max, y_min + (double)r*dy);

    for (c = 0; c < cols; c++)
       {
       // Only need to draw every other line

       xf = x_min + (double) c * dx;
       yf = y_min + (double) r * dy;

       if ((r % 2 == 0) && (c % 2 == 0))
	  {
          vis->DrawLine(xf-dx/2.0,yf,xf+dx/2.0,yf+dy);
          vis->DrawLine(xf+3*dx/2.0,yf,xf+dx/2.0,yf+dy);	  
	  }
       else if ((r % 2 != 0) && (c % 2 == 0))
	  {
          vis->DrawLine(xf-dx/2.0,yf+dy,xf+dx/2.0,yf);
          vis->DrawLine(xf+3*dx/2.0,yf+dy,xf+dx/2.0,yf);	  
	  }
       
       }
    }
}

/*****************************************************************************/

int CATriangularGrid::CountNeighbours(int cell_index, int level)

  // How many neighbours do the cells have. Corner cells have only one
  // face neighbour, edge cells have two and all body cells have 3.
  // Returns each level separately, not the sum of all levels
    
{ int r,c;

if (periodic)
   {
   switch (level)
      {
      case 1: return 3;
      case 2: return 6;
      case 3: return 3;
      case 4: return 6;
      case 5: return 6;
      default: printf("CountNeighbours in EnvTri level > 5\n");
	       exit(1);
      }
   }

// If level > 3 we demand a periodic grid

if (level > 3)
   {
   printf("Neighbour level > 3 on a non-periodic grid\n");
   }

QueryGridPosition(cell_index,r,c);

if (r == 0 && c == 0)
   {
   switch(level)
      {
      case 1: return 1;
      case 3: return 2;
      }
   }

if (r == rows-1 && c == 0)
   {
   switch(level)
      {
      case 1: return 1;
      case 3: return 2;
      }
   }

if (r == 0 && c == cols-1)
   {
   switch(level)
      {
      case 1: return 2;
      case 3: return 1;
      }
   }

if (r == rows-1 && c == cols-1)
   {
   switch (level)
      {
      case 1: return 2;
      case 3: return 4;
      }
   }

if (r == 0 || r == rows-1 || c == 0 || c == cols-1)
   {
   switch(level)
     {
     case 1: return 2;
     case 3: return 1;
     }
   }

switch (level)
   {
   case 1: return 3;
   case 2: return 6;
   case 3: return 3;
   }
}

/*****************************************************************************/

void CATriangularGrid::QueryNeighbours(int cell_index, int level, index_array &neighbour)

{ int sign,r,c,count,fudge,nbr;

count = fudge = CountNeighbours(cell_index,level);

if (!neighbour.SetSize(count))
   {
   printf("Memory allocation trouble\n");
   }

QueryGridPosition(cell_index,r,c);

if (cell_index % 2 == 0)
   {
   sign = 1;
   }
else
   {
   sign = -1;
   }

if (r % 2 != 0)
   {
   sign = -sign;
   }

if (level == 1)
  {
  nbr = RowCol2Index(r,c+1);  // Start from count-1 and fill array

  if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r,c-1);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-sign,c);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }
  }

if (level == 2)
   {
  nbr = RowCol2Index(r-sign,c+1);   //2
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 


  nbr = RowCol2Index(r,c+2);    //3
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 


  nbr = RowCol2Index(r+sign,c+1); //5
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

  nbr = RowCol2Index(r+sign,c-1);//6
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 


  nbr = RowCol2Index(r,c-2);  //4
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-sign,c-1);  //1
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }
   }

if (level == 3)
   {

   nbr = RowCol2Index(r-sign,c+2);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 


   nbr = RowCol2Index(r+sign,c);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-sign,c-2);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

   }

if (level == 4)
   {
   nbr = RowCol2Index(r-2*sign,c-1);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-2*sign,c+1);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r,c-3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r,c+3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r+sign,c-2);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r+sign,c+2);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }
   }

if (level == 5)
   {
   nbr = RowCol2Index(r-2*sign,c);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-sign,c-3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r-sign,c+3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r+sign,c-3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r+sign,c+3);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }

  if (count == 0)
     {
     return;
     } 

   nbr = RowCol2Index(r+2*sign,c);
   if (nbr != NoCell)
      {
      neighbour.array[--count] = nbr;
      }
   }


while (count > 0)
  {
  neighbour.array[--count] = neighbour.array[fudge-1];
  }
}

/*****************************************************************************/

void CATriangularGrid::QueryPolygon(int cell_index,polygon &poly)

{ int r, c, upsidedown = false;
  double xf,yf;

QueryGridPosition(cell_index, r, c);

CalcGeometry();

if (! poly.SetSize(4))
   {
   printf("Memory allocation failure in QueryPolygon()\n");
   exit(1);
   }

xf = x_min + (double) c * dx;
yf = y_min + (double) r * dy;

upsidedown = cell_index %2;

if (r % 2 != 0)
   {
   upsidedown = !upsidedown;
   }

if (! upsidedown)
   {
   poly.x[0] = xf - dx/2.0;
   poly.y[0] = yf;
   poly.x[1] = xf + dx/2.0;
   poly.y[1] = yf + dy;
   poly.x[2] = xf + 3.0*dx/2.0;
   poly.y[2] = yf;
   poly.x[3] = xf - dx/2.0;
   poly.y[3] = yf;
   }
else
   {
   yf = y_min + (double) r * dy;
   poly.x[0] = xf - dx/2.0;
   poly.y[0] = yf + dy;
   poly.x[1] = xf + dx/2.0;
   poly.y[1] = yf;
   poly.x[2] = xf + 3.0*dx/2.0;
   poly.y[2] = yf + dy;
   poly.x[3] = xf - dx/2.0;
   poly.y[3] = yf + dy;
   } 
}

/*****************************************************************************/
