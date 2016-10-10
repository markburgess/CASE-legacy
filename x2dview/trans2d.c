/***********************************************************************/
/* trans2d.c: 2-dimensional transformations                            */
/*                                                                     */
/***********************************************************************/
/* copyright (c) 1995,1996 Thomas E. Sevaldrud <thomasen@a.sn.no>      */
/***********************************************************************/

#include <conf.h>
#include "trans2d.h"

/*********************************************************************

int round (double x)
{
if (x >= 0)
   {
   x = floor (x + 0.5);
   }
else
   {
   x = ceil (x - 0.5);
   }
return x;
}
'/
/*********************************************************************/

void transform_a_point (const double xi, const double yi,
			double *xo, double *yo, matrix t)
{
/* ------------------------------------------------------------
   the vector (xi,yi,1) is multiplied with matrix t from left 
   and yields the vector (xo,yo,1). 
   ------------------------------------------------------------ */

*xo = t[0][0] * xi + t[0][1] * yi + t[0][2];
*yo = t[1][0] * xi + t[1][1] * yi + t[1][2];

}

/*********************************************************************/

void transform_points (int n, double *xi, double *yi,
		       double *xo, double *yo,
		       matrix t)

{
/* ------------------------------------------------------------
   The points (xi[k], yi[k], 1) are transformed with matrix t,
   yielding the points (xo[k], yo[k], 1) for k=0,...,n-1.
   ------------------------------------------------------------ */

int k;
for (k = 0; k < n; k++)
   {
   transform_a_point (xi[k], yi[k], &xo[k], &yo[k], t);
   }
}
/*********************************************************************/

void make_identity (matrix m)
{
int c,r;

for (r = 0; r < 3; r++)
   {
   for (c = 0; c < 3; c++)
      {
      if (r == c)
	 {
	 m[r][c] = 1;
	 }
      else
	 {
	 m[r][c] = 0;
	 }
      }
   }
}

/*********************************************************************/

void invert_matrix(matrix t, matrix i)
{
make_identity(i);

i[0][0] = t[1][1]/(t[0][0]*t[1][1]-t[0][1]*t[1][0]);
i[0][1] = -t[0][1]/(t[0][0]*t[1][1]-t[0][1]*t[1][0]);
i[1][0] = -t[1][0]/(t[0][0]*t[1][1]-t[0][1]*t[1][0]);
i[1][1] = t[0][0]/(t[0][0]*t[1][1]-t[0][1]*t[1][0]);

}

/*********************************************************************/

void combine_transformations (matrix m, matrix t)

/* Matrix t multiplies m from the left. */

{
int r,c;
matrix temp;
for (r = 0; r < 3; r++)
   {
   for (c = 0; c < 3; c++)
      {
      temp[r][c] = m[r][0] * t[0][c] + m[r][1] * t[1][c] + m[r][2] * t[2][c];
      }
   }
for (r = 0;r < 3;r++)
   {
   for (c = 0;c < 3;c++)
      {
      t[r][c] = temp[r][c];
      }
   }
}

/*********************************************************************/

void translate (matrix t, double tx, double ty)

/* Translation by (tx,ty). */

{
matrix m;
make_identity (m);
m[0][2] = tx;
m[1][2] = ty;
combine_transformations (m,t);
}

/*********************************************************************/

void scale(matrix t, double sx, double sy)

/* Scaling by factors sx and sy, med origin
 * som fixed point */

{
 matrix m;
 make_identity(m);
 
 m[0][0] = sx;
 m[1][1] = sy;
  
 combine_transformations(m,t);
}

/*********************************************************************/

void scale_about(matrix t, double sx, double sy, double xf, double yf)

/* Scaling by xf, with yf as fixed point. */
{
 translate (t, -xf, -yf);
 scale(t, sx,sy);
 translate (t, xf, yf);
}

/*********************************************************************/

void rotate(matrix t, double a)

/* Rotation a degrees about the origin */

{
 double rad;
 matrix m;

 /* convert angle to radians */
 rad = a/180*M_PI;


 make_identity(m);
 
 m[0][0] = cos(rad);
 m[0][1] = -sin(rad);
 m[1][0] = -m[0][1];
 m[1][1] = m[0][0];

 combine_transformations(m,t);
}

/*********************************************************************/

void rotate_about (matrix t, double a, double xr, double yr)

/* Rotation a degrees about point (xr,yr). */

{
  translate (t, -xr, -yr);
  rotate (t, a);
  translate (t, xr, yr);
}

/*********************************************************************/

void reflect(matrix t, double a)

/* Reflection in a line between origin which makes
 * an angle a/2 med x-axis. */
{
  matrix m;

  a = a*2;
  
  /* Convert to radians: */
  a = a/180*M_PI;

  make_identity(m);

  m[0][0] = cos(a);
  m[0][1] = sin(a);
  m[1][0] = m[0][1];
  m[1][1] = -m[0][0];

  combine_transformations(m,t);
}

/*********************************************************************/

void reflect_about(matrix t, double a,double xr, double yr)

/* Reflection in a straight line between xr/yr which makes
 * an angle a/2 with the x-aksen. */

{
  translate (t, -xr, -yr);
  reflect(t, a);
  translate (t, xr, yr);
}


/*********************************************************************/

void shear_x(matrix t, double k)

/* Shear i x-direction by factor k */
{
  matrix m;

  make_identity(m);

  m[0][0] = 1;
  m[0][1] = k;
  m[1][0] = 0;
  m[1][1] = 1;

  combine_transformations(m,t);
}

/*********************************************************************/

void shear_y(matrix t, double k)

/* Shear i y-directions by factor k */
{
  matrix m;

  make_identity(m);

  m[0][0] = 1;
  m[0][1] = 0;
  m[1][0] = k;
  m[1][1] = 1;

  combine_transformations(m,t);
}
/*********************************************************************/
