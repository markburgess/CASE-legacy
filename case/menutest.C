/*
** menutest.C: test of menu objects
*/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include "menu.h"


int main(int argc, char **argv)
{
 XtAppContext ac;
 Widget toplevel, form;

 XtVaAppInitialize(&ac, "MenuTest", NULL, 0, &argc, argv, NULL, NULL);

 form = XtVaCreateManagedWidget("mainwin", formWidgetClass, toplevel, NULL);

 XtRealize(toplevel);
