/*****************************************************************************/
/*                                                                           */
/* DiffModel.C: Header file for CAModel object.                              */
/*                                                                           */
/*****************************************************************************/

/* $Id: DiffModel.C,v 1.4 1998 3/5 15:30:17 olem Exp $ */

#include <Palladium.h>

  // This model currently relies on the special properties of the
  // triangular grid, and will not work in rectangular grids.


double ITERATIONS = 0.0;
double ENTROPY = 0.0;
int NESCAPED = 0;
const int NOUTPUT = 1;

   const double k_B = 8.617E-5;      // Boltzmann's constant, in eV/K
   const double T_kin = 20;          // Change in T due to kinetic energy
   const double T_exo = 40;          // Change in T due to exothermic breakup

   const double TMax = 300;          // These decide the colour scale
   const double EMax = 7.0;

   const double E_fcc = -0.44;       // From loevvik3 unless other stated.  All
   const double E_hcp = -0.41;       // energies are in eV, relative to a free H2
   const double E_sub = -0.14;       // molecule.  
   const double E_brg = -0.39;       // 
   const double E_subtst = -0.04;
   const double E_gasads1 = -0.45;   // E_gasads/des are measures of the gas density
   const double E_gasads2 = -0.95;
   const double E_gasdes1 = -0.45;
   const double E_gasdes2 = -0.95;
   const double E_gas = 0.0;         // By definition
   const double E_gastst0 = 0.18;    // Supposedly; needs confirmation
   const double E_gastst1 = 0.09;    // From olsen3
   const double E_gastst2 = 0.09;    // Supposedly; needs confirmation
   const double dE_neigh1 = 0.50;
   const double dE_neigh2 = 0.007;
   const double dE_neigh2sym=-0.02;
   const double dE_neigh3 = 0.012;
   const double dE_neigh4 = 0.0;      // Unknown!
   const double dE_neigh5 = -0.017;  // dE_neigh6 = 0 (def.)
   const double dE_current = 0.306;  // From loevvik3; theta = 1/3!
   const double dE_neigh1sub = 0.115;// From loevvik3; theta = 1/3!
   const double dE_neigh2sub = 0.044;// From loevvik3; theta = 1/3!
   const double dE_neigh3sub = 0.0;  // Unknown!
   const double dE_neigh4sub = 0.0;  // Unknown!
   const double dE_neigh5sub = 0.0;  // Unknown!
   const double dE_brgn1 = 0.2245;   // From loevvik3; number unpublished; theta = 1/3!
   const double dE_brgn2 = 0.0;      // Unknown!
   const double dE_subtst = 0.10;
   const double dE_bulktst = 1.0;    // From olsen1; unrelaxed!

   const double tau = 1.0;           // Timestep length
   const double nu_diff = 1.0;      // Hopping frequency
   const double nu_des = 000.0;     // Desorption hopping frequency
   const double nu_ads = 0.0;       // Adsorption hopping frequency
   const double m = 1.0;             // Mass of intrusive particles
   const double sigma = 0.5;         // Thermal conductivity & heat capacity
   const double alpha = 10.0;        // Thermal barrier height
   const double exo = 1.0;           // Exothermic energy added by one arriving particle

/*****************************************************************************/
/* Palladium object:                                                           */
/*****************************************************************************/

Palladium::Palladium(Widget new_parent, CASymmetry Environment):ParticleModel(new_parent,Environment)
    
{ int i;
  long ltime;
  Widget temperature, hydrogennumber, symmetrytype;
  char beta_string[40], hydrogen_string[40], symmetry_string[40];

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

switch (Environment)
   {
   case CATriangular:  subenviron = subgrid = new CATriangularGrid(0,0,8,8,24,24,60);
                       break;

   case CARectangular: subenviron = subgrid = new CARectangularGrid;
   }

symbol = new CABackgroundedCircle(visual, environ);
subsymbol = new CABackgroundedCircle(visual, environ);

widgetT = 100.0;
widgetH = 0.33;

AllocateCells(grid->GetNumberOfCells());
   
 srand48(lrand48());  // This needs to be randomized better!!!!! Hack
 
environ->SyncViewport(visual);
environ->SetDrawGrid(TRUE);
environ->SetPeriodic(TRUE);
visual->Popup();

visual->SetJustification(1, CATextVisual::CENTER);
visual->SetFont(1, "Times", 'b', 'r', 14);
visual->SetJustification(3, CATextVisual::CENTER);
visual->SetFont(3, "Times", 'b', 'r', 12);
visual->SetJustification(4, CATextVisual::CENTER);
visual->SetFont(4, "Times", 'b', 'r', 12);
visual->SetJustification(5, CATextVisual::CENTER);
visual->SetFont(5, "Times", 'b', 'r', 12);
visual->SetJustification(6, CATextVisual::CENTER);
visual->SetFont(6, "Times", 'b', 'r', 12);

visual->Print(1,"Hydrogen Palladium model");
visual->Print(3,"Surf.  Subsurf.  Coverage");
//visual->Print(5,"Average temperature:");
visual->Printf(7,"Symmetries:");
visual->Printf(13, "Timesteps      escaped");

// Set color palette:
visual->SetPaletteRGB(0, 255, 255, 255); /* white */
visual->SetPaletteRGB(1, 0, 0, 0);       /* black */

/* background colours - blue shades */

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

 temp_req.Initialize(parent, "Set temperature");
 hydrogen_req.Initialize(parent, "Set hydrogen number");
 symmetry_req.Initialize(parent, "Set symmetry number");
 
 temp_req.SetCallbacks(temp_ok, temp_cancel, (XtPointer)this);
 hydrogen_req.SetCallbacks(hydrogen_ok, hydrogen_cancel, (XtPointer)this);
 symmetry_req.SetCallbacks(symmetry_ok, symmetry_cancel, (XtPointer)this);
 
 temperature = temp_req.AddWidget("Temperature", NULL, dialogWidgetClass);
 hydrogennumber = hydrogen_req.AddWidget("H filling factor", NULL, dialogWidgetClass);
 symmetrytype = symmetry_req.AddWidget("Symmetry (1-5)", NULL, dialogWidgetClass);
 
 sprintf(beta_string, "%lf", 100.0);
 
 XtVaSetValues(temperature,
	       XtNlabel, "Temperature",
	       XtNvalue, beta_string,
	       NULL);


 sprintf(hydrogen_string, "%lf", 0.33);
 
 XtVaSetValues(hydrogennumber,
	       XtNlabel, "H filling factor",
	       XtNvalue, hydrogen_string,
	       NULL);

 sprintf(symmetry_string, "%d", 2);
 
 XtVaSetValues(symmetrytype,
	       XtNlabel, "Symmetry (1-5)",
	       XtNvalue, symmetry_string,
	       NULL);

 visual->AddControlMenuItem("Set Temperature", set_temp, (XtPointer)this);
 visual->AddControlMenuItem("Set H filling factor", set_hydrogen, (XtPointer)this);
 visual->AddControlMenuItem("Set symmetric covering", set_symmetry, (XtPointer)this);
 
LoadState();
Redraw();
}

/*****************************************************************************/

Palladium::~Palladium()

{
}

/*****************************************************************************/

void Palladium::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();

if (cells != grid->GetNumberOfCells())
   {
   AllocateCells(grid->GetNumberOfCells());
   }

Redraw();
}

/*****************************************************************************/

void Palladium::temp_ok(Req *req, XtPointer data)

{ Widget temperature;
  Palladium *model = (Palladium *) data;
  temperature = req->GetWidget("Temperature", NULL);
  ThermoCell *current;
  int i;

model->widgetT = atof(XawDialogGetValueString(temperature));

for (i = 0; i < model->cells; i++)
   {
   current = (ThermoCell*) (model->cell)[i];
   current->T = model->widgetT;
   }
}

/*****************************************************************************/

void Palladium::hydrogen_ok(Req *req, XtPointer data)

{ Widget hydrogen;
  Palladium *model = (Palladium *) data;
  hydrogen = req->GetWidget("H filling factor", NULL);
  int NH,i,cell_index;
  ThermoCell *current;
  
 model->widgetH = atof(XawDialogGetValueString(hydrogen));

 NH = (int)((double)(model->cells) * model->widgetH / 2.0);
 
 ITERATIONS = 0.0;
 NESCAPED = 0;

 for (i = 0; i < model->cells; i++)
    {
    current = (ThermoCell*)(model->cell)[i];
    current->SetAlive(FALSE);
    current->state = 0.0;
    current->T = model->widgetT;
    current->E = 0.0;
    }

 for (i = 0; i < NH; i++)
    {
    cell_index = (int)((double)(model->cells) * drand48());
    current = (ThermoCell*)(model->cell)[cell_index];
    if (current->Alive())
       {
       i--;
       }
    else
       {
       current->SetAlive(TRUE);
       }
    }
 
 model->Redraw();
}

/*****************************************************************************/

void Palladium::symmetry_ok(Req *req, XtPointer data)

{ Widget symmetry;
 Palladium *model = (Palladium *) data;
 int i,j,r,c,delta11,delta33,n0=0,n1=0;
 ThermoCell *current;
 symmetry = req->GetWidget("Symmetry (1-5)", NULL);
  
 model->widgetS = atoi(XawDialogGetValueString(symmetry));

 if ((model->widgetS) < 1 || (model->widgetS > 5))
    {
    printf("Symmetry not in range 1-5\n");
    return;
    }
 
 ITERATIONS = 0.0;
 NESCAPED = 0;

 for (i = 0; i < model->cells; i++)
    {
    current = (ThermoCell*)(model->cell)[i];
    current->SetAlive(FALSE);
    current->state = 0.0;
    current->T = model->widgetT;
    current->E = 0.0;
    }

 for (i = 0; i < model->cells; i++)
    {
    current = (ThermoCell*)(model->cell)[i];

    model->grid->QueryGridPosition(i,r,c);
    
    delta11 = abs((r-c)%2);
    delta33 = (i + (r%2)*3)%6;
    
    switch (model->widgetS)
       {
       case 1: if (delta11 == n0)
	  {
	  current->SetAlive(TRUE);
	  }
       break;
	       
       case 2: if (delta33 == n0)
	  {
	  current->SetAlive(TRUE);
	  }
       break;

       case 3: if ((delta33 == n0)||(delta33 == ((n0+2)%6)))
	  {
	  current->SetAlive(TRUE);
	  }
       break;

       case 4: if ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		   (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)))
	  {
	  current->SetAlive(TRUE);
	  }
       break;

       case 5: if (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))
	  {
	  current->SetAlive(TRUE);
	  }
       break;
       }
    }
 
 model->Redraw();
}

/*****************************************************************************/

void Palladium::set_temp(Widget w, XtPointer data, XtPointer garb)

{ Palladium *model = (Palladium *)data;
  model->temp_req.Popup();
}

/*****************************************************************************/

void Palladium::set_hydrogen(Widget w, XtPointer data, XtPointer garb)

{ Palladium *model = (Palladium *)data;
  model->hydrogen_req.Popup();
}

/*****************************************************************************/

void Palladium::set_symmetry(Widget w, XtPointer data, XtPointer garb)

{ Palladium *model = (Palladium *)data;
  model->symmetry_req.Popup();
}

/*****************************************************************************/

void Palladium::temp_cancel(Req *req, XtPointer data)

{
}

/*****************************************************************************/

void Palladium::hydrogen_cancel(Req *req, XtPointer data)

{
}

/*****************************************************************************/

void Palladium::symmetry_cancel(Req *req, XtPointer data)

{
}

/*****************************************************************************/

void Palladium::Draw(int cell_index)
    
{
 ThermoCell *current_cell = (ThermoCell*) cell[cell_index];
 ThermoCell *currentsub_cell = (ThermoCell*) subcell[cell_index];

 if (currentsub_cell->Alive())
    {
    if (current_cell->Alive())
       {
       subsymbol->DrawBigSmall(cell_index,SColour(currentsub_cell->state),EColour(current_cell->E),EColourSub(currentsub_cell->E));
       }
    else
       {
       subsymbol->DrawBig(cell_index,SColour(currentsub_cell->state),EColourSub(currentsub_cell->E));
       }
    }
 else if (current_cell->Alive())
    {
    symbol->Draw(cell_index,SColour(current_cell->state),EColour(current_cell->E));
    }
 else
    {
    symbol->Draw(cell_index,SColour(current_cell->state),CANoSymbol);   
    }
}

/*****************************************************************************/

void Palladium::Update()
    
{ int i, nalive, nalivesub, cell_index;
  ThermoCell *current_cell, *currentsub_cell;
  int count[cells];
  index_array neighbours;

 nalive = 0;
 nalivesub = 0;

 ITERATIONS++;
 
 for (i = 0; i < cells; i++)
    {
    cell_index = (int) (drand48()*cells);

    DoFluctuations(cell_index);
    }

 visual->CurrentPageBack();

 Taverage = 0;

 for (i = 0; i < cells; i++)
    {
    current_cell = (ThermoCell*) cell[i];
    currentsub_cell = (ThermoCell*) subcell[i];

    Taverage += current_cell->T; //what about currentsub_cell->T?
    if (currentsub_cell->Alive())
       {
       nalivesub++;
       }
    
    if (current_cell->Alive())
       {
       nalive++;
       }

    grid->QueryNeighbours(i, 1, neighbours);

    //ThermalLatticeConduction(current_cell,neighbours);

    // Need to call this again since returning from TLC seems to trash
    // "neighbours"
    
    grid->QueryNeighbours(i, 1, neighbours);

    //ThermalLatticeConduction(currentsub_cell,neighbours);
    
    Draw(i);
    } 

 if ((int)ITERATIONS % NOUTPUT == 0)
    {
    visual->Printf(4,"%4d     %d     %.2f", nalive,nalivesub,(double)(2*(nalive+nalivesub))/(double)cells);
    //visual->Printf(6, "%f",Taverage/(double)cells);
    visual->Printf(14, "   %10.0f        %10d",ITERATIONS,NESCAPED);
    }

 if ((int)ITERATIONS % 100 == 0) // Every couple of minutes dump to disk
    {
    SaveState();
    }

 if (ITERATIONS > 0)
    {
    CheckSymmetries();
    FindPattern();
    }

 environ->DrawGrid(visual);
 visual->CurrentPageFront(); 
}

/*****************************************************************************/

void Palladium::DblClick(double x, double y)

{ int cell_index;

cell_index = grid->QueryCell(x, y);

ThermoCell *currentsub_cell = (ThermoCell*) subcell[cell_index];

if (currentsub_cell->Alive())
   {
   currentsub_cell->SetAlive(False);
   }
else
   {
   currentsub_cell->SetAlive(True);
   }

Draw(cell_index);
}

/*****************************************************************************/

void Palladium::Click(double x, double y)

{ int cell_index;

cell_index = grid->QueryCell(x, y);

ThermoCell *current_cell = (ThermoCell*) cell[cell_index];

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

void Palladium::AllocateCells(int new_cells)

{ int i, row, col;
  ThermoCell *current_cell;
  ThermoCell *currentsub_cell;

 for (i = 0; i < cells; i++)
    {
    delete cell[i];
    delete subcell[i];
    }
 
 delete [] cell;
 delete [] subcell;

 cells = new_cells;
 
 cell = new CACell*[cells];
 subcell = new CACell*[cells];

 for (i = 0; i < cells; i++)
    {
    cell[i] = new ThermoCell;
    current_cell = (ThermoCell *)cell[i];
    current_cell->E = 0;
    current_cell->T = widgetT;
    current_cell->SetAlive(False);

    subcell[i] = new ThermoCell;
    currentsub_cell = (ThermoCell *)subcell[i];
    currentsub_cell->E = 0;
    currentsub_cell->T = widgetT;
    // insert if(Triangular grid) 
    grid->QueryGridPosition(i,row,col);
    if ((row - col) % 2 == 0)
       { // If the cell is a hcp hollow site, the subsurface site is forbidden
       currentsub_cell->SetForbidden(True);
       }
    else
      {
      currentsub_cell->SetAlive(False);
      currentsub_cell->SetForbidden(False);
      }
    // insert else currentsub_cell->SetAlive(False) & SetForbidden(False);  // Rectangular grid
    }
}

/*****************************************************************************/
// Level 2
/*****************************************************************************/

void Palladium::DoFluctuations(int cell_index)

{ int favourite = cell_index, neigh_index, neigh2_index, neighsub_index, i, type=0;
  int neigh_count, current_alive, neigh1_alive, neigh2_alive, neigh3_alive,
    neigh4_alive, neigh5_alive, currentsub_alive, neigh_currentsub_alive,
    neigh1sub_alive, neigh2sub_alive, neigh3sub_alive,
    neigh4sub_alive, neigh5sub_alive, row, col;
  double P_max=0.0, P_dummy;
  double random = drand48(), P_diff_T, P_diff, highest = 0.0, beta, deltaE, bridgeE;
  ThermoCell *current_cell, *neigh_cell, *neigh2_cell, *currentsub_cell, *neighsub_cell, *neigh2sub_cell;
  index_array neigh_array, neigh2_array, neighsub_array;
  
current_cell = (ThermoCell *) cell[cell_index];
currentsub_cell = (ThermoCell *) subcell[cell_index];

// Find occupied cell neighbours

neigh1_alive = CountAlive(cell_index,1);
neigh2_alive = CountAlive(cell_index,2);
neigh3_alive = CountAlive(cell_index,3);
neigh4_alive = CountAlive(cell_index,4);
neigh5_alive = CountAlive(cell_index,5);

// insert if Rectangular grid neigh1sub_alive = CountAliveSub(cell_index,1) etc;
currentsub_alive = currentsub_cell->Alive();
neigh1sub_alive = CountAliveSub(cell_index,1);
neigh2sub_alive = CountAliveSub(cell_index,2);
neigh3sub_alive = CountAliveSub(cell_index,3);
neigh4sub_alive = CountAliveSub(cell_index,4);
neigh5sub_alive = CountAliveSub(cell_index,5);
// We don't know whether current is a fcc or hcp site, thus we need all neighsubs.

// Find number of alive neighbours of any neighbour for later

// Hard-code neighbour sizes for now

grid->QueryNeighbours(cell_index,1,neigh_array);

int neigh_neigh1_alive[3]; 
int neigh_neigh2_alive[6];
int neigh_neigh3_alive[3];
int neigh_neigh4_alive[6];
int neigh_neigh5_alive[6];
int neigh_neigh1sub_alive[3];
int neigh_neigh2sub_alive[6];
int neigh_neigh3sub_alive[3];
int neigh_neigh4sub_alive[6];
int neigh_neigh5sub_alive[6];

for (neigh_index=0; neigh_index < 3; neigh_index++)
   {
   neigh_neigh1_alive[neigh_index] = CountAlive(neigh_array.array[neigh_index],1);
   neigh_neigh3_alive[neigh_index] = CountAlive(neigh_array.array[neigh_index],3);
   neigh_neigh1sub_alive[neigh_index] = CountAliveSub(neigh_array.array[neigh_index],1);
   neigh_neigh3sub_alive[neigh_index] = CountAliveSub(neigh_array.array[neigh_index],3);
   }
for (neigh_index=0; neigh_index < 6; neigh_index++)
   {
   neigh_neigh2_alive[neigh_index] = CountAlive(neigh_array.array[neigh_index],2);   
   neigh_neigh4_alive[neigh_index] = CountAlive(neigh_array.array[neigh_index],4);   
   neigh_neigh5_alive[neigh_index] = CountAlive(neigh_array.array[neigh_index],5);   
   neigh_neigh2sub_alive[neigh_index] = CountAliveSub(neigh_array.array[neigh_index],2);
   neigh_neigh4sub_alive[neigh_index] = CountAliveSub(neigh_array.array[neigh_index],4);
   neigh_neigh5sub_alive[neigh_index] = CountAliveSub(neigh_array.array[neigh_index],5);
   }

// Calculate the adsorption energy of the current cell, surface and subsurface:

grid->QueryGridPosition(cell_index,row,col);  
current_cell->E = AdsorptionEnergy(neigh1_alive, neigh2_alive, neigh3_alive, neigh4_alive, neigh5_alive, currentsub_alive, neigh1sub_alive, neigh2sub_alive, neigh3sub_alive, neigh4sub_alive, neigh5sub_alive, (row-col)%2 );
//printf("n1 %d n2 %d currentE %f\n",current_cell->E);
if (current_cell->Alive()) { current_alive = 1; } else { current_alive = 0; }
currentsub_cell->E = AdsorptionEnergySub(current_alive, neigh1_alive, neigh2_alive, neigh3_alive, neigh4_alive, neigh5_alive, neigh2sub_alive, neigh5sub_alive);

// Now we can start... 

// If the current cell is not alive, the following may happen:
// 1. Adsorption, current and neigh1
// 2. Adsorption, currentsub and neigh1
// 3. Adsorption, current and neigh1sub
// 4. Diffusion, neigh1 to current
// 5. Desorption, currentsub and neigh1
// 6. Adsorption, current and neigh2
// 7. Adsorption, current and currentsub
// 8. Diffusion, currentsub to current
// 9. Diffusion, neigh2 to current (Not implemented yet)
//
// Classical diffusion is implemented, so that the probability of a
// transition is
// P = tau*nu*Exp(-deltaE/kT),
// where tau is the time step, nu is the hopping frequency, deltaE is
// the activation barrier, and k is Boltzmann's constant.
// 

if (! current_cell->Alive())
   {
   int method = (int)(drand48()*8.0+1.0);
   
   switch (method)
      {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
	  
         // Loop over nearest neighbours (neigh1's):
	 for (neigh_index = 0; neigh_index < neigh_array.GetSize(); neigh_index++)
	    {
	    if (neigh_array.array[neigh_index] == NoCell)
	       {
	       continue; // Edge effect
	       }
	    neigh_cell = (ThermoCell *) cell[neigh_array.array[neigh_index]];
	    neighsub_cell = (ThermoCell *) subcell[neigh_array.array[neigh_index]];
	    neigh_currentsub_alive = neighsub_cell->Alive();
	    // Calculate the adsorption energy of the neighbour
	    grid->QueryGridPosition(neigh_array.array[neigh_index],row,col);  
	    neigh_cell->E = AdsorptionEnergy(neigh_neigh1_alive[neigh_index], neigh_neigh2_alive[neigh_index], neigh_neigh3_alive[neigh_index], neigh_neigh4_alive[neigh_index], neigh_neigh5_alive[neigh_index],neigh_currentsub_alive,neigh_neigh1sub_alive[neigh_index],neigh_neigh2sub_alive[neigh_index],neigh_neigh3sub_alive[neigh_index],neigh_neigh4sub_alive[neigh_index], neigh_neigh5sub_alive[neigh_index], (row-col)%2 );
	     
	     
	    if (! neigh_cell->Alive())// && (method == 1 || method == 2 || method == 3))
	       {
	       switch (method)
	          {
	          case 1:
		    // Dissociative adsorption, current and neigh1:
		    // First find the highest "barrier":
		    if ( E_gastst1 > neigh_cell->E + current_cell->E + dE_neigh1)
		       {
		       deltaE = E_gastst1 - E_gas;
		       // Should make E_gastst1 dependent of neighbours!
		       }
		    else
		       {
		       deltaE = neigh_cell->E + current_cell->E + dE_neigh1 - E_gas;
		       // Add dE_neigh1, since current and neigh become nearest neighbours
		       }
	             beta = 2.0 / ((current_cell->T + neigh_cell->T)* k_B );
		     P_dummy = tau*nu_ads*exp(-beta*deltaE);
		     //printf("method %d neigh %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,current_cell->E,deltaE,beta,P_dummy);
		     if ( P_dummy > P_max )  // Find `least action'
		        {
		        P_max = P_dummy;
		        type = 1;
		        favourite = neigh_index;
		        }
		     break;

		  case 2:
		     // Dissociative adsorption, currentsub and neigh1:
		     if (!currentsub_cell->Alive() && !currentsub_cell->Forbidden())
		        {
			if (E_gastst1 > neigh_cell->E + currentsub_cell->E + dE_neigh1 + dE_subtst)
			   {
			   deltaE = E_gastst1 - E_gas;
			   // E_gastst1 should depend on neigh's!
			   }
			else
			   {
			   deltaE = neigh_cell->E + currentsub_cell->E + dE_neigh1 + dE_subtst - E_gas;
			   }
			// Add dE_neigh1, since currentsub and neigh become nearest
			// neighbours.  This should be specified better! (dE_neigh1sub
			// or something)  dE_subtst should depend on neigh's!
			beta = 2.0 / ((currentsub_cell->T + neigh_cell->T)* k_B );
			P_dummy = tau*nu_ads*exp(-beta*deltaE);
			//printf("method %d neigh %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,current_cell->E,deltaE,beta,P_dummy);
			if ( P_dummy > P_max )
			   {
			   P_max = P_dummy;
			   type = 2;
			   favourite = neigh_index;
			   }
			}
		     break;

		  case 3:
		     // Dissociative adsorption, current and neigh1sub:
		     if (!neighsub_cell->Alive() && !neighsub_cell->Forbidden())
		        {
			if (E_gastst1 > neighsub_cell->E + current_cell->E + dE_neigh1 + dE_subtst)
			   {
			   deltaE = E_gastst1 - E_gas;
			   // E_gastst1 should depend on neigh's!
			   }
			else
			   {
			   deltaE = neighsub_cell->E + current_cell->E + dE_neigh1 + dE_subtst - E_gas;
			   }
			// Add dE_neigh1, since current and neighsub are nearest
			// neighbours.  This should be specified better! (dE_neigh1sub
			// or something)  dE_subtst should depend on neigh's!
			beta = 2.0 / ((current_cell->T + neighsub_cell->T)* k_B );
			P_dummy = tau*nu_ads*exp(-beta*deltaE);
			//printf("method %d neighsub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neighsub_cell->E,current_cell->E,deltaE,beta,P_dummy);
			if ( P_dummy > P_max )
			   {
			   P_max = P_dummy;
			   type = 3;
			   favourite = neigh_index;
			   }
			}
		     break;
		  } // End switch 1 2 3
	       } // End if (!neigh_cell->Alive())
		 
	    if (neigh_cell->Alive()) 
	       {
	       switch (method)
	          {
		  case 4:
	             // Diffusion, neigh1 to current:
		     // Calculate the energy at the bridge (subtract the known neigh from neigh1_alive):
 		     bridgeE = BridgeEnergy((neigh1_alive - 1), neigh2_alive, neigh_neigh1_alive[neigh_index], neigh_neigh2_alive[neigh_index]);
		     if ( bridgeE > current_cell->E - dE_neigh1)
		        {
			deltaE = bridgeE - neigh_cell->E;
		        }
		     else
		        {
			deltaE = current_cell->E - neigh_cell->E - dE_neigh1;
			// subtract dE_neigh1, since it has been added in currentE
		        }
		     //printf("notalive4 bridgeE %f currentE %f neighE %f deltaE %f n1 %d nn1 %d ns %d nns %d\n",bridgeE,current_cell->E,neigh_cell->E,deltaE,neigh1_alive,neigh_neigh1_alive[neigh_index],currentsub_cell->Alive(),neighsub_cell->Alive());
		     beta = 2.0 / ((current_cell->T + neigh_cell->T)* k_B );
		     P_dummy = tau*nu_diff*exp(-beta*deltaE);
		     //printf("method %d neighsub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,current_cell->E,deltaE,beta,P_dummy);
		     if (P_dummy > P_max)   
		        {
			P_max = P_dummy;
			type = 4;
			favourite = neigh_index;
			}
		     break;
		       
		  case 5:
 		     if (currentsub_cell->Alive())
		        {
			// Associative desorption, currentsub and neigh1:
			deltaE = E_gastst1 - currentsub_cell->E - neigh_cell->E + dE_neigh1;
			// E_gastst1 is higher than E_subtst (olsen3)
			// Subtract dE_neigh1, since it has been added both in currentsub_cell->E and neigh_cell->E
			beta = 2.0 / ((currentsub_cell->T + neigh_cell->T)* k_B );
			P_dummy = tau*nu_des*exp(-beta*deltaE);
			//printf("method %d neighsub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,currentsub_cell->E,deltaE,beta,P_dummy);
			if ( P_dummy > P_max )  // Find `least action'
			   {
			   P_max = P_dummy;
			   type = 5;
			   favourite = neigh_index;
			   }
			}
		     break;
		  } // End switch 4 5
	       } // End if neigh_cell->Alive()
	    } // End loop over nearest neighbours
	 break;
	  
      case 6:
	 // Loop over next nearest neighbours (neigh2's):
	 grid->QueryNeighbours(cell_index,2,neigh2_array);
	 for (neigh2_index = 0; neigh2_index < neigh2_array.GetSize(); neigh2_index++)
	    {
	    if (neigh2_array.array[neigh2_index] == NoCell)
	       {
	       continue; // Edge effect
	       }
	    neigh2_cell = (ThermoCell *) cell[neigh2_array.array[neigh2_index]];
	
	    if (! neigh2_cell->Alive())
	       {
	       // First calculate the adsorption energy of the 2nd neighbour
	       grid->QueryGridPosition(neigh2_array.array[neigh2_index],row,col);  
	       neigh2sub_cell = (ThermoCell *) subcell[neigh2_array.array[neigh2_index]];
	       neigh2_cell->E = AdsorptionEnergy(CountAlive(neigh2_array.array[neigh2_index],1), CountAlive(neigh2_array.array[neigh2_index],2), CountAlive(neigh2_array.array[neigh2_index],3), CountAlive(neigh2_array.array[neigh2_index],4), CountAlive(neigh2_array.array[neigh2_index],5), neigh2sub_cell->Alive(),CountAliveSub(neigh2_array.array[neigh2_index],1),CountAliveSub(neigh2_array.array[neigh2_index],2),CountAliveSub(neigh2_array.array[neigh2_index],3),CountAliveSub(neigh2_array.array[neigh2_index],4), CountAliveSub(neigh2_array.array[neigh2_index],5), (row-col)%2 );
			
	       // Dissociative adsorption.  Both on the surface, 2nd neighbours:
	       if ( E_gastst2 > neigh2_cell->E + current_cell->E + dE_neigh2)
		  {
	          deltaE = E_gastst2 - E_gas;
	          // Should make E_gastst2 dependent of neighbours!
		  }
	       else
		  {
	          deltaE = neigh2_cell->E + current_cell->E + dE_neigh2 - E_gas;
	          // Add dE_neigh2, since current and neigh2 are 2nd neighbours
		  }
	       beta = 2.0 / ((current_cell->T + neigh2_cell->T)* k_B );
	       P_dummy = tau*nu_ads*exp(-beta*deltaE);
	       //printf("method %d neighsub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh2_cell->E,current_cell->E,deltaE,beta,P_dummy);
	       if (P_dummy > P_max)   
	          {
	          P_max = P_dummy;
	          type = 6;
	          favourite = neigh2_index;
	          }
	       }
	    } // End neigh2-loop
	 break;
	 
      case 7:
	 // Dissociative adsorption.  Surface and subsurface, same cell:
	 if (!currentsub_cell->Alive() && !currentsub_cell->Forbidden())
	    {
	    if ( E_gastst0 > current_cell->E + currentsub_cell->E + dE_current + dE_subtst)
	       {
	       deltaE = E_gastst0 - E_gas;
               // Should make E_gastst0 dependent of neighbours!
	       }
	    else
	       {
	       deltaE = current_cell->E + currentsub_cell->E + dE_current + dE_subtst - E_gas;
	       // Add dE_current, since currentsub and neigh are at the same site.
	       // dE_subtst should be dependent of neighbours!
	       }
	    beta = 2.0 / ((current_cell->T + currentsub_cell->T)* k_B );
	    P_max = tau*nu_ads*exp(-beta*deltaE);
	    ////printf("method %d sub %lf curr %lf deltaE %lf beta %lf P_dummy %e\n",method,currentsub_cell->E,current_cell->E,deltaE,beta,P_max);
	    type = 7;
	    }
	 break;  

      case 8:
	 // Diffusion.  Subsurface to surface:
	 if (currentsub_cell->Alive())
	    {
	    if ( dE_subtst > current_cell->E - currentsub_cell->E)
	       {
	       deltaE = dE_subtst;
	       }
	    else
	       {
	       deltaE = current_cell->E - currentsub_cell->E;
	       }
	    beta = 2.0 / ((current_cell->T + currentsub_cell->T)* k_B );
	    P_max = tau*nu_diff*exp(-beta*deltaE);
	    //printf("method %d sub %f curr %f deltaE %f beta %f P_dummy %f\n",method,currentsub_cell->E,current_cell->E,deltaE,beta,P_max);
	    type = 8;
	    }
	 break;  
	 
      case 9:
	 // Loop over next nearest neighbours (neigh2's):  Wait until later...
	 grid->QueryNeighbours(cell_index,2,neigh2_array);
	 for (neigh2_index = 0; neigh2_index < neigh2_array.GetSize(); neigh2_index++)
	    {
	    if (neigh2_array.array[neigh2_index] == NoCell)
		{
		continue; // Edge effect
		}
	    neigh2_cell = (ThermoCell *) cell[neigh2_array.array[neigh2_index]];
	
	    if (neigh2_cell->Alive())
		{
		// First calculate the adsorption energy of the 2nd neighbour
		grid->QueryGridPosition(neigh2_array.array[neigh2_index],row,col);  
		neigh2sub_cell = (ThermoCell *) subcell[neigh2_array.array[neigh2_index]];
		neigh2_cell->E = AdsorptionEnergy(CountAlive(neigh2_array.array[neigh2_index],1), CountAlive(neigh2_array.array[neigh2_index],2), CountAlive(neigh2_array.array[neigh2_index],3), CountAlive(neigh2_array.array[neigh2_index],4), CountAlive(neigh2_array.array[neigh2_index],5), neigh2sub_cell->Alive(),CountAliveSub(neigh2_array.array[neigh2_index],1),CountAliveSub(neigh2_array.array[neigh2_index],2),CountAliveSub(neigh2_array.array[neigh2_index],3),CountAliveSub(neigh2_array.array[neigh2_index],4), CountAliveSub(neigh2_array.array[neigh2_index],5), (row-col)%2 );
		bridgeE = BridgeEnergy((neigh1_alive - 1), neigh2_alive, neigh_neigh1_alive[neigh_index], neigh_neigh2_alive[neigh_index]);
			
		// Diffusion from 2nd neighbour to current

		if ( dE_subtst > current_cell->E - currentsub_cell->E)
		   {
		   deltaE = dE_subtst;
		   }
		else
		   {
		   deltaE = current_cell->E - currentsub_cell->E;
		   }
		deltaE = current_cell->E - neigh2_cell->E;
		beta = 2.0 / ((current_cell->T + neigh2_cell->T)* k_B );
		P_dummy = exp(-beta*deltaE);
		//printf("method %d neighsub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh2_cell->E,current_cell->E,deltaE,beta,P_dummy);
		if (P_dummy > P_max)   
		   {
		   P_max = P_dummy;
		   type = 9;
		   favourite = neigh2_index;
		   }
		}
	    } // End neigh2-loop
	 break;
      } // end switch(method) 1..9
   

   // Some of the cases are not allowed (eg. because the subsurface
   // cell is forbidden), so we use switch(type) instead of switch(method).

   //   if (type!=0) {printf(" Notalive%d deltaE %f   beta %f  Pmax %e\n",method,deltaE,beta,P_max);}

   if (P_max > random)
      {
      switch (type)
	 {
	 case 0: return;
	 
	 case 1: // Adsorption, current and neigh1
	     current_cell->SetAlive(True);
	     current_cell->T += T_exo + T_kin;
	     neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	     neigh_cell->SetAlive(True);
	     neigh_cell->T += T_exo + T_kin;
	     Draw(cell_index);
	     Draw(neigh_array.array[favourite]);
	     return;


	 case 2:// Adsorption, currentsub and neigh1
	     neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	     neigh_cell->SetAlive(True);
	     neigh_cell->T += T_exo + T_kin;
	     currentsub_cell->SetAlive(True);
	     currentsub_cell->T += T_exo + T_kin;
	     Draw(cell_index);
	     Draw(neigh_array.array[favourite]);
	     return;
	     
	 case 3:// Adsorption, current and neigh1sub
	     
	     neighsub_cell = (ThermoCell *) subcell[neigh_array.array[favourite]];
	     neighsub_cell->SetAlive(True);
	     neighsub_cell->T += T_exo + T_kin;
	     current_cell->SetAlive(True);
	     current_cell->T += T_exo + T_kin;
	     Draw(cell_index);
	     Draw(neigh_array.array[favourite]);
	     return;
	     
	 case 4:// Diffusion, neigh1 to current
	     
	     neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	     neigh_cell->SetAlive(False);
	     current_cell->SetAlive(True);
	     Draw(cell_index);
	     Draw(neigh_array.array[favourite]);
	     return;
	     
	 case 5: // Desorption, currentsub and neigh1
	     
	     currentsub_cell->SetAlive(False);
	     currentsub_cell->T -= T_kin;
	     neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	     neigh_cell->SetAlive(False);
	     neigh_cell->T -= T_kin;
	     Draw(cell_index);
	     Draw(neigh_array.array[favourite]);
	     NESCAPED++;
	     return;
	     
	 case 6:// Adsorption, current and neigh2
	     
	     current_cell->SetAlive(True);
	     current_cell->T += T_exo + T_kin;
	     neigh2_cell = (ThermoCell *) cell[neigh2_array.array[favourite]];
	     neigh2_cell->SetAlive(True);
	     neigh2_cell->T += T_exo + T_kin;
	     Draw(cell_index);
	     Draw(neigh2_array.array[favourite]);
	     return;
	     
	 case 7:// Adsorption, current and currentsub
	     current_cell->SetAlive(True);
	     current_cell->T += T_exo + T_kin;
	     currentsub_cell->SetAlive(True);
	     currentsub_cell->T += T_exo + T_kin;
	     Draw(cell_index);
	     return;
	     
	 case 8:// Diffusion, currentsub to current
	     
	     current_cell->SetAlive(True);
	     currentsub_cell->SetAlive(False);
	     Draw(cell_index);
	     return;

	 case 9:// Diffusion, neigh2 to current
	     
	     neigh2_cell = (ThermoCell *) cell[neigh2_array.array[favourite]];
	     neigh2_cell->SetAlive(False);
	     current_cell->SetAlive(True);
	     Draw(cell_index);
	     Draw(neigh2_array.array[favourite]);
	     return;	     
	 }  // End switch
      }
   }  // End if (!current_cell->Alive())


// If the current cell is alive, the following may happen:
// 1. Desorption, current and neigh1
// 2. Desorption, current and neigh1sub
// 3. Diffusion, current to neigh1
// 4. Desorption, current and neigh2
// 5. Desorption, current and currentsub
// 6. Diffusion, current to currentsub
// 7. Diffusion, current to neigh2 (Not implemented yet)

else //if (current_cell->Alive())
   {
   P_max = 0.0;
   favourite = cell_index;
   type = 0;

   int method = (int)(drand48()*6.0+1.0);

   switch (method)
      {
      case 1:
      case 2:
      case 3:
	 // Loop over nearest neighbours (neigh1's):
	 for (neigh_index = 0; neigh_index < neigh_array.GetSize(); neigh_index++)
	    {
	    if (neigh_array.array[neigh_index] == NoCell)
	 	{
	 	continue; // Edge effect
	 	}
	    neigh_cell = (ThermoCell *) cell[neigh_array.array[neigh_index]];
	    neighsub_cell = (ThermoCell *) subcell[neigh_array.array[neigh_index]];
	    // Calculate the adsorption energy of the neighbour
	    grid->QueryGridPosition(neigh_array.array[neigh_index],row,col);  
	    neigh_cell->E = AdsorptionEnergy(neigh_neigh1_alive[neigh_index]-1, neigh_neigh2_alive[neigh_index], neigh_neigh3_alive[neigh_index], neigh_neigh4_alive[neigh_index], neigh_neigh5_alive[neigh_index], neighsub_cell->Alive(), neigh_neigh1sub_alive[neigh_index], neigh_neigh2sub_alive[neigh_index], neigh_neigh3sub_alive[neigh_index], neigh_neigh4sub_alive[neigh_index], neigh_neigh5sub_alive[neigh_index], (row-col)%2 );
	    
	    
	    if ((method == 1) && neigh_cell->Alive())
	 	{
	 	// Associative desorption, current and neigh1:
	 	deltaE = E_gastst1 - current_cell->E - neigh_cell->E + dE_neigh1;
		// Subtract dE_neigh1, since it has been added twice in AdsEnrg
		// E_gastst1 should be neigh-dependent!
	 	beta = 2.0 / ((current_cell->T + neigh_cell->T)* k_B );
	 	P_dummy = tau*nu_des*exp(-beta*deltaE);
		//printf("method %d neigh %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,current_cell->E,deltaE,beta,P_dummy);
	 	if ( P_dummy > P_max )  // Find `least action'
	 	   {
	 	   P_max = P_dummy;
	 	   type = 1;
	 	   favourite = neigh_index;
	 	   }
	 	} // End if neigh_cell->Alive()
	    
	    if (!neigh_cell->Alive())
	 	{
	 	switch (method)
	 	   {
	 	   case 2:
	 	       // Associative desorption, current and neigh1sub:
	 	       if (neighsub_cell->Alive())
	 		  {
			  deltaE = E_gastst1 - current_cell->E - neighsub_cell->E + dE_neigh1;
			  // E_gastst1 is higher than E_subtst (olsen3)
			  // Subtract dE_neigh1, since it has been added twice in AdsEnrg
	 		  // This should be specified better! (dE_neigh1sub
	 		  // or something)
	 		  beta = 2.0 / ((current_cell->T + neighsub_cell->T)* k_B );
	 		  P_dummy = tau*nu_des*exp(-beta*deltaE);
			  //printf("method %d sub %f curr %f deltaE %f beta %f P_dummy %f\n",method,neighsub_cell->E,current_cell->E,deltaE,beta,P_dummy);
	 		  if ( P_dummy > P_max )
	 		     {
	 		     P_max = P_dummy;
	 		     type = 2;
	 		     favourite = neigh_index;
	 		     }
	 		  }
	 	       break;
	 
	 	   case 3:
	 	       // Diffusion, current to neigh1:
		       // Calculate the energy at the bridge (subtract the known neigh from neigh1_alive):
		       bridgeE = BridgeEnergy((neigh1_alive - 1), neigh2_alive, neigh_neigh1_alive[neigh_index], neigh_neigh2_alive[neigh_index]);
		       if ( bridgeE > neigh_cell->E)
			  {
			  deltaE = bridgeE - current_cell->E;
			  }
		       else
			 {
			 deltaE =  neigh_cell->E - current_cell->E;
			 }
		       //printf("   alive3 bridgeE %f currentE %f neighE %f deltaE %f n1 %d nn1 %d ns %d nns %d\n",bridgeE,current_cell->E,neigh_cell->E,deltaE,neigh1_alive,neigh_neigh1_alive[neigh_index],currentsub_cell->Alive(),neighsub_cell->Alive());
	 	       beta = 2.0 / ((current_cell->T + neigh_cell->T)* k_B );
	 	       P_dummy = tau*nu_diff*exp(-beta*deltaE);
		       //printf("method %d neigh %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh_cell->E,current_cell->E,deltaE,beta,P_dummy);
		       if (P_dummy > P_max)   
	 		  {
	 		  P_max = P_dummy;
	 		  type = 3;
	 		  favourite = neigh_index;
	 		  }
	 	       // break;
	 	   } // End switch 1 2 3
	 	} //End if (!neigh_cell->Alive())
	    } // End loop over nearest neighbours
	 break;
	 
      case 4:
      
	  grid->QueryNeighbours(cell_index,2,neigh2_array);
	  // Loop over next nearest neighbours (neigh2's):
	  for (neigh2_index = 0; neigh2_index < neigh2_array.GetSize(); neigh2_index++)
	     {
	     if (neigh2_array.array[neigh2_index] == NoCell)
		{
		continue; // Edge effect
		}
	     neigh2_cell = (ThermoCell *) cell[neigh2_array.array[neigh2_index]];
	     
	     if (neigh2_cell->Alive())
		{
		// First calculate the adsorption energy of the 2nd neighbour
		grid->QueryGridPosition(neigh2_array.array[neigh2_index],row,col);  
		neigh2sub_cell = (ThermoCell *) subcell[neigh2_array.array[neigh2_index]];
		neigh2_cell->E = AdsorptionEnergy(CountAlive(neigh2_array.array[neigh2_index],1), CountAlive(neigh2_array.array[neigh2_index],2), CountAlive(neigh2_array.array[neigh2_index],3), CountAlive(neigh2_array.array[neigh2_index],4), CountAlive(neigh2_array.array[neigh2_index],5), neigh2sub_cell->Alive(),CountAliveSub(neigh2_array.array[neigh2_index],1),CountAliveSub(neigh2_array.array[neigh2_index],2),CountAliveSub(neigh2_array.array[neigh2_index],3),CountAliveSub(neigh2_array.array[neigh2_index],4), CountAliveSub(neigh2_array.array[neigh2_index],5), (row-col)%2 );
		
		// Associative desorption.  Both on the surface, 2nd neighbours:
		deltaE = E_gastst2 - neigh2_cell->E - current_cell->E + dE_neigh2;
		// Subtract dE_neigh2, since it has been added twice in AdsEnrg
		beta = 2.0 / ((current_cell->T + neigh2_cell->T)* k_B );
		P_dummy = tau*nu_des*exp(-beta*deltaE);
		//printf("method %d neigh2 %f curr %f deltaE %f beta %f P_dummy %f\n",method,neigh2_cell->E,current_cell->E,deltaE,beta,P_dummy);
		if (P_dummy > P_max)   
		   {
		   P_max = P_dummy;
		   type = 4;
		   favourite = neigh2_index;
		   }
		}
	     }
	  break;

      case 5:	  

	  // Associative desorption.  Surface and subsurface, same cell:
	  if (currentsub_cell->Alive())
	     {
	     deltaE = E_gastst0 - current_cell->E - currentsub_cell->E + dE_current;
	     // Suppose that E_gastst0 is higher than E_subtst
	     // Should make E_gastst0 dependent of neighbours!
	     // Subtract dE_current, since it has been added in both currentsub_cell->E and current_cell->E
	     beta = 2.0 / ((current_cell->T + currentsub_cell->T)* k_B );
	     P_dummy = tau*nu_des*exp(-beta*deltaE);
	     //printf("method %d sub %f curr %f deltaE %f beta %f P_dummy %f\n",method,currentsub_cell->E,current_cell->E,deltaE,beta,P_dummy);
	     if (P_dummy > P_max)   
		{
		P_max = P_dummy;
		type = 5;
		}
	     }
	  break;
      
      case 6:

	  // Diffusion.  Surface to subsurface:
	  if (!currentsub_cell->Alive())
	     {
	     if ( dE_subtst > currentsub_cell->E - current_cell->E - dE_current)
		{
		deltaE = dE_subtst;
		}
	     else
		{
		deltaE = currentsub_cell->E - current_cell->E - dE_current;
		// Subtract dE_current, since it has been added in currentsub_cell->E
		}
	     beta = 2.0 / ((current_cell->T + currentsub_cell->T)* k_B );
	     P_dummy = tau*nu_diff*exp(-beta*deltaE);
	     //printf("method %d sub %f curr %f deltaE %f beta %f P_dummy %f\n",method,currentsub_cell->E,current_cell->E,deltaE,beta,P_dummy);
	     if (P_dummy > P_max)   
		{
		P_max = P_dummy;
		type = 6;
		}
	     }
	  break;

      case 7:
	  grid->QueryNeighbours(cell_index,2,neigh2_array);
	  // Loop over next nearest neighbours (neigh2's):
	  for (neigh2_index = 0; neigh2_index < neigh2_array.GetSize(); neigh2_index++)
	     {
	     if (neigh2_array.array[neigh2_index] == NoCell)
		{
		continue; // Edge effect
		}
	     neigh2_cell = (ThermoCell *) cell[neigh2_array.array[neigh2_index]];
	     
	     if (! neigh2_cell->Alive())
		{
		// First calculate the adsorption energy of the 2nd neighbour
		grid->QueryGridPosition(neigh2_array.array[neigh2_index],row,col);  
		neigh2sub_cell = (ThermoCell *) subcell[neigh2_array.array[neigh2_index]];
		neigh2_cell->E = AdsorptionEnergy(CountAlive(neigh2_array.array[neigh2_index],1), CountAlive(neigh2_array.array[neigh2_index],2), CountAlive(neigh2_array.array[neigh2_index],3), CountAlive(neigh2_array.array[neigh2_index],4), CountAlive(neigh2_array.array[neigh2_index],5), neigh2sub_cell->Alive(),CountAliveSub(neigh2_array.array[neigh2_index],1),CountAliveSub(neigh2_array.array[neigh2_index],2),CountAliveSub(neigh2_array.array[neigh2_index],3),CountAliveSub(neigh2_array.array[neigh2_index],4), CountAliveSub(neigh2_array.array[neigh2_index],5), (row-col)%2 );
		
		// Diffusion.  Current to neigh2: wait...
		deltaE = neigh2_cell->E - current_cell->E;
		beta = 2.0 / ((current_cell->T + neigh2_cell->T)* k_B );
		P_dummy = exp(-beta*deltaE);
		if (P_dummy > P_max)   
		   {
		   P_max = P_dummy;
		   type = 7;
		   favourite = neigh2_index;
		   }
		}
	     }
	  break;
      } // End switch
   

   // The roulette is playing:
   //   if (type!=0) {printf("     Alive%d deltaE %f   beta %f  Pmax %e\n",method,deltaE,beta,P_max);}

   if (P_max > random)
     {
     switch (type)
       {

       case 0:
	 {
	 return;
	 }

       case 1: // Desorption, current and neigh1
	 {
	 current_cell->SetAlive(False);
	 current_cell->T -= T_kin;
	 neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	 neigh_cell->SetAlive(False);
	 neigh_cell->T -= T_kin;
	 Draw(cell_index);
	 Draw(neigh_array.array[favourite]);
         NESCAPED++;
	 return;
	 }

       case 2:// Desorption, current and neigh1sub
	 {
	 neighsub_cell = (ThermoCell *) subcell[neigh_array.array[favourite]];
	 neighsub_cell->SetAlive(False);
	 neighsub_cell->T -= T_kin;
	 current_cell->SetAlive(False);
	 current_cell->T -= T_kin;
	 Draw(cell_index);
	 Draw(neigh_array.array[favourite]);
         NESCAPED++;
	 return;
	 }

       case 3:// Diffusion, current to neigh1
	 {
	 neigh_cell = (ThermoCell *) cell[neigh_array.array[favourite]];
	 neigh_cell->SetAlive(True);
	 current_cell->SetAlive(False);
	 Draw(cell_index);
	 Draw(neigh_array.array[favourite]);
	 return;
	 }

       case 4:// Desorption, current and neigh2
	 {
	 current_cell->SetAlive(False);
	 current_cell->T -= T_kin;
	 neigh2_cell = (ThermoCell *) cell[neigh2_array.array[favourite]];
	 neigh2_cell->SetAlive(False);
	 neigh2_cell->T -= T_kin;
	 Draw(cell_index);
	 Draw(neigh2_array.array[favourite]);
         NESCAPED++;
	 return;
	 }


       case 5:// Desorption, current and currentsub
	 {
	 current_cell->SetAlive(False);
	 current_cell->T -= T_kin;
	 currentsub_cell->SetAlive(False);
	 currentsub_cell->T -= T_kin;
	 Draw(cell_index);
         NESCAPED++;
	 return;
	 }

       case 6:// Diffusion, current to currentsub
	 {
	 current_cell->SetAlive(False);
	 currentsub_cell->SetAlive(True);
	 Draw(cell_index);
	 return;
	 }


       case 7:// Diffusion, current to neigh2
	 {
	 neigh2_cell = (ThermoCell *) cell[neigh2_array.array[favourite]];
	 neigh2_cell->SetAlive(True);
	 current_cell->SetAlive(False);
	 Draw(cell_index);
	 Draw(neigh2_array.array[favourite]);
	 return;
	 }
       }  // End switch
     }
   }  // End if (current_cell->Alive())
  

if (currentsub_cell->Alive())
   { 
   // Diffusion in the subsurface layer, in a triangular grid only.
   // Second neighbours are not forbidden:
   P_max = 0.0;
   favourite = cell_index;
   grid->QueryNeighbours(cell_index,2,neighsub_array);
   for (neighsub_index = 0; neighsub_index < neighsub_array.GetSize(); neighsub_index++)
      {
      if (neighsub_array.array[neighsub_index] == NoCell)
         {
	 continue; // Edge effect
	 }
      neighsub_cell = (ThermoCell *) subcell[neighsub_array.array[neighsub_index]];

      if (neighsub_cell->Alive())
	 {
	 continue;
	 }
      // Calculate the energy of neighsub:
      neighsub_cell->E = AdsorptionEnergySub(current_alive, CountAlive(neighsub_array.array[neighsub_index],1), CountAlive(neighsub_array.array[neighsub_index],2), CountAlive(neighsub_array.array[neighsub_index],3), CountAlive(neighsub_array.array[neighsub_index],4), CountAlive(neighsub_array.array[neighsub_index],5), CountAliveSub(neighsub_array.array[neighsub_index],2), CountAliveSub(neighsub_array.array[neighsub_index],5));

      // Diffusion from currentsub to neighsub:
      deltaE = dE_bulktst;
      beta = 2.0 / ((currentsub_cell->T + neighsub_cell->T)* k_B );
      P_dummy = exp(-beta*deltaE);
      if (P_dummy > P_max)   
	 {
	 P_max = P_dummy;
	 favourite = neighsub_index;
	 }
      } // End loop over neighsub
   
   if ( P_max >= random)
      {
      neighsub_cell = (ThermoCell *) subcell[neighsub_array.array[favourite]];
      currentsub_cell->SetAlive(False);
      neighsub_cell->SetAlive(True);
      neighsub_cell->E = currentsub_cell->E;    // Transfer particle energy
      Draw(cell_index);
      Draw(neighsub_array.array[favourite]);
      }
   } // End if (currentsub_cell->Alive())
      
}

/*****************************************************************************/

void Palladium::ThermalLatticeConduction(ThermoCell *current_cell, index_array neighbours)

{ int i;
  double T_new, ensemble;
  ThermoCell *neigh_cell;
  
  T_new = current_cell->T;
  ensemble = 1.0;

 for (i = 0; i < neighbours.GetSize(); i++)
    {
    if (neighbours.array[i] != NoCell)
       {
       neigh_cell = (ThermoCell *) cell[neighbours.array[i]];
       T_new += sigma * neigh_cell->T;
       ensemble += sigma;
       }
    }

 T_new = T_new / ensemble;
 current_cell->T = T_new;

 for (i = 0; i < neighbours.GetSize(); i++)
    {
    if (neighbours.array[i] != NoCell)
       {
       neigh_cell = (ThermoCell *) cell[neighbours.array[i]];
       neigh_cell->T = T_new;
       }
    }
}

/*****************************************************************************/

void Palladium::CheckSymmetries()

{ const int symmetries = 26;
  const double scale = 1.0;
  int cell_index,i,r,c,delta11,delta33,n0,n1,biggest=0, degeneracy;
  double N[symmetries], S[symmetries], nalive = 0.0, pbiggest=0.0;
  CATriangularGrid *grid = (CATriangularGrid*) environ;
  bool symmetrical, alive;
  double p = 0, prenorm;
  ThermoCell *current;

for (i = 0; i < symmetries; i++)
   {
   N[i] = 0;
   S[i] = 0;
   }

degeneracy = 0;
  
for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell*) cell[cell_index];
   alive = current->Alive();   
   if (alive)
      {
      nalive++;
      }
   }
 
for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell*) cell[cell_index];
    
   grid->QueryGridPosition(cell_index,r,c);
    
   delta11 = abs((r-c)%2);
   delta33 = (cell_index + (r%2)*3)%6;
   
   for (i = 0; i < symmetries; i++)
      {
      switch(i)
	 {
	 case 0: // 1x1 symmetry
	     n0=0;
	     symmetrical = (delta11 == n0);
	     break;

	 case 1: // 1x1 symmetry
	     n0=1;
	     symmetrical = (delta11 == n0);
	     break;

	 case 2: // 3x3, 1/3
	     n0 = 0;
	     symmetrical = (delta33 == (n0)); //n0=(0,1) n1=(0,2)
	     break;

	 case 3: // 3x3, 1/3
	     n0 = 1;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 4: // 3x3, 1/3
	     n0 = 2;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 5: // 3x3, 1/3

	     n0 = 3;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 6: // 3x3, 1/3
	     n0 = 4;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 7: // 3x3, 1/3
	     n0 = 5;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 8: // 3x3, 2/3
	     n0 = 0;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;
	 case 9: // 3x3, 2/3
	     n0 = 1;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 10: // 3x3, 2/3
	     n0 = 2;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 11: // 3x3, 2/3
	     n0 = 3;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 12: // 3x3, 2/3

	     n0 = 4;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 13: // 3x3, 2/3
	     n0 = 5;
	     symmetrical =  ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 14: // 2x2, 1/2
	     n0 = 0;
	     symmetrical =  ((((r)%2 == 0 ) && (abs((r+n0-c)%4) == 0))||
		 (((r+1)%2 == 0 ) && (abs((r+n0-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;

	 case 15: // 2x2, 1/2
	     n0 = 1;
	     symmetrical = ((((r)%2 == 0 ) && (abs((r+n0-c)%4) == 0))||
		 (((r+1)%2 == 0 ) && (abs((r+n0-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;

	 case 16: // 2x2, 1/2
	     n0 = 2;
	     symmetrical =  ((((r)%2 == 0 ) && (abs((r+n0-c)%4) == 0))||
		 (((r+1)%2 == 0 ) && (abs((r+n0-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;

	 case 17: // 2x2, 1/2
	     n0 = 3;
	     symmetrical =  ((((r)%2 == 0 ) && (abs((r+n0-c)%4) == 0))||
		 (((r+1)%2 == 0 ) && (abs((r+n0-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;

	 case 18: // 2x2, 1/4
	     n0 = 0;
	     n1 = 0;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 19: // 2x2, 1/4
	     n0 = 0;
	     n1 = 1;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 20: // 2x2, 1/4
	     n0 = 0;
	     n1 = 2;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 21: // 2x2, 1/4
	     n0 = 0;
	     n1 = 3;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 22: // 2x2, 1/4
	     n0 = 1;
	     n1 = 0;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 23: // 2x2, 1/4
	     n0 = 1;
	     n1 = 1;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 24: // 2x2, 1/4
	     n0 = 1;
	     n1 = 2;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 25: // 2x2, 1/4
	     n0 = 1;
	     n1 = 3;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;
	 } //end switch

      alive = current->Alive();

      if (symmetrical && alive)
	 {
	 N[i]++;
	 }
      } //end for(i= 0; i < symmetries; i++)
   }


 ENTROPY = 0.0; 

 // 1x1-symmetry:
 
 for (i = 0; i < 2; i++)
    {
    p = (double)N[i]/nalive;
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    ENTROPY -= p*log(p)/scale;
    if (p > pbiggest)
       {
       biggest = i;
       pbiggest = p;
       }
    else if (p == pbiggest)
       {
       degeneracy++;
       }
    }
 prenorm = 2.0*(pbiggest-0.5); // Gives P = 0 for random distribution and P = 1 for all particles belonging to a certain symmetry.
 
 if ((int)ITERATIONS % NOUTPUT == 0)
    {
      visual->Printf(8,"1x1:                 %f",prenorm);
    }
 pbiggest = 0;


 // sq3xsq3-symmetry:
 
 for (i = 2; i < 8; i++)
    {
    p = (double)N[i]/nalive;
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    ENTROPY -= p*log(p)/scale;
    if (p > pbiggest)
       {
       biggest = i;
       pbiggest = p;
       }
    else if (p == pbiggest)
       {
       degeneracy++;
       }
    }
 prenorm = 0.2*(6.0*pbiggest-1.0); // Gives P = 0 for random distribution and P = 1 for all particles belonging to a certain symmetry.
 if ((int)ITERATIONS % NOUTPUT == 0)
    {
      visual->Printf(9,"sq3xsq3(1/3): %f",prenorm);
    }
 pbiggest = 0;

 // sq3xsq3-symmetry:
 
 for (i = 8; i < 14; i++)
    {
    p = (double)N[i]/nalive;
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    ENTROPY -= p*log(p)/scale;
    if (p > pbiggest)
       {
       biggest = i;
       pbiggest = p;
       }
    else if (p == pbiggest)
       {
       degeneracy++;
       }
    }
 prenorm = 0.5*(3.0*pbiggest-1.0); // Gives P = 0 for random distribution and P = 1 for all particles belonging to a certain symmetry.
 if ((int)ITERATIONS % NOUTPUT == 0)
    {
      visual->Printf(10,"sq3xsq3(2/3): %f",prenorm);
    }
 pbiggest = 0;


 // 2x2-symmetry:
 
 for (i = 14; i < 18; i++)
    {
    p = (double)N[i]/nalive;
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    ENTROPY -= p*log(p)/scale;
    if (p > pbiggest)
       {
       biggest = i;
       pbiggest = p;
       }
    else if (p == pbiggest)
       {
       degeneracy++;
       }
    }
 prenorm = (4.0*pbiggest-1.0)/3.0; // Gives P = 0 for random distribution and P = 1 for all particles belonging to a certain symmetry.
 if ((int)ITERATIONS % NOUTPUT == 0)
    {
      visual->Printf(11,"2x2(1/2):          %f",prenorm);
    }
 pbiggest = 0;


 // 2x2-symmetry:
 
 for (i = 18; i < 26; i++)
    {
    p = (double)N[i]/nalive;
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    ENTROPY -= p*log(p)/scale;
    if (p > pbiggest)
       {
       biggest = i;
       pbiggest = p;
       }
    else if (p == pbiggest)
       {
       degeneracy++;
       }
    }
 prenorm = (8.0*pbiggest-1.0)/7.0; // Gives P = 0 for random distribution and P = 1 for all particles belonging to a certain symmetry.
 if ((int)ITERATIONS % NOUTPUT == 0)
    {
      visual->Printf(12,"2x2(1/4):          %f",prenorm);
    }
 pbiggest = 0;


}


/*****************************************************************************/

void Palladium::FindPattern()

    // Look for periodical structures in the lattice
    // The main problem here is that overlapping symmetries are possible.
    // One might imagine that the best way to look for symmetries on the
    // lattice would be to search along independent basis vectors for
    // periodicity, but this will not work, since it will simply overwrite
    // the recognized regions along linearly independent basis vectors.
    // Instead we have to search on multiple hierachies, but still
    // linearly. I.e. we think of the 2d lattice as structures within
    // structures and parse accordingly, knowing the lattice size.
    // Thus all symmetric structures in the lattice are one dimensional
    // periodic structures also, but those which are separated by more
    // than the cutoff should not be marked contiguously because they belong
    // to an orthogonal direction.
    
{ int cell_index, i, test_period, string_length, delta, stepsize, r, c;
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
      SearchPeriod(cell_index,test_period,string_length,2);
      //Use stepsize 2 to find patterns with periods 2, 4, and 6.
      if (test_period > 0 && string_length > 2)
	 {
	 // Colour the string as far as it goes. The formula is
	 // from cell_index to cell_index+(string_length-1)*test_period+1
	 // in order to not colour empty spaces at the end
         // Use 2*test_period, since stepsize = 2 above.

	 MarkRegion(cell_index,2*test_period,string_length,1);
	 }

      // Now get sliced periods of order of lattice size within +/- delta/3
      // grid->cols is the lattice width. Delta finds diagonal strings
      // grid->cols+delta is now the test period and we can add to this
      // multiples of the lattice period.

      //For palladium, we only want delta = +-1.
      //for (delta = 0; delta < 3; delta++)

      delta = 1;
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

// Pattern 3 search by looking for empty rings of level 2 neighbours

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   index_array neighbours;
   current = (Particle *) cell[cell_index];
   const double pattern3 = 15.0;

   if (current->Alive())
      {
      continue;
      }

   if (CountAlive(cell_index,1) > 0)
      {
      continue;
      }

   if (CountAlive(cell_index,2) < 5)  // Arbitrary, 6 would be a complete symmetry ring
      {
      continue;
      }

   current->state = pattern3;         // Now fill in the colours

   grid->QueryNeighbours(cell_index,1,neighbours);
   
   for (i = 0; i < neighbours.GetSize(); i++) 
     {
     ThermoCell *neigh_cell = (ThermoCell *) cell[neighbours.array[i]];
     neigh_cell->state = pattern3;
     }

   grid->QueryNeighbours(cell_index,2,neighbours);
   
   for (i = 0; i < neighbours.GetSize(); i++) 
      {
      ThermoCell *neigh_cell = (ThermoCell *) cell[neighbours.array[i]];
      
      if (neigh_cell->Alive())
	 {
	 neigh_cell->state = pattern3;
	 }
      }
   }
}


/*****************************************************************************/

void Palladium::LoadState()

{ int cell_index, alive, forbidden, ncells, nescaped;
  double T,E,iterations,state;
  FILE *fp;
  ThermoCell *current, *next;

if ((fp = fopen("CASE-Palladium-Sim.bak","r")) == NULL)
   {
   printf("Couldn't load state from file CASE-Palladium-Sim.bak\n");
   return;
   }

fscanf(fp,"%d",&ncells);
fscanf(fp,"%lf",&iterations);
fscanf(fp,"%d",&nescaped);

if (cells!=ncells)
  {
    AllocateCells(ncells);
  }
ITERATIONS = iterations;
NESCAPED = nescaped;

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell *) cell[cell_index];

   alive = 0;
   forbidden = 0;
   T = E = state = 0.0;

   fscanf(fp,"%d",&alive);
   fscanf(fp,"%d",&forbidden);
   fscanf(fp,"%lf",&T);
   fscanf(fp,"%lf",&E);
   fscanf(fp,"%lf",&state);

   current->SetAlive(alive);
   current->SetForbidden(forbidden);
   current->E = E;
   current->T = T;
   current->state = state;
   } 

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell *) subcell[cell_index];

   alive = 0;
   forbidden = 0;
   T = E = state = 0.0;

   fscanf(fp,"%d",&alive);
   fscanf(fp,"%d",&forbidden);
   fscanf(fp,"%lf",&T);
   fscanf(fp,"%lf",&E);
   fscanf(fp,"%lf",&state);

   current->SetAlive(alive);
   current->SetForbidden(forbidden);
   current->E = E;
   current->T = T;
   current->state = state;
   } 


fclose(fp);
printf("Succeeded in loading state from disk\n");
}

/*****************************************************************************/

void Palladium::SaveState()

{ int cell_index;
  FILE *fp;
  ThermoCell *current, *next;

if ((fp = fopen("CASE-Palladium-Sim.bak","w")) == NULL)
   {
   printf("Couldn't save state to CASE-Palladium-Sim.bak\n");
   return;
   }

printf("Saving state...\n");

fprintf(fp,"%d\n",cells);
fprintf(fp,"%lf\n",ITERATIONS);
fprintf(fp,"%d\n\n",NESCAPED);

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell *) cell[cell_index];

   fprintf(fp,"%d\n",current->Alive());
   fprintf(fp,"%d\n",current->Forbidden());
   fprintf(fp,"%f\n",current->T);
   fprintf(fp,"%f\n",current->E);
   fprintf(fp,"%f\n",current->state);
   } 

for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell *) subcell[cell_index];

   fprintf(fp,"%d\n",current->Alive());
   fprintf(fp,"%d\n",current->Forbidden());
   fprintf(fp,"%f\n",current->T);
   fprintf(fp,"%f\n",current->E);
   fprintf(fp,"%f\n",current->state);
   } 

fclose(fp);
}

/*****************************************************************************/

void Palladium::SearchPeriod(int cell_index, int &test_period, int &string_length, int stepsize)

{ unsigned int i,j,k;
  Particle *next;
  const int cutoff = 3; // Arbitrary, this chosen for Palladium
  
  // Look for next cell to determine a possible period length. 
  // For now, only search within rows

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

 // Search for pattern with alternating period (1 2 1 2, etc...)

 if (test_period == 1 && string_length < 3)
    {
    k=0;
    for (i = cell_index; i < cells; i+= test_period*stepsize*(2-(k%2)))
      {
      next = (Particle *) cell[i];
      if (!next->Alive())
	 {
	 test_period = 3;
	 string_length= (int) (string_length-1)/2;
	 break;
	 }
      string_length++;
      k++;
      } 
    }

}

/*****************************************************************************/

void Palladium::MarkRegion(int cell_index, int test_period, int string_length, int step)

{ int i,row,col;
  Particle *current;

 for (i = cell_index; (i < cell_index+((string_length-1)*test_period*step)) && (i < cells); i+=step)
    {
    current = (Particle *) cell[i];
    
    if (step == 1)
       {
       current->state = (double) test_period/2;
       }
    else
       {
       current->state = (double) test_period;
       }

    grid->QueryGridPosition(i,row,col);  
    // Also mark cells between when searching vertically:
    if ((step == grid->cols+1) && (i < cells-1) && (i < cell_index+((string_length-1)*test_period*step)))
       {
       if ((row - col) % 2 == 0)
	  {
	  current = (Particle *) cell[i+1];
	  }
       else
	  {
	  current = (Particle *) cell[(i+step-1)%cells];
	  }

       current->state = (double) test_period;
       }
    
    if ((step == grid->cols-1) && (i < cells) && (i < cell_index+((string_length-1)*test_period*step)))
       {
       if ((row - col) % 2 == 0)
	  {
	    if (i!=0)
	      {
	      current = (Particle *) cell[i-1];
	      }
	    else
	      {
	      current = (Particle *) cell[cells-1];
	      }
	  }
       else
	  {
	  current = (Particle *) cell[(i+step+1) % cells];
	  }
       current->state = (double) test_period;
       }
    } 
}


/*****************************************************************************/

int Palladium::SColour(double value)

{ const double no_of_colours = 8.0;
return (6 + (int) value);
// return (4+(int)(0.5*(value)+1.5));
}

/*****************************************************************************/

int Palladium::EColour(double value)

// Colours 11 to 19
    
{ const double no_of_colours = 8.0;

return 0;
}

/*****************************************************************************/

int Palladium::EColourSub(double value)

// Colours 21 to 29
    
{ const double no_of_colours = 8.0;

return 12;
}

/*****************************************************************************/
// Level 3
/*****************************************************************************/

double Palladium::WorkFunction(ThermoCell *x, ThermoCell *xprime, int c, int cprime)

{ double work;
   
return 1.0;
   work = alpha * sqrt(0.5*(x->T,xprime->T)) + m * fabs(c-cprime) / cells;

 return work;
}

/*****************************************************************************/

int Palladium::CountAlive(int cell_index, int level)

// How many neighbours are alive?

{ int i, neighboursum = 0, symfactor = 0;
  ThermoCell *neigh_cell;
  index_array neighbours;

grid->QueryNeighbours(cell_index,level,neighbours);

for (i = 0; i < neighbours.GetSize(); i++) 
   {
   neigh_cell = (ThermoCell *) cell[neighbours.array[i]];

   if (neigh_cell->Alive())
      {
      neighboursum++;
      }
   }

if (level == 2 && neighboursum == 3) // Check symmetry of the neighbours
   {
   for (i = 0; i < neighbours.GetSize(); i++) 
      {
      neigh_cell = (ThermoCell *) cell[neighbours.array[i]];
      if (neigh_cell->Alive() && i%2 == 0)
	 {
	 symfactor++;
	 }
      }
   if (symfactor == 0 || symfactor == 3)
      {
      neighboursum == 7;
      }
   } // End if (level == 2 && neighboursum == 3)

return neighboursum;
}

/*****************************************************************************/

int Palladium::CountAliveSub(int cell_index, int level)

// How many subsurface neighbours are alive?

{ int i, neighboursum = 0;
  ThermoCell *neighsub_cell;
  index_array neighbours;

grid->QueryNeighbours(cell_index,level,neighbours);

for (i = 0; i < neighbours.GetSize(); i++) 
   {
   neighsub_cell = (ThermoCell *) subcell[neighbours.array[i]];

   if (neighsub_cell->Alive())
      {
      neighboursum++;
      }
   }

return neighboursum;
}

/*****************************************************************************/

double Palladium::AdsorptionEnergy(int neigh1_alive, int neigh2_alive, int neigh3_alive, int neigh4_alive, int neigh5_alive, int currentsub_alive, int neigh1sub_alive, int neigh2sub_alive, int neigh3sub_alive, int neigh4sub_alive, int neigh5sub_alive, int fcc)
  //if fcc = 0, this is a hcp site
{ double energy;
   
if (fcc == 0)
   {
   energy = E_hcp;
   }
else
   {
   energy = E_fcc;
   }

// Suppose for now that the energy is linearly dependent of the number of neighbours:

energy += neigh1_alive*dE_neigh1 + neigh2_alive*dE_neigh2 + neigh3_alive*dE_neigh3 + neigh4_alive*dE_neigh4 + neigh5_alive*dE_neigh5 + currentsub_alive*dE_current + neigh1sub_alive*dE_neigh1sub + neigh2sub_alive*dE_neigh2sub + neigh3sub_alive*dE_neigh3sub + neigh4sub_alive*dE_neigh4sub + neigh5sub_alive*dE_neigh5sub;

// Exception: theta = 2/3.
if (neigh2_alive == 3) // Every second 2nd neighbour is alive (from QueryNeighbours)
  {
  energy -= 3*dE_neigh2 - dE_neigh2sym;
  }

return energy;
}

/*****************************************************************************/

double Palladium::AdsorptionEnergySub(int current_alive, int neigh1_alive, int neigh2_alive, int neigh3_alive, int neigh4_alive, int neigh5_alive, int subalive2, int subalive5)
  //if current_alive = 0, the current cell is not occupied.
{ double energy;
   
// Suppose for now that the energy is linearly dependent of the number of neighbours:

energy = E_sub + current_alive*dE_current + neigh1_alive*dE_neigh1 + neigh2_alive*dE_neigh2 + neigh3_alive*dE_neigh3 + neigh4_alive*dE_neigh4 + neigh5_alive*dE_neigh5 + subalive2*dE_neigh2sub + subalive5*dE_neigh5sub;

// Exception: theta = 2/3.
if (neigh2_alive == 3)
  {
  energy -= 6*dE_neigh2;
  }

return energy;
}

/*****************************************************************************/

double Palladium::BridgeEnergy(int current_neigh1_alive, int current_neigh2_alive, int neigh_neigh1_alive, int neigh_neigh2_alive)
{ double energy;

// Suppose for now that the energy is linearly dependent of the number of neighbours:

 energy = E_brg + current_neigh1_alive*dE_brgn1 + neigh_neigh1_alive*dE_brgn1 + current_neigh2_alive*dE_brgn2 + neigh_neigh2_alive*dE_brgn2;

return energy;
}
