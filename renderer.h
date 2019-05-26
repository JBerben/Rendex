#ifndef RENDERER
    #define RENDERER

    #include "definitions.h"

    extern double thetra_x;
    extern double thetra_y;
    extern double thetra_z;

    void initializeRenderer (double near, double far, double _fov);
    void renderMesh (mesh* mesh, int rotnum);
#endif // RENDERER
