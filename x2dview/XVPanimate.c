/*********************************************************************/
/* XVPanimate.c:                                                     */
/* Animation demo for XVPlib V0.1                                    */
/* (c)1995 Thomas Engh Sevaldrud                                     */  
/*********************************************************************/

#include <XVPlib.h>
#include <conf.h>

void RefreshGfx();
void RotateLeft();
void RotateRight();
void DrawGraphics(viewport vp);
void DrawLines(viewport vp);
void WriteText();

/*********************************************************************/
/* Level 0 : main()                                                  */
/*********************************************************************/
viewport vp;
matrix rot_left, rot_right;
point_coordinates x,y,x2,y2, StarX, StarY;

main (int argc,char **argv)
{ 
  XtAppContext AC;
  Widget TOPLEVEL, DRAWBOX, WIN;
  Widget dbox;

  /* X logo coordinates*/
  x[0] = -1.36; y[0] = -1.91;
  x[1] = -0.71; y[1] = -0.96;
  x[2] = -1.76; y[2] =  0.29;
  x[3] = -1.76; y[3] =  0.29;
  x[4] = -1.01; y[4] = -0.81;
  x[5] = -1.76; y[5] = -1.91;
  x[6] = -1.36; y[6] = -1.91;

  x2[0] = -0.22; y2[0] =  0.30;
  x2[1] = -0.87; y2[1] = -0.65;
  x2[2] = -0.02; y2[2] = -1.90;
  x2[3] =  0.18; y2[3] = -1.90;
  x2[4] = -0.57; y2[4] = -0.80;
  x2[5] = -0.18; y2[5] =  0.3;
  x2[6] = -0.22; y2[6] =  0.3;
  /*** KOORDINATES FOR STAR ****/
  StarX[0] = -130.901; StarY[0] =  29.384;
  StarX[1] =  -50.000; StarY[1] = -29.384;
  StarX[2] =  -80.902; StarY[2] =-124.490;
  StarX[3] =    0.000; StarY[3] = -62.245;
  StarX[4] =   80.902; StarY[4] =-124.490;
  StarX[5] =   50.000; StarY[5] = -29.384;
  StarX[6] =  130.901; StarY[6] =  29.384;
  StarX[7] =   30.901; StarY[7] =  29.384;
  StarX[8] =    0.000; StarY[8] = 124.490;
  StarX[9] =  -30.901; StarY[9] =  29.384;

  make_identity(rot_left);
  make_identity(rot_right);
  rotate(rot_left,2);
  rotate(rot_right,-2);

  TOPLEVEL = XtVaAppInitialize(&AC,"CASE-model",NULL,0,&argc,argv,NULL,NULL);
  WIN = XtVaCreateManagedWidget("mainwin",formWidgetClass,TOPLEVEL,NULL);
  dbox = XtVaCreateManagedWidget("drawpane",panedWidgetClass,WIN,NULL);
/*  DRAWBOX = XtVaCreateManagedWidget("drawbox",boxWidgetClass,dbox,NULL);*/

  XtAddEventHandler(WIN, VisibilityChangeMask,FALSE, RefreshGfx,NULL);
  XtAddEventHandler(WIN, Button1MotionMask, FALSE, RotateLeft,NULL);
  XtAddEventHandler(WIN, Button3MotionMask, FALSE, RotateRight,NULL);
  XtAddEventHandler(WIN, ButtonReleaseMask, FALSE, WriteText,NULL);
  XtRealizeWidget(TOPLEVEL);

  XVPaddGraphicsWindow(&vp, dbox, WIN, 4, 4, -1, -1);
  XVPbuildColourMap(&vp);
  XVPsetViewport(&vp, -200.0, 200.0, -200.0, 200.0);
  XVPclearPixmap(&vp);

  WriteText();
  XVPdrawPolygon(10,StarX,StarY,600,100,200,vp);

  XtAppMainLoop(AC);
  XFreePixmap(vp.display, vp.pixmap);
}
/*********************************************************************/
/* Level 1                                                           */
/*********************************************************************/

void RefreshGfx()
{
  if(DEBUG) printf("\nIn RefreshGfx()\n");
  XVPupdateGraphics(vp);                  
}
void WriteText()
{
  char *text1 = "Press left mousebutton and move mouse to rotate box left.";
  char *text2 = "Press right mousebutton and move pointer to rotate right...";

  XVPdrawText(text1, strlen(text1), -190,190, 0,0,0,vp); 
  XVPdrawText(text2, strlen(text2), -190,180, 0,0,0,vp); 
}
void RotateLeft()
{
  transform_points (10,StarX,StarY,StarX,StarY,rot_left);
  XVPclearPixmap(&vp);
  XVPpixmapDrawPolygon(10,StarX,StarY,600,100,200,vp);
  XVPupdateGraphics(vp);                  
}
void RotateRight()
{
  transform_points (10,StarX,StarY,StarX,StarY,rot_right);
  XVPclearPixmap(&vp);
  XVPpixmapDrawPolygon(10,StarX,StarY,600,100,200,vp);
  XVPupdateGraphics(vp);                  
}

/*********************************************************************/

void DrawGraphics(viewport vp)
{
  float x,y;
  y = 0;
  for(x = vp.xwmin ; x < vp.xwmax; x += vp.pixx)
    {
      XVPdrawPoint(x, y, 600, 200, 100, vp);
    }
    
  x = 0;
  for(y = vp.ywmin ; y < vp.ywmax ; y += vp.pixy)
    {
      XVPdrawPoint(x, y, 600, 200, 100, vp);
    }
}
void DrawLines(viewport vp)
{
  XVPdrawLine(vp.xwmin,vp.ywmin,vp.xwmax,vp.ywmax, 100,600,200, vp);
  XVPdrawLine(vp.xwmin,vp.ywmax,vp.xwmax,vp.ywmin, 100,600,200, vp);
}

/*********************************************************************/
/* Level 2                                                           */
/*********************************************************************/













