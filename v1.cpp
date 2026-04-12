#include "raylib.h"
#include "render.h"


static const int screenWidth = 800;
static const int screenHeight = 600;

static void CalcMandelWrote(MandelWrote* view);

int Render() {
    
    InitWindow(screenWidth, screenHeight, "Window");
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    MandelWrote view = {};
    view.maxIterations = 200;
    view.offsetX = -0.5;
    view.offsetY = 0;
    view.zoom = 1.0;
    
    while (!WindowShouldClose()) {

        HandleInput(&view);
    
        BeginTextureMode(target);
        ClearBackground(BLACK);
        
        CalcMandelWrote(&view);
        
        EndTextureMode();
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2) {0, 0}, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

static void CalcMandelWrote(MandelWrote* view) {
    for (int py = 0; py < screenHeight; py++) {
        for (int px = 0; px < screenWidth; px++) {
            double x0 = (double)(px - screenWidth / 2.0) / (0.3 * view->zoom * screenWidth) + view->offsetX;
            double y0 = (double)(py - screenHeight / 2.0) / (0.3 * view->zoom * screenHeight) + view->offsetY;
    
            double x = 0.0;
            double y = 0.0;
            int iteration = 0;
    
            double x2 = 0.0;
            double y2 = 0.0;
    
            while (x2 + y2 <= 4.0 && iteration < view->maxIterations) {
                y = 2.0 * x * y + y0;
                x = x2 - y2 + x0;
    
                x2 = x * x;
                y2 = y * y;
                iteration++;
            }
    
            if (iteration < view->maxIterations) {
                unsigned char val = (unsigned char)(255 * iteration / view->maxIterations);
                DrawPixel(px, py, (Color){ val, 0, val, 255 });
            } else {
                DrawPixel(px, py, PURPLE);
            }
        }
    
    }
}