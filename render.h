#ifndef RENDER_H_
#define RENDER_H_

#include "raylib.h"

struct MandelWrote {
    double zoom;
    double offsetX; 
    double offsetY;
    int maxIterations;
    int screenWidth;
    int screenHeight;
};

int Calculate(Color* buffer, int countPix, MandelWrote* view);

void HandleInput(MandelWrote* view);

#endif // RENDER_H_