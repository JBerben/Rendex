#include <stdlib.h>

#include "renderer.h"
#include "definitions.h"

#include <display.h>
#include <gray.h>
#include <keyboard.h>

static mesh* cube;

static void initializeCube ()
{
    cube = malloc (sizeof(mesh));
    cube->anchor = (vector3) {0.0, 0.0, 0.0};
    cube->trisc = 12;
    cube->tris = calloc (12, sizeof(triangle));

    triangle* cubes_triangle = cube->tris;
    //SOUTH
    (*(cubes_triangle + 0)).vertex[0] = (vector3) {0.0, 0.0, 0.0};
    (*(cubes_triangle + 0)).vertex[1] = (vector3) {0.0, 1.0, 0.0};
    (*(cubes_triangle + 0)).vertex[2] = (vector3) {1.0, 1.0, 0.0};

    (*(cubes_triangle + 1)).vertex[0] = (vector3) {0.0, 0.0, 0.0};
    (*(cubes_triangle + 1)).vertex[1] = (vector3) {1.0, 1.0, 0.0};
    (*(cubes_triangle + 1)).vertex[2] = (vector3) {1.0, 0.0, 0.0};
    //
    //EAST
    (*(cubes_triangle + 2)).vertex[0] = (vector3) {1.0, 0.0, 0.0};
    (*(cubes_triangle + 2)).vertex[1] = (vector3) {1.0, 1.0, 0.0};
    (*(cubes_triangle + 2)).vertex[2] = (vector3) {1.0, 1.0, 1.0};

    (*(cubes_triangle + 3)).vertex[0] = (vector3) {1.0, 0.0, 0.0};
    (*(cubes_triangle + 3)).vertex[1] = (vector3) {1.0, 1.0, 1.0};
    (*(cubes_triangle + 3)).vertex[2] = (vector3) {1.0, 0.0, 1.0};
    //
    //NORTH
    (*(cubes_triangle + 4)).vertex[0] = (vector3) {1.0, 0.0, 1.0};
    (*(cubes_triangle + 4)).vertex[1] = (vector3) {1.0, 1.0, 1.0};
    (*(cubes_triangle + 4)).vertex[2] = (vector3) {0.0, 1.0, 1.0};

    (*(cubes_triangle + 5)).vertex[0] = (vector3) {1.0, 0.0, 1.0};
    (*(cubes_triangle + 5)).vertex[1] = (vector3) {0.0, 1.0, 1.0};
    (*(cubes_triangle + 5)).vertex[2] = (vector3) {0.0, 0.0, 1.0};
    //
    //WEST
    (*(cubes_triangle + 6)).vertex[0] = (vector3) {0.0, 0.0, 1.0};
    (*(cubes_triangle + 6)).vertex[1] = (vector3) {0.0, 1.0, 1.0};
    (*(cubes_triangle + 6)).vertex[2] = (vector3) {0.0, 1.0, 0.0};

    (*(cubes_triangle + 7)).vertex[0] = (vector3) {0.0, 0.0, 1.0};
    (*(cubes_triangle + 7)).vertex[1] = (vector3) {0.0, 1.0, 0.0};
    (*(cubes_triangle + 7)).vertex[2] = (vector3) {0.0, 0.0, 0.0};
    //
    //TOP
    (*(cubes_triangle + 8)).vertex[0] = (vector3) {0.0, 1.0, 0.0};
    (*(cubes_triangle + 8)).vertex[1] = (vector3) {0.0, 1.0, 1.0};
    (*(cubes_triangle + 8)).vertex[2] = (vector3) {1.0, 1.0, 1.0};

    (*(cubes_triangle + 9)).vertex[0] = (vector3) {0.0, 1.0, 0.0};
    (*(cubes_triangle + 9)).vertex[1] = (vector3) {1.0, 1.0, 1.0};
    (*(cubes_triangle + 9)).vertex[2] = (vector3) {1.0, 1.0, 0.0};
    //
    //BOTTOM
    (*(cubes_triangle + 10)).vertex[0] = (vector3) {1.0, 0.0, 1.0};
    (*(cubes_triangle + 10)).vertex[1] = (vector3) {0.0, 0.0, 1.0};
    (*(cubes_triangle + 10)).vertex[2] = (vector3) {0.0, 0.0, 0.0};

    (*(cubes_triangle + 11)).vertex[0] = (vector3) {1.0, 0.0, 1.0};
    (*(cubes_triangle + 11)).vertex[1] = (vector3) {0.0, 0.0, 0.0};
    (*(cubes_triangle + 11)).vertex[2] = (vector3) {1.0, 0.0, 0.0};
    //
}

void initializeGame ()
{

    initializeCube();
    initializeRenderer(NEAR, FAR, FOV);
}


void updateGame ()
{
   
    key_t event = 0;
    keyboard_setRepeatRate(125, 50);

    dclear();
    renderMesh(cube, 2);
    dupdate();

    while(event != KEY_EXIT) {
        dclear();

        event = getkey_opt(getkey_repeat_all_keys, 200);        

        switch(event) {
            case KEY_EXIT:
            dclear();
            break;

            case KEY_RIGHT:
            thetra_x += 0.08;
            renderMesh(cube, 1);
            dupdate();
            break;

            case KEY_LEFT:
            thetra_x -= 0.08;
            renderMesh(cube, 1);
            dupdate();
            break;

            case KEY_UP:
            thetra_y += 0.08;
            renderMesh(cube, 2);
            dupdate();
            break;

            case KEY_DOWN:
            thetra_y -= 0.08;
            renderMesh(cube, 2);
            dupdate();
            break;

            case KEY_9:
            thetra_z += 0.08;
            renderMesh(cube, 3);
            dupdate();
            break;

            case KEY_1:
            thetra_z -= 0.08;
            renderMesh(cube, 3);
            dupdate();
            break;
        }
        
    }

}

