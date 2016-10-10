/*****************************************************************************/
/* menu.h: menubar object.                                                   */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Jon A. Mikkelsen / Thomas E. Sevaldrud                 */
/*****************************************************************************/

#ifndef _MENU_H
#define _MENU_H 1
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

class menu
{
private:

protected:
   Widget buttonwidget;
   Widget menuwidget;
   int    nitems;
   char   *id;
   
public:
   menu();
   menu(Widget parent, char *title);
   ~menu();

   Widget AddItem(char *title, XtCallbackProc function, XtPointer data);
   Widget GetButtonWidget();
   Widget GetMenuWidget();
};

/*****************************************************************************/

class menubar
{
private:

protected:
   Widget bar;
   
public:
   menubar();
   menubar(Widget parent);
   ~menubar();
   Widget Initialize(Widget parent);
   menu AddMenu(char *title);
   Widget GetWidget();
};

/*****************************************************************************/

#endif 






