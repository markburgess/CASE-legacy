/*****************************************************************************/
/* menu.C: menubar object.                                                   */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include "menu.h"


/*****************************************************************************/
/* MENUBAR OBJECT:                                                           */
/*****************************************************************************/

menubar::menubar()
{
}

/*****************************************************************************/

menubar::menubar(Widget parent)
{
 Initialize(parent);
}

/*****************************************************************************/

menubar::~menubar()
{
}

/*****************************************************************************/

Widget menubar::Initialize(Widget parent)
{
 bar = XtVaCreateManagedWidget("menubar", boxWidgetClass, parent,
			       XtNorientation, XtorientHorizontal,
			       XtNleft, XawChainLeft,
			       XtNright, XawChainLeft,
			       XtNtop, XawChainTop,
			       XtNbottom, XawChainTop,
			       NULL);
 
 return bar;
}

/*****************************************************************************/

menu menubar::AddMenu(char *title)
{
 menu new_menu(bar, title);
 
 return new_menu;
}

/*****************************************************************************/

Widget menubar::GetWidget()
{
 return bar;
}

/*****************************************************************************/
/* MENU OBJECT:                                                              */
/*****************************************************************************/

menu::menu()
{
nitems = 0;
}

/*****************************************************************************/

menu::menu(Widget parent, char *title)
{
 buttonwidget = XtVaCreateManagedWidget(title, menuButtonWidgetClass, parent,
					XtNmenuName, title,
					XtNtitle, title,
					XtNborderWidth, 0,
					NULL);
 menuwidget = XtCreatePopupShell(title, simpleMenuWidgetClass, buttonwidget, NULL,0);
}

/*****************************************************************************/

menu::~menu()
{
}

/*****************************************************************************/

Widget menu::AddItem(char *title, XtCallbackProc function, XtPointer data)
{
 Widget new_item;

 new_item = XtCreateManagedWidget(title, smeBSBObjectClass, menuwidget, NULL, 0);
 XtAddCallback(new_item, XtNcallback, function, data);

 nitems++;

 return new_item;
}

/*****************************************************************************/

Widget menu::GetButtonWidget()
{
 return buttonwidget;
}

/*****************************************************************************/

Widget menu::GetMenuWidget()
{
 return menuwidget;
}

/*****************************************************************************/
