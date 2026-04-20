#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>

#include "render.h"

static const int screenWidth = 800;
static const int screenHeight = 600;

int main() {
    clock_t startMain = 0, endMain = 0;
    startMain = clock();
    int countPix = screenHeight * screenWidth;
    Color* buffer = (Color*)calloc(countPix, sizeof(Color));
    if (buffer == NULL) { return -1; }

    MandelWrote view = {};
    view.maxIterations = 200;
    view.offsetX = -0.5;
    view.offsetY = 0;
    view.zoom = 1.0;
    view.screenHeight = 600;
    view.screenWidth = 800;

#ifdef GMODE
    InitWindow(screenWidth, screenHeight, "Window");
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    
    Image image = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    

    while (!WindowShouldClose()) {
        HandleInput(&view);
        
        #ifdef VERS1
            BeginDrawing();
            ClearBackground(BLACK);
            
            Calculate(buffer, countPix, &view);

            DrawFPS(10, 10);
            EndDrawing();
        #else
            Calculate(buffer, countPix, &view);

            UpdateTexture(texture, buffer);

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(texture, 0, 0, WHITE);
            DrawFPS(10, 10);
            EndDrawing();
        #endif

    }
    
    free(buffer);
    CloseWindow();

#else

    uint64_t start = 0, end = 0;
    double res = 0;
    int n = 5000;

    printf("Iteration, Tacts\n");
    
    
    for (int i = 0; i < n; i++) {
        start = __rdtsc();
        
        Calculate(buffer, countPix, &view);
        
        end = __rdtsc();
        
        res = (end - start) / (double)n;

        printf("%d,%lf\n", i, res);
        
    }
    
#endif
    endMain = clock();
    fprintf(stderr, "%lf\n", (double)(endMain - startMain) / CLOCKS_PER_SEC);

    return 0;
}