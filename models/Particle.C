/*****************************************************************************/
/*                                                                           */
/* File: Particle.C                                                          */
/*                                                                           */
/* Revision: $Id$                                                            */
/*                                                                           */
/*****************************************************************************/

#include <Particle.h>

Particle::Particle()
{
}

/*****************************************************************************/

Particle::~Particle()
{
}

/*****************************************************************************/

void Particle::SetupRequester(Req *requester)
{
 alive.InitWidgets(requester, "Alive");
}

/*****************************************************************************/

void Particle::ReadWidgets(Req *requester)
{
 alive.ReadWidgets(requester, "Alive");
}

/*****************************************************************************/

void Particle::UpdateWidgetValue(Req *requester)
{
 alive.UpdateWidgetValue(requester, "Alive");
}

/*****************************************************************************/

Boolean Particle::Alive()
{
 return(alive.GetValue());
}

/*****************************************************************************/

void Particle::SetAlive(Boolean new_value)
{
 alive.SetValue(new_value);
}

