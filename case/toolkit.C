/*****************************************************************************/
/*                                                                           */
/* toolkit.h:  general data types, classes and functions                     */
/*                                                                           */
/*****************************************************************************/

/* $Id */

#include "toolkit.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************/
/* TOOLKIT CLASSES:                                                          */
/*****************************************************************************/

polygon::polygon()
{
npoints = 0;
}

/*****************************************************************************/

polygon::polygon(int new_size)

{
npoints = 0;
SetSize(new_size);
}
/*****************************************************************************/

polygon::~polygon()
{
if (npoints)
   {
   delete[] x;
   delete[] y;
   }
}

/*****************************************************************************/

int polygon::SetSize(int new_size)
{
if (npoints)
   {
   delete[] x;
   delete[] y;
   }

npoints = new_size;

x = new double[npoints];
y = new double[npoints];

if (x && y)
   {
   return 1;
   }

return 0;
}

/*****************************************************************************/

int polygon::GetSize()
{
return npoints;
}

/*****************************************************************************/

index_array::index_array()
    
{
size = 0;
array = NULL;
}

/*****************************************************************************/

index_array::~index_array()

  // This routine causes a consistent segmentation fault under
  // linux. It seems to destroy the heap... But if we do not
  // deallocate the memory there will be a leak and the system
  // will run out of virtual memory after a time.
  // What the hell is the REAL problem here???

{
if (size != 0)
   {
   delete [] array;
   array = NULL;
   size = 0;
   }
}

/*****************************************************************************/

int index_array::SetSize(int new_size)

{
if (size != 0)
   {
   delete [] array;
   size = 0;
   }

if (new_size != 0)
  {
  array = new int[new_size];
  }
else
   {
   array = NULL;
   size = 0;
   }

if (array)
   {
   size = new_size;
   return 1;
   }
else
   {
   return 0;
   }
}

/*****************************************************************************/

int index_array::GetSize()

{
return size;
}

/*****************************************************************************/
/* BITMAP ROUTINES:                                                          */
/*****************************************************************************/

void AddBitmapToWidget(Widget w, int width, int height, char *data)
{
 Pixmap pix;
 Pixmap bits;
 GC gc;
 XGCValues button_values;
 
 bits = XCreateBitmapFromData(XtDisplay(w), XtWindow(w),
			      data, width, height);
 pix = XCreatePixmap(XtDisplay(w), XtWindow(w),
		     width, height, 
		     DefaultDepthOfScreen(XtScreen(w)));
 XtVaGetValues(w,
	       XtNforeground, &button_values.foreground,
	       XtNbackground, &button_values.background,
	       NULL);
 gc = XtGetGC(w, GCForeground | GCBackground, &button_values);
 
 XCopyPlane(XtDisplay(w), bits, pix, gc, 0, 0,
	    width, height, 0, 0, 1);
 XtVaSetValues(w,
	       XtNbitmap, pix,
	       NULL);
 
} 


/*********************************************************************/
//
// C++ Itemlist library
//
/*********************************************************************/

ItemList::ItemList()   // Constructor

{
RootPtr = CurrPtr = NULL;
}

/*********************************************************************/

ItemList::~ItemList()   // Destructor

{
DeleteItemList(RootPtr);
RootPtr = CurrPtr = NULL;
}

/*********************************************************************/

void ItemList::PrependItem (char *itemstring)

{ Item *ip;
  char *sp,*spe;

ip = new Item;
if (ip == NULL)
   {
   printf("new failed\n");
   exit(0);
   }

sp = new char[strlen(itemstring)+2];
if (sp == NULL)
   {
   printf("new failed\n");
   exit(0);
   }

strcpy(sp,itemstring);
ip->name = sp;
ip->next = RootPtr;
RootPtr = ip;
}

/*********************************************************************/

void ItemList::AppendItem (char *itemstring)

{  Item *ip, *lp;
  char *sp,*spe;

ip = new  Item;

if (ip == NULL)
 {
 printf("new failed\n");
 exit(0);
 }

sp = new char[strlen(itemstring)+2];

if (sp == NULL)
   {
   printf("new failed\n");
   exit(0);
   }

if (RootPtr == NULL)
   {
   RootPtr = ip;
   }
else
   {
   for (lp = RootPtr; lp->next != NULL; lp=lp->next)
      {
      }

   lp->next = ip;
   }

strcpy(sp,itemstring);
ip->name = sp;
ip->next = NULL;
}

/*********************************************************************/

void ItemList::DeleteItemList(Item *item)
 
{  Item *ip;

if (item != NULL)
   {
   DeleteItemList(item->next);
   item->next = NULL;

   if (item->name != NULL)
      {
      delete item->name;
      }

   delete item;
   }
}

/*********************************************************************/

void ItemList::DeleteItem(Item *item)
 
{  Item *ip, *sp;

if (item != NULL)
   {
   if (item->name != NULL)
      {
      delete item->name;
      }

   sp = item->next;

   if (item == RootPtr)
      {
      RootPtr = sp;
      }
   else
      {
      for (ip = RootPtr; ip->next != item; ip=ip->next)
         {
         }

      ip->next = sp;
      }

   delete item;
   }
}

/*********************************************************************/

void ItemList::ListItemList()

{ Item *ip; 

for (ip = RootPtr; ip != NULL; ip=ip->next)
   {
   printf ("%s\n", ip->name);
   }
}


/*********************************************************************/

bool ItemList::ItemExists(char *item)

{ Item *ip; 

for (ip = RootPtr; ip != NULL; ip=ip->next)
   {
   if (strcmp(ip->name,item) == 0)
      {
      return(true);
      }
   }

return(false);
}

/*********************************************************************/

Item* ItemList::LocateNextItemContaining(char *string)

  // Note that this is declared Item * class::name and not
  // Item class:: *name. The latter means 

{ Item *ip;

for (ip = RootPtr; ip != NULL; ip=ip->next)
   {
   if (strstr(ip->name,string))
      {
      return ip;
      }
   }

return NULL;
}

/*********************************************************************/

bool ItemList::DeleteItemStarting(char *string)

{ Item *ip, *next, *last;

for (ip = RootPtr; ip != NULL; ip=ip->next)
   {
   if (strncmp(ip->name,string,strlen(string)) == 0)
      {
      if (ip == RootPtr)
         {
         delete RootPtr->name;

         RootPtr = ip->next;
         delete ip;

         return true;
         }
      else
         {
         last->next = ip->next; 

         delete ip->name;
         delete ip;

         return true;
         }

      }
   last = ip;
   }

return false;
}

/*********************************************************************/
