#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdio.h>

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

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
    
    Image image = GenImageColor(screenWidth, screenHeight, BLACK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);



    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_UP)) zoom *= 1.05;
        if (IsKeyDown(KEY_DOWN)) zoom /= 1.05;
        if (IsKeyDown(KEY_D)) offsetX += 0.01 / zoom;
        if (IsKeyDown(KEY_A)) offsetX -= 0.01 / zoom;
        if (IsKeyDown(KEY_W)) offsetY -= 0.01 / zoom;
        if (IsKeyDown(KEY_S)) offsetY += 0.01 / zoom;
        if (IsKeyDown(KEY_Q)) maxIterations -= 5;
        if (IsKeyDown(KEY_E)) maxIterations += 5;        

        UpdateTexture(texture, buffer);

        Calculate(buffer, countPix);

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

        __m256 x0 = _mm256_loadu_ps(tempX);
        __m256 y0 = _mm256_loadu_ps(tempY);
        __m256 x = _mm256_set1_ps(0);
        __m256 y = _mm256_set1_ps(0);
        __m256 four = _mm256_set1_ps(4);
        __m256i iters = _mm256_setzero_si256();
        __m256i one = _mm256_set1_epi32(1);

        for (int iteration = 0; iteration < maxIterations; iteration++) {

            __m256 x2 = _mm256_mul_ps(x, x);
            __m256 y2 = _mm256_mul_ps(y, y);

            __m256 mask = _mm256_cmp_ps(_mm256_add_ps(x2, y2), four, _CMP_LE_OQ);

            if (_mm256_movemask_ps(mask) == 0) {
                break;
            }

            iters = _mm256_add_epi32(iters, _mm256_and_si256(one, _mm256_castps_si256(mask)));

            __m256 xy = _mm256_mul_ps(x, y);
            y = _mm256_add_ps(_mm256_add_ps(xy, xy), y0);

            x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
        }

        int final[8] = {};
        _mm256_storeu_si256((__m256i*)final, iters);

        for (int j = 0; j < 8; j++) {
            if (i + j >= countPix) { break; }

            int iteration = final[j];
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