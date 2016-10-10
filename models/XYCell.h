#ifndef _XYCELL_H
#define _XYCELL_H 1

#define PIx2    6.283185307179586476925287
#include <CA.h>
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <CACell.h>
#include <conf.h>

/*****************************************************************************/

class XYCell : public CACell

{

public:
   XYCell();
   virtual ~XYCell();
   virtual void SetupRequester(Req *requester);
   virtual void ReadWidgets(Req *requester);
   virtual void UpdateWidgetValue(Req *requester);
   double GetAngle();
   void SetAngle(double new_Angle);
   
protected:
   CADouble angle;
private:
};

/*****************************************************************************/
#endif // _XYCELL_H
