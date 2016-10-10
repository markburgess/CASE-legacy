/*****************************************************************************/
/*                                                                           */
/* color.c: color handling routines                                          */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Thomas E. Sevaldrud <thomasen@a.sn.no>                 */
/*                    Steinar Johannesen <steinarj@iu.hioslo.no>             */
/*****************************************************************************/

#include "color.h"
#include <math.h>
#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <conf.h>
#include "X2Dview.h"

/*****************************************************************************/

void restore_vga_colors(Widget vp)
{
  X2DsetPaletteRGB (vp, 0 , 0, 0, 0); /* Black        */
  X2DsetPaletteRGB (vp, 1 , 0, 0,42); /* Blue         */
  X2DsetPaletteRGB (vp, 2 , 0,42, 0); /* Green        */
  X2DsetPaletteRGB (vp, 3 , 0,42,42); /* Cyan         */
  X2DsetPaletteRGB (vp, 4 ,42, 0, 0); /* Red          */
  X2DsetPaletteRGB (vp, 5 ,42, 0,42); /* Magenta      */
  X2DsetPaletteRGB (vp, 6 ,42,21, 0); /* Brown        */
  X2DsetPaletteRGB (vp, 7 ,42,42,42); /* LightGray    */
  X2DsetPaletteRGB (vp, 8 ,21,21,21); /* DarkGray     */
  X2DsetPaletteRGB (vp, 9 ,21,21,63); /* LightBlue    */
  X2DsetPaletteRGB (vp, 10,21,63,21); /* LightGreen   */
  X2DsetPaletteRGB (vp, 11,21,63,63); /* LightCyan    */
  X2DsetPaletteRGB (vp, 12,63,21,21); /* LightRed     */
  X2DsetPaletteRGB (vp, 13,63,21,63); /* LightMagenta */
  X2DsetPaletteRGB (vp, 14,63,63,21); /* Yellow       */
  X2DsetPaletteRGB (vp, 15,63,63,63); /* White        */
}

/*****************************************************************************/

void make_grey_scale_64a(Widget vp)

/* Creates a palette with 64 level greyscale in which the intensity
   goes smoothly from 0 to 1, so the difference between each value
   and the previous value is constant. The program must be initialized
   with initialize_vga2 and you need the bgi file  vga256.bgi */
{
 int i;

 for (i = 0; i < 64; i++)
    {
    X2DsetPaletteRGB (vp, i,i,i,i);
    }
}

/*****************************************************************************/

void make_grey_scale_16a(Widget vp)

/* Creates a palette with 16 level greyscale in which the intensity
   goes smoothly from 0 to 1, so the difference between each value
   and the previous value is constant. The program must be initialized
   with initialize (i) where i= 1,2 or 4 and you use the bgi file
   egavga.bgi */

{
 int i;

 for(i = 0; i < 16; i++)
    {
    X2DsetPaletteRGB (vp, i, 4 * i, 4 * i, 4 * i);
    }
}

/*  make_grey_scale_64a () og make_grey_scale_16a () could
    be concatenated into make_grey_scale_a */

/*****************************************************************************/

void make_grey_scale_a (Widget vp, int nc, int n)

/* Make a grey scale with nc levels where the intensity goes continuously
   from 0 to 1.. etc. The grey tones are inserted starting from colour  n
   in the palette. */
{
 float d;
 int i;
 d = 255.999 / (nc - 1);

 for(i = 0; i < nc; ++i)
    {
    X2DsetPaletteRGB (vp, n + i, d*i, (d * i), (d * i));
    }
}

/*****************************************************************************/

void make_grey_scale_m (Widget vp, int nc, int n, float vmin)

/* Make a grey scale with nc levels where the intensity goes continuously
   from 0 to 1.. etc. The intensity of the first tone is zero.
   The grey tones are inserted starting from colour n
   in the palette. */


{
 float mv,v;
 int i;

 X2DsetPaletteRGB(vp, n, 0, 0, 0);

 mv = pow (1 / vmin, 1 / (float) (nc - 2));
 v  = vmin;
 
 for (i = 1; i < nc; i++ )
    {
    X2DsetPaletteRGB (vp, n + i, v * 255.999 , v * 255.999, v * 255.999);
    v = v * mv;
    }
}

/*****************************************************************************/

float max3(float a, float b, float c)
{
  float x;
  x = a;
  if (b > x) x = b;
  if (c > x) x = c;
  return x;
} 

/*****************************************************************************/

float min3(float a, float b, float c)
{
 float x;
 x = a;
 if (b < x) x = b;
 if (c < x) x = c;
 return x;
} 

/*****************************************************************************/

void rgb_to_hsv (float r, float g, float b , float *h, float *s, float *v)

/* Red, Green and Blue (r,g,b) to Hue, Saturation, Value (h,s,v) in [0,1]
   When s=0, h has no value */

{
  float m,r1,g1,b1;

  *v = max3(r,g,b); /* intensiteten */
  m = min3(r,g,b);

  /* metningen */
  if (*v != 0)
    *s = (*v - m) / *v;
   else
    *s = 0;

  /* fargetonen */
  if (*s != 0)
  {
    if      (m == b) *h = 60 * (1 + (g - r) / (*v - m));
    else if (m == r) *h = 60 * (3 + (b - g) / (*v - m));
    else if (m == g) *h = 60 * (5 + (r - b) / (*v - m));
  }
  /* The three cases define h i the intervals [0,120], <120,240]
     og <240,360>, i.e. between red og green, green and blue and
     between blue and red. */
}

/*****************************************************************************/

void hsv_to_rgb (float h, float s, float v , float *r, float *g, float *b)

/* Hue h i [0,360], Saturations s og intensity v in [0,1]
   to r,g,b in [0,1]. */
{
  int i;
  float f,m,p1,p2;

  if (h == 360) h = 0; /* convert to degrees i [0,360> */
  h = h / 60;          /* h is now in [0,6> */

  i = h;
  f = h - i;

  m = v * (1 - s);
  p1 = (1 - f) * m +       f * v;
  p2 =       f * m + (1 - f) * v;

  /* When f increases from 0 til 1 p1 increases smoothly from m
     to v and p2 decreases smoothly from v to m. */

  switch ( i )
  {
    case 0 : *r = v;  *g = p1; *b = m;  break; /* betw. Red & yellow */
    case 1 : *r = p2; *g = v;  *b = m;  break; /* betw. yellow & green */
    case 2 : *r = m;  *g = v;  *b = p1; break; /* betw. green & turquoise  */
    case 3 : *r = m;  *g = p2; *b = v;  break; /* betw. turquoise & blue  */
    case 4 : *r = p1; *g = m;  *b = v;  break; /* betw. blue & purple */
    case 5 : *r = v;  *g = m;  *b = p2; break; /* betw. purple & red  */
  } 
} 

/*****************************************************************************/

void make_intensity_scale_a (Widget vp, int nc, int n , float h, float s)

/* Make a palette with nc colours, with hsv from 0 to 1, so that the
   diff. between each value and the previous is constant. Colours start
   from colour n in the palette. */

{
  float r,g,b,d;
  int i;
  
  d = 1 / (float)(nc - 1);

  for (i = 0; i < nc; i++)
  {
    hsv_to_rgb (h, s, d*i, &r, &g, &b);
    X2DsetPaletteRGB (vp, n + i, r * 255.999, g * 255.999, b * 255.999);
  }
} 

/*****************************************************************************/

void make_intensity_scale_m (Widget vp, int nc, int n , float h, float s, float vmin)

/* Create a palette with nc colours and hs, where first intensity v is zero
   and thereafter increases smoothly from a small value vmin to 1 so
   that the ratio between each value and previous is constant. Colours are
   inserted starting from n in the palette.
 */

{
  float mv,v,r,g,b;
  int i;
  X2DsetPaletteRGB(vp, n, 0, 0, 0);

  mv = pow (vmin, -1 / (float) (nc - 2));
  v = vmin;

  for (i = 1; i < nc; i++)
  {
    hsv_to_rgb (h,s,v, &r, &g, &b);
    X2DsetPaletteRGB (vp, n+i, r * 255.999, g * 255.999, b * 255.999);
    v *= mv;
  }
} 

/*****************************************************************************/

void blend (Widget vp, int i1, float r1, float g1, float b1,
            int i2, float r2, float g2, float b2)

/* Make a colour scale by following a straight line i the colour cube
 from (r1,g1,b1) to (r2,g2,b2). Colours are inserted from i1 to i2
 in the palette. */

{
   int n,i;
   float r,g,b;

   n = i2 - i1;

   for (i = 0; i <= n; i++)
   {
     r = ((n - i) * r1 + i * r2) / n;
     g = ((n - i) * g1 + i * g2) / n;
     b = ((n - i) * b1 + i * b2) / n;

     X2DsetPaletteRGB (vp, i1 + i, r * 255.999, g * 255.999, b * 255.999);
   }
} 

/*****************************************************************************/

void make_rgb_palette_a (Widget vp, int nr, int ng, int nb, int n,
                         float rmin, float rmax,
                         float gmin, float gmax,
                         float bmin, float bmax)

/* Make a palette with nr red cols between rmin and rmax, ng greens
   between gmin and gmax etc, so that the difference between each
   colour and previous rgb is constant. Colours (r,g,b) in this
   palette kan be referred to by integers r_int between 0 and nr-1,
   g_int from 0-ng-1 etc, and are inserted as colour number
   (r_int * ng*nb + g_int * nb + b_int + n) in the palette. */

{
  int i,r_int,g_int,b_int;
  float ar,ag,ab,r,g,b;

  if (nr == 1) ar = 0; else  ar = (rmax - rmin) / (nr - 1);
  if (ng == 1) ag = 0; else  ag = (gmax - gmin) / (ng - 1);
  if (nb == 1) ab = 0; else  ab = (bmax - bmin) / (nb - 1);

  i = n;
  r = rmin;

  for (r_int = 0; r_int < nr; r_int++)
  {
    g = gmin;

    for (g_int = 0; g_int < ng; g_int++)
    {
      b = bmin;

      for (b_int = 0; b_int < nb; b_int++)
      {
        X2DsetPaletteRGB (vp, i, r * 255.999, g * 255.999, b * 255.999);
        ++i;
        b += ab;
      } 

      g += ag;
    } 

    r += ar;
  }
} 

/*****************************************************************************/

void make_rgb_palette_m (Widget vp, int nr, int ng, int nb, int n,
                         float rmin, float rmax,
                         float gmin, float gmax,
                         float bmin, float bmax)

/* Make a palette with nr red cols between rmin and rmax, ng greens
   between gmin and gmax etc, so that the difference between each
   colour and previous rgb is constant. Colours (r,g,b) in this
   palette kan be referred to by integers r_int between 0 and nr-1,
   g_int from 0-ng-1 etc, and are inserted as colour number
   (r_int * ng*nb + g_int * nb + b_int + n) in the palette. */

{
  int i,r_int,g_int,b_int;
  float mr,mg,mb,r,g,b;

  if (nr == 1) mr = 1; else  mr = pow (rmax / rmin, 1 / (float) (nr - 1));
  if (ng == 1) mg = 1; else  mg = pow (gmax / gmin, 1 / (float) (ng - 1));
  if (nb == 1) mb = 1; else  mb = pow (bmax / bmin, 1 / (float) (nb - 1));

  i = n;
  r = rmin;

  for (r_int = 0; r_int < nr; r_int++ )
  {
    g = gmin;

    for (g_int = 0;  g_int < ng; g_int++ )
    {
      b = bmin;

      for (b_int = 0; b_int < nb; b_int++ )
      {
        X2DsetPaletteRGB (vp, i, r * 255.999, g * 255.999, b * 255.999);
	++i;
	b *= mb;
      } 

      g *= mg;
    } 

    r *= mr;
  } 
}

/*****************************************************************************/

void make_hsv_palette_a (Widget vp, int nh, int ns, int nv, int n,
                         float hmin, float hmax,
                         float smin, float smax,
                         float vmin, float vmax)

/* Make a palette with nh hues, between hmin, hmax, ns saturations
   smin to smax and nv values, so that the difference between each
   value and previous is constant. Colour (h,s,v) in this
   palette can be specified as an integer h_int betw. 0 and nh-1,
   s_int betw. 0 and ns-1 etc and is inserted as a colour
   (h_int * ns*nv + s_int * nv + v_int + n) in the paletten.
 */

{
  int i,h_int,s_int,v_int;
  float ah,as,av,h,s,v,r,g,b;

  if (nh == 1) ah = 0; else  ah = (hmax - hmin) / (nh - 1);
  if (ns == 1) as = 0; else  as = (smax - smin) / (ns - 1);
  if (nv == 1) av = 0; else  av = (vmax - vmin) / (nv - 1);

  i = n;
  h = hmin;

  for (h_int = 0; h_int < nh; h_int++)
  {
    s = smin;

    for (s_int = 0; s_int < ns; s_int++)
    {
      v = vmin;

      for (v_int = 0; v_int < nv; v_int++)
      {
        hsv_to_rgb (h, s, v, &r, &g, &b);
        X2DsetPaletteRGB (vp, i, r * 255.999, g * 255.999, b * 255.999);
        i++;
        v += av;
      } 

      s += as;
    } 

    h += ah;
  } 
} 

/*****************************************************************************/

void make_hsv_palette_m (Widget vp, int nh, int ns, int nv, int n,
                         float hmin, float hmax,
                         float smin, float smax,
                         float vmin, float vmax)

/* Make a palette with nh hues, between hmin, hmax, ns saturations
   smin to smax and nv values, so that the difference between each
   value and previous is constant. Colour (h,s,v) in this
   palette can be specified as an integer h_int betw. 0 and nh-1,
   s_int betw. 0 and ns-1 etc and is inserted as a colour
   (h_int * ns*nv + s_int * nv + v_int + n) in the paletten.
 */

{
   int i,h_int,s_int,v_int;
   float ah,as,mv,h,s,v,r,g,b;

  if (nh == 1) ah = 0; else  ah = (hmax - hmin) / (nh - 1);
  if (ns == 1) as = 0; else  as = (smax - smin) / (ns - 1);
  if (nv == 1) mv = 1; else  mv = pow (vmax / vmin, 1 / (float)(nv - 1));

  i = n;
  h = hmin;

  for (h_int = 0; h_int < nh; h_int++)
  {
    s = smin;

    for (s_int = 0; s_int < ns; s_int++)
    {
      v = vmin;

      for (v_int = 0; v_int < nv; v_int++)
      {
        hsv_to_rgb (h, s, v, &r, &g, &b);
        X2DsetPaletteRGB (vp, i, r * 255.999, g * 255.999, b * 255.999);
        i += 1;
        v *= mv;
      }

      s += as;
    } 

    h += ah;
  } 
} 

/*****************************************************************************/




