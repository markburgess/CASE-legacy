/*****************************************************************************/
/*                                                                           */
/* XYBfield cell type. Note that the cell values are integer in this model   */
/* more like the game of life than the XY model, since we have vortex or     */
/* no vortex. The links are then drawn on the grid as arrows to indicate     */
/* the discrete gradient current.                                            */
/*                                                                           */
/*****************************************************************************/

/* $Id: XYBCell.h,v 1.8 1996/11/25 16:53:38 mark Exp $ */

/* ################################################################### */ 
/* ##### The lattice and the direction of the phases is defined  ##### */ 
/* ##### like this:						 ##### */ 
/* ##### 			  				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  V 3				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 	      2		  |		0		 ##### */ 
/* #####         ------->------ (x,y) ------->--------	(x+1,y)	 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  | 1				 ##### */ 
/* ##### 			  V				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			  |				 ##### */ 
/* ##### 			(x,y-1)				 ##### */ 
/* ##### 			  				 ##### */ 
/* ##### These are the four links link[0-3][x,y] with the        ##### */
/* ##### positive direction indicated. The values of the links   ##### */
/* ##### are stored just once in the array link[0,1] with	 ##### */
/* ##### just two link values corresponding to each site.	 ##### */
/* ##### value as link[2][x+1][y].	     		         ##### */
/* ################################################################### */ 



#ifndef _XYBCELL_H
#define _XYBCELL_H 1

#define PIx2    6.283185307179586476925287
#include <CA.h>
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CACell.h>
#include <conf.h>

/*****************************************************************************/

class XYBCell : public CACell
{

public:
   XYBCell();
   virtual ~XYBCell();
   virtual void SetupRequester(Req *requester);
   virtual void ReadWidgets(Req *requester);
   virtual void UpdateWidgetValue(Req *requester);
   double  GetCharge();
   void    SetCharge(double new_charge);

   int charge;
   
protected:

private:
};

/*****************************************************************************/
#endif // _XYBCELL_H
