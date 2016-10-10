/*****************************************************************************/
/* taglist.h: Amiga Intuition-like taglist functions for X11.                */
/* (c) 1995 Thomas Engh Sevaldrud                                            */
/*****************************************************************************/

#ifndef _TAGLIST_H
#define _TAGLIST_H

#include "types.h"
#include <stdarg.h>
#include <stdio.h>

struct TagItem 
{
  ULONG ti_tag;			/* identifies the type of this item */
  ULONG ti_data;		/* type-specific data, can be a pointer */
};

/* GLOBAL TAGS */
#define TAG_DONE -1

/*****************************************************************************/

class TagList
{
public:  
   TagList(); 
   ~TagList();

   int SetValue(ULONG tag, ULONG value);
   ULONG GetValue(ULONG tag);
   void InitTags(int n_tags, ...);

protected:
   struct TagItem *tags;
   int n_tags;
private:
};

/*****************************************************************************/

#endif
