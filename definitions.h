#ifndef DEFINITIONS
    #define DEFINITIONS
    #include <gray.h>

    #define FRAMEDELTA 16000

    #define PI 3.14159

    #define WINDOWNAME "PANIC IF YOU CAN SEE THIS ;-;"
    #define WINDOWSTYLE sfFullscreen
    #define WIDTH 128
    #define HEIGHT 64
    #define COLORDEPTH 32

    #define BACKGROUND sfBlack
    #define FOREGROUND sfWhite

    #define NEAR 0.2
    #define FAR 100.0
    #define FOV 75.0

    #define OBJECTDISTANCE 3.0

    typedef struct {
        double x, y;
    } vector2;

    typedef struct {
        double x,y,z;
    } vector3;

    typedef struct {
        vector3 vertex[3];
        color_t color;
    } triangle;

    typedef struct {
        vector3 anchor;
        int trisc;
        triangle* tris;
    } mesh;

    typedef struct {
        double m[4][4];
    } matrix4x4;
#endif // DEFINITIONS
