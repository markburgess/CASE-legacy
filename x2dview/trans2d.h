/***********************************************************************/
/* trans2d.c: 2-dimensional transformations                            */
/*                                                                     */
/***********************************************************************/
/* copyright (c) 1995,1996 Thomas E. Sevaldrud <thomasen@a.sn.no>      */
/***********************************************************************/

#ifndef _TRANS2D_H
#define _TRANS2D_H 1


#ifdef HAVE_SYS_CDEFS_H
# include <sys/cdefs.h>
#endif

#include <math.h>

/***********************************************************************/
/* MACROS/DEFINITIONS.                                                 */
/***********************************************************************/

/***********************************************************************/
/* DATATYPES.                                                          */
/***********************************************************************/

typedef double matrix[3][3];

/***********************************************************************/
/* PROTOTYPES.                                                         */
/***********************************************************************/

#ifdef __cplusplus
extern "C" 
{
#endif

void transform_a_point(const double xi, const double yi,
		       double *xo, double *yo, matrix  t);
void transform_points (int n, double *xi, double *yi,
		       double *xo, double *yo,
		       matrix t);
void make_identity (matrix m);

void invert_matrix(matrix t, matrix i);

void combine_transformations (matrix m, matrix t);

void translate(matrix t, double tx, double ty);

void scale (matrix t, double sx, double sy);

void scale_about (matrix t, double sx, double sy, double xf, double yf);

void rotate (matrix t, double a);

void rotate_about (matrix t, double a, double xr, double yr);

void reflect (matrix t, double a);

void reflect_about (matrix t, double a, double xr, double yr);

void shear_x (matrix t, double k);

void shear_y (matrix t, double k);

void copy(matrix t,matrix co);

#ifdef __cplusplus
}
#endif

#endif /* _TRANS2D_H */







