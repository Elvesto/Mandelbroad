#include "raylib.h"
#include "render.h"

#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>


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
    for (int i = 0; i < countPix; i++) {
        int px = i % screenWidth;
        int py = i / screenWidth;

        double x0 = (double)(px - screenWidth / 2.0) / (0.3 * zoom * screenWidth) + offsetX;
        double y0 = (double)(py - screenHeight / 2.0) / (0.3 * zoom * screenHeight) + offsetY;
        double x = 0, y = 0, x2 = 0, y2 = 0;
        int iteration = 0;

        while (x2 + y2 <= 4.0 && iteration <= maxIterations) {
            y = 2.0 * x * y + y0;
            x = x2 - y2 + x0;
            x2 = x * x;
            y2 = y * y;
            iteration++;
        }
        if (iteration < maxIterations) {
                unsigned char val = (unsigned char)(255 * iteration / maxIterations);
                buffer[i] = (Color){ val, 0, val, 255 };
            } else {
                buffer[i] = PURPLE;
            }
    }

    return 0;
}