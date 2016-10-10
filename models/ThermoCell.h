
/*****************************************************************************/
/*                                                                           */
/* ThermoCell.h: Header file for CACell object  .                            */
/*                                                                           */
/*****************************************************************************/

// $Id: ThermoCell.h,v 1.2 1997/05/29 15:30:18 mark Exp $

#ifndef _THERMOCELL_H
#define _THERMOCELL_H 1

#include <CAReq.h>
#include <CAProperty.h>
#include <CACell.h>
#include <Particle.h>
#include <conf.h>

/*****************************************************************************/

class ThermoCell : public Particle

{
public:

   ThermoCell();
   virtual ~ThermoCell();

   Boolean Forbidden();
   void SetForbidden(Boolean new_value);

   double T;         /* lattice temperature */
   double E;         /* particle energy */
   
   CABoolean alive;
   CABoolean forbidden;

protected:   
   
private:
};

/*****************************************************************************/

#endif







