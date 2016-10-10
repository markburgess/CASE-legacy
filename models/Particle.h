/*****************************************************************************/
/*                                                                           */
/* File: Particle.h                                                          */
/*                                                                           */
/* Revision: $Id$                                                            */
/*                                                                           */
/* Description: Mother of all particle classes                               */
/*                                                                           */
/*****************************************************************************/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <CA.h>
#include <CACell.h>

class Particle: public CACell
{
 public:
   Particle();
   virtual ~Particle();

   virtual void SetupRequester(Req *requester);
   virtual void ReadWidgets(Req *requester);
   virtual void UpdateWidgetValue(Req *requester);

   virtual Boolean Alive();
   virtual void SetAlive(Boolean new_value);

   double    state;     // A general parameter
   
protected:
   CABoolean alive;
   
private:
};



#endif
