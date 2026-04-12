#ifndef RENDER_H_
#define RENDER_H_

struct MandelWrote {
    double zoom;
    double offsetX; 
    double offsetY;
    int maxIterations;
};

int Render();

void HandleInput(MandelWrote* view);

#endif // RENDER_H_