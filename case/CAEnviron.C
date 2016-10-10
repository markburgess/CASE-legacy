/*****************************************************************************/
/* CAEnviron.C: Header file for CAEnviron object                             */
/*              All coordinate systems have positive direction from left to  */
/*              right in x-direction and from top downwards in y-direction.  */
/*                                                                           */
/*****************************************************************************/

/* $Id: CAEnviron.C,v 1.19 1997/05/30 13:34:45 mark Exp $ */

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
/* CAEnviron                                                                 */
/*****************************************************************************/

CAEnviron::CAEnviron(double new_x_min, double new_y_min,
		     double new_x_max, double new_y_max)
{
 x_min = new_x_min;
 y_min = new_y_min;
 x_max = new_x_max;
 y_max = new_y_max;
 periodic = FALSE;
 draw_grid = FALSE;
}

/*****************************************************************************/

CAEnviron::CAEnviron (CAVisual *visual)
{
 periodic = FALSE;
 SyncViewport(visual);
 draw_grid = FALSE;
}

/*****************************************************************************/

CAEnviron::~CAEnviron()
{
}

/*****************************************************************************/

void CAEnviron::SetPeriodic(Boolean new_periodic)
{
periodic = new_periodic;
}

/*****************************************************************************/

void CAEnviron::SetDrawGrid(Boolean new_value)
{
draw_grid = new_value;
}

/*****************************************************************************/

void CAEnviron::SyncViewport(CAVisual *visual)
{
visual->GetCoordinateSystem(x_min, y_min, x_max, y_max);
}

/*****************************************************************************/

void CAEnviron::SetupRequester(Req *req)
{
 Widget periodic_w, draw_w;
 periodic_w = req->AddWidget("toggler", NULL, toggleWidgetClass);
 XtVaSetValues(periodic_w,
	       XtNlabel, "Periodic",
	       XtNstate, periodic,
	       NULL);

 draw_w = req->AddWidget("Draw grid", NULL, toggleWidgetClass);
 XtVaSetValues(draw_w,
	       XtNstate, draw_grid,
	       NULL);
}

/*****************************************************************************/

void CAEnviron::ReadRequester(Req *req)
{
 Widget periodic_w, draw_w;
 periodic_w = req->GetWidget("toggler",NULL);
 XtVaGetValues(periodic_w,
	       XtNstate, &periodic,
	       NULL);
 draw_w = req->GetWidget("Draw grid",NULL);
 XtVaGetValues(draw_w,
	       XtNstate, &draw_grid,
	       NULL);
}

/*****************************************************************************/
/* CAGrid                                                                    */
/*****************************************************************************/

CAGrid::CAGrid(double new_x_min, double new_y_min,
	       double new_x_max, double new_y_max,
	       int new_cols, int new_rows)
	:CAEnviron(new_x_min, new_y_min, new_x_max, new_y_max)
{
SetRowsCols(new_cols, new_rows);
}

/*****************************************************************************/

CAGrid::CAGrid()

{
SetRowsCols(20,20);  // Default size
}

/*****************************************************************************/

CAGrid::~CAGrid()
{

}

/*****************************************************************************/

void CAGrid::SetRowsCols(int new_cols, int new_rows)
{
 rows = new_rows;
 cols = new_cols;
}

/*****************************************************************************/

int CAGrid::GetNumberOfCells()

{
 return rows*cols;
}

/*****************************************************************************/

void CAGrid::SetupRequester(Req *req)

{ Widget label1, rows_l, rows_t, cols_l, cols_t;
  char rows_string[10], cols_string[10];
  CAEnviron::SetupRequester(req);
  sprintf(rows_string,"%d", rows);
  sprintf(cols_string,"%d", cols);
 
 req->AddGroup("rows_cols", formWidgetClass);

 label1 = req->AddWidget("label", "rows_cols", labelWidgetClass);
 rows_l = req->AddWidget("label", "rows_cols", labelWidgetClass);
 cols_l = req->AddWidget("label", "rows_cols", labelWidgetClass);
 cols_t = req->AddWidget("cols", "rows_cols", asciiTextWidgetClass );
 rows_t = req->AddWidget("rows", "rows_cols", asciiTextWidgetClass );
    
 XtVaSetValues(label1,
	       XtNlabel, "Grid size",
	       XtNborderWidth, 0,
	       NULL);

 XtVaSetValues(cols_l,
	       XtNlabel, "Cols:",
	       XtNborderWidth, 0,
	       XtNfromVert, label1,
	       NULL);

 XtVaSetValues(cols_t,
	       XtNeditType, XawtextEdit,
	       XtNresizable, True,
	       XtNresize, XawtextResizeBoth,
	       XtNstring, cols_string,
	       XtNfromVert, label1,
	       XtNfromHoriz, cols_l,
	       NULL);

 XtVaSetValues(rows_t,
	       XtNeditType, XawtextEdit,
	       XtNresizable, True,
	       XtNresize, XawtextResizeBoth,
	       XtNstring, rows_string,
	       XtNfromVert, cols_l,
	       XtNfromHoriz, rows_l,
	       NULL);

 XtVaSetValues(rows_l,
	       XtNlabel, "Rows:",
	       XtNfromVert, cols_l,
	       XtNborderWidth, 0,
	       NULL);
}

/*****************************************************************************/

void CAGrid::ReadRequester(Req *req)
{
 Widget rows_w, cols_w;
 int new_cols, new_rows;
 char *input;
 CAEnviron::ReadRequester(req);
 rows_w = req->GetWidget("rows","rows_cols");
 XtVaGetValues(rows_w,
	       XtNstring, &input,
	       NULL);

 new_rows = atoi(input);
 if (new_rows >0 )
    {
    rows = new_rows;
    }
    
 cols_w = req->GetWidget("cols","rows_cols");
 XtVaGetValues(cols_w,
	       XtNstring, &input,
	       NULL);
 new_cols = atoi(input);

 if (new_cols >0)
    {
    cols = new_cols;
    }
}

/*****************************************************************************/

int CAGrid::RowCol2Index(int row, int col)

{
if (Inbounds(row, col))
    {
    return (row*cols + col);
    }
 
if (!periodic)
   {
   return (NoCell);
   }

while (row < 0)
   {
   row += rows;
   }

while (col < 0)
   {
   col += cols;
   }

row = row % rows;
col = col % cols;
 
return (row*cols + col);
}

/*****************************************************************************/

Boolean CAGrid::IsEdgeCell(int cell_index)

{ int r,c;

QueryGridPosition(cell_index,r,c);

if (r == 0 || r == rows-1 || c == 0 || c == cols-1)
   {
   return TRUE;
   }
else
   {
   return FALSE;
   }
}


/*****************************************************************************/

Boolean CAGrid::Inbounds(int row, int col)
{

if (row < 0 || row >= rows || col < 0 || col >= cols)
   {
   return FALSE;
   }

return TRUE;
}

/*****************************************************************************/

Boolean CAGrid::IsCornerCell(int cell_index)

{ int r,c;

QueryGridPosition(cell_index,r,c);

if (r == 0 && c == 0)
   {
   return TRUE;
   }

if (r == 0 && c == cols-1)
   {
   return TRUE;
   }

if (r == rows-1 && c == 0)
   {
   return TRUE;
   }

if (r == rows-1 && c == cols-1)
   {
   return TRUE;
   }

return FALSE;
}

/*****************************************************************************/

int CAGrid::QueryMaxColumns()

{
return cols;
}

/*****************************************************************************/

int CAGrid::QueryMaxRows()

{
return rows;
}

/*****************************************************************************/

