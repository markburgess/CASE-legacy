/*****************************************************************************/
/*                                                                           */
/* CASymbol.h: Header file for CASymbol object.                              */
/*                                                                           */
/*****************************************************************************/

/* $Id */

#ifndef _CASYMBOL_H
#define _CASYMBOL_H 1
#include <CAVisual.h>
#include <CAEnviron.h>

/*****************************************************************************/

class CASymbol
{
public:
   CASymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL);
   ~CASymbol();
   void Initialize(CAVisual *new_vis, CAEnviron *new_env);

#define CANoSymbol -1
   // int CANoSymbol = -1;
protected:
   CAVisual *vis;
   CAEnviron *env;
private:
};

/*****************************************************************************/

class CAColorSymbol : public CASymbol
{
public:
   CAColorSymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CASymbol(new_vis, new_env){};
   ~CAColorSymbol();
   virtual void Draw(int cell, int color);

protected:

private:
};

/*****************************************************************************/

class CAArrowSymbol : public CASymbol
{
public:
   CAArrowSymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CASymbol(new_vis, new_env){};
   ~CAArrowSymbol();
   virtual void Draw(int cell, double direction);
   virtual void Clear(int cell);
   
protected:

private:
};

/*****************************************************************************/

class CAColoredArrowSymbol : public CAArrowSymbol
{
public:
   CAColoredArrowSymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CAArrowSymbol(new_vis, new_env){};
   ~CAColoredArrowSymbol();
   virtual void Draw(int cell, int color, double direction);

protected:

private:
};

/*****************************************************************************/

class CAColoredEllipseSymbol : public CASymbol
{
public:
   CAColoredEllipseSymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CASymbol(new_vis, new_env){};
   ~CAColoredEllipseSymbol();
   virtual void Draw(int cell, int color);
   virtual void Clear(int cell);

protected:

private:
};

/*****************************************************************************/

class CAColoredSignSymbol : public CASymbol
{
  public:
    CAColoredSignSymbol(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CASymbol(new_vis, new_env){};
    ~CAColoredSignSymbol();
    virtual void Draw(int cell, int value, int pcolor, int mcolor);
    virtual void Clear(int cell);

protected:

private:
};

/*****************************************************************************/

class CABackgroundedCircle : public CASymbol

{
  public:
    CABackgroundedCircle(CAVisual *new_vis = NULL, CAEnviron *new_env = NULL):
	   CASymbol(new_vis, new_env){};
    ~CABackgroundedCircle();
    virtual void Draw(int cell, int fgcolor, int bgcolor);
    virtual void DrawBig(int cell, int fgcolor, int bgcolor);
    virtual void DrawBigSmall(int cell, int fgcolor, int bgcolor, int bgsubcolor);

protected:

private:
};

/*****************************************************************************/



#endif //_CASYMBOL_H

