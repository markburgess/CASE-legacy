/*****************************************************************************/
/*                                                                           */
/* XY model simulation                                                       */
/*                                                                           */
/*****************************************************************************/

/* $Id: XYModel.C,v 1.14 1997/06/25 14:25:40 mark Exp $ */


#include <XYModel.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <X11/Xaw/Dialog.h>

#ifndef M_PI
# define  M_PI            3.14159265358979323846
#endif


   const double J = 1.0;
   const double temp_factor = 0.4;
   const double delta = 0.001;


XYModel::XYModel()
{
}

/*****************************************************************************/

XYModel::XYModel(Widget new_parent):
	CAModel(new_parent)
{int i;
 int stime,ltime;
 Widget temperature;
 CAGrid *grid;
 char beta_string[40];
 time_t time(time_t *t);
 visual = new CATextVisual(this,
			   VIS_Height, 500,
			   VIS_Width, 500,
			   VIS_Fontname, "Times",
			   VIS_Fontsize, 12,
			   VIS_TextLines, 10,
			   TAG_DONE);

 iter = 0;
 beta = 10;
 
 environ = grid = new CARectGrid;
 symbol = new CAArrowSymbol(visual, environ);
 
 AllocateCells(grid->GetNumberOfCells());
 
 environ->SyncViewport(visual);
 environ->SetPeriodic(TRUE);
 visual->Popup();
 // Set color palette:
 
 visual->SetPaletteRGB(0, 255, 255, 255); //White
 visual->SetPaletteRGB(1, 0, 0, 0);       //Black
 visual->SetPaletteRGB(2, 90, 90, 90);    //Arrows
 visual->SetPaletteRGB(3, 200, 0, 0);     //Plus 
 visual->SetPaletteRGB(4, 0, 0, 200);     //Minus
 visual->SetPaletteRGB(5, 210, 210, 100);  // Writing

 visual->SetJustification(1, CATextVisual::CENTER);
 visual->SetFont(1, "Times", 'b', 'r', 18);
 visual->SetJustification(2, CATextVisual::CENTER);
 visual->SetJustification(3, CATextVisual::CENTER);
 visual->SetLineColor(0, 5, 4);
 visual->SetLineColor(1, 5, 4);
 visual->SetLineColor(2, 5, 4);
 visual->SetLineColor(3, 5, 4);
 visual->SetLineColor(4, 5, 4);
 visual->SetLineColor(5, 5, 4);
 visual->SetLineColor(6, 5, 4);
 visual->SetLineColor(7, 5, 4);
 visual->SetLineColor(8, 5, 4);
 visual->SetLineColor(9, 5, 4);
 visual->Print(1, "XY Model");
 visual->Print(2, "Oslo College/University");
 visual->Print(3, "Centre of Sci and Tech");
 visual->SetJustification(5, CATextVisual::LEFT);
 visual->Printf(5, "T: %lf", 1.0/beta);
 visual->SetJustification(6, CATextVisual::LEFT);
 visual->Printf(6, "iter: %d", 0);

 srand48(lrand48());

 temp_req.Initialize(parent, "Set temperature");
 temp_req.SetCallbacks(temp_ok, temp_cancel, (XtPointer)this);
 temperature = temp_req.AddWidget("Temperature", NULL, dialogWidgetClass);
 sprintf(beta_string, "%lf", 1.0/beta);
 XtVaSetValues(temperature,
	       XtNlabel, "Temperature",
	       XtNvalue, beta_string,
	       NULL);

 visual->AddControlMenuItem("Set Temperature", set_temp, (XtPointer)this);
 Redraw();
}

/*****************************************************************************/

XYModel::~XYModel()
{
}

/*****************************************************************************/

void XYModel::Redraw()
{
 CAModel::Redraw();
 CheckVortices();
}

/*****************************************************************************/

void XYModel::Update()
    
{ int spin;
  int cell_index;
  XYCell *current;
  int accept = 0;
  double old_angle;
  double x,y,sx,sy;
  CAGrid *grid = (CAGrid*) environ;
 
 for (spin = 1; spin <= cells; spin++)
    {
    cell_index = (int) (drand48()*cells);
    current = (XYCell *) cell[cell_index];
    old_angle = current->GetAngle();
    if (Monte_Carlo(cell_index)==1)
       {
       symbol->Clear(cell_index);
       Draw(cell_index);
       accept++;
      }
   }

 CheckVortices();
 visual->Printf(5, "T: %lf", 1.0/beta);
 visual->Printf(6, "Iter: %d", iter++);
}

/*****************************************************************************/

void XYModel::RightClick(double x, double y)
{
 int cell_index;
 double sx, sy, centre_x, centre_y, angle;
 CAGrid *grid = (CAGrid *) environ;
 
 cell_index = environ->QueryCell(x, y);
 XYCell *current = (XYCell*) cell[cell_index];
 grid->QueryPosition(cell_index, centre_x, centre_y);
 grid->QueryScaleFactors(sx, sy);

 if ((x - centre_x) == 0)
    {
    if (y < centre_y)
       {
       angle = -M_PI / 2;
       }
    else
       {
       angle = M_PI / 2;
       }
    }
 else 
    {
    angle = atan(((y - centre_y)/sy) / ((x - centre_x) /sx));
    }
 if (x < centre_x)    
    {
    angle += M_PI; 
    }

 current->SetAngle(angle);
     
 Draw(cell_index);

}

/*****************************************************************************/

void XYModel::Draw(int cell_index)
{
 XYCell *current = (XYCell*) cell[cell_index];

 symbol->Clear(cell_index);
 visual->SetDrawingColor(2);
 symbol->Draw(cell_index, current->GetAngle());
}

/*****************************************************************************/

void XYModel::AllocateCells(int new_cells)
{
 int i;
 for (i=0; i<cells; i++)
    {
    delete cell[i];
    }
 
 delete [] cell;

 cells = new_cells;
 cell = new CACell*[cells];

 for (i = 0;i < cells; i++)
    {
    cell[i]=new XYCell;
    }
 Init_spinconfig();
}

/*****************************************************************************/

void XYModel::ReadEnvironRequester()
{
 CAModel::ReadEnvironRequester();
 CAGrid *grid = (CAGrid*) environ;
 if (cells != grid->GetNumberOfCells())
    {
    AllocateCells(grid->GetNumberOfCells());
    }
 Redraw();
}

/*****************************************************************************/

void XYModel::Init_spinconfig()
{
 int cell_index;
 XYCell *current;
 for (cell_index = 0; cell_index < cells ;cell_index++)
    {
    current = (XYCell *) cell[cell_index];
    current->SetAngle(PIx2*drand48());
    }
}

/*****************************************************************************/

int XYModel::Monte_Carlo(int cell_index)
    
{ double trial_angle,sum_cos_ny,sum_cos_gam,delta_E,boltzmann;
  int updated;
  XYCell *current;
  current = (XYCell *) cell[cell_index];
 
 trial_angle = Trial_angle(current);
 sum_cos_ny = New_energy(cell_index, trial_angle);
 sum_cos_gam = Old_energy(cell_index);
 delta_E = (-J)*(sum_cos_ny - sum_cos_gam);
 boltzmann = exp((-beta)*delta_E);

 if (boltzmann >= drand48())
    {
    updated = 1;
    current->SetAngle(trial_angle);
    }
 else 
   {
   updated = 0;
   }  

 return(updated);
}

/*****************************************************************************/

double XYModel::Trial_angle(XYCell *current)
{
  double buffer;

  buffer=current->GetAngle() + temp_factor*PIx2 * (drand48() - 0.5);
  
  /* Trial angle is plus/minus pi/4 */
  
  return(buffer);                                 /* from the old */
}

/*****************************************************************************/

double XYModel::New_energy(int cell_index, double trial_angle)

{ CAGrid *grid = (CAGrid*) environ;
  XYCell *current;
  int i;
  double sum_cos;
  index_array neighbours;

 grid->QueryNeighbours(cell_index, 1, neighbours);
 
 sum_cos = 0;
 
 for (i = 0; i < neighbours.GetSize(); i++)
    {
    current = (XYCell *) cell[neighbours.array[i]];
    sum_cos += cos(trial_angle - current->GetAngle());
    }
 return(sum_cos);
}

/*****************************************************************************/

double XYModel::Old_energy(int cell_index)
{
 CAGrid *grid = (CAGrid*) environ;
 XYCell *current;
 double sum_cos, angle;
 int i;
 index_array neighbours;
 grid->QueryNeighbours(cell_index, 1, neighbours);
 
 sum_cos = 0;

 current = (XYCell *) cell[cell_index];
 angle = current->GetAngle();

 for (i = 0; i < neighbours.GetSize(); i++)
    {
    current = (XYCell *) cell[neighbours.array[i]];
    sum_cos += cos(angle - current->GetAngle());
    }
 return(sum_cos);
}

/*****************************************************************************/

void  XYModel::CheckVortices()
{
 int spin;
 int cell_index;
 XYCell *current;
 int accept = 0;
 double old_angle;
 double x,y,sx,sy;
 double scale;                                   /* Find a uniform XY scale */
 CARectangularGrid *grid = (CARectangularGrid*) environ;

 visual->SetLineWidth(CA_DOUBLE_LINEWIDTH);

 for (cell_index = 0; cell_index < cells; cell_index++)
    {
    grid->QueryCornerXY(cell_index, x, y);
    grid->QueryScaleFactors(sx, sy);
    scale = max(sx,sy) * 0.4;
    
    if (CheckVortex(cell_index) == -1)
       {
       // Draw minus symbol.
       visual->SetDrawingColor(4);
       visual->DrawLine(x - scale, y, x + scale, y);
       }
    else if (CheckVortex(cell_index) == 1)
       {
       // Draw plus symbol
       visual->SetDrawingColor(3);
       visual->DrawLine(x - scale, y, x + scale, y);
       visual->DrawLine(x, y - scale, x, y + scale);
       }
    else
       {
       // Clear symbol
       visual->SetDrawingColor(0);
       visual->DrawLine(x - scale, y, x + scale, y);
       visual->DrawLine(x, y - scale, x, y + scale);
       }
    }

 visual->SetLineWidth(CA_DEFAULT_LINEWIDTH);
}

/*****************************************************************************/

int XYModel::CheckVortex(int cell_index)
{
 XYCell *current;
 XYCell * next;
 index_array corner_cells;
 CARectangularGrid *grid = (CARectangularGrid *) environ;
 int i;
 
 double b = 0;

 grid->QueryCornerCells(cell_index, corner_cells);
 current = (XYCell *) cell[corner_cells.array[0]];

 for (i = 0; i < corner_cells.GetSize(); i++)
    {
    next = (XYCell *) cell[corner_cells.array[(i + 1) %
					     corner_cells.GetSize()]];
    
    // b += next->GetAngle() - current->GetAngle();
    
    if ((next->GetAngle() - current->GetAngle()) > 0.5*PIx2)
       {
       b -= PIx2;
       }

    if ((next->GetAngle() - current->GetAngle()) < -0.5*PIx2)
       {
       b += PIx2;
       }
    current = next;
    }

 if ((b <= (PIx2 + delta)) && (b >=(PIx2 - delta)))
    {
    return (-1);
    }
 if ((b <= (-PIx2 + delta)) && (b >=(-PIx2 - delta)))
    {
    return (1);
    }
 return (0);
}

/*****************************************************************************/

void XYModel::temp_ok(Req *req, XtPointer data)
{
 Widget temperature;
 char beta_string[40];
 XYModel *model = (XYModel *) data;
 temperature = req->GetWidget("Temperature", NULL);

 model->beta = 1/atof(XawDialogGetValueString(temperature));
}

/*****************************************************************************/

void XYModel::set_temp(Widget w, XtPointer data, XtPointer garb)
{
 XYModel *model = (XYModel *)data;
 model->temp_req.Popup();
}

/*****************************************************************************/

void XYModel::temp_cancel(Req *req, XtPointer data)
{
}

/*****************************************************************************/

