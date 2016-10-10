/****************************************************************************/
/*                                                                          */
/* This is the application root: the start of everything                    */
/*                                                                          */
/****************************************************************************/

#include <CAApp.h>
#include <XYModel.h>
#include <GoLModel.h>
#include <GameModel.h>
#include <XYBModel.h>
#include <TriModel.h>
#include <DiffModel.h>
#include <Periodic.h>
#include <Palladium.h>

class App : public CAApp
{
private:

protected:
   
public:
   
   App();
   ~App();
   
   virtual void Initialize(int argc, char **argv);
   static void NewGameRectModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewGoLRectModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewGoLTriModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewXYModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewXYBModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewTriModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewTriPattern(Widget w, XtPointer data, XtPointer garbage);
   static void NewDiffModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewPeriodicModel(Widget w, XtPointer data, XtPointer garbage);
   static void NewPalladiumModel(Widget w, XtPointer data, XtPointer garbage);
};


/****************************************************************************/
/* Main program                                                             */
/****************************************************************************/

main(int argc, char **argv)

{ App app;

app.Initialize(argc, argv);
app.Popup();
app.MainLoop();
}

/****************************************************************************/

App::App() :CAApp()

{
}

/****************************************************************************/

App::~App()

{
}

/****************************************************************************/

void App::Initialize(int argc, char **argv)

{ CAApp::Initialize(argc, argv);

model_menu.AddItem("Rect XY Model", NewXYModel, (XtPointer) this);
model_menu.AddItem(".... Quantum XY in magnetic field", NewXYBModel, (XtPointer) this);
model_menu.AddItem(".... Game of life (Rectangular)", NewGoLRectModel, (XtPointer) this);
model_menu.AddItem(".... GameGraph (Rectangular)", NewGameRectModel, (XtPointer) this);

model_menu.AddItem("Tri. Neighbours", NewTriModel, (XtPointer) this);
model_menu.AddItem(".... Patterns (demo)", NewTriPattern, (XtPointer) this);
model_menu.AddItem(".... Period pattern detect", NewPeriodicModel, (XtPointer) this);
model_menu.AddItem(".... Game of life", NewGoLTriModel, (XtPointer) this);
 
model_menu.AddItem("Diffusion", NewDiffModel, (XtPointer) this);
model_menu.AddItem("Palladium simulation", NewPalladiumModel, (XtPointer) this);
}

/****************************************************************************/

void App::NewTriModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

CAModel *model = new TriModel(app->toplevel);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewTriPattern(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

CAModel *model = new TriPattern(app->toplevel);
app->model_list->Add(model);
}


/****************************************************************************/

void App::NewXYBModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

CAModel *model = new XYBModel(app->toplevel);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewXYModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

CAModel *model = new XYModel(app->toplevel);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewGoLRectModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

GameOfLife *model = new GameOfLife(app->toplevel,CARectangular);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewGameRectModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

Politics *model = new Politics(app->toplevel,CARectangular);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewGoLTriModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

GameOfLife *model = new GameOfLife(app->toplevel,CATriangular);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewDiffModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

Diffuse *model = new Diffuse(app->toplevel,CATriangular);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewPeriodicModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

Periodic *model = new Periodic(app->toplevel,CATriangular);
app->model_list->Add(model);
}

/****************************************************************************/

void App::NewPalladiumModel(Widget w, XtPointer data, XtPointer garbage)

{ App *app = (App*)data;

Palladium *model = new Palladium(app->toplevel,CATriangular);
app->model_list->Add(model);
}




