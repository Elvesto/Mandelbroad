#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>
#include <stdint.h>


static const int screenWidth = 800;
static const int screenHeight = 600;

static int Calculate(Color* buffer, int countPix, MandelWrote* view);

int Render() {

    MandelWrote view = {};
    view.maxIterations = 200;
    view.offsetX = -0.5;
    view.offsetY = 0;
    view.zoom = 1.0;

    int countPix = screenHeight * screenWidth;
    Color* buffer = (Color*)calloc(countPix, sizeof(Color));
    if (buffer == NULL) { return -1; }

    #ifdef GMODE
    InitWindow(screenWidth, screenHeight, "Window");
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    
    

    Image image = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    

    while (!WindowShouldClose()) {

        HandleInput(&view);
        
        Calculate(buffer, countPix, &view);

        UpdateTexture(texture, buffer);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        DrawFPS(10, 10);
        EndDrawing();

    }
    
    free(buffer);
    CloseWindow();

    #else

    uint64_t start = 0, end = 0;
    double res = 0;
    int n = 5000;
    
    
    for (int i = 0; i < n; i++) {
        start = __rdtsc();
        
        Calculate(buffer, countPix, &view);
        
        end = __rdtsc();
        
        res = (end - start) / (double)n;

        printf("%d,%lf\n", i, res);
        
    }
    
    #endif


    return 0;
}

int Calculate(Color* buffer, int countPix, MandelWrote* view) {
    for (int i = 0; i < countPix; i++) {
        int px = i % screenWidth;
        int py = i / screenWidth;

        double x0 = (double)(px - screenWidth / 2.0) / (0.3 * view->zoom * screenWidth) + view->offsetX;
        double y0 = (double)(py - screenHeight / 2.0) / (0.3 * view->zoom * screenHeight) + view->offsetY;
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