/*****************************************************************************/

/* $Id */


#ifndef _TRICELL_H
#define _TRICELL_H 1

#define PIx2    6.283185307179586476925287
#include <CA.h>
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CACell.h>
#include <Particle.h>
#include <conf.h>

/*****************************************************************************/

class TriCell : public Particle
{

public:
   TriCell();
   virtual ~TriCell();
   void SetCharge(double new_charge);
   int charge;

protected:

private:
};

/*****************************************************************************/
#endif // _TRICELL_H
