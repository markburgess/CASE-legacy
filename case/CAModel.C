/*****************************************************************************/
/*                                                                           */
/* CAModel.h: Header file for CAModel object.                                */
/*                                                                           */
/*****************************************************************************/

/* $Id: CAModel.C,v 1.4 1997/01/28 17:23:54 mark Exp $ */

#include <CAModel.h>
#include <stdlib.h>

/*****************************************************************************/
/* CAModel object:                                                           */
/*****************************************************************************/

CAModel::CAModel()
{
environ=NULL;
visual=NULL;
cell=NULL;
cells=0;
}

/*****************************************************************************/

CAModel::CAModel(Widget new_parent)
{
 environ=NULL;
 visual=NULL;
 cell=NULL;
 cells=0;
 parent = new_parent;
 cell_req = NULL;
 environ_req = NULL;
}

/*****************************************************************************/

CAModel::~CAModel()
{
 delete environ;
 delete visual;
 delete [] cell;
}

/*****************************************************************************/

/*void CAModel::Initialize()
{
}*/

/*****************************************************************************/

void CAModel::Redraw()
{
 int i;
 
 visual->ClearAll();
 
 for(i = 0; i < cells; i++)
    {
    Draw(i);
    }

 environ->DrawGrid(visual);
}

/*****************************************************************************/

void CAModel::PopupCellRequester(int cell_index)
{
 CACell *current = cell[cell_index];
 if (cell_req==NULL)
   {
   cell_req = new Req(parent,"Cell requester");
   cell_req->SetCallbacks(cell_ok, cell_cancel, (XtPointer) this);
   current->SetupRequester(cell_req);
   }

 if (cell_req->PoppedUp())
    {
    cell[active_cell]->ReadWidgets(cell_req);
    Draw(active_cell);
    }
 active_cell = cell_index;
 cell[active_cell]->UpdateWidgetValue(cell_req);
 cell_req->Popup();
}

/*****************************************************************************/

void CAModel::PopupEnvironRequester()
{
 if (environ_req==NULL)
   {
   environ_req = new Req(parent,"Environ requester");
   environ_req->SetCallbacks(environ_ok, environ_cancel, (XtPointer) this);
   environ->SetupRequester(environ_req);
   }
 if (!environ_req->PoppedUp())
    {
    environ_req->Popup();
    }
}

/*****************************************************************************/

void CAModel::ReadEnvironRequester()
{
 environ->ReadRequester(environ_req);
 environ->DrawGrid(visual);
}

/*****************************************************************************/

void CAModel::CoordinatesChanged()
{
 Redraw();
}

/*****************************************************************************/

void CAModel::Click(double x, double y)
{
 int cell_index;

 cell_index = environ->QueryCell(x, y);
 PopupCellRequester(cell_index);
}

/*****************************************************************************/

void CAModel::RightClick(double x, double y)
{
 int cell_index;

 cell_index = environ->QueryCell(x, y);
 PopupCellRequester(cell_index);
 /* To be defined by user */
}

/*****************************************************************************/

Widget CAModel::GetToplevelWidget()
{
 return parent;
}
/*****************************************************************************/

void CAModel::environ_ok(Req *req, XtPointer data)
{
 CAModel *mod = (CAModel*) data;
 mod->ReadEnvironRequester();
 mod->Redraw();
}

/*****************************************************************************/

void CAModel::environ_cancel(Req *req, XtPointer data)
{
}

/*****************************************************************************/

void CAModel::cell_ok(Req *req, XtPointer data)
{
 CAModel *mod = (CAModel*) data;
 CACell *current = mod->cell[mod->active_cell];

 current->ReadWidgets(req);
 mod->Draw(mod->active_cell);
}

/*****************************************************************************/

void CAModel::cell_cancel(Req *req, XtPointer data)
{
}

/*****************************************************************************/
/* CAModelList object:                                                       */
/*****************************************************************************/

CAModelList::CAModelList()
{
 current = this;
 next = NULL;
}

/*****************************************************************************/

CAModelList::~CAModelList()

{
 if(model)
    {
    delete model;
    }

 if(next)
    {
    delete next;
    }
}

/*****************************************************************************/

void CAModelList::Add(CAModel *new_model)
{
 current = new CAModelList;

 current->next = next;
 current->model = new_model;

 next = current;
}

/*****************************************************************************/

int CAModelList::Remove(CAModel *dead_meat)

{
 CAModelList *tmp;
 if(next == NULL)
    {
    return(0);
    }
 
 if(next->model == dead_meat)
    {
    tmp = next;
    next = tmp->next;
    tmp->next = NULL;
    delete tmp;
    return(1);
    }
 
 return(next->Remove(dead_meat));
}

/*****************************************************************************/

void CAModelList::Reset()
{
 current = this;
}

/*****************************************************************************/

CAModel *CAModelList::Next()
{
 if(current->next)
    {
    current = current->next;
    return(current->model);
    }
 return(NULL);
}









