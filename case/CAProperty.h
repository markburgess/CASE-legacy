/*****************************************************************************/
/* CAProperty.h: Header file for CAProperty object.                          */
/*                                                                           */
/*****************************************************************************/

/* $Id */

#ifndef _CAPROPERTY_H
#define _CAPROPERTY_H 1

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CAReq.h>

/*****************************************************************************/

class CAProperty
{

public:
   CAProperty();
   ~CAProperty();
   virtual void InitWidgets(Req *req, char *identifier)=0; 
   virtual void ReadWidgets(Req *req, char *identifier)=0;
   virtual void UpdateWidgetValue(Req *req, char *identifier)=0;
   
protected:

private:
};

/*****************************************************************************/

class CADouble : public CAProperty
{
public:
   CADouble();
   ~CADouble();
   virtual void InitWidgets(Req *requester, char *identifier);
   virtual void ReadWidgets(Req *requester, char *identifier);
   virtual void UpdateWidgetValue(Req *req, char *identifier);
   virtual void SetValue(double new_value);
   virtual double GetValue();
protected:
   double value;
private:
};
/*****************************************************************************/

class CABoolean : public CAProperty
{
public:
   CABoolean();
   ~CABoolean();

   Boolean GetValue();
   void SetValue(Boolean new_val);
   
   virtual void InitWidgets(Req *req, char *identifier); 
   virtual void ReadWidgets(Req *req, char *identifier);
   virtual void UpdateWidgetValue(Req *req, char *identifier);

   Boolean value;

protected:
private:
};

/*****************************************************************************/

#endif // _CAPROPERTY_H









