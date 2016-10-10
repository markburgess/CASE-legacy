/*****************************************************************************/
/*                                                                           */
/* filereq.C: A C++ File Requester class object for X11.                     */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1995,1996 Thomas Engh Sevaldrud <thomasen@a.sn.no>          */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Viewport.h>

#include "taglist.h"
#include "filereq.h"

/****************************************************************************/
/* MACROS                                                                   */
/****************************************************************************/
#define FONTHEIGHT(f) ((f)->max_bounds.ascent + \
		       (f)->max_bounds.descent)

/****************************************************************************/
/* PUBLIC FUNCTIONS:                                                        */
/****************************************************************************/

FileRequester::FileRequester()
{
 rf_Parent = NULL;
 init_tags();
}

/****************************************************************************/

FileRequester::FileRequester(Widget parent, ...)
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent);
 init_tags();
 
 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    value = va_arg(parameters, ULONG);
    taglist.SetValue(tag, value);
    }
 va_end(parameters);

 rf_Parent = parent;

 initialize();
}

/****************************************************************************/

FileRequester::~FileRequester()
{

#ifdef DEBUG
 printf("freeing rf_File.\n");
#endif
 free(rf_File);

#ifdef DEBUG
 printf("freeing rf_Dir.\n");
#endif
 free(rf_Dir);

 free_file_list();
}

/****************************************************************************/

void FileRequester::Initialize(Widget parent, ...)
{
 ULONG tag = 1;
 ULONG value;
 va_list parameters;
 va_start(parameters, parent);

 /* RESOLVE TAGLIST PARAMETERS */
 while(tag != TAG_DONE)
    {
    tag = va_arg(parameters, ULONG);
    value = va_arg(parameters, ULONG);
    taglist.SetValue(tag, value);
    }
 va_end(parameters);

 rf_Parent = parent;

 initialize();
}

/****************************************************************************/

int FileRequester::Popup(void)
{
 Arg wargs[2];

 XtSetArg(wargs[0], XtNx, taglist.GetValue(FRQ_LeftEdge));
 XtSetArg(wargs[1], XtNy, taglist.GetValue(FRQ_TopEdge));
 XtSetValues(rf_Toplevel, wargs, 2);

 if(rf_Toplevel != NULL)
    {
    XtRealizeWidget(rf_Toplevel);
    }
 else
    {
    printf ("File Requester hasn't got a parent (poor thing!)\n");
    return(0);
    }

 return(1);
}

/****************************************************************************/

char *FileRequester::GetFileName()
{
 return rf_File;
}

/****************************************************************************/

char *FileRequester::GetDirName()
{
 return rf_Dir;
}

/****************************************************************************/
/* PRIVATE FUNCTIONS:                                                       */ 
/****************************************************************************/

void FileRequester::initialize()
{
 int i, j;
 Widget parent_button, home_button, root_button;
 XtTranslations string_trans_table;
 XawTextEditType edit_type = XawtextRead;
 Boolean caret_on = False;
 Arg wargs[1];
 int list_width, list_height;
 int button_width, button_height;
 short done = 0;

 rf_Dir = (char *)malloc(MAX_PATHNAME_LENGTH);
 bzero(rf_Dir, MAX_PATHNAME_LENGTH);

 if(taglist.GetValue(FRQ_Dir) != (ULONG)NULL)
    {
    strcpy(rf_Dir, (char *)taglist.GetValue(FRQ_Dir));
    if(chdir(rf_Dir) != 0)
       {
       perror("chdir");
       }
    }      
 else
    {
    if(getcwd(rf_Dir, MAX_PATHNAME_LENGTH) == 0)
       {
       perror("getcwd");
       }
    }
#ifdef DEBUG
 printf("Current directory is: %s\n", rf_Dir);
#endif
 
 get_file_list(); 

 rf_File = (char *)malloc(MAX_FILENAME_LENGTH);

 printf("Got filelist\n");

 bzero(rf_File, MAX_FILENAME_LENGTH);
 if(taglist.GetValue(FRQ_File) != (ULONG)NULL)
     strcpy(rf_File, (char *)taglist.GetValue(FRQ_File));
/*  else
    strcpy(rf_File, rf_FileList[0]);*/

 printf("Default file set\n");

 /* 
 ** Check if we have a FRQ_Save requester. If so, change edit type to append
 ** and display the caret cursor.
 */

 if(taglist.GetValue(FRQ_Type) == FRQ_Save)
    {
    edit_type = XawtextAppend;
    caret_on = True;
    }

 /*
 ** Set a function to be called when the file requester is finished.
 */
 file_done_func = (void (*)(FileRequester *))taglist.GetValue(FRQ_FileDoneFunc);

 /* 
 ** Set/Calculate some sizes for the widgets:
 */
 button_height = 20;
 button_width = 80;
 list_width = taglist.GetValue(FRQ_Width) - button_width - 15;
 list_height = taglist.GetValue(FRQ_Height) - 3*button_height - 45;

 /*
 ** Initialize the widgets
 */
#ifdef DEBUG
 printf("Setting up widgets:\n");
#endif
 
 rf_Toplevel = XtVaAppCreateShell(NULL, "FileRequester", applicationShellWidgetClass, XtDisplay(rf_Parent), 
				  XtNtitle, taglist.GetValue(FRQ_Title),
				  XtNwidth, taglist.GetValue(FRQ_Width),
				  XtNheight,taglist.GetValue(FRQ_Height),
				  NULL);


#ifdef DEBUG
 printf("Toplevel, \n");
#endif

 rf_Form = XtVaCreateManagedWidget("fileform",formWidgetClass,rf_Toplevel,
				   XtNresize, False,
				   XtNvertDistance, 15,
				   XtNhorizDistance, 15,
				   NULL);
#ifdef DEBUG
 printf("Form, ");
#endif
 
 rf_PathNameWidget = XtVaCreateManagedWidget("pathnamewidget", labelWidgetClass ,rf_Form, 
					     XtNwidth, list_width,
					     XtNheight, button_height,
					     XtNborderWidth, 0,
					     XtNlabel, rf_Dir,
					     NULL);
#ifdef DEBUG
 printf("PathNameWidget, ");
#endif
 
 rf_Scroll = XtVaCreateManagedWidget("filescroll",viewportWidgetClass,rf_Form,
				     XtNfromVert, rf_PathNameWidget,
				     XtNwidth, list_width,
				     XtNheight, list_height,
				     XtNforceBars, True,
				     XtNallowHoriz, True,
				     XtNallowVert, True,
				     XtNuseBottom, True,
				     XtNuseRight, True,
				     NULL);
#ifdef DEBUG
 printf("Scroll, ");
#endif


 rf_ListView = XtVaCreateManagedWidget("filelist",listWidgetClass,rf_Scroll,
				       XtNlist, rf_FileList,
				       XtNdefaultColumns, 1,
				       NULL);

 XtAddCallback(rf_ListView, XtNcallback, (XtCallbackProc)&FileRequester::current_file_notify,(XtPointer)this);
#ifdef DEBUG
 printf("ListView, ");
#endif
 
 parent_button = XtVaCreateManagedWidget("parent",commandWidgetClass,rf_Form,
					 XtNlabel, "Parent",
					 XtNfromHoriz, rf_Scroll,
					 XtNfromVert, rf_PathNameWidget,
					 XtNleft, XawChainRight,
					 XtNright, XawChainRight,
					 XtNtop, XawChainTop,
					 XtNbottom, XawChainTop,
					 XtNwidth, button_width,
					 XtNheight, button_height,
					 NULL);
 XtAddCallback(parent_button, XtNcallback, &FileRequester::goto_parent,(XtPointer)this);
#ifdef DEBUG
 printf("ParentButton, ");
#endif

 home_button = XtVaCreateManagedWidget("home",commandWidgetClass,rf_Form,
				       XtNlabel, "Home",
				       XtNfromHoriz, rf_Scroll,
				       XtNfromVert, parent_button,
				       XtNleft, XawChainRight,
				       XtNright, XawChainRight,
				       XtNtop, XawChainTop,
				       XtNbottom, XawChainTop,
				       XtNwidth, button_width,
				       XtNheight, button_height,
				       NULL);
 XtAddCallback(home_button, XtNcallback, &FileRequester::goto_home,(XtPointer)this);

#ifdef DEBUG
 printf("home_button, ");
#endif

 root_button = XtVaCreateManagedWidget("root",commandWidgetClass,rf_Form,
				       XtNlabel, "Root",
				       XtNfromHoriz, rf_Scroll,
				       XtNfromVert, home_button,
				       XtNleft, XawChainRight,
				       XtNright, XawChainRight,
				       XtNtop, XawChainTop,
				       XtNbottom, XawChainTop,
				       XtNwidth, button_width,
				       XtNheight, button_height,
				       NULL);
 XtAddCallback(root_button, XtNcallback, &FileRequester::goto_root,(XtPointer)this);

#ifdef DEBUG
 printf("root_button, ");
#endif
 
 rf_FileNameWidget = XtVaCreateManagedWidget("filenamewidget", asciiTextWidgetClass ,rf_Form, 
					     XtNeditType,edit_type,
					     XtNdisplayCaret, caret_on,
					     XtNfromVert, rf_Scroll,
					     XtNstring, rf_File,
					     XtNleft, XawChainLeft,
					     XtNright, XawChainLeft,
					     XtNtop, XawChainBottom,
					     XtNbottom, XawChainBottom,
					     XtNwidth, list_width,
					     XtNheight, button_height,
					     NULL);

#ifdef DEBUG
 printf("FileNameWidget, ");
#endif
 
 rf_OKButton = XtVaCreateManagedWidget("filedone",commandWidgetClass,rf_Form,
				       XtNlabel, taglist.GetValue(FRQ_OKText),
				       XtNfromVert, rf_FileNameWidget,
				       XtNleft, XawChainLeft,
				       XtNright, XawChainLeft,
				       XtNtop, XawChainBottom,
				       XtNbottom, XawChainBottom,
				       XtNwidth, button_width,
				       XtNheight, button_height,
				       NULL);
 XtAddCallback(rf_OKButton,XtNcallback, &FileRequester::select_file,(XtPointer)this);

#ifdef DEBUG
 printf("OKButton, ");
#endif

 rf_CancelButton = XtVaCreateManagedWidget("filecancel",commandWidgetClass,rf_Form,
					   XtNlabel, taglist.GetValue(FRQ_CancelText),
					   XtNfromVert, rf_FileNameWidget,
					   XtNfromHoriz, rf_Scroll,
					   XtNleft, XawChainRight,
					   XtNright, XawChainRight,
					   XtNtop, XawChainBottom,
					   XtNbottom, XawChainBottom,
					   XtNwidth, button_width,
					   XtNheight, button_height,
					   NULL);
 XtAddCallback(rf_CancelButton,XtNcallback,&FileRequester::cancel_file_req, this);

#ifdef DEBUG
 printf("CancelButton.\n ");
#endif

}
/****************************************************************************/

void FileRequester::goto_parent(Widget w, XtPointer data, XtPointer nothing)
{
 FileRequester *fr = (FileRequester *)data;

 strcpy(fr->rf_File, "..");
 fr->change_dir();

 XtVaSetValues(fr->rf_PathNameWidget,
	       XtNlabel, fr->rf_Dir,
	       NULL);

}
/****************************************************************************/
void FileRequester::goto_home(Widget w, XtPointer data, XtPointer nothing)
{
 FileRequester *fr = (FileRequester *)data;
 char *ptr;

 bzero(fr->rf_Dir, MAX_PATHNAME_LENGTH);
 ptr = getenv("HOME");
 strcpy(fr->rf_File, ptr);
 fr->change_dir();
 
 XtVaSetValues(fr->rf_PathNameWidget,
	       XtNlabel, fr->rf_Dir,
	       NULL);
}
/****************************************************************************/
void FileRequester::goto_root(Widget w, XtPointer data, XtPointer nothing)
{
 FileRequester *fr = (FileRequester *)data;

 bzero(fr->rf_Dir, MAX_PATHNAME_LENGTH);
 strcpy(fr->rf_File, "/");
 fr->change_dir();

 XtVaSetValues(fr->rf_PathNameWidget,
	       XtNlabel, fr->rf_Dir,
	       NULL);

}

/****************************************************************************/
void FileRequester::select_file(Widget w, XtPointer data, XtPointer nothing)
{
 FileRequester *fr = (FileRequester *)data;
 Arg wargs[1];
 char link[MAX_PATHNAME_LENGTH];
 String filename;
 XawListReturnStruct *item;
 struct stat statbuf;
 Boolean done = False;
 XEvent event;
 
 /* Check if a file name is given in the string widget */
 XtSetArg(wargs[0], XtNstring, &filename);
 XtGetValues(fr->rf_FileNameWidget, wargs, 1);
#ifdef DEBUG
 printf("Length of FileNameWidget string: %d\n", strlen(filename));
#endif
 
 /* If no filename is given in string widget, use name from list widget */
 if(strlen(filename) == 0)
    {
#ifdef DEBUG
    printf("Getting file name from ListView widget\n");
#endif
    item = XawListShowCurrent(fr->rf_ListView);
    sscanf(item->string, "%s %*s %*s", fr->rf_File);
    }
 else
     strcpy(fr->rf_File, filename);

#ifdef DEBUG
 printf("Got filename: %s\n", fr->rf_File);
#endif
 
 /* Sjekk om den valgte filen er et directory */
 while(! done)
    {
    if(lstat(fr->rf_File, &statbuf) == -1)
       {
       if(fr->taglist.GetValue(FRQ_Type) == FRQ_Save)
	  {
#ifdef DEBUG
	  printf("new file: %s\n", fr->rf_File);
#endif
	  XtUnrealizeWidget(fr->rf_Toplevel);
	  if(fr->taglist.GetValue(FRQ_FileDoneFunc) != (ULONG)NULL)
	     {
	     (*fr->file_done_func)(fr);
	     }
	  }
       else
	  {
	  perror("select_file(): stat");
	  }
       break;
       }
    if(S_ISDIR(statbuf.st_mode))
       {
#ifdef DEBUG
       printf("error: %s is a directory!\n", fr->rf_File);
#endif
       fr->select_dir();
       done = True;
       }
    else if(S_ISLNK(statbuf.st_mode))
       {
       bzero(link, MAX_PATHNAME_LENGTH); /* readlink does not append NUL character... */
       readlink(fr->rf_File, link, MAX_PATHNAME_LENGTH);
       if(link[0] == '/')
	   bzero(fr->rf_Dir, MAX_PATHNAME_LENGTH); /* If we have an absolute path.. */
#ifdef DEBUG
       printf("selected link: %s -> %s\n", fr->rf_File, link);
#endif
       strcpy(fr->rf_File, link);
       }
    else 
       {
#ifdef DEBUG
       printf("selected file: %s\n", fr->rf_File);
#endif
       XtUnrealizeWidget(fr->rf_Toplevel);
       if(fr->taglist.GetValue(FRQ_FileDoneFunc) != (ULONG)NULL)
	  {
	  (*fr->file_done_func)(fr);
	  }
       done = True;
       }
    }
}
/****************************************************************************/
void FileRequester::select_dir()
{
 Arg wargs[1];
 char link[MAX_PATHNAME_LENGTH];
 XawListReturnStruct *item;
 struct stat statbuf;
 Boolean done = False;
 
 XtVaSetValues(rf_FileNameWidget,
	       XtNstring, "",
	       NULL);

 item = XawListShowCurrent(rf_ListView);
 bzero(rf_File, MAX_FILENAME_LENGTH);
 sscanf(item->string, "%s %*s %*s", rf_File);

#ifdef DEBUG
 printf("selected directory: %s\n", rf_File);
#endif
 
 /* Sjekk om den valgte filen er et directory */
 while(! done)
    {
    if(lstat(rf_File, &statbuf) == -1)
       {
       perror("select_dir): stat");
       break;
       }
    if(S_ISDIR(statbuf.st_mode))
       {
       change_dir();
       XtSetArg(wargs[0], XtNlabel, rf_Dir);
       XtSetValues(rf_PathNameWidget, wargs, 1);
       done = True;
       }
    else if(S_ISLNK(statbuf.st_mode))
       {
       bzero(link, MAX_PATHNAME_LENGTH); /* readlink does not append NUL character... */
       readlink(rf_File, link, MAX_PATHNAME_LENGTH);
       if(link[0] == '/')
	   bzero(rf_Dir, MAX_PATHNAME_LENGTH); /* If we have an absolute path.. */
#ifdef DEBUG
       printf("selected link: %s -> %s\n", rf_File, link);
#endif
       strcpy(rf_File, link);
       }
    else 
       {
#ifdef DEBUG
       printf("error: %s is not a directory!\n", rf_File);
#endif
       done = True;
       }
    }
}

/****************************************************************************/

void FileRequester::cancel_file_req(Widget w, XtPointer data, XtPointer nothing)
{
 FileRequester *fr = (FileRequester *)data;

 XtUnrealizeWidget(fr->rf_Toplevel);
}  

/****************************************************************************/

void FileRequester::current_file_notify(Widget w, XtPointer data, XawListReturnStruct *item)
{
 FileRequester *fr = (FileRequester *)data;

 sscanf(item->string, "%s %*s %*s", fr->rf_File);

 XtVaSetValues(fr->rf_FileNameWidget,
	       XtNstring, fr->rf_File,
	       NULL);
}

/****************************************************************************/
/* FILE LIST TOOLKIT                                                        */
/****************************************************************************/

void FileRequester::get_file_list(void)
/*
** Denne rutinen henter en liste over filene i naavaerende katalog og skriver
** dette til et array av strenger.
*/
{
 DIR *dirh;
 struct dirent *dirp;
 struct stat statbuf;
 char *perm;
 char currdir[MAX_PATHNAME_LENGTH];
 char dir;
 int i;

 get_file_list_length();
 rf_FileList = (char **)malloc((rf_FileListLength+1)*sizeof(char *));
#ifdef DEBUG
 printf("allocated %d * sizeof(char *) bytes for file list\n\n",rf_FileListLength+1);
#endif
  
 /* Store current directory */
 if(getcwd(currdir, MAX_PATHNAME_LENGTH) == 0)
    {
    perror("getcwd");
    }

#ifdef DEBUG
 printf("Current directory is: %s\n", rf_Dir);
#endif
 
 if(chdir(rf_Dir) != 0)
    {
    perror("chdir");
    }

 if(!(dirh = opendir(rf_Dir)))
    {
    perror("opendir");
    exit(-1);
    }

 i = 0;
 for(dirp = readdir(dirh); dirp != NULL; dirp = readdir(dirh))
    {
    /* Hent litt info om filen: */
    if(lstat(dirp->d_name, &statbuf) == -1)
       {
       printf("%s:",dirp->d_name); 
       perror("get_file_list(): stat");
       continue;
       }
    if(S_ISDIR(statbuf.st_mode))
	dir = 'd';
    else dir = '-';
    perm = get_perm_string(statbuf.st_mode);
    /*
    ** Sett av minne nok til filnavnet, og rettighetene + 32 tegn til for filens
    ** stoerrelse + formattering av linjen.
    */
#ifdef DEBUG
    printf("allocating %d bytes for FileList[%d].\n", strlen(perm) + strlen(dirp->d_name) + 32, i);
#endif
    rf_FileList[i] = (char *)malloc(strlen(perm) + strlen(dirp->d_name) + 32);
    
    sprintf(rf_FileList[i], "%-20s %06ld %c%s", dirp->d_name, statbuf.st_size, dir, perm);
    i++;
    }
 rf_FileList[i] = NULL;
#ifdef DEBUG
 printf("FileList[%d] set to NULL\n", i);
#endif
 closedir(dirh);

 /* Change back to original directory */
 if(chdir(currdir) != 0)
    {
    perror("chdir");
    }

}

/****************************************************************************/

void FileRequester::free_file_list()
{
 int i = 0;
 
 while(rf_FileList[i] != NULL)
    {
/*      if(DEBUG) printf("freeing rf_FileList[%d].\n", i);*/
    free(rf_FileList[i]);
    i++;
    }
#ifdef DEBUG
 printf("freeing rf_FileList[%d].\n", i);
#endif
 free(rf_FileList[i]);
 free(rf_FileList);
}

/****************************************************************************/

char *FileRequester::get_perm_string(unsigned short mode)
{
 char *ret;
 char buf[10]; 
 char str[8];
 int i;
 bzero(buf, 10);
 bzero(str, 6);

 mode &= 07777;
  
 sprintf(str, "%o", mode);

 for(i = 0; i < 3; i++)
    {
    if(str[i] == '0')
       {
       strcat(buf, "---");
       }
    else if(str[i] == '1')
       {
       strcat(buf, "--x");
       }
    else if(str[i] == '2')
       {
       strcat(buf, "-w-");
       }
    else if(str[i] == '3')
       {
       strcat(buf, "-xw");
       }
    else if(str[i] == '4')
       {
       strcat(buf, "r--");
       }
    else if(str[i] == '5')
       {
       strcat(buf, "r-x");
       }
    else if(str[i] == '6')
       {
       strcat(buf, "rw-");
       }
    else strcat(buf, "rwx");
    }
 ret = (char *)malloc (10);
 strcpy(ret, buf);
 return ret;
}

/****************************************************************************/

int FileRequester::longest_filename(char *dirname)
{
 /*
 DIR *dirh;
 struct dirent *dirp;
 int longest = 0;

 if(!(dirh = opendir(dirname)))
    {
    perror("opendir");
    }

 for(dirp = readdir(dirh); dirp != NULL; dirp = readdir(dirh))
    {
    if(strlen(dirp->d_name) > longest) longest = strlen(dirp->d_name);
    }
 closedir(dirh);
#ifdef DEBUG
 printf("Longest filename in directory %s is %ld characters wide\n", dirname, longest);
#endif
 return longest;
 */
}

/****************************************************************************/

void FileRequester::get_file_list_length(void)
{
 DIR *dirh;
 struct dirent *dirp;

 if(!(dirh = opendir(rf_Dir)))
    {
    perror("opendir");
    }
 rf_FileListLength = 0;

 for(dirp = readdir(dirh); dirp != NULL; dirp = readdir(dirh))
    {
    rf_FileListLength++;
    }
 closedir(dirh);
}

/****************************************************************************/

int FileRequester::change_dir(void)
{

 if(strcmp(".", rf_Dir) == 0)
    {
#ifdef DEBUG
    printf("Staying in directory\n");
#endif
    return 1;
    }
  
/*  if(strcmp("/", rf_Dir) != 0)*/
 if(rf_Dir[strlen(rf_Dir)] != '/')
    {
    strcat(rf_Dir, "/");
    }
 strcat(rf_Dir, rf_File);

#ifdef DEBUG
 printf("Changing to directory: %s\n", rf_Dir);
#endif
 
 if(chdir(rf_Dir) != 0)
    {
    perror("chdir");
    }

 if(getcwd(rf_Dir, MAX_PATHNAME_LENGTH) == 0)
    {
    perror("getcwd");
    }

#ifdef DEBUG
 printf("Current directory is: %s\n", rf_Dir);
#endif
 
 free_file_list();
 get_file_list();

 XawListChange(rf_ListView, rf_FileList, 0,0,True);
 return 1;
}

/****************************************************************************/

void FileRequester::init_tags()
{
 taglist.InitTags(FRQ_TAGS,
		  FRQ_Title, "FileRequester",
		  FRQ_Width, 310,
		  FRQ_Height, 310,
		  FRQ_LeftEdge, 10,
		  FRQ_TopEdge, 10,
		  FRQ_OKText, "OK",
		  FRQ_ChDirText, "Change Dir",
		  FRQ_CancelText, "Cancel",
		  FRQ_File, "",
		  FRQ_Dir, NULL,
		  FRQ_Type, FRQ_Load,
		  FRQ_FileDoneFunc, NULL);
}

/****************************************************************************/





