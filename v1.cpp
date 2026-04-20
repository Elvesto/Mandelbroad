#include "raylib.h"
#include "render.h"

#include <stdio.h>

int Calculate(Color* buffer, int countPix, MandelWrote* view) {
    for (int py = 0; py < view->screenHeight; py++) {
        for (int px = 0; px < view->screenWidth; px++) {
            double x0 = (double)(px - view->screenWidth / 2.0) / (0.3 * view->zoom * view->screenWidth) + view->offsetX;
            double y0 = (double)(py - view->screenHeight / 2.0) / (0.3 * view->zoom * view->screenHeight) + view->offsetY;
    
            double x = 0.0;
            double y = 0.0;
            int iteration = 0;
    
            volatile double x2 = 0.0;
            volatile double y2 = 0.0;
    
            while (x2 + y2 <= 4.0 && iteration < view->maxIterations) {
                y = 2.0 * x * y + y0;
                x = x2 - y2 + x0;
    
                x2 = x * x;
                y2 = y * y;
                iteration++;
            }
            #ifdef GMODE
                if (iteration < view->maxIterations) {
                    unsigned char val = (unsigned char)(255 * iteration / view->maxIterations);
                    DrawPixel(px, py, (Color){ val, 0, val, 255 });
                } else {
                    DrawPixel(px, py, PURPLE);
                }
            #endif //GMODE
        }
    }


    return 0;
}