
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <conf.h>

/*********************************************************************/
/* str2double:                                                       */
/* Resource converter for string to double                           */
/*********************************************************************/

void CvtStringToDouble(args, nargs, fromVal, toVal)
     XrmValuePtr args, fromVal, toVal;
     int *nargs;
{
  static double result;

  if(*nargs != 0)
      XtWarning("CvtStringToDouble: No arguments needed!");

  if(sscanf((char *)fromVal->addr, "%lf", &result) == 1)
    {
      toVal->size = sizeof(double);
      toVal->addr = (caddr_t) &result;
    }
  else
    XtStringConversionWarning((char *) fromVal->addr, "Double");
}
/*********************************************************************/
/* str2GCvalue:                                                      */
/* Resource converter for string to Graphics Context identifier      */
/*********************************************************************/

void CvtStringToGCvalue(args, nargs, fromVal, toVal)
     XrmValuePtr args, fromVal, toVal;
     int *nargs;
{
  char *str = (char *)fromVal->addr;
  int result;

  if(strcmp(str, "LineSolid")==0) 
    {
      result = 0;
      toVal->size = sizeof(int);
      toVal->addr = (caddr_t) &result;
    }
  else if(strcmp(str, "LineOnOffDash")==0) 
    {
      result = 1;
      toVal->size = sizeof(int);
      toVal->addr = (caddr_t) &result;
    }
  else if(strcmp(str, "LineDoubleDash")==0) 
    {
      result = 2;
      toVal->size = sizeof(int);
      toVal->addr = (caddr_t) &result;
    }
  else
    XtStringConversionWarning((char *) fromVal->addr, "GCvalue");
}


