/*****************************************************************************/
/* CAProperty.C: CAProperty object.                                          */
/*                                                                           */
/*****************************************************************************/

/* $Id: CAProperty.C,v 1.3 1997/05/29 15:30:01 mark Exp $ */

#include <CAProperty.h>

#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Toggle.h>
#include <stdlib.h>
#include <stdio.h>
/*****************************************************************************/

CAProperty::CAProperty()
{
}

/*****************************************************************************/

CAProperty::~CAProperty()
{
}

/*****************************************************************************/
/*CADouble:                                                                  */
/*****************************************************************************/

CADouble::CADouble()
{
 value = 0;
}

/*****************************************************************************/

CADouble::~CADouble()
{
}

/*****************************************************************************/

void CADouble::InitWidgets(Req *req, char *identifier)
{
 Widget dialog;
 
 req->AddGroup(identifier, formWidgetClass);
 dialog = req->AddWidget("dialog", identifier, dialogWidgetClass);
 XtVaSetValues(dialog,
	       XtNlabel, identifier,
	       XtNvalue, " ",
	       NULL);
}

/*****************************************************************************/

void CADouble::ReadWidgets(Req *req, char *identifier)
{
 Widget dialog;

 dialog = req->GetWidget("dialog", identifier);
 value = atof(XawDialogGetValueString(dialog));
}

/*****************************************************************************/

void CADouble::UpdateWidgetValue(Req *req, char *identifier)
{
 Widget dialog;
 char string_value[40];

 dialog = req->GetWidget("dialog", identifier);
 sprintf(string_value, "%f", value);
 XtVaSetValues(dialog,
	       XtNvalue, string_value,
	       NULL);
}

/*****************************************************************************/

void CADouble::SetValue(double new_value)
{
 value = new_value;
}

/*****************************************************************************/

double CADouble::GetValue()
{
 return value;
}

/*****************************************************************************/
/*CADouble:                                                                  */
/*****************************************************************************/

CABoolean::CABoolean()

{
 value = FALSE;
}

/*****************************************************************************/

CABoolean::~CABoolean()

{
}

/*****************************************************************************/

Boolean CABoolean::GetValue()
{
 return (value);
}

/*****************************************************************************/

void CABoolean::SetValue(Boolean new_val)

{
 if (new_val)
    {
    value = TRUE;
    }
 else
    {
    value = FALSE;
    }
}

/*****************************************************************************/

void CABoolean::InitWidgets(Req *req, char *identifier)
{
 Widget toggle, label;
 
 req->AddGroup(identifier, formWidgetClass);
 toggle = req->AddWidget("toggle", identifier, toggleWidgetClass);
 XtVaSetValues(toggle,
	       XtNlabel, identifier,
	       NULL);
		       
}

/*****************************************************************************/

void CABoolean::ReadWidgets(Req *req, char *identifier)
    
{ Widget toggle;

 toggle = req->GetWidget("toggle", identifier);
 XtVaGetValues(toggle,
               XtNstate, &value,
               NULL);
}

/*****************************************************************************/

void CABoolean::UpdateWidgetValue(Req *req, char *identifier)
    
{ Widget toggle;

 toggle = req->GetWidget("toggle", identifier);
 XtVaSetValues(toggle,
               XtNstate, value,
               NULL);
}

/*****************************************************************************/
