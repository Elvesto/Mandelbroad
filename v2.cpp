#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>
#include <stdint.h>

int Calculate(Color* buffer, int countPix, MandelWrote* view) {
    for (int i = 0; i < countPix; i++) {
        int px = i % view->screenWidth;
        int py = i / view->screenWidth;

        double x0 = (double)(px - view->screenWidth / 2.0) / (0.3 * view->zoom * view->screenWidth) + view->offsetX;
        double y0 = (double)(py - view->screenHeight / 2.0) / (0.3 * view->zoom * view->screenHeight) + view->offsetY;
        double x = 0, y = 0, x2 = 0, y2 = 0;
        int iteration = 0;

        while (x2 + y2 <= 4.0 && iteration <= view->maxIterations) {
            y = 2.0 * x * y + y0;
            x = x2 - y2 + x0;
            x2 = x * x;
            y2 = y * y;
            iteration++;
        }
        if (iteration < view->maxIterations) {
            unsigned char val = (unsigned char)(255 * iteration / view->maxIterations);
            buffer[i] = (Color){ val, 0, val, 255 };
        } else {
            buffer[i] = PURPLE;
        }
    }

    return 0;
}