#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mathutils.h"
#include "definitions.h"

#include <display.h>
#include <gray.h>
#include <math.h>

static vector3 camera;

static double nearPlane = NEAR;
static double farPlane = FAR;
static double fov = FOV;

static matrix4x4 projectionMatrix;
double thetra_x;
double thetra_y;
double thetra_z;

static triangle* triProjected;
static triangle* triTranslated;
static triangle* triRotatedZ;
static triangle* triRotatedZX;
static triangle* triRotatedY;
static triangle* triRotatedYX;
static triangle* triRotatedYXZ;

static matrix4x4* zRotationMatrix;
static matrix4x4* xRotationMatrix;
static matrix4x4* yRotationMatrix;

// Custom Round
//int round(float f) {
//    return (int)f;
//}

color_t get_colour(double lum) {
		short bg_col, fg_col;
		int pixel_bw = (int)(2*lum);
		switch (pixel_bw)
		{
		case 0: bg_col = color_black; fg_col = color_black; break;

		case 1: bg_col = color_black; fg_col = color_dark; break;

		case 3: bg_col = color_light; fg_col = color_light; break;

        case 4: bg_col = color_white; fg_col = color_white; break;

		default:
			bg_col = color_black; fg_col = color_black;
	}

		color_t c = fg_col;
		return c;
}

static void initializeProjectionMatrix ()
{
    double aspectRatio = (double) HEIGHT / (double) WIDTH;
    double fovRad = 1.0 / tan(fov * 0.5 / 180.0 * PI);

    projectionMatrix.m[0][0] = aspectRatio * fovRad;
    projectionMatrix.m[1][1] = fovRad;
    projectionMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
    projectionMatrix.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
    projectionMatrix.m[2][3] = 1.0;
    projectionMatrix.m[3][3] = 0.0;
}

static vector3 multiplyMatrixVector (vector3 input, matrix4x4 matrix)
{
    vector3 output;
    output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
    output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
    output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
    double w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

    if (w != 0.0)
    {
        output.x /= w;
        output.y /= w;
        output.z /= w;
    }
    return output;
}

static void processRotationMatrix (const double angle, matrix4x4* matRotZ, matrix4x4* matRotX)
{
    // Rotation Z
    matRotZ->m[0][0] = cos(angle);
    matRotZ->m[0][1] = sin(angle);
    matRotZ->m[1][0] = -sin(angle);
    matRotZ->m[1][1] = cos(angle);
    matRotZ->m[2][2] = 1.0;
    matRotZ->m[3][3] = 1.0;

    // Rotation X
    matRotX->m[0][0] = 1.0;
    matRotX->m[1][1] = cos(angle * 0.5);
    matRotX->m[1][2] = sin(angle * 0.5);
    matRotX->m[2][1] = -sin(angle * 0.5);
    matRotX->m[2][2] = cos(angle * 0.5);
    matRotX->m[3][3] = 1.0;
}

static void process_xrot_matrix(const double angle, matrix4x4* matRotX) {
    matRotX->m[0][0] = 1.0;
    matRotX->m[1][1] = cos(angle * 0.5);
    matRotX->m[1][2] = sin(angle * 0.5);
    matRotX->m[2][1] = -sin(angle * 0.5);
    matRotX->m[2][2] = cos(angle * 0.5);
    matRotX->m[3][3] = 1.0;
}

static void process_yrot_matrix(const double angle, matrix4x4* matRotY) {
	matRotY->m[0][0] = cos(angle);
	matRotY->m[0][2] = sin(angle);
	matRotY->m[2][0] = -sin(angle);
	matRotY->m[1][1] = 1.0;
	matRotY->m[2][2] = cos(angle);
    matRotY->m[3][3] = 1.0;
}

static void process_zrot_matrix(const double angle, matrix4x4* matRotZ) {
    matRotZ->m[0][0] = cos(angle);
    matRotZ->m[0][1] = sin(angle);
    matRotZ->m[1][0] = -sin(angle);
    matRotZ->m[1][1] = cos(angle);
    matRotZ->m[2][2] = 1.0;
    matRotZ->m[3][3] = 1.0;
}

void draw_line(int x1, int y1, int x2, int y2, color_t color)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			dpixel(x, y, color_black);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				dpixel(x, y, color);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			dpixel(x, y, color);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				dpixel(x, y, color);
			}
		}
}

static void drawTriangle (triangle* triangle)
{
    draw_line(triangle->vertex[0].x, triangle->vertex[0].y, triangle->vertex[1].x, triangle->vertex[1].y, triangle->color);
    draw_line(triangle->vertex[1].x, triangle->vertex[1].y, triangle->vertex[2].x, triangle->vertex[2].y, triangle->color);
    draw_line(triangle->vertex[2].x, triangle->vertex[2].y, triangle->vertex[0].x, triangle->vertex[0].y, triangle->color);
}

void initializeRenderer (double near, double far, double _fov)
{
    camera.x = 0.0;
    camera.y = 0.0;
    camera.z = 0.0;

    nearPlane = near;
    farPlane = far;
    fov = _fov;

    initializeProjectionMatrix();

    yRotationMatrix = calloc (1, sizeof(matrix4x4));
    zRotationMatrix = calloc (1, sizeof(matrix4x4));
    xRotationMatrix = calloc (1, sizeof(matrix4x4));

    triProjected = calloc (1, sizeof(triangle));
    triRotatedY = calloc (1, sizeof(triangle));
    triRotatedYX = calloc (1, sizeof(triangle));
    triRotatedYXZ = calloc (1, sizeof(triangle));
    triTranslated = calloc (1, sizeof(triangle));
}

//Renders the frame
//This function does not yet take any camera parameter into account
//It just behaves as if the camera was at ORIGIN with angle [0f, 0f]
void renderMesh (mesh* mesh, int rotnum)
{

    //processRotationMatrix (thetra, zRotationMatrix, xRotationMatrix);
    if (rotnum == 1) {
        process_xrot_matrix(thetra_x, xRotationMatrix);
    } else if (rotnum == 2) {
        process_yrot_matrix(thetra_y, yRotationMatrix);
    } else if (rotnum == 3) {
        process_zrot_matrix(thetra_z, zRotationMatrix);
    }
    
    

    for (int i = 0; i < mesh->trisc; i++)
    {
        triangle currentTriangle = *(mesh->tris + i);

        //Rotate on Y axis
        triRotatedY->vertex[0] = multiplyMatrixVector(currentTriangle.vertex[0], *yRotationMatrix);
        triRotatedY->vertex[1] = multiplyMatrixVector(currentTriangle.vertex[1], *yRotationMatrix);
        triRotatedY->vertex[2] = multiplyMatrixVector(currentTriangle.vertex[2], *yRotationMatrix);

        *triRotatedYX = *triRotatedY;

        //Rotate on X axis
        triRotatedYX->vertex[0] = multiplyMatrixVector(triRotatedY->vertex[0], *xRotationMatrix);
        triRotatedYX->vertex[1] = multiplyMatrixVector(triRotatedY->vertex[1], *xRotationMatrix);
        triRotatedYX->vertex[2] = multiplyMatrixVector(triRotatedY->vertex[2], *xRotationMatrix);

        *triRotatedYXZ = *triRotatedYX; 

        // Rotate on Z axis
        triRotatedYXZ->vertex[0] = multiplyMatrixVector(triRotatedYX->vertex[0], *zRotationMatrix);
        triRotatedYXZ->vertex[1] = multiplyMatrixVector(triRotatedYX->vertex[1], *zRotationMatrix);
        triRotatedYXZ->vertex[2] = multiplyMatrixVector(triRotatedYX->vertex[2], *zRotationMatrix);

        // Update positions of all triangles.
        *triTranslated = *triRotatedYXZ;
        triTranslated->vertex[0].z = triRotatedYXZ->vertex[0].z + OBJECTDISTANCE;
        triTranslated->vertex[1].z = triRotatedYXZ->vertex[1].z + OBJECTDISTANCE;
        triTranslated->vertex[2].z = triRotatedYXZ->vertex[2].z + OBJECTDISTANCE;

        /**
         * Working out the normals of triangles so that we don't have
         * to render all of them at once.
         * TODO: IMPLEMENT Z BUFFER TECHNIQUE IN FUTURE...
         */
        vector3 normal, edge1, edge2;

        edge1.x = triTranslated->vertex[1].x - triTranslated->vertex[0].x;
        edge1.y = triTranslated->vertex[1].y - triTranslated->vertex[0].y;
        edge1.z = triTranslated->vertex[1].z - triTranslated->vertex[0].z;

        edge2.x = triTranslated->vertex[2].x - triTranslated->vertex[0].x;
        edge2.y = triTranslated->vertex[2].y - triTranslated->vertex[0].y;
        edge2.z = triTranslated->vertex[2].z - triTranslated->vertex[0].z;
        
        normal = vector3_cross(edge1, edge2);
        double length = vector3_length(normal);

        /**
         * This is where we project our 3D representation of our model
         * onto 2D space (aka our screen).
         */

        //TESTING Z NORMAL STUFF
        if (normal.x * (triTranslated->vertex[0].x - camera.x) +
            normal.y * (triTranslated->vertex[0].y - camera.y) +
            normal.z * (triTranslated->vertex[0].z - camera.z) < 0.0)
        {
         
            /**
             * BARYCENTRIC Triangle filling algorithm. Will improve
             * to a faster algorithm in the future. This is mainly
             * to get a better grip on how to work the gray engine
             * with triangle rasterisation.
             */

            /* Illumination. */
            vector3 light_direction = (vector3) {0.0, 0.0, -1.0};
            double light_length = vector3_length(light_direction);
            light_direction.x /= 1.0; light_direction.y /= 1.0; light_direction.z /= 1.0;

            // How similar is normal to light direction
            double dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
            //color_t c = get_colour(dp);
            color_t c = color_black;

            *triProjected = *triTranslated;
            triProjected->color = c;
            triProjected->vertex[0] = multiplyMatrixVector(triTranslated->vertex[0], projectionMatrix);
            triProjected->vertex[1] = multiplyMatrixVector(triTranslated->vertex[1], projectionMatrix);
            triProjected->vertex[2] = multiplyMatrixVector(triTranslated->vertex[2], projectionMatrix);

            //Scale to viewport
            triProjected->vertex[0].x += 1.0;
            triProjected->vertex[0].y += 1.0;

            triProjected->vertex[1].x += 1.0;
            triProjected->vertex[1].y += 1.0;

            triProjected->vertex[2].x += 1.0;
            triProjected->vertex[2].y += 1.0;

            triProjected->vertex[0].x *= 0.5 * (double) WIDTH;
            triProjected->vertex[0].y *= 0.5 * (double) HEIGHT;

            triProjected->vertex[1].x *= 0.5 * (double) WIDTH;
            triProjected->vertex[1].y *= 0.5 * (double) HEIGHT;

            triProjected->vertex[2].x *= 0.5 * (double) WIDTH;
            triProjected->vertex[2].y *= 0.5 * (double) HEIGHT;

             /**
         * START OF TRIANGLE FILLING.
         */
        int maxX = Math.max(triProjected->vertex[0].x, Math.max(triProjected->vertex[1].x, triProjected->vertex[2].x));
        int minX = Math.min(triProjected->vertex[0].x, Math.min(triProjected->vertex[1].x, triProjected->vertex[2].x));
        int maxY = Math.max(triProjected->vertex[0].y, Math.max(triProjected->vertex[1].y, triProjected->vertex[2].y));
        int minY = Math.min(triProjected->vertex[0].y, Math.min(triProjected->vertex[1].y, triProjected->vertex[2].y));

        /* spanning vectors of edge (v1,v2) and (v1,v3) */
        vector2 vs1 = { edge1.x, edge1.y };
        vector2 vs2 = { edge2.x, edge2.y };

        for (int x = minX; x <= maxX; x++) {   
            for (int y = minY; y <= maxY; y++) {
                vector2 q = { x - triProjected->vertex[0].x, y - triProjected->vertex[0].y };

                float s = (float)crossProduct(q, vs2) / crossProduct(vs1, vs2);
                float t = (float)crossProduct(vs1, q) / crossProduct(vs1, vs2);

                if ( (s >= 0) && (t >= 0) && (s + t <= 1)) { 
                    /* inside triangle */
                    dpixel(x, y, color_black);
                }
            }
        }
        /**
         * END OF TRIANGLE FILLING.
         */

        }
        //Draw the result on screen
        drawTriangle(triProjected);


        
    }
}
