/*****************************************************************************/
/*                                                                           */
/* CACell.h: Header file for CACell object    .                              */
/*                                                                           */
/*****************************************************************************/

#ifndef _GoLCELL_H
#define _GoLCELL_H 1

#include <CA.h>
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CACell.h>
#include <Particle.h>
#include <conf.h>

/*****************************************************************************/

class LifeCell : public Particle

{
public:
   LifeCell();
   virtual ~LifeCell();
protected:
   CABoolean alive;
private:
};

/*****************************************************************************/
#endif // _GoLCELL_H









