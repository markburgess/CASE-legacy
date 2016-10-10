/*****************************************************************************/
/*                                                                           */
/* filereq.C: A C++ File Requester class object for X11.                     */
/*                                                                           */
/* Version: 1.0                                                              */
/*    date: may 12 1996                                                      */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1995, 1996 Thomas E. Sevaldrud <thomasen@a.sn.no>           */
/*                         Jon Anders Mikkelsen <mikkelj@iu.hioslo.no>       */
/*****************************************************************************/

#ifndef _FILEREQ_H
#define _FILEREQ_H 1

#include <types.h>
#include <taglist.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/List.h>

/*****************************************************************************/
/* DEFINITIONS:                                                              */
/*****************************************************************************/

#define MAX_PATHNAME_LENGTH 255
#define MAX_FILENAME_LENGTH 255

/* TAGITEMS */
#define FRQ_Title 0
#define FRQ_Width 1
#define FRQ_Height 2
#define FRQ_LeftEdge 3
#define FRQ_TopEdge 4
#define FRQ_OKText 5
#define FRQ_ChDirText 6
#define FRQ_CancelText 7
#define FRQ_File 8
#define FRQ_Dir 9
#define FRQ_Type 10
#define FRQ_FileDoneFunc 11
#define FRQ_TAGS 12

/* Filerequester type flags */
#define FRQ_Save 0
#define FRQ_Load 1

/*****************************************************************************/
/* DATATYPES:                                                                */
/*****************************************************************************/

class FileRequester
{
public:
   FileRequester();
   FileRequester(Widget parent,...);
   ~FileRequester();

   void Initialize(Widget parent,...);
   int  Popup();
   char *GetFileName();
   char *GetDirName();
   
private:
   void initialize();
   static void goto_parent(Widget w, XtPointer data, XtPointer nothing);
   static void goto_home(Widget w, XtPointer data, XtPointer nothing);
   static void goto_root(Widget w, XtPointer data, XtPointer nothing);
   static void select_file(Widget w, XtPointer data, XtPointer nothing);
   void select_dir();
   static void cancel_file_req(Widget w, XtPointer data, XtPointer nothing);

   int  change_dir();
   void get_file_list();
   void free_file_list();
   void get_file_list_length();
   int  longest_filename(char *dirname);
   char *get_perm_string(unsigned short mode);
   static void current_file_notify(Widget w, XtPointer data, XawListReturnStruct *item);

   void (*file_done_func)(FileRequester *);

   TagList taglist;
   void init_tags();

   char *rf_File;		/* Filename pointer                          */
   char *rf_Dir;       		/* Directory name pointer                    */
   char **rf_FileList;          /* The file list                             */
   WORD rf_FileListLength;      /* no. of items in file_list                 */
   Widget rf_Parent;		/* Parent widget of file requester           */
   Widget rf_Toplevel;          /* File requester shell widget               */
   Widget rf_Form;              /* File requester form widget                */
   Widget rf_ListView;          /* The file list view widget                 */
   Widget rf_Scroll;            /* The scroller (viewport) widget            */
   Widget rf_PathNameWidget;	/* Pathname widget                           */
   Widget rf_FileNameWidget;    /* File name widget (for save requesters)    */
   Widget rf_OKButton;          /* OK button                                 */
   Widget rf_CancelButton;      /* Cancel button                             */
};

/*****************************************************************************/
#endif

  












