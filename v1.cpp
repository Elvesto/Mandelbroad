#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 600;

int Render() {
    
    InitWindow(screenWidth, screenHeight, "Window");
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    
    Color color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    
    double zoom = 1.0;
    double offsetX = -0.5; 
    double offsetY = 0.0;
    int maxIterations = 200;
    
    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_UP)) zoom *= 1.05;
        if (IsKeyDown(KEY_DOWN)) zoom /= 1.05;
        if (IsKeyDown(KEY_D)) offsetX += 0.1 / zoom;
        if (IsKeyDown(KEY_A)) offsetX -= 0.1 / zoom;
        if (IsKeyDown(KEY_W)) offsetY -= 0.1 / zoom;
        if (IsKeyDown(KEY_S)) offsetY += 0.1 / zoom;
        if (IsKeyDown(KEY_Q)) maxIterations -= 5;
        if (IsKeyDown(KEY_E)) maxIterations += 5;
    
        BeginTextureMode(target);
        ClearBackground(BLACK);
        
        for (int py = 0; py < screenHeight; py++) {
            for (int px = 0; px < screenWidth; px++) {
                double x0 = (double)(px - screenWidth / 2.0) / (0.3 * zoom * screenWidth) + offsetX;
                double y0 = (double)(py - screenHeight / 2.0) / (0.3 * zoom * screenHeight) + offsetY;
        
                double x = 0.0;
                double y = 0.0;
                int iteration = 0;
        
                double x2 = 0.0;
                double y2 = 0.0;
        
                while (x2 + y2 <= 4.0 && iteration < maxIterations) {
                    y = 2.0 * x * y + y0;
                    x = x2 - y2 + x0;
        
                    x2 = x * x;
                    y2 = y * y;
                    iteration++;
                }
        
                if (iteration < maxIterations) {
                    DrawPixel(px, py, color);
                } else {
                    DrawPixel(px, py, (Color){255- color.r, 255 - color.g, 255 - color.b, 255});
                }
            }
        
        }
        EndTextureMode();
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureRec(target.texture, (Rectangle){0, 0, (double)target.texture.width, (double)-target.texture.height}, (Vector2) {0, 0}, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();
}
