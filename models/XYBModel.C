/*****************************************************************************/
/*                                                                           */
/* XYBfield model simulation                                                 */
/*                                                                           */
/* In its present form, this simulation is tied to a rectangular grid        */
/*                                                                           */
/*****************************************************************************/

/* $Id: XYBModel.C,v 1.21 1997/06/25 14:25:38 mark Exp $ */


#include <XYBModel.h>


   const int East = 0;
   const int South = 1;
   const double J = 1.0;
   const double temp_factor = 0.4;
   const double delta = 0.001;


/*****************************************************************************/

XYBModel::XYBModel()
{
}

/*****************************************************************************/

XYBModel::XYBModel(Widget new_parent): CAModel(new_parent)
{
 int i;
 long ltime;
 Widget temperature, vortexnumber;
 CAGrid *grid;
 char beta_string[40], vortex_string[40];
 visual = new CATextVisual(this,
			   VIS_Height, 500,
			   VIS_Width, 500,
			   VIS_Fontname, "Times",
			   VIS_Fontsize, 12,
			   VIS_TextLines, 11,
			   TAG_DONE
			   );
 iter = 0;
 beta = 10;
 fudge_factor = 0.1;
 vortices = 15;

 srand48(lrand48());

 environ = grid = new CARectGrid;

 symbol = new CAColoredSignSymbol(visual, environ);

 environ->SetPeriodic(TRUE);

 AllocateCells(grid->GetNumberOfCells());

 environ->SyncViewport(visual);

 visual->Popup();
 visual->SetJustification(1, CATextVisual::CENTER);
 visual->SetFont(1, "Lucida", 'b', 'r', 24);
 visual->SetJustification(3, CATextVisual::CENTER);
 visual->SetFont(3, "Lucida", 'b', 'r', 14);

 // Set color palette:
 
 visual->SetPaletteRGB(0, 255, 255, 255); //White
 visual->SetPaletteRGB(1, 0, 0, 0);       //Black
 visual->SetPaletteRGB(2, 90, 90, 90);    //Arrows
 visual->SetPaletteRGB(3, 200, 0, 0);     //Plus
 visual->SetPaletteRGB(4, 0, 0, 200);     //Minus
 visual->SetPaletteRGB(5, 210, 210, 100); // Writing
 visual->SetPaletteRGB(6, 130, 0, 0);     //Background writing
 visual->SetPaletteRGB(7, 200, 0, 0);     //Background title

 visual->SetLineColor(0, 5, 7);
 visual->SetLineColor(1, 5, 7);
 visual->SetLineColor(2, 5, 7);
 visual->SetLineColor(3, 5, 7);
 visual->SetLineColor(4, 5, 7);
 visual->SetLineColor(5, 5, 6);
 visual->SetLineColor(6, 5, 6);
 visual->SetLineColor(7, 5, 6);
 visual->SetLineColor(8, 5, 6);
 visual->SetLineColor(9, 5, 6);
 visual->SetLineColor(10, 5, 6);

 visual->Print(1, "XY model");
 visual->Print(3, "in a magnetic field");
 visual->SetJustification(6, CATextVisual::LEFT);
 visual->Printf(6, "Temperature: %3.2lf", 1.0/beta);
 visual->SetJustification(7, CATextVisual::LEFT);
 visual->Printf(7, "Iterations: %d", 0);
 visual->SetJustification(9, CATextVisual::LEFT);
 visual->Printf(9, "Vortices: %d", vortices);
 
 temp_req.Initialize(parent, "Set temperature");
 vort_req.Initialize(parent, "Set vortex number");
 
 temp_req.SetCallbacks(temp_ok, temp_cancel, (XtPointer)this);
 vort_req.SetCallbacks(vort_ok, vort_cancel, (XtPointer)this);
 
 temperature = temp_req.AddWidget("Temperature", NULL, dialogWidgetClass);
 vortexnumber = vort_req.AddWidget("Vortices", NULL, dialogWidgetClass);
 
 sprintf(beta_string, "%lf", 1.0/beta);
 
 XtVaSetValues(temperature,
	       XtNlabel, "Temperature",
	       XtNvalue, beta_string,
	       NULL);


 sprintf(vortex_string, "%d", vortices);
 
 XtVaSetValues(vortexnumber,
	       XtNlabel, "Vortices",
	       XtNvalue, vortex_string,
	       NULL);

 visual->AddControlMenuItem("Set Temperature", set_temp, (XtPointer)this);
 visual->AddControlMenuItem("Set vortex number", set_vort, (XtPointer)this);
 
Redraw();
}

/*****************************************************************************/

XYBModel::~XYBModel()
{
}

/*****************************************************************************/

void XYBModel::Redraw()
{
CAModel::Redraw();
}

/*****************************************************************************/

void XYBModel::Update()

{ int x,y,i;
  int site, r,c;
  int cell_index;
  XYBCell *current;
  CAGrid *grid = (CAGrid*) environ;
  int before;

 for (site = 1; site <= cells; site++)
    {
    cell_index = (int) (drand48()*(double)cells);
    current = (XYBCell *) cell[cell_index];
    grid->QueryGridPosition(cell_index,r,c);

    before = current->charge;
    
    Monte_Carlo(cell_index);

    current->charge = Vorticity(cell_index);
    }

 visual->CurrentPageBack();

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    current = (XYBCell *) cell[cell_index];
    grid->QueryGridPosition(cell_index,r,c);
    symbol->Draw(cell_index, current->charge, 3, 4);
    DrawArrow(cell_index,-Link(South,c,r)/M_PI,East);
    DrawArrow(cell_index,Link(East,c,r)/M_PI,South); 
    }
 
 visual->CurrentPageFront();
 visual->Printf(6, "Temperature: %3.2lf", 1.0/beta);
 visual->Printf(7, "Iterations: %d", ++iter);
 visual->Printf(9, "Vortices: %d", vortices);

}

/*****************************************************************************/

void XYBModel::DblClick(double x, double y)
{

}

/*****************************************************************************/

void XYBModel::Click(double x, double y)
{
}

/*****************************************************************************/

void XYBModel::Draw(int cell_index)

{
symbol->Clear(cell_index);
symbol->Draw(cell_index,Vorticity(cell_index),3,4);
}

/*****************************************************************************/

void XYBModel::AllocateCells(int new_cells)

{ int cell_index, x,y;
  CARectangularGrid *grid = (CARectangularGrid*) environ;


 for (cell_index=0; cell_index < cells; cell_index++)
    {
    delete cell[cell_index];
    }
 
 delete [] cell;
 delete [] south;
 delete [] east;

 cells = new_cells;
 cell  = new CACell*[cells];
 south = new double[cells];
 east  = new double[cells];

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    cell[cell_index] = new XYBCell;
    }

 SetInitialPlaquetteState();
 InitCells();
}

/*****************************************************************************/

void XYBModel::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();
  CAGrid *grid = (CAGrid*) environ;
  
 if (cells != grid->GetNumberOfCells())
    {
    AllocateCells(grid->GetNumberOfCells());
    }
 Redraw();
}

/*****************************************************************************/

void XYBModel::SetInitialPlaquetteState()

{ int vortex, cell_index;
  XYBCell *current;

Debug(" XYBModel::SetInitialPlaquetteState()\n");

init_cell = (double *) new double[cells];

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   init_cell[cell_index] = 0.0;
   current = (XYBCell *)cell[cell_index];
   current->charge = 0;
   }

vortex = 0;

while (vortex < vortices)
   {
   cell_index = (int) (drand48() * (double) cells);

   if (init_cell[cell_index] != 0.0)
      {
      continue;
      }
   else
      {
      vortex++;
      }

   Debug("Initializing cell %d\n",cell_index);
   init_cell[cell_index] = 1.0;
   }
}

/*****************************************************************************/

void XYBModel::InitCells() 

{ int cell_index,i,r,c,rows,cols;
  double filling;
  CARectGrid *grid = (CARectGrid *) environ; 

rows = grid->QueryMaxRows();
cols = grid->QueryMaxColumns();

filling = vortices/((double)cells);

for (c = 0; c < cols; c++)
   {
   for (r = 0; r < rows; r++)
      {
      cell_index = grid->RowCol2Index(r,c);

      if (c == 0)
         {
         if (r == 0)
            {
	    SetLink(1,c,r,PIx2*(init_cell[cell_index]-filling)/4.0);
 	    SetLink(0,c,r+1,PIx2*(init_cell[cell_index]-filling)/4.0);
 	    SetLink(0,c,r,-PIx2*(init_cell[cell_index]-filling)/4.0);
            SetLink(1,c+1,r,-PIx2*(init_cell[cell_index]-filling)/4.0);
	    continue;
	    }

         if (r > 0 && r < rows-1)
	    {
	    SetLink(1,c,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r))/3.0);
	    SetLink(0,c,r+1,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r))/3.0);
	    SetLink(1,c+1,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r))/-3.0);
	    continue;
	    }

         if (r == rows-1)
            {
	    SetLink(1,c,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)-Link(0,c,r+1))/2.0);
	    SetLink(1,c+1,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)-Link(0,c,r+1))/-2.0);
	    continue;
	    }
         }

      if (c > 0 && c < cols-1)
         {
         if (r == 0)
	    {
	    SetLink(0,c,r+1,(PIx2*(init_cell[cell_index]-filling)-Link(1,c,r))/3.0);
	    SetLink(0,c,r,(PIx2*(init_cell[cell_index]-filling)-Link(1,c,r))/-3.0);
	    SetLink(1,c+1,r,(PIx2*(init_cell[cell_index]-filling)-Link(1,c,r))/-3.0);
	    continue;
            }

         if (r > 0 && r < rows-1)
	    {
	    SetLink(0,c,r+1,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)-Link(1,c,r))/2.0);
	    SetLink(1,c+1,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)-Link(1,c,r))/-2.0);
	    continue;
	    }

         if (r == rows-1)
  	    {
	    SetLink(1,c+1,r,(PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)-Link(1,c,r)-Link(0,c,r+1))/-1.0);
	    continue;
            }
         }
   
      if (c == cols-1)
         {
         if (r == 0)
	    { 
	    SetLink(0,c,r+1,(PIx2*(init_cell[cell_index]-filling)+Link(1,c+1,r)-Link(1,c,r))/2.0);
            SetLink(0,c,r,(PIx2*(init_cell[cell_index]-filling)+Link(1,c+1,r)-Link(1,c,r))/-2.0);
	    continue;
	    }

         if (r > 0 && r < rows-1)
	    {
	    SetLink(0,c,r+1,PIx2*(init_cell[cell_index]-filling)+Link(0,c,r)+Link(1,c+1,r)-Link(1,c,r));
	    continue;
	    }
	 }
      Debug("No link set\n");
      }
   }

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   grid->QueryGridPosition(cell_index,c,r);
   CheckLinks(c,r);
   }
}

/*****************************************************************************/

void XYBModel::Monte_Carlo(int cell_index)

{ double phi, Enew = 0, Eold = 0;
  double DeltaE,boltzmann;
  CAGrid *grid = (CAGrid *) environ;
  int updated,i,r,c;
  
grid->QueryGridPosition(cell_index,r,c);
 
phi = PIx2 * (drand48()-0.5) * fudge_factor;

for (i = 0; i < 4; i++)
   {
   Eold += cos(Link(i,c,r));
   }

for (i = 0; i < 2; i++)
   {
   Enew += cos(Link(i,c,r) - phi);
   }

for (i = 2; i < 4; i++)
   {
   Enew += cos(Link(i,c,r) + phi);
   }

DeltaE = -J*(Enew - Eold);
boltzmann=exp(-beta*DeltaE);

if (boltzmann >= drand48())
   {
   for (i = 0; i < 2; i++)
      {
      SetLink(i,c,r,Link(i,c,r) - phi);
      }
   
   for (i = 2; i < 4; i++)
      {
      SetLink(i,c,r,Link(i,c,r) + phi);
      }
   
    CheckLinks(c,r);
    }
}

/*****************************************************************************/

int XYBModel::Vorticity(int cell_index)

{ double phase,charge;
  double filling;
  CAGrid *grid = (CAGrid *) environ;
  int r,c;

filling = vortices/(double)cells;

grid->QueryGridPosition(cell_index,r,c);

phase  = Link(1,c,r);
phase -= Link(0,c,r);
phase += Link(0,c,r+1);
phase -= Link(1,c+1,r);

charge = phase/PIx2 + filling;

if (-2.0 - delta < charge && charge < -2.0 + delta)
   {
   return(-2);
   }

if (-1.0 - delta < charge && charge < -1.0 + delta)
   {
   return(-1);
   }

if ( 0.0 - delta < charge && charge <  0.0 + delta)
   {
   return(0);
   }

if ( 1.0 - delta < charge && charge <  1.0 + delta)
   {
   return(1);
   }

if ( 2.0 - delta < charge && charge <  2.0 + delta)
   {
   return(2);
   }

if ( 3.0 - delta < charge && charge <  3.0 + delta)
   {
   return(3);
   }

Debug("Vorticity test failed\n");
}

/*****************************************************************************/

void XYBModel::temp_ok(Req *req, XtPointer data)

{ Widget temperature;
  XYBModel *model = (XYBModel *) data;
  temperature = req->GetWidget("Temperature", NULL);

model->beta = 1/atof(XawDialogGetValueString(temperature));
}

/*****************************************************************************/

void XYBModel::vort_ok(Req *req, XtPointer data)

{ Widget vortex;
  XYBModel *model = (XYBModel *) data;
  vortex = req->GetWidget("Vortices", NULL);
  
 model->vortices = atoi(XawDialogGetValueString(vortex));
 model->SetInitialPlaquetteState();
 model->InitCells();
 model->iter = 0;
 model->Redraw();
}

/*****************************************************************************/

void XYBModel::set_temp(Widget w, XtPointer data, XtPointer garb)

{ XYBModel *model = (XYBModel *)data;
  model->temp_req.Popup();
}

/*****************************************************************************/

void XYBModel::set_vort(Widget w, XtPointer data, XtPointer garb)

{ XYBModel *model = (XYBModel *)data;
  model->vort_req.Popup();
}

/*****************************************************************************/

void XYBModel::temp_cancel(Req *req, XtPointer data)

{
}

/*****************************************************************************/

void XYBModel::vort_cancel(Req *req, XtPointer data)

{
}

/*****************************************************************************/

double XYBModel::Link(int direction, int col, int row)

{ CARectangularGrid *grid = (CARectangularGrid*) environ;
  int cell_index;

switch (direction)
   {
   case 0:
           cell_index = grid->RowCol2Index(row,col);
           return east[cell_index];

   case 1:
           cell_index = grid->RowCol2Index(row,col);
           return south[cell_index];

   case 2: col--;
           cell_index = grid->RowCol2Index(row,col);
           return east[cell_index];

   case 3: row--;
           cell_index = grid->RowCol2Index(row,col);
           return south[cell_index];
   default:
           Debug("Uh oh - direction off in Link()\n");
   }
}

/*****************************************************************************/

void XYBModel::SetLink(int direction, int col, int row, double value)

{ CARectangularGrid *grid = (CARectangularGrid*) environ;
  int cell_index;

cell_index = grid->RowCol2Index(row,col);

switch (direction)
   {
   case 0:
           cell_index = grid->RowCol2Index(row,col);
           east[cell_index] = value;
	   return;

   case 1:
	   cell_index = grid->RowCol2Index(row,col);
           south[cell_index] = value;
	   return;

   case 2: col--;
	   cell_index = grid->RowCol2Index(row,col);
           east[cell_index] = value;
	   return;

   case 3: row--;
	   cell_index = grid->RowCol2Index(row,col);
           south[cell_index] = value;
	   return;
   default:
           Debug("Uh oh - direction off in SetLink()\n");
   }
}

/*****************************************************************************/

void XYBModel::CheckLinks(int c, int r)

{ int i, counter=0;

for(i = 0; i < 4; i++)
  {
  if (Link(i,c,r) > PIx2/2.0)
     {
     SetLink(i,c,r,Link(i,c,r) - PIx2);
     }
  else
     {
     if (Link(i,c,r) < -PIx2/2.0)
	{
	SetLink(i,c,r,Link(i,c,r) + PIx2);
	}
     }
  }
}

/*****************************************************************************/

void XYBModel::DrawArrow(int cell_index, double length, int EorS)

 // Remember that positive x is left to right, positive y is bottom to top
 // but the first cell is in the bottom left hand corner!!
  // This means we have to actually plot S,E as N,W (!)

{ CAGrid *grid = (CAGrid*) environ;
  double x,y;                       // Centre of each cell
  double x1,y1,x2,y2,xtip,ytip;
  double sx,sy,scale,headangle;
  double orientation = (double) EorS;
  double xhead1,xhead2,yhead1,yhead2;
  const double headbodyangle=0.4;
  const double fudge = 1.5;
  const double fudge_arrow = 0.4;

switch (EorS)
   {
   case South: if (length > 0)        // X
                  {
                  headangle = M_PI;
                  }
               else
                  {
                  headangle =  0.0;
                  }
               break;
	   
   case East: if (length < 0)       // Y
                 {
                 headangle = 3.0*M_PI/2.0;
                 }
              else
                 {
                 headangle = M_PI/2.0;
                 }
   }

  
grid->QueryScaleFactors(sx,sy);
grid->QueryPosition(cell_index,x,y);

scale = max(sx,sy) * fudge * (double) fabs(length);

switch (EorS)
   {
   case South: x1 = x - scale/2.0;
               y1 = y - sy;         // + instead of -
               x2 = x1 + scale;
	       y2 = y1;

	       break;

   case East:  x1 = x - sx;        // - instead of +
               y1 = y + scale/2.0;
	       x2 = x1;
	       y2 = y1 - scale;
   }

               if (length < 0.0)
                  {
                  xtip = x1;
                  ytip = y1;
                  }
               else
                  {
                  xtip = x2;
                  ytip = y2;
                  }

xhead1 = xtip + fudge_arrow*scale*cos(headangle - headbodyangle);
xhead2 = xtip + fudge_arrow*scale*cos(headangle + headbodyangle);
yhead1 = ytip + fudge_arrow*scale*sin(headangle - headbodyangle);
yhead2 = ytip + fudge_arrow*scale*sin(headangle + headbodyangle);

visual->SetDrawingColor(2);

visual->DrawLine(x1,y1,x2,y2);
visual->DrawLine(xhead1,yhead1,xtip,ytip);
visual->DrawLine(xhead2,yhead2,xtip,ytip);
}

/*****************************************************************************/

