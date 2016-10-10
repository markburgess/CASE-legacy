/*****************************************************************************/
/* CACell.h: Header file for CACell object    .                              */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/


#ifndef _CACELL_H
#define _CACELL_H 1
class CACell;
#include <CAReq.h>
#include <CAProperty.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

/*****************************************************************************/

class CACell
{

public:
   CACell();
   virtual ~CACell();
   virtual void SetupRequester(Req *req)=0;
   virtual void ReadWidgets(Req *req)=0;
   virtual void UpdateWidgetValue(Req *req)=0;

protected:
private:
};

/*****************************************************************************/
#endif // _CACELL_H


