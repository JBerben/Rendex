#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#include "definitions.h"
#include <display.h>
#include <math.h>

/**
 * Mathutils is used to clean and speed up the computation
 * of vector and matrix mathematics.
 * 
 * @author: JBerben
 */

/* General use */
double mysqrt (double y);
double get_pdistance(int x0, int x1, int y0, int y1);
double get_v2distance(vector2 va, vector2 vb);
double get_v3distance(vector3 va, vector3 vb);
double max(double n0, double n1);
double min(double n0, double n1);
double clamp(double value, double mn, double mx);
double interpolate(double mn, double mx, double gradient);
void process_scan_line(int y, vector2 va, vector2 vb, vector2 vc, vector2 vd, color_t color);
void fill_triangle(vector2 v0, vector2 v1, vector2 v2, color_t color);

/* 3D Vector maths */
vector3 vector3_add(vector3 va, vector3 vb);
vector3 vector3_subtract(vector3 va, vector3 vb);
vector3 vector3_multiply(vector3 va, vector3 vb);
vector3 vector3_divide(vector3 va, vector3 vb);
double vector3_length(vector3 v);
vector3 vector3_normalize(vector3 v);
vector3 vector3_dot(vector3 va, vector3 vb);
vector3 vector3_cross(vector3 va, vector3 vb);
vector3 vector3_project(vector3 va, vector3 vb);
double vector3_scalar_product(vector3 va, vector3 vb);

/* 2D Vector maths */
vector2 vector2_add(vector2 va, vector2 vb);
vector2 vector2_subtract(vector2 va, vector2 vb);
vector2 vector2_multiply(vector2 va, vector2 vb);
vector2 vector2_divide(vector2 va, vector2 vb);
double vector2_magnitude(vector2 v);
vector2 vector2_normalize(vector2 v);
vector2 vector2_dot(vector2 va, vector2 vb);
vector2 vector2_cross(vector2 va);
vector2 vector2_project(vector2 va, vector2 vb);

/* 4x4 Matrix maths */
matrix4x4 make_identity_matrix(void);
matrix4x4 make_xrot_matrix(double angle_rad);
matrix4x4 make_yrot_matrix(double angle_rad);
matrix4x4 make_zrot_matrix(double angle_rad);
matrix4x4 make_matrix_translation(double x, double y, double z);
matrix4x4 make_matrix_projection(double fov_degrees, double aspect_ratio, double near, double far);
matrix4x4 m4x4_mul(matrix4x4 m1, matrix4x4 m2);
matrix4x4 m4x4_lookat(matrix4x4 m1, vector3 pos, vector3 target, vector3 up);
matrix4x4 m4x4_speedyinverse(matrix4x4 m);

/* Handy extras */
vector3 vector3_isect_plane(vector3 plane_p, vector3 plane_n, vector3 lineStart, vector3 lineEnd);
int triangle_planeclipping(vector3 plane_p, vector3 plane_n, triangle in_tri, triangle out_tri1, triangle out_tri2);

#endif