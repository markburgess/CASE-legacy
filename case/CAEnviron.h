/*****************************************************************************/
/* CAEnviron.h: Header file for CAEnviron object                             */
/*****************************************************************************/

/* $Id: CAEnviron.h,v 1.18 1997/05/30 13:34:46 mark Exp $ */

#ifndef _CAENVIRON_H
#define _CAENVIRON_H 1
class CAEnviron;

#include <toolkit.h>
#include <CAVisual.h>
#include <CAReq.h>

/*****************************************************************************/

class CAEnviron
{
public:
   CAEnviron(double new_x_min=0, double new_y_min=0,
	     double new_x_max=10, double new_y_max=10);
   ~CAEnviron();
   CAEnviron (CAVisual *visual);
   virtual void SyncViewport(CAVisual *visual);

   virtual void QueryPosition(int cell_index, double &x, double &y)=0;
   virtual void QueryGridPosition(int cell_index, int &row, int &column)=0;
   virtual int  QueryCell(double x, double y)=0;
   virtual void QueryScaleFactors(double &sx, double &sy)=0;
   virtual void SetPeriodic(Boolean new_periodic);
   virtual void DrawGrid(CAVisual *vis)=0;
   virtual void SetDrawGrid(Boolean new_value);
   virtual void SetupRequester(Req *req);
   virtual void ReadRequester(Req *req);

   virtual int RowCol2Index(int row, int col) = 0;

   //   double CAInfinity = 9.0E307;

#define CAInfinity 9.0E307
#define NoCell -1

   //   int NoCell = -1;


protected:

   double x_min, x_max, y_min, y_max;
   double theta;                       // An angle for non-square lattices.
   Boolean periodic;
   Boolean draw_grid;

private:
};

/*****************************************************************************/

class CAGrid : public CAEnviron
{
public:
   CAGrid(void);
   CAGrid(double new_x_min=0, double new_y_min=0,
	  double new_x_max=10, double new_y_max=10,
	  int new_cols=20, int new_rows=20);
   ~CAGrid();
   virtual void SetRowsCols(int new_cols, int new_rows);
   virtual int  GetNumberOfCells();
   virtual int  CountNeighbours(int cell_index, int level) = 0;
   virtual void QueryNeighbours(int cell_index, int levels,
				index_array &neighbours) = 0;
   virtual void QueryPolygon(int cell_index, polygon &poly) = 0;

   virtual int RowCol2Index(int row, int col);

   virtual int QueryMaxColumns(void);
   virtual int QueryMaxRows(void);

   virtual Boolean IsCornerCell(int cell_index);
   virtual Boolean IsEdgeCell(int cell_index);

   int cols,rows;
   
   
protected:
   virtual void SetupRequester(Req *req);
   virtual void ReadRequester(Req *req);
   virtual Boolean Inbounds(int row, int col);

private:
};

/*****************************************************************************/

class CARectangularGrid : public CAGrid
{
public:

   CARectangularGrid(double new_x_min=0, double new_y_min=0,
		     double new_x_max=10, double new_y_max=10,
		     int new_cols=20, int new_rows=20);

   ~CARectangularGrid();

   virtual void QueryPosition(int cell_index, double &x, double &y);
   virtual void QueryGridPosition(int cell_index, int &row, int &col);
   virtual int  QueryCell(double x, double y);
   virtual int  CountNeighbours(int cell_index, int level);
   virtual void QueryNeighbours(int cell_index, int levels,
				index_array &neighbour);

   virtual void QueryCornerXY(int cell_index, double &corner_x, double &corner_y);
   virtual void QueryCornerCells(int cell_index, index_array &corner_cells);
   virtual void QueryPolygon(int cell_index, polygon &poly);
   virtual void QueryScaleFactors(double &sx, double &sy);
   virtual void DrawGrid(CAVisual *vis);

protected:
   
private:
};

/*****************************************************************************/

class CARectGrid : public CARectangularGrid
{
public:

   CARectGrid(double new_x_min=0, double new_y_min=0,
		     double new_x_max=10, double new_y_max=10,
		     int new_cols=20, int new_rows=20);
   ~CARectGrid();
   
   virtual void QueryNeighbours(int cell_index, int levels,
				index_array &neighbour);
private:
};

/*****************************************************************************/

class CATriangularGrid : public CAGrid
{
public:

   CATriangularGrid(double new_x_min, double new_y_min,
		     double new_x_max, double new_y_ma,
		     int new_cols, int new_rows, double angle);
   
   ~CATriangularGrid();
   
   virtual void QueryPosition(int cell_index, double &x, double &y);
   virtual void QueryGridPosition(int cell_index, int &row, int &col);
   virtual int  QueryCell(double x, double y);
   virtual void QueryScaleFactors(double &sx, double &sy);
   
   virtual void DrawGrid(CAVisual *vis);

   void CalcGeometry();

   virtual int  CountNeighbours(int cell_index, int level);
   virtual void QueryNeighbours(int cell_index, int levels,
				index_array &neighbour);
   virtual void QueryPolygon(int,polygon &);

protected:


   
private:
   double theta ;          // Angle /_
   double dx,dy;           // Lattice internals theta = dy/dx
			  
   double dx_offset_odd;   // Work out some offsets to save computation later
   double dy_offset_odd;
   double dx_offset_even;
   double dy_offset_even;

};



#endif // _CAENVIRON_H





