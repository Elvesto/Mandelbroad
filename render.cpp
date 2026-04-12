#include "render.h"
#include "raylib.h"

void HandleInput(MandelWrote* view) {
    if (IsKeyDown(KEY_UP)) view->zoom *= 1.05;
    if (IsKeyDown(KEY_DOWN)) view->zoom /= 1.05;
    if (IsKeyDown(KEY_D)) view->offsetX += 0.1 / view->zoom;
    if (IsKeyDown(KEY_A)) view->offsetX -= 0.1 / view->zoom;
    if (IsKeyDown(KEY_W)) view->offsetY -= 0.1 / view->zoom;
    if (IsKeyDown(KEY_S)) view->offsetY += 0.1 / view->zoom;
    if (IsKeyDown(KEY_Q)) view->maxIterations -= 5;
    if (IsKeyDown(KEY_E)) view->maxIterations += 5;
}