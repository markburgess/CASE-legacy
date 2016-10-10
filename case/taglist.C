/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include "taglist.h"

/****************************************************************************/

TagList::TagList()
{
 n_tags = 0;
 tags = NULL;
}

/****************************************************************************/

void TagList::InitTags(int new_ntags, ...)
{
 ULONG tag = 1;
 ULONG value;
 int i;
 va_list parameters;
 va_start(parameters, new_ntags);
 n_tags = new_ntags;
 tags = new struct TagItem[n_tags];
 
 for(i = 0; i < n_tags; i++)
    {
    tag = va_arg(parameters, ULONG);
    value = va_arg(parameters, ULONG);
    tags[i].ti_tag = tag;
    tags[i].ti_data = value;
    }
 va_end(parameters);
}

/****************************************************************************/

TagList::~TagList()
{
 delete[] tags;
}

/****************************************************************************/

int TagList::SetValue(ULONG item, ULONG value)
{
 int i;
 
 for(i = 0; i < n_tags; i++)
    {
    if(tags[i].ti_tag == item)
       {
       tags[i].ti_data = value;
       return 1;
       }
    }

 printf("warning: tag not found\n");
 return 0;
}

/****************************************************************************/

ULONG TagList::GetValue(ULONG item)
{
 int i;
 
 for(i = 0; i < n_tags; i++)
    {
    if(tags[i].ti_tag == item)
       {
       return tags[i].ti_data;
       }
    }

 printf("warning: tag not found\n");
 return 0;
}

/****************************************************************************/

