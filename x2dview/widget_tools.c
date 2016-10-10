/**************************************************************************/
/*                                                                        */
/* widget_tools: generic toolkit of widget operations                     */
/*                                                                        */
/*------------------------------------------------------------------------*/
/* copyright (c) 1996 Thomas E. Sevaldrud <thomasen@a.sn.no>              */
/*                    Jon Anders Mikkelsen                                */
/**************************************************************************/

#include <conf.h>
#include <widget_tools.h>

/**************************************************************************/

Widget GetToplevelWidget(Widget w)
{
Widget ret = w;

while(!XtIsTopLevelShell(ret))
   {
   ret = XtParent(ret);
   }

return ret;
}

/**************************************************************************/
