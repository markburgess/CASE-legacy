/*****************************************************************************/
/*                                                                           */
/* DiffModel.C: Program file for CAModel object.                             */
/*                                                                           */
/*****************************************************************************/

/* $Id: DiffModel.C,v 1.3 1997/10/9 15:30:17 olem Exp $ */

#include <DiffModel.h>

  // This model currently relies on the special properties of the
  // triangular grid, and will not work in rectangular grids.

   const double Tscale_fudge = 3.0; // Overall temperature scale for animation rate

   const double T_kin = 2.0;         // Change in T due to kinetic energy
   const double T_exo = 4.0;         // Change in T due to exothermic breakup

   const double T_equ = 1.1;         // Ambient equilibrium temperature
				     
   const double TMax = 7.0;          // These decide the colour scale
   const double EMax = 7.0;

   const double P_ads0 = 0.001; // Probability of adsorption with 0 neighbours
   const double P_ads1 = 0.0005; // Probability of adsorption with 1 neighbour...
   const double P_ads2 = 0.0001; 
   const double P_ads3 = 0.0; 
   const double P_ads4 = 0.0; 
   const double P_adsnn0 = 1.0; // The change in probability with 0 next neighbours...
   const double P_adsnn1 = 0.9; 
   const double P_adsnn2 = 0.8; 
   const double P_adsnn3 = 0.7; 
   const double P_adsnn4 = 0.6; 
   const double P_adsnn5 = 0.5; 
   const double P_adsnn6 = 0.4; 
   const double P_adsnn7 = 0.3; 
   const double P_adsnn8 = 0.2; 
   const double P_adsnn9 = 0.1; 
   const double P_adsnn10 = 0.0; 
   const double P_adsnn11 = 0.0; 
   const double P_adsnn12 = 0.0; 
   const double P_diff00 = 1.0;  // Relative probabilities of diffusion from
   const double P_diff01 = 0.1;  // a site with 0 neighbours to one with 1,
   const double P_diff02 = 0.0;  // from 0 to 2, ...
   const double P_diff10 = 3.0;
   const double P_diff20 = 10.0;
   const double P_diff30 = 100.0;
   const double P_diffnn6= 4.0; // The change in probability with 6 
   const double P_diffnn5= 32.0; // fewer next neighbours...
   const double P_diffnn4= 16.0; 
   const double P_diffnn3= 8.0; 
   const double P_diffnn2= 4.0; 
   const double P_diffnn1= 2.0; 
   const double P_diffnn0= 1.0; 
   const double P_diffnn_1= 0.5; 
   const double P_diffnn_2= 0.1; 
   const double P_diffnn_3= 0.05; 
   const double P_diffnn_4 = 0.01; 
   const double P_diffnn_5 = 0.0; 
   const double P_diffnn_6 = 0.0; 
   const double P_loss = 0.001;
   const double P_ss = 0.001;

   const double m = 1.0;            // Mass of intrusive particles
   const double sigma = 0.5;        // Thermal conductivity & heat capacity
   const double alpha = 10.0;       // Thermal barrier height
   const double exo = 1.0;          // Exothermic energy added by one arriving particle

/*****************************************************************************/
/* Diffuse object:                                                           */
/*****************************************************************************/

Diffuse::Diffuse(Widget new_parent, CASymmetry Environment):ParticleModel(new_parent,Environment)
    
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

switch (Environment)
   {
   case CATriangular:  subenviron = subgrid = new CATriangularGrid(0,0,8,8,24,24,60);
                       break;

   case CARectangular: subenviron = subgrid = new CARectangularGrid;
   }

symbol = new CABackgroundedCircle(visual, environ);
subsymbol = new CABackgroundedCircle(visual, environ);

subcell = NULL;
 
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

visual->Print(1,"Thermolattice diffusion");
visual->Print(2,"(Local temperature variation)"); 
visual->Print(3,"Surface: Subsurface: Escaped:");

visual->Print(5,"Average temperature:");

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


entropy = iterations = 0.0; 

Redraw();
}

/*****************************************************************************/

Diffuse::~Diffuse()

{
}

/*****************************************************************************/

void Diffuse::ReadEnvironRequester()

{ CAModel::ReadEnvironRequester();

if (cells != grid->GetNumberOfCells())
   {
   AllocateCells(grid->GetNumberOfCells());
   }

Redraw();
}


/*****************************************************************************/

void Diffuse::Draw(int cell_index)
    
{ ThermoCell *current_cell = (ThermoCell*) cell[cell_index];
  ThermoCell *currentsub_cell = (ThermoCell*) subcell[cell_index];

if (currentsub_cell->Alive())
   {
   if (current_cell->Alive())
      {
      subsymbol->DrawBigSmall(cell_index,TColour(currentsub_cell->T),EColour(current_cell->E),EColourSub(currentsub_cell->E));
      }
   else
      {
      subsymbol->DrawBig(cell_index,TColour(currentsub_cell->T),EColourSub(currentsub_cell->E));
      }
   }
else if (current_cell->Alive())
   {
   symbol->Draw(cell_index,TColour(current_cell->T),EColour(current_cell->E));
   }
else
   {
   symbol->Draw(cell_index,TColour(current_cell->T),CANoSymbol);   
   }
}

/*****************************************************************************/

void Diffuse::Update()
    
{ int i, nalive, nalivesub, cell_index;
  ThermoCell *current_cell, *currentsub_cell;
  int count[cells];
  index_array neighbours;

 nalive = 0;
 nalivesub = 0;
 nescaped = 0;

 iterations++;
 
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
    ThermalLatticeConduction(current_cell,neighbours);

    // TLC seems to trash nrighbuors on return... WHY!
    grid->QueryNeighbours(i, 1, neighbours);
    ThermalLatticeConduction(currentsub_cell,neighbours);
    
    Draw(i);
    } 

 visual->Printf(4,"%d     %d     %d", nalive,nalivesub,nescaped);
 visual->Printf(6, "%f",Taverage/(double)cells);

 if (iterations > 10)
    {
    CheckSymmetries();
    }

 environ->DrawGrid(visual);
 visual->CurrentPageFront(); 
}

/*****************************************************************************/

void Diffuse::DblClick(double x, double y)

{
}

/*****************************************************************************/

void Diffuse::Click(double x, double y)

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

void Diffuse::AllocateCells(int new_cells)

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
    current_cell->T = T_equ;
    current_cell->SetAlive(False);

    subcell[i] = new ThermoCell;
    currentsub_cell = (ThermoCell *)subcell[i];
    currentsub_cell->E = 0;
    currentsub_cell->T = T_equ;
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

void Diffuse::DoFluctuations(int cell_index)

{ double P_diff, P_ads, P_sstos, P_stoss, P_ssdiff;
  int favourite = cell_index, neigh_index, neighsub_index;
  double random = drand48(), P_diff_T, highest = 0.0, beta;
  int neigh_count, alive1, alive2, alive3, alivesub1, alivesub2;
  ThermoCell *current_cell, *neigh_cell, *currentsub_cell, *neighsub_cell;
  index_array neigh_array, neighsub_array;
  
current_cell = (ThermoCell *) cell[cell_index];
currentsub_cell = (ThermoCell *) subcell[cell_index];

// Find occupied cell neighbours

alive1 = CountAlive(cell_index,1);
alive2 = CountAlive(cell_index,2);
alivesub2 = CountAliveSub(cell_index,2); //We only need second neighbours; first neighbours are forbidden.

// Find number of alive neighbours of any neighbour for later

// Hard-code neighbour sizes for now

neigh_count = 3; //   grid->CountNeighbours(cell_index,1);

grid->QueryNeighbours(cell_index,1,neigh_array);

int neigh_neighbours1[3]; 
int neigh_neighbours2[6];
int neigh_neighbours3[3];

for (neigh_index = 0; neigh_index < 3; neigh_index++)
   {
   neigh_neighbours1[neigh_index] = CountAlive(neigh_array.array[neigh_index],1);
   neigh_neighbours3[neigh_index] = CountAlive(neigh_array.array[neigh_index],3);
   }

grid->QueryNeighbours(cell_index,2,neigh_array);

for (neigh_index=0; neigh_index < 6; neigh_index++)
   {
   neigh_neighbours2[neigh_index] = CountAlive(neigh_array.array[neigh_index],2);   
   }

// Now we can start... 

if (currentsub_cell->Alive())
   { 
   //Diffusion in the subsurface layer, in a triangular grid only
   //second neighbours are not forbidden:
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
         
      beta = Tscale_fudge / (currentsub_cell->T + neighsub_cell->T);
      P_diff_T = exp(-beta*WorkFunction(neighsub_cell,currentsub_cell,cell_index,neighsub_index));
   
      switch (alivesub2 - CountAliveSub(neighsub_index,2)) //Change in number of next neighbours
	{
	case -6 : P_diff = P_diff_T*P_diffnn_6; break;
	case -5 : P_diff = P_diff_T*P_diffnn_5; break;
	case -4 : P_diff = P_diff_T*P_diffnn_4; break;
	case -3 : P_diff = P_diff_T*P_diffnn_3; break;
	case -2 : P_diff = P_diff_T*P_diffnn_2; break;
	case -1 : P_diff = P_diff_T*P_diffnn_1; break;
	case  0 : P_diff = P_diff_T*P_diffnn0; break;
	case  1 : P_diff = P_diff_T*P_diffnn1; break;
	case  2 : P_diff = P_diff_T*P_diffnn2; break;
	case  3 : P_diff = P_diff_T*P_diffnn3; break;
	case  4 : P_diff = P_diff_T*P_diffnn4; break;
	case  5 : P_diff = P_diff_T*P_diffnn5; break;
	case  6 : P_diff = P_diff_T*P_diffnn6; break;
	}
      neigh_count = 0;
      // The subsurface diffusion is dependent on the nearest surface neighbours:
      if (current_cell->Alive())
	 {
	 neigh_count += 1;
	 }
      neigh_cell = (ThermoCell *) cell[neighsub_array.array[neighsub_index]];
      if (neigh_cell->Alive())
	 {
	 neigh_count -= 1;
	 }
      switch (neigh_count) // Change in number of surface neighbours
	{
	case -1 : P_diff = P_diff*P_diff02; // Change these!
	case  0 : P_diff = P_diff*P_diff00;
	case  1 : P_diff = P_diff*P_diff20;
	}

      if (P_diff > highest)     // Find `least action'
	 {
	 highest = P_diff;
	 favourite = neighsub_array.array[neighsub_index];
	 }
      }

   if (highest/10 >= drand48())
      {
      neighsub_cell = (ThermoCell *) subcell[favourite];
      currentsub_cell->SetAlive(False);
      neighsub_cell->SetAlive(True);
      neighsub_cell->E = currentsub_cell->E;    // Transfer particle energy
      Draw(cell_index);
      Draw(favourite);
      }
   }
      

if (! current_cell->Alive())
   {
   // Pick a random neighbour, 0.01 make sure we round down to GetSize-1
   // but not -1 since we want an unbiased distribution of random numbers

   neigh_index = (int)(drand48()*((double)neigh_array.GetSize()-0.01));
   neigh_cell = (ThermoCell *) cell[neigh_array.array[neigh_index]];

   // If two free places, try to adsorb a molecule.  First both on the surface:

   if (! neigh_cell->Alive())
      {
      switch (neigh_neighbours1[neigh_index]+alive1) // Total number of neighbours
	 {
	 case 0 : P_ads = P_ads0; break;
	 case 1 : P_ads = P_ads1; break;
	 case 2 : P_ads = P_ads2; break;
	 case 3 : P_ads = P_ads3; break;
	 case 4 : P_ads = P_ads4; break;
	 }
	
      switch (neigh_neighbours2[neigh_index]+alive2) // Total number of next neighbours
	 {
	 case 0 : P_ads = P_ads*P_adsnn0; break;
	 case 1 : P_ads = P_ads*P_adsnn1; break;
	 case 2 : P_ads = P_ads*P_adsnn2; break;
	 case 3 : P_ads = P_ads*P_adsnn3; break;
	 case 4 : P_ads = P_ads*P_adsnn4; break;
	 case 5 : P_ads = P_ads*P_adsnn5; break;
	 case 6 : P_ads = P_ads*P_adsnn6; break;
	 case 7 : P_ads = P_ads*P_adsnn7; break;
	 case 8 : P_ads = P_ads*P_adsnn8; break;
	 case 9 : P_ads = P_ads*P_adsnn9; break;
	 case 10 : P_ads = P_ads*P_adsnn10; break;
	 case 11 : P_ads = P_ads*P_adsnn11; break;
	 case 12 : P_ads = P_ads*P_adsnn12; break;
// fix
         default:  P_ads = P_ads*P_adsnn12; break;
	 }
            
      if (random <= P_ads)
	 {
	 current_cell->SetAlive(True);
	 current_cell->T += T_exo + T_kin;
	 neigh_cell->SetAlive(True);
	 neigh_cell->T += T_exo + T_kin;
	 Draw(cell_index);
	 Draw(neigh_array.array[neigh_index]);

	 return;
	 }
      // Next one surface and one subsurface:
      if (!currentsub_cell->Alive() && !currentsub_cell->Forbidden())
      {
      switch (neigh_neighbours1[neigh_index]+alive1) // Total number of neighbours
	 {
	 case 0 : P_ads = P_ads0; break; //change these!
	 case 1 : P_ads = P_ads1; break;
	 case 2 : P_ads = P_ads2; break;
	 case 3 : P_ads = P_ads3; break;
	 case 4 : P_ads = P_ads4; break;
	 }
	
      switch (neigh_neighbours2[neigh_index]+alivesub2) //Total number of next neighbours
	 {
	 case 0 : P_ads = P_ads*P_adsnn0; break; //and these!
	 case 1 : P_ads = P_ads*P_adsnn1; break;
	 case 2 : P_ads = P_ads*P_adsnn2; break;
	 case 3 : P_ads = P_ads*P_adsnn3; break;
	 case 4 : P_ads = P_ads*P_adsnn4; break;
	 case 5 : P_ads = P_ads*P_adsnn5; break;
	 case 6 : P_ads = P_ads*P_adsnn6; break;
	 case 7 : P_ads = P_ads*P_adsnn7; break;
	 case 8 : P_ads = P_ads*P_adsnn8; break;
	 case 9 : P_ads = P_ads*P_adsnn9; break;
	 case 10 : P_ads = P_ads*P_adsnn10; break;
	 case 11 : P_ads = P_ads*P_adsnn11; break;
	 case 12 : P_ads = P_ads*P_adsnn12; break;
	 }
            
      if (random <= P_ads)
	 {
	 currentsub_cell->SetAlive(True);
	 currentsub_cell->T += T_exo + T_kin;
	 neigh_cell->SetAlive(True);
	 neigh_cell->T += T_exo + T_kin;
	 Draw(cell_index);
	 Draw(neigh_array.array[neigh_index]);

	 return;
	 }
      }
   } // End if !neighbour_cell->Alive()
   // If the current subsurface site is not occupied, try to adsorb
   // simultaneously on the surface and current subsurface sites
   if (!currentsub_cell->Alive() && !currentsub_cell->Forbidden())
      {
      switch (alive1) // Number of (new) neighbours
	{
	case 0 : P_ads = P_ads1; break;//change ads1 etc!
	case 1 : P_ads = P_ads2; break;
	case 2 : P_ads = P_ads3; break;
	case 3 : P_ads = P_ads4; break;
	}
      switch (alive2) // Number of (new) next neighbours
	{
	case 0 : P_ads = P_ads*P_adsnn0; break;
	case 1 : P_ads = P_ads*P_adsnn1; break;
	case 2 : P_ads = P_ads*P_adsnn2; break;
	case 3 : P_ads = P_ads*P_adsnn3; break;
	case 4 : P_ads = P_ads*P_adsnn4; break;
	case 5 : P_ads = P_ads*P_adsnn5; break;
	case 6 : P_ads = P_ads*P_adsnn6; break;
	}
      // insert switch number of subsurface neighbours!
      if (P_ads >= drand48())
	 {
	 currentsub_cell->SetAlive(True);
	 current_cell->SetAlive(True);
	 currentsub_cell->T += T_exo + T_kin;
	 current_cell->T += T_exo + T_kin;
	 Draw(cell_index);
	 return;
	 }
      }
   // If the subsurface site is occupied, try to get to the surface
   else if (currentsub_cell->Alive())
     {
     switch (alive1) // Number of (new) neighbours
       {
       case 0 : P_sstos = P_ads0; break;//change ads0 etc!
       case 1 : P_sstos = P_ads2; break;
       case 2 : P_sstos = P_ads3; break;
       case 3 : P_sstos = P_ads4; break;
       }
     switch (alive2) // Number of (new) next neighbours
       {
       case 0 : P_sstos = P_sstos*P_adsnn0; break;
       case 1 : P_sstos = P_sstos*P_adsnn1; break;
       case 2 : P_sstos = P_sstos*P_adsnn2; break;
       case 3 : P_sstos = P_sstos*P_adsnn3; break;
       case 4 : P_sstos = P_sstos*P_adsnn4; break;
       case 5 : P_sstos = P_sstos*P_adsnn5; break;
       case 6 : P_sstos = P_sstos*P_adsnn6; break;
       }

     if (P_sstos >= drand48())
        {
	currentsub_cell->SetAlive(False);
	current_cell->SetAlive(True);
	current_cell->E = currentsub_cell->E;    // Transfer particle energy
	Draw(cell_index);
	return;
	}
     }
   return;
   }
  
  
// Now the current_cell is alive.  Will it go subsurface, escape or
// diffuse to a neighbour site?

if (!currentsub_cell->Alive() && !currentsub_cell->Forbidden())
   { //If the subsurface site is not occupied, try to go
     //subsurface (only fcc sites are allowed)
   P_stoss = P_ads0; // As a starter...
   switch (alive1) // Number of neighbours in the surface
     {
     case 0 : P_stoss = P_stoss*P_diff00; break; //change these!
     case 1 : P_stoss = P_stoss*P_diff10; break;
     case 2 : P_stoss = P_stoss*P_diff20; break;
     case 3 : P_stoss = P_stoss*P_diff30; break;
     }
   switch (alive2-alivesub2) // Change in number of second neighbours.
     {
     case -6 : P_stoss = P_stoss*P_diffnn_6; break; //and these!
     case -5 : P_stoss = P_stoss*P_diffnn_5; break;
     case -4 : P_stoss = P_stoss*P_diffnn_4; break;
     case -3 : P_stoss = P_stoss*P_diffnn_3; break;
     case -2 : P_stoss = P_stoss*P_diffnn_2; break;
     case -1 : P_stoss = P_stoss*P_diffnn_1; break;
     case  0 : P_stoss = P_stoss*P_diffnn0; break;
     case  1 : P_stoss = P_stoss*P_diffnn1; break;
     case  2 : P_stoss = P_stoss*P_diffnn2; break;
     case  3 : P_stoss = P_stoss*P_diffnn3; break;
     case  4 : P_stoss = P_stoss*P_diffnn4; break;
     case  5 : P_stoss = P_stoss*P_diffnn5; break;
     case  6 : P_stoss = P_stoss*P_diffnn6; break;
     }
   if (P_stoss >= drand48())
     {
       currentsub_cell->SetAlive(True);
       currentsub_cell->E = current_cell->E;
       current_cell->SetAlive(False);
       Draw(cell_index);
     }
   }

for (neigh_index = 0; neigh_index < neigh_array.GetSize(); neigh_index++)
   {
   if (neigh_array.array[neigh_index] == NoCell)
      {
      continue; // Edge effect
      }

   neigh_cell = (ThermoCell *) cell[neigh_array.array[neigh_index]];

   if (neigh_cell->Alive())  // Associative desorption if enuff energy
      {
      if ((random <= P_loss) && ((current_cell->T+neigh_cell->T)/2 >= T_kin))
	 {
	 current_cell->SetAlive(False);
	 current_cell->T -= T_kin;
	 Draw(cell_index);
	 neigh_cell->SetAlive(False);
	 neigh_cell->T -= T_kin;
	 Draw(neigh_array.array[neigh_index]);
         nescaped++;

	 return;
	 }
      continue;
      }

   if (alive1 == neigh_array.GetSize())
      {
      continue;
      }

         
   beta = Tscale_fudge / (current_cell->T + neigh_cell->T);
   P_diff_T = exp(-beta*WorkFunction(neigh_cell,current_cell,cell_index,neigh_index));
   highest = 0.0;
      // The prob. of diffusion also depends on the number of
      // neighbours and next neighbours at the old and new site

   switch (alive1 - neigh_neighbours1[neigh_index]) // Change in number of neighbours
      {
      case -2 :P_diff = P_diff02*P_diff_T; break;
      case -1 :P_diff = P_diff01*P_diff_T; break;
      case  0 :P_diff = P_diff00*P_diff_T; break;
      case  1 :P_diff = P_diff10*P_diff_T; break;
      case  2 :P_diff = P_diff20*P_diff_T; break;
      }
   
   switch (alive2 - neigh_neighbours2[neigh_index]) //Change in number of next neighbours
      {
      case -6 : P_diff = P_diff*P_diffnn_6; break;
      case -5 : P_diff = P_diff*P_diffnn_5; break;
      case -4 : P_diff = P_diff*P_diffnn_4; break;
      case -3 : P_diff = P_diff*P_diffnn_3; break;
      case -2 : P_diff = P_diff*P_diffnn_2; break;
      case -1 : P_diff = P_diff*P_diffnn_1; break;
      case  0 : P_diff = P_diff*P_diffnn0; break;
      case  1 : P_diff = P_diff*P_diffnn1; break;
      case  2 : P_diff = P_diff*P_diffnn2; break;
      case  3 : P_diff = P_diff*P_diffnn3; break;
      case  4 : P_diff = P_diff*P_diffnn4; break;
      case  5 : P_diff = P_diff*P_diffnn5; break;
      case  6 : P_diff = P_diff*P_diffnn6; break;
      }

   if (P_diff > highest)     // Find `least action'
      {
      highest = P_diff;
      favourite = neigh_array.array[neigh_index];
      }
   }


 // Fluctuate if the roulette wheel favours the wicked ...

if (highest >= drand48())
   {
   neigh_cell = (ThermoCell *) cell[favourite];
   current_cell->SetAlive(False);
   neigh_cell->SetAlive(True);
   neigh_cell->E = current_cell->E;    // Transfer particle energy
   Draw(cell_index);
   Draw(favourite);
   }
}

/*****************************************************************************/

void Diffuse::ThermalLatticeConduction(ThermoCell *current_cell, index_array neighbours)

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

void Diffuse::CheckSymmetries()

{ const int symmetries = 30;
  const double scale = 1.0;
  int cell_index,i,r,c,delta11,delta33,n0,n1,biggest=0, degeneracy;
  double N[symmetries], S[symmetries], nalive = 0.0, pbiggest=0.0;
  CATriangularGrid *grid = (CATriangularGrid*) environ;
  bool symmetrical, alive;
  double p = 0;
  ThermoCell *current;

for (i = 0; i < symmetries; i++)
   {
   N[i] = 0;
   }
  
for (cell_index = 0; cell_index < cells; cell_index++)
   {
   current = (ThermoCell*) cell[cell_index];
    
   grid->QueryGridPosition(cell_index,r,c);
    
   delta11 = abs((r-c)%2);
   delta33 = (cell_index + (r%2)*3)%6;
   
   alive = current->Alive();

   if (alive)
      {
      nalive++;
      }
   
   for (i = 0; i < symmetries; i++)
      {
      switch(i)
	 {
	 case 0:
	     n0=0;
	     // 1x1 symmetry
	     symmetrical = (delta11 == n0);
	     break;
	   
	 case 1:
	     n0=1;
	     // 1x1 symmetry
	     symmetrical = (delta11 == n0);
	     break; 
	   
	 case 2:
	     // 3x3, 1/3
	     n0 = 0;
	     symmetrical = (delta33 == (n0)); //n0=(0,1) n1=(0,2)
	     break;

	 case 3:
	     // 3x3, 1/3
	     n0 = 1;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 4:
	     // 3x3, 1/3
	     n0 = 2;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 5:
	     // 3x3, 1/3
	     n0 = 3;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 6:
	     // 3x3, 1/3	   
	     n0 = 4;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 7:
	     // 3x3, 1/3
	     n0 = 5;
	     symmetrical = (delta33 == (n0));
	     break;

	 case 8:
	     // 3x3, 2/3
	     n0 = 0;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 9:
	     // 3x3, 2/3
	     n0 = 1;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 10:
	     // 3x3, 2/3
	     n0 = 2;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 11:
	     // 3x3, 2/3
	     n0 = 3;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 12:
	     // 3x3, 2/3
	     n0 = 4;
	     symmetrical = ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;

	 case 13:
	     // 3x3, 2/3
	     n0 = 5;
	     symmetrical =  ((delta33 == n0)||(delta33 == ((n0+2)%6)));
	     break;
	   
	 case 14:
	     // 2x2, 1/2
	     n0 = 0;
	     n1 = 0;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;
	   
	 case 15:
	     // 2x2, 1/2
	     n0 = 0;
	     n1 = 1;
	     symmetrical = ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));

	     break;

	 case 16:
	     // 2x2, 1/2
	     n0 = 0;
	     n1 = 2;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;
	   
	 case 17:
	     // 2x2, 1/2
	     n0 = 0;
	     n1 = 3;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));

	     break;

	 case 18:
	     // 2x2, 1/2
	     n0 = 1;
	     n1 = 0;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));

	     break;
	   
	 case 19:
	     // 2x2, 1/2
	     n0 = 1;
	     n1 = 1;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;
	   
	 case 20:
	     // 2x2, 1/2
	     n0 = 1;
	     n1 = 2;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));
	     break;
	   
	 case 21:
	     // 2x2, 1/2
	     n0 = 1;
	     n1 = 3;
	     symmetrical =  ((((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0))||
		 (((r+n0+1)%2 == 0 ) && (abs((r+n1-c-1+2*((r-c+1)%2))%4) == 0)));

	     break;
	   
	 case 22:
	     // 2x2, 1/4
	     n0 = 0;
	     n1 = 0;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 23:
	     // 2x2, 1/4
	     n0 = 0;
	     n1 = 1;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 24:
	     // 2x2, 1/4
	     n0 = 0;
	     n1 = 2;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 25:
	     // 2x2, 1/4
	     n0 = 0;
	     n1 = 3;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;

	 case 26:
	     // 2x2, 1/4
	     n0 = 1;
	     n1 = 0;
	     symmetrical =  (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;
	   
	 case 27:
	     // 2x2, 1/4
	     n0 = 1;
	     n1 = 1;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;
	   
	 case 28:
	     // 2x2, 1/4
	     n0 = 1;
	     n1 = 2;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;
	   
	 case 29:
	     // 2x2, 1/4
	     n0 = 1;
	     n1 = 3;
	     symmetrical = (((r+n0)%2 == 0 ) && (abs((r+n1-c)%4) == 0));
	     break;
	 }

      if (symmetrical)
	 {
	 S[i]++;
	 
	 if (alive)
	    {
	    N[i]++;
	    }
	 else
	    {
	    N[i]--;
	    }
	 }
      else
	 {
	 if (alive)
	    {
	    N[i]--;
	    }
	 else
	    {
	    N[i]++;
	    }
	 }
      }
   }

 degeneracy = 1;
 entropy = 0;
 
 for (i = 0; i < symmetries; i++)
    {
    p = (double)N[i]/(double)S[i];
    
    if (p < 1.0/(double)cells)  // cut-off
       {
       continue;
       }
    
    entropy -= p*log(p)/scale;

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

 if (pbiggest < 0.001)
    {
    visual->Printf(8,"Rate of entropy: %f ",entropy/iterations);
    visual->Printf(10,"Unsymmetrical");
    visual->Printf(11,"");    
    }
 else
    {
    visual->Printf(8,"Rate of entropy: %f ",entropy/iterations);
    visual->Printf(10,"Symm.fit P[%d] = %f",biggest,pbiggest);
    visual->Printf(11,"Symm degeneracy = %d",degeneracy);
    visual->Printf(12,"Occupancy = %f%%",nalive/(double)cells*200.0);
    }
}

/*****************************************************************************/

int Diffuse::TColour(double value)

// Colours 2 to 10 are lattice temp
    
{ const double no_of_colours = 8.0;

if (value > TMax)
   {
   value = TMax;
   }

 return (2 + (int)(value/TMax * no_of_colours));
}

/*****************************************************************************/

int Diffuse::EColour(double value)

// Colours 11 to 19
    
{ const double no_of_colours = 8.0;

return 0;
 
//if (value > EMax)
//   {
//   printf("Temperature exceeds maximum, truncating to TMax..\n");
//   value = EMax;
//   }
// return (11 + (int)(value/EMax * no_of_colours));
}
/*****************************************************************************/

int Diffuse::EColourSub(double value)

// Colours 21 to 29
    
{ const double no_of_colours = 8.0;

return 12;
 
//if (value > EMax)
//   {
//   printf("Temperature exceeds maximum, truncating to TMax..\n");
//   value = EMax;
//   }
// return (21 + (int)(value/EMax * no_of_colours));
}

/*****************************************************************************/
// Level 3
/*****************************************************************************/

double Diffuse::WorkFunction(ThermoCell *x, ThermoCell *xprime, int c, int cprime)

{ double work;
   
return 1.0;
   work = alpha * sqrt(0.5*(x->T,xprime->T)) + m * fabs(c-cprime) / cells;

 return work;
}

/*****************************************************************************/

int Diffuse::CountAlive(int cell_index, int level)
// How many neighbours are alive?
{ int i, neighboursum = 0;
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

return neighboursum;
}

/*****************************************************************************/

int Diffuse::CountAliveSub(int cell_index, int level)
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


