#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>

#include "immit.h"

static const int screenWidth = 800;
static const int screenHeight = 600;

static double zoom = 1.0;
static double offsetX = -0.5; 
static double offsetY = 0.0;
static int maxIterations = 200;

static int Calculate(Color* buffer, int countPix);

int Render() {
    int countPix = screenHeight * screenWidth;
    Color* buffer = (Color*)calloc(countPix, sizeof(Color));
    if (buffer == NULL) { return -1; }

    #ifdef GMODE
    InitWindow(screenWidth, screenHeight, "Window");
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    
    Color color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    

    Image image = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_UP)) zoom *= 1.05;
        if (IsKeyDown(KEY_DOWN)) zoom /= 1.05;
        if (IsKeyDown(KEY_D)) offsetX += 0.1 / zoom;
        if (IsKeyDown(KEY_A)) offsetX -= 0.1 / zoom;
        if (IsKeyDown(KEY_W)) offsetY -= 0.1 / zoom;
        if (IsKeyDown(KEY_S)) offsetY += 0.1 / zoom;
        if (IsKeyDown(KEY_Q)) maxIterations -= 5;
        if (IsKeyDown(KEY_E)) maxIterations += 5;
        
        Calculate(buffer, countPix);

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

    int64_t start = 0, end = 0;
    double res = 0;
    int n = 0;
    scanf("%d", &n);
    
    start = __rdtsc();
    for (int i = 0; i < n; i++) {
        Calculate(buffer, countPix);
    }
    end = __rdtsc();

    res = (end - start) / (double)n;

    printf("%lf\n", res);
    
    #endif


    return 0;
}

int Calculate(Color* buffer, int countPix) {


    for (int i = 0; i < countPix; i += 8) {
        
        float tempX[8] = {};
        float tempY[8] = {};
        for (int j = 0; j < 8; j++) {
            int index = i + j;
            if (index >= countPix) { break; }

            int px = index % screenWidth;
            int py = index / screenWidth;

            tempX[j] = (float)((px - screenWidth / 2.0) / (0.3 * zoom * screenWidth) + offsetX);
            tempY[j] = (float)((py - screenHeight / 2.0) / (0.3 * zoom * screenHeight) + offsetY);
        }

        m256 x0 = {tempX[0], tempX[1], tempX[2], tempX[3], tempX[4], tempX[5], tempX[6], tempX[7]};
        m256 y0 = {tempY[0], tempY[1], tempY[2], tempY[3], tempY[4], tempY[5], tempY[6], tempY[7]};
        m256 x = mm_set1_ps(0);
        m256 y = mm_set1_ps(0);
        m256 four = mm_set1_ps(4);
        m256i iters = {0, 0, 0, 0, 0, 0, 0, 0};

        for (int iteration = 0; iteration < maxIterations; iteration++) {

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
            if (iteration < maxIterations) {
                unsigned char val = (unsigned char)(255 * iteration / maxIterations);
                buffer[i + j] = (Color){ val, 0, val, 255 };
            } else {
                buffer[i + j] = PURPLE;
            }
        }
    }

    return 0;
}
