/*****************************************************************************/
/*                                                                           */
/* color.h: color handling routines                                          */
/*                                                                           */
/*****************************************************************************/
/* copyright (c) 1996 Thomas E. Sevaldrud <thomasen@a.sn.no>                 */
/*                    Steinar Johannesen <steinarj@iu.hioslo.no>             */
/*****************************************************************************/

#ifndef _COLOR_H
#define _COLOR_H

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C" 
{
#endif

void restore_vga_colors(Widget vp);

void make_grey_scale_64a(Widget vp);

void make_grey_scale_16a(Widget vp);

void make_grey_scale_a (Widget vp, int nc, int n);

void make_grey_scale_m (Widget vp, int nc, int n, float vmin);

void rgb_to_hsv (float r, float g, float b , float *h, float *s, float *v);

void hsv_to_rgb (float h, float s, float v , float *r, float *g, float *b);

void make_intensity_scale_a (Widget vp, int nc, int n , float h, float s);

void make_intensity_scale_m (Widget vp, int nc, int n , float h, float s, float vmin);

float max3(float a, float b, float c);

float min3(float a, float b, float c);

void blend (Widget vp, int i1, float r1, float g1, float b1,
            int i2, float r2, float g2, float b2);

void make_rgb_palette_a (Widget vp, int nr, int ng, int nb, int n,
                         float rmin, float rmax,
                         float gmin, float gmax,
                         float bmin, float bmax);

void make_rgb_palette_m (Widget vp, int nr, int ng, int nb, int n,
                         float rmin, float rmax,
                         float gmin, float gmax,
                         float bmin, float bmax);

void make_hsv_palette_a (Widget vp, int nh, int ns, int nv, int n,
                         float hmin, float hmax,
                         float smin, float smax,
                         float vmin, float vmax);

void make_hsv_palette_m (Widget vp, int nh, int ns, int nv, int n,
                         float hmin, float hmax,
                         float smin, float smax,
                         float vmin, float vmax);
#ifdef __cplusplus
}
#endif

#endif


