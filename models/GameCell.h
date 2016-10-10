/*****************************************************************************/
/*                                                                           */
/* CACell.h: Header file for CACell object    .                              */
/*                                                                           */
/*****************************************************************************/

#ifndef _GameCELL_H
#define _GameCELL_H 1

#include <CA.h>
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CACell.h>
#include <Particle.h>
#include <conf.h>

/*****************************************************************************/

class GameCell : public Particle

{
public:
   GameCell();
   virtual ~GameCell();
protected:
   CABoolean alive;
private:
};

/*****************************************************************************/
#endif // _GameCELL_H









