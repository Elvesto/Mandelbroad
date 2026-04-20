#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>

#include "immit.h"

static const int screenWidth = 800;
static const int screenHeight = 600;

static int Calculate(Color* buffer, int countPix, MandelWrote* view);

int Render() {
    int countPix = screenHeight * screenWidth;
    Color* buffer = (Color*)calloc(countPix, sizeof(Color));
    if (buffer == NULL) { return -1; }

    MandelWrote view = {};
    view.maxIterations = 100;
    view.offsetX = -0.5;
    view.offsetY = 0;
    view.zoom = 1.0;

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


    for (int i = 0; i < countPix; i += 8) {
        
        float tempX[8] = {};
        float tempY[8] = {};
        for (int j = 0; j < 8; j++) {
            int index = i + j;
            if (index >= countPix) { break; }

            int px = index % screenWidth;
            int py = index / screenWidth;

            tempX[j] = (float)((px - screenWidth / 2.0) / (0.3 * view->zoom * screenWidth) + view->offsetX);
            tempY[j] = (float)((py - screenHeight / 2.0) / (0.3 * view->zoom * screenHeight) + view->offsetY);
        }

        m256 x0 = {tempX[0], tempX[1], tempX[2], tempX[3], tempX[4], tempX[5], tempX[6], tempX[7]};
        m256 y0 = {tempY[0], tempY[1], tempY[2], tempY[3], tempY[4], tempY[5], tempY[6], tempY[7]};
        m256 x = mm_set1_ps(0);
        m256 y = mm_set1_ps(0);
        m256 four = mm_set1_ps(4);
        m256i iters = {0, 0, 0, 0, 0, 0, 0, 0};

        for (int iteration = 0; iteration < view->maxIterations; iteration++) {

            m256 x2 = mm_mul_ps(x, x);
            m256 y2 = mm_mul_ps(y, y);

            m256i mask = mm_cmple_ps(mm_add_ps(x2, y2), four);

            if (mm_movemask_ps(mask) == 0) {
                break;
            }

            for (int j = 0; j < 8; j++) {
                if (mask.value[j]) {
                    iters.value[j]++;
                }
            }

            m256 xy = mm_mul_ps(x, y);
            y = mm_add_ps(mm_add_ps(xy, xy), y0);

            m256 newX = {};
            for (int j = 0; j < 8; j++) {
                newX.value[j] = x2.value[j] - y2.value[j];
            }
            x = mm_add_ps(newX, x0);
        }

        for (int j = 0; j < 8; j++) {
            if (i + j >= countPix) { break; }

            int iteration = iters.value[j];
            if (iteration < view->maxIterations) {
                unsigned char val = (unsigned char)(255 * iteration / view->maxIterations);
                buffer[i + j] = (Color){ val, 0, val, 255 };
            } else {
                buffer[i + j] = PURPLE;
            }
        }
    }

    return 0;
}
