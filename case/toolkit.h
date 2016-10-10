/*****************************************************************************/
/* toolkit.h:  general data types, classes and functions                     */
/*                                                                           */
/*****************************************************************************/

/* $Id */

#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

/*****************************************************************************/
/* MACROS/DEFINITIONS:                                                       */
/*****************************************************************************/

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif
#ifndef max
#define max(a,b) (a > b ? a : b)
#endif

/*****************************************************************************/

 // This is currently obsolete, use array of XPoint instead.

class polygon
{
private:
   int npoints;

protected:

public:
   double *x;
   double *y;

   polygon();
   polygon(int new_size);
   ~polygon();

   int SetSize(int new_size);
   int GetSize();
};   

/*****************************************************************************/

class index_array

{
private:
   int size;

protected:

public:
   int *array;
   
   index_array();
   index_array(int new_size);
   ~index_array();

   int SetSize(int new_size);
   int GetSize();
   
};

/*****************************************************************************/
/* FUNCTION PROTOTYPES:                                                      */
/*****************************************************************************/

void AddBitmapToWidget(Widget w, int width, int height, char *data);


/*********************************************************************/
/*                                                                   */
/* TOOLKIT : items,  rewritten for C++                               */
/*                                                                   */
/*********************************************************************/

class Item;      // Just to say that this class exists to the compiler

class ItemList
   {
   private:

   Item *RootPtr;
   Item *CurrPtr;

   public:

    ItemList();
   ~ItemList();

   void AppendItem(char *);
   void InsertItem(char *);
   void PrependItem(char *);
   void DeleteItemList(struct Item *);
   void DeleteItem(struct Item *);
   void ListItemList();

   bool ItemExists(char *);
   bool DeleteItemStarting(char *);
   bool LoadItemList(char *);
   bool IsItemList(struct Item *, char *);

   Item *LocateNextItemContaining(char *);    // See note in fn declar.

   ItemList &ItemList::operator+=(const ItemList &);
   }; 

class Item
   {
   Item *next;
   int  value;
   char *name;

   friend class ItemList;   // Item list routines need to initialize data.
   };



#endif // _TOOLKIT_H
