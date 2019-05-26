#include "mathutils.h"
#include <math.h>

/**
 * GENERAL USE FUNCTIONS
 */
double mysqrt (double y) {
    double x, z, tempf;
    unsigned long *tfptr = ((unsigned long *)&tempf) + 1;
    tempf = y;
    *tfptr = (0xbfcdd90a - *tfptr)>>1; 
    x =  tempf;
    z =  y*0.5;                       
    x = (1.5*x) - (x*x)*(x*z);    //The more you make replicates of this statement 
                               //the higher the accuracy, here only 2 replicates are used  
    x = (1.5*x) - (x*x)*(x*z);       
    
    return x*y; 
}  


double get_pdistance(int x0, int x1, int y0, int y1) {
    double xdiff = fabs(x0-x1);
    double ydiff = fabs(y0-y1);
    return mysqrt((xdiff*xdiff)+(ydiff*ydiff));
}

double get_v2distance(vector2 va, vector2 vb) {
    double xdiff = fabs(va.x-vb.x);
    double ydiff = fabs(va.y-vb.y);
    return mysqrt((xdiff*xdiff)+(ydiff*ydiff));
}

double get_v3distance(vector3 va, vector3 vb) {
    double xdiff = fabs(va.x-vb.x);
    double ydiff = fabs(va.y-vb.y);
    double zdiff = fabs(va.z-vb.z);
    return mysqrt((xdiff*xdiff)+(ydiff*ydiff)+(zdiff*zdiff));
}

double max(double n0, double n1) {
    return (n0 > n1) ? n0:n1;
}

double min(double n0, double n1) {
    return (n0 > n1) ? n1:n0;
}

// mn = 0.0, mx = 1.0 (always?)
double clamp(double value, double mn, double mx) {
    return max(mn, min(value, mx));
}

double interpolate(double mn, double mx, double gradient) {
    return mn + (mx - mn) * clamp(gradient, 0.0, 1.0);
}

void process_scan_line(int y, vector2 va, vector2 vb, vector2 vc, vector2 vd, color_t color) {
    double g1 = ((va.y != vb.y) ? ((double)(y - va.y)/(vb.y-va.y)) : 1);
    double g2 = ((vc.y != vd.y) ? ((double)(y - vc.y)/(vd.y-vc.y)) : 1);
    
    double sx = interpolate(va.x, vb.x, g1);
    double ex = interpolate(vc.x, vd.x, g2);

    for (int x = (int)sx; x < (int)ex; x++) {
        if (color == color_black) {
            dpixel(x, y, color_black);
        } else {
            gpixel(x, y, color);
        }
        
    }
}

void fill_triangle(vector2 v0, vector2 v1, vector2 v2, color_t color) {
    vector2 temp;
    
    if (v0.y > v1.y) {
        temp = v1;
        v1 = v0;
        v0 = temp;
    }
        
    if (v1.y > v2.y) {
        temp = v1;
        v1 = v2;
        v2 = temp;
    }
        
    if (v0.y > v1.y) {
        temp = v1;
        v1 = v0;
        v0 = temp;
    }
        
    double dv0v1 = 0, dv0v2 = 0;
        
    if (v1.y - v0.y > 0) {
        dv0v1 = (v1.x - v0.x) / (v1.y - v0.y);
    } else {
        dv0v1 = 0;
    }
        
    if (v2.y - v0.y > 0) {
        dv0v2 = (v2.x - v0.x) / (v2.y - v0.y);
    } else {
        dv0v2 = 0;
    }
        
    if (dv0v1 > dv0v2) {
        for (int y = v0.y; y < v2.y+1; y++) {
            if (y < v1.y) {
                process_scan_line(y, v0, v2, v0, v1, color);
            } else {
                process_scan_line(y, v0, v2, v1, v2, color);
                }
        }
    } else {
        for (int y = v0.y; y < v2.y+1; y++) {
            if (y < v1.y) {
                process_scan_line(y, v0, v1, v0, v2, color);
            } else {
                process_scan_line(y, v1, v2, v0, v2, color);
            }
        }
    } 
}


/**
 * 3D VECTOR FUNCTIONS
 */

/**
 * Takes 2 3D vectors and returns a new one,
 * being the addition of the two.
 */
vector3 vector3_add(vector3 va, vector3 vb) {
    va.x += vb.x;
    va.y += vb.y;
    va.z += vb.z;
    return va;
}

/**
 * Takes 2 3D vectors and returns a new one,
 * being the subtraction of the two.
 * 
 * Example:
 * vector3_subtract(va, vb);
 * returns (va - vb) as a vector3.
 */
vector3 vector3_subtract(vector3 va, vector3 vb) {
    va.x -= vb.x;
    va.y -= vb.y;
    va.z -= vb.z;
    return va;
}

/**
 * Takes 2 3D vectors and returns a new one,
 * being the multiplication of the two.
 * 
 * Example:
 * vector3_multiply(va, vb);
 * returns (va * vb) as a vector3.
 */
vector3 vector3_multiply(vector3 va, vector3 vb) {
    va.x *= vb.x;
    va.y *= vb.y;
    va.z *= vb.z;
    return va;
}

/**
 * Multiplies a vector by a scalar quantity.
 */
vector3 vector3_smultiply(vector3 va, double scalar) {
    va.x *= scalar;
    va.y *= scalar;
    va.z *= scalar;
    return va;
}

/**
 * Takes 2 3D vectors and returns a new one,
 * being the division of the two.
 * 
 * Example:
 * vector3_divide(va, vb);
 * returns (va / vb) as a vector3.
 */
vector3 vector3_divide(vector3 va, vector3 vb) {
    va.x /= vb.x;
    va.y /= vb.y;
    va.z /= vb.z;
    return va;
}

/**
 * Divides a vector by a scalar quantity.
 */
vector3 vector3_sdivide(vector3 va, double scalar) {
    va.x /= scalar;
    va.y /= scalar;
    va.z /= scalar;
    return va;
}

/**
 * Returns the magnitude of a vector.
 */
double vector3_length(vector3 v) {
    return mysqrt(vector3_scalar_product(v, v));
}

/**
 * Normalizes a given 3D vector.
 */
vector3 vector3_normalize(vector3 v) {
    double dist = vector3_length(v);
    if (dist != 0) {
        v.x /= dist;
        v.y /= dist;
        v.z /= dist;
    } else {
        return v;
    }
}

/**
 * Dot product of two 3D vectors.
 * Returns as 3d vector rather than a scalar.
 */
vector3 vector3_dot(vector3 va, vector3 vb) {
    vector3 dot;
    dot.x = va.x * vb.x;
    dot.y = va.y * vb.y;
    dot.z = va.z * vb.z;
    return dot;
}

double vector3_scalar_product(vector3 va, vector3 vb) {
    return (va.x*vb.x + va.y*vb.y + va.z*vb.z);
}

/**
 * Cross product of two 3D vectors.
 * TODO: Might need some work since I've generated
 * a local vector3 struct copy within this function.
 */
vector3 vector3_cross(vector3 va, vector3 vb) {
    vector3 product;
    product.x = (va.y * vb.z) - (va.z * vb.y);
    product.y = (-va.x * vb.z) + (va.z * vb.x);
    product.z = (va.x * vb.y) - (va.y * vb.x);
    return product;
}

/**
 * I hope this works.
 */
vector3 vector3_project(vector3 va, vector3 vb) {
    vector3 normal = vector3_normalize(vb);
    // va = dotproduct of va & normal x normal.
    return vector3_multiply(vector3_dot(va, normal), normal);
}


/**
 * 2D VECTOR FUNCTIONS
 */
vector2 vector2_add(vector2 va, vector2 vb) {
    va.x += vb.x;
    va.y += vb.y;
    return va;
}

vector2 vector2_subtract(vector2 va, vector2 vb) {
    va.x -= vb.x;
    va.y -= vb.y;
    return va;
}

vector2 vector2_multiply(vector2 va, vector2 vb) {
    va.x *= vb.x;
    va.y *= vb.y;
    return va;
}

vector2 vector2_divide(vector2 va, vector2 vb) {
    va.x /= vb.x;
    va.y /= vb.y;
    return va;
}

double vector2_magnitude(vector2 v) {
    return mysqrt((v.x * v.x) + (v.y * v.y));
}

vector2 vector2_normalize(vector2 v) {
    double dist = vector2_magnitude(v);
    if (dist != 0) {
        v.x /= dist;
        v.y /= dist;
    } else {
        return v;
    }
}

vector2 vector2_dot(vector2 va, vector2 vb) {
    vector2 dot;
    dot.x = va.x * vb.x;
    dot.y = va.y * vb.y;
    return dot;
}

vector2 vector2_cross(vector2 va) {
    vector2 cross;
    cross.x = va.y;
    cross.y = -va.x;
    return cross;
}

vector2 vector2_project(vector2 va, vector2 vb){
    vector2 normal = vector2_normalize(vb);
    return (vector2_multiply(normal, vector2_dot(va, normal)));
}


/**
 * 4x4 MATRIX FUNCTIONS
 */

matrix4x4 make_identity_matrix(void) {
	matrix4x4 matrix;
	matrix.m[0][0] = 1.0;
	matrix.m[1][1] = 1.0;
	matrix.m[2][2] = 1.0;
	matrix.m[3][3] = 1.0;
	return matrix;
}

matrix4x4 make_xrot_matrix(double angle_rad) {
    matrix4x4 matrix;
	matrix.m[0][0] = 1.0;
	matrix.m[1][1] = cos(angle_rad);
	matrix.m[1][2] = sin(angle_rad);
	matrix.m[2][1] = -sin(angle_rad);
	matrix.m[2][2] = cos(angle_rad);
	matrix.m[3][3] = 1.0;
	return matrix;
}

matrix4x4 make_yrot_matrix(double angle_rad) {
    matrix4x4 matrix;
	matrix.m[0][0] = cos(angle_rad);
	matrix.m[0][2] = sin(angle_rad);
	matrix.m[2][0] = -sin(angle_rad);
	matrix.m[1][1] = 1.0;
	matrix.m[2][2] = cos(angle_rad);
    matrix.m[3][3] = 1.0;
	return matrix;
}

matrix4x4 make_zrot_matrix(double angle_rad) {
    matrix4x4 matrix;
	matrix.m[0][0] = cos(angle_rad);
	matrix.m[0][1] = sin(angle_rad);
	matrix.m[1][0] = -sin(angle_rad);
	matrix.m[1][1] = cos(angle_rad);
	matrix.m[2][2] = 1.0;
    matrix.m[3][3] = 1.0;
	return matrix;
}

matrix4x4 make_matrix_translation(double x, double y, double z) {
		matrix4x4 matrix;
		matrix.m[0][0] = 1.0;
		matrix.m[1][1] = 1.0;
		matrix.m[2][2] = 1.0;
		matrix.m[3][3] = 1.0;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
}

matrix4x4 make_matrix_projection(double fov_degrees, double aspect_ratio, double near, double far) {
    double fov_rad = 1.0 / tan(fov_degrees * 0.5 / 180.0 * PI);
	matrix4x4 matrix;
	matrix.m[0][0] = aspect_ratio * fov_rad;
	matrix.m[1][1] = fov_rad;
	matrix.m[2][2] = far / (far - near);
	matrix.m[3][2] = (-far * near) / (far - near);
	matrix.m[2][3] = 1.0;
	matrix.m[3][3] = 0.0;
	return matrix;
}

matrix4x4 m4x4_mul(matrix4x4 m1, matrix4x4 m2) {
		matrix4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
}

matrix4x4 m4x4_lookat(matrix4x4 m1, vector3 pos, vector3 target, vector3 up) {
	// Calculate new forward direction
	vector3 new_forward = vector3_subtract(target, pos);
	new_forward = vector3_normalize(new_forward);

	// Calculate new Up direction
	vector3 a = vector3_smultiply(new_forward, vector3_scalar_product(up, new_forward));
	vector3 new_up = vector3_subtract(up, a);
	new_up = vector3_normalize(new_up);

	// New Right direction is easy, its just cross product
	vector3 new_right = vector3_cross(new_up, new_forward);

	// Construct Dimensioning and Translation Matrix	
	matrix4x4 matrix;
	matrix.m[0][0] = new_right.x;	matrix.m[0][1] = new_right.y;	matrix.m[0][2] = new_right.z;	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = new_up.x;		matrix.m[1][1] = new_up.y;		matrix.m[1][2] = new_up.z;		matrix.m[1][3] = 0.0;
	matrix.m[2][0] = new_forward.x;	matrix.m[2][1] = new_forward.y;	matrix.m[2][2] = new_forward.z;	matrix.m[2][3] = 0.0;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0;
	return matrix;
}

// Only for Rotation/Translation Matrices
matrix4x4 m4x4_speedyinverse(matrix4x4 m) {
	matrix4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0;
	return matrix;
}


/**
 * HANDY EXTRA FUNCTIONS
 */

vector3 vector3_isect_plane(vector3 plane_p, vector3 plane_n, vector3 lineStart, vector3 lineEnd) {
    plane_n = vector3_normalize(plane_n);
	double plane_d = -vector3_scalar_product(plane_n, plane_p);
	double ad = vector3_scalar_product(lineStart, plane_n);
	double bd = vector3_scalar_product(lineEnd, plane_n);
	double t = (-plane_d - ad) / (bd - ad);
	vector3 lineStartToEnd = vector3_subtract(lineEnd, lineStart);
	vector3 lineToIntersect = vector3_smultiply(lineStartToEnd, t);
	return vector3_add(lineStart, lineToIntersect);
}