/*******************************************************************************************
*
*   graphics experiment with raylib
*   
*   author: Austin Merrick
*
*/

#include "raylib.h"

struct Character {
    int width;
    int height;
    int x;
    int y;
    int speed;
    int dir;
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1920/3;
    int screenHeight = 1080/3;
    const int gameHeight = 160;
    const int gameWidth = 90;

    InitWindow(screenWidth, screenHeight, "graphics experiment");
    //ToggleFullscreen();

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    Texture2D characterTexture = LoadTexture("resources/character.png");
    SetTextureWrap(characterTexture, WRAP_CLAMP);

    struct Character character;
    character.width = characterTexture.width;
    character.height = characterTexture.height;
    character.x = 0;
    character.y = 0;
    character.speed = 1;
    character.dir = 1;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0};
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();

            if (IsWindowFullscreen()) {
                SetWindowSize( GetMonitorWidth(0), GetMonitorHeight(0) );
            }
        }

        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        camera.target = (Vector2){ 0, 0};
        camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
        camera.zoom = (float)screenHeight/(float)gameHeight;

        if (IsKeyDown(KEY_LEFT)) {
            character.x -= character.speed;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            character.x += character.speed;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            character.dir = -1;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            character.dir = 1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode2D(camera);
                DrawTexturePro(characterTexture, 
                    (Rectangle){0.0f,0.0f,character.dir*character.width,character.height},
                    (Rectangle){character.x, character.y, character.width, character.height},
                    (Vector2){character.width/2.0,character.height/2.0}, 0.0f, WHITE);
            EndMode2D();

            DrawFPS(10,10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}