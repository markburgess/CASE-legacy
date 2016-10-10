/*****************************************************************************/
/* CASymbol.C: CASymbol object.                                              */
/*                                                                           */
/*****************************************************************************/

/* $Id: CASymbol.C,v 1.13 1997/05/29 15:30:02 mark Exp $ */

#include "CASymbol.h"
#include <stdio.h>
#include <math.h>
#include "CA.h"

/*****************************************************************************/

CASymbol::CASymbol(CAVisual *new_vis, CAEnviron *new_env)
{
 vis = new_vis;
 env = new_env;
}

/*****************************************************************************/

void CASymbol::Initialize(CAVisual *new_vis, CAEnviron *new_env)
{
 vis = new_vis;
 env = new_env;
}

/*****************************************************************************/

void CAColorSymbol::Draw(int cell, int color)

  // Intended as a way of filling in one cell, so must
  // respond to different shapes.

{ polygon poly;
  CAGrid *grid = (CAGrid*) env;

 grid->QueryPolygon(cell, poly);
 vis->SetDrawingColor(color);
 vis->FilledPolygon(poly);
}

/*****************************************************************************/

void CABackgroundedCircle::Draw(int cell, int bgcolor, int fgcolor)

{ polygon poly;
  CAGrid *grid = (CAGrid*) env;
  double centre_x, centre_y, sx,sy;

grid->QueryPolygon(cell, poly);
vis->SetDrawingColor(bgcolor);
vis->FilledPolygon(poly);

env->QueryPosition(cell,centre_x,centre_y);
env->QueryScaleFactors(sx,sy);

if (fgcolor != CANoSymbol) // Optimize
   {
   vis->SetDrawingColor(fgcolor);
   vis->FilledEllipse(centre_x,centre_y,sx*0.8,sx*0.8);
   }
}

/*****************************************************************************/

void CABackgroundedCircle::DrawBig(int cell, int bgcolor, int fgcolor)

{ polygon poly;
  CAGrid *grid = (CAGrid*) env;
  double centre_x, centre_y, sx,sy;

grid->QueryPolygon(cell, poly);
vis->SetDrawingColor(bgcolor);
vis->FilledPolygon(poly);

env->QueryPosition(cell,centre_x,centre_y);
env->QueryScaleFactors(sx,sy);

if (fgcolor != CANoSymbol) // Optimize
   {
   vis->SetDrawingColor(fgcolor);
   vis->FilledEllipse(centre_x,centre_y,sx*0.9,sx*0.9);
   }
}

/*****************************************************************************/

void CABackgroundedCircle::DrawBigSmall(int cell, int bgcolor, int fgcolor, int fgsmallcolor)

{ polygon poly;
  CAGrid *grid = (CAGrid*) env;
  double centre_x, centre_y, sx,sy;

grid->QueryPolygon(cell, poly);
vis->SetDrawingColor(bgcolor);
vis->FilledPolygon(poly);

env->QueryPosition(cell,centre_x,centre_y);
env->QueryScaleFactors(sx,sy);

if (fgcolor != CANoSymbol && fgsmallcolor != CANoSymbol) // Optimize
   {
   vis->SetDrawingColor(fgsmallcolor);
   vis->FilledEllipse(centre_x,centre_y,sx,sx);
   vis->SetDrawingColor(fgcolor);
   vis->FilledEllipse(centre_x,centre_y,sx*0.5,sx*0.5);
   }
}

/*****************************************************************************/

void CAArrowSymbol::Draw(int cell, double direction)
{
 /* ---------------------------------------------------------
    This method draws the arrow symbol with base and length
    given by the largest drawable circle in the given cell.
    The direction is given in radians.
    --------------------------------------------------------- */
 double x1, y1, sx ,sy;
 double x2, y2, x2b, y2b;
 double x_head1,x_head2,y_head1,y_head2;
 double body_frac = 0.65;	   
 double head_angle = 0.35;	

 env->QueryPosition(cell, x1, y1);
 env->QueryScaleFactors(sx, sy);
 sx *= 0.85;
 sy *= 0.85;
 
 x2  = x1 + sx*cos(direction);
 y2  = y1 + sy*sin(direction);
 x2b = x1 + 0.9*sx*cos(direction);
 y2b = y1 + 0.9*sy*sin(direction);

 x_head1 = x1 + body_frac*sx*cos(direction - head_angle);
 x_head2 = x1 + body_frac*sx*cos(direction + head_angle);
 y_head1 = y1 + body_frac*sy*sin(direction - head_angle);
 y_head2 = y1 + body_frac*sy*sin(direction + head_angle);
 
 vis->DrawLine(x_head1,y_head1,x2,y2);
 vis->DrawLine(x_head2,y_head2,x2,y2);
 vis->DrawLine(x1,y1,x2b,y2b);
}

/*****************************************************************************/

void CAArrowSymbol::Clear(int cell)
{
 double centre_x, centre_y, sx, sy;
 env->QueryPosition(cell, centre_x, centre_y);
 env->QueryScaleFactors(sx, sy);

 vis->SetDrawingColor(0);
 vis->FilledEllipse(centre_x, centre_y, sx*0.95, sy*0.95);
}

/*****************************************************************************/

void CAColoredArrowSymbol::Draw(int cell, int color, double direction)
{
 vis->SetDrawingColor(color);
 CAArrowSymbol::Draw(cell, direction);
}

/*****************************************************************************/

void CAColoredEllipseSymbol::Clear(int cell)
{
 double centre_x, centre_y, sx, sy;
 env->QueryPosition(cell, centre_x, centre_y);
 env->QueryScaleFactors(sx, sy);

 vis->SetDrawingColor(0);
 vis->FilledEllipse(centre_x, centre_y, sx, sy);
}


/*****************************************************************************/

void CAColoredEllipseSymbol::Draw(int cell, int color)
{
 double centre_x, centre_y, sx, sy;
 env->QueryPosition(cell, centre_x, centre_y);
 env->QueryScaleFactors(sx, sy);

 vis->SetDrawingColor(color);
 vis->FilledEllipse(centre_x, centre_y, sx, sy);
}

/*****************************************************************************/

void CAColoredSignSymbol::Clear(int cell)

{ double centre_x, centre_y, sx, sy;

env->QueryPosition(cell, centre_x, centre_y);
env->QueryScaleFactors(sx, sy);

vis->SetDrawingColor(0);                             // bgcolour = 0
vis->FilledEllipse(centre_x, centre_y, sx, sy);
}

/*****************************************************************************/

void CAColoredSignSymbol::Draw(int cell, int value, int pcolor, int mcolor)

  // Draw a plus or a minus symbol in the cell in the  
  // prescribed colour. Scale symbol according to value.
  // pcolor for >0 mcolour for < 0. Blank out for zero.

{ double centre_x, centre_y, sx, sy, scale;

 env->QueryPosition(cell, centre_x, centre_y);
 env->QueryScaleFactors(sx, sy);

 scale = max(sx,sy) * 0.4 * (double) fabs(value);  // 0.3 looks good

 if (value < 0)
    {
    vis->SetDrawingColor(mcolor);
    vis->SetLineWidth(CA_DOUBLE_LINEWIDTH); 
    vis->DrawLine(centre_x - scale, centre_y, centre_x + scale, centre_y);
    vis->SetLineWidth(CA_DEFAULT_LINEWIDTH);
    return;
    }

 if (value > 0)
    {
    vis->SetDrawingColor(pcolor);
    vis->SetLineWidth(CA_DOUBLE_LINEWIDTH);
    vis->DrawLine(centre_x - scale, centre_y, centre_x + scale, centre_y);
    vis->DrawLine(centre_x, centre_y - scale, centre_x, centre_y + scale);

    vis->SetLineWidth(CA_DEFAULT_LINEWIDTH);
    return;    
    }
}

/*****************************************************************************/
