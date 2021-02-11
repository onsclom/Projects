/*******************************************************************************************
*
*   graphics experiment with raylib
*   
*   author: Austin Merrick
*
*/

#include "raylib.h"

#define START_WIDTH 160
#define START_HEIGHT 90

#define ZOOM_MOD 2

#define COLOR_WHITE (Color) {0xff, 0xff, 0xff, 0xff}
#define COLOR_LIGHT_BLUE (Color) {0x67, 0x72, 0xa9, 0xff}
#define COLOR_DARK_BLUE (Color) {0x3a, 0x32, 0x77, 0xff}
#define COLOR_BLACK (Color) {0x28, 0x20, 0x3d, 0xff}

#define BLOCKS_HEIGHT 6
#define BLOCKS_WIDTH 20

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
struct Character {
    int width;
    int height;
    float x;
    float y;
    float yVel;
    float gravity;
    float jumpStrength;
    float speed;
    int dir;
    Texture2D texture;
};

struct Block {
    int width;
    int height;
    float x;
    float y;
    float randomx;
    float randomy;
    float angle;
    Texture2D texture;
};

float distanceModifierCalculate(struct Character character, struct Block block) {
    float distance = (character.x-block.x)*(character.x-block.x)
        + (character.y-block.y)*(character.y-block.y)*0.0f;
    
    const maxDistance = 5000.0f;
    distance /= maxDistance;

    distance = MIN(1.0f, distance);
    return distance;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = START_WIDTH;
    int screenHeight = START_HEIGHT;
    const int gameHeight = 160;
    const int gameWidth = 90;

    InitWindow(screenWidth, screenHeight, "graphics experiment");

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    // Character info
    struct Character character;
    character.texture = LoadTexture("resources/character.png");
    character.width = character.texture.width;
    character.height = character.texture.height;
    character.x = 0;
    character.y = 0;
    character.gravity = .1;
    character.speed = .5;
    character.dir = 1;
    character.jumpStrength = .75;
    SetTextureWrap(character.texture, WRAP_CLAMP);

    // Block info
    Texture2D blockTexture = LoadTexture("resources/block.png");

    const blockRandMax = 10;
    struct Block blocks[BLOCKS_WIDTH*BLOCKS_HEIGHT];
    for (int i=0; i<BLOCKS_WIDTH*BLOCKS_HEIGHT; i++) {
        blocks[i].x = (i%BLOCKS_WIDTH)*(blockTexture.width);
        blocks[i].y = i/BLOCKS_WIDTH*blockTexture.height;
        blocks[i].randomx = GetRandomValue(-blockRandMax, blockRandMax);
        blocks[i].randomy = GetRandomValue(-blockRandMax, blockRandMax);
        blocks[i].angle = GetRandomValue(-90.0f, 90.0f);


        //now to center
        blocks[i].x -= BLOCKS_WIDTH*blockTexture.width/2;

        //hacky placement of y
        blocks[i].y += blockTexture.height/2+character.height/2;


    }

    // Camera info
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
        
        // fullscreen toggle
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }

        if (IsKeyPressed(KEY_S)) {
            TakeScreenshot(".////very_cool");
        }

        // camera update
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        camera.target = (Vector2){ 0, 0};
        camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
        camera.zoom = (float)screenHeight/(float)gameHeight;
        camera.zoom *= ZOOM_MOD;

        // character control
        int curDirection = 0;
        if (IsKeyDown(KEY_LEFT)) {
            character.x -= character.speed;
            character.dir = -1;
            curDirection -= 1;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            character.x += character.speed;
            character.dir = 1;
            curDirection += 1;
        }

        //character hops
        character.yVel += character.gravity;
        character.y += character.yVel;
        if (character.y > 0) {
            character.y = 0;
            if (curDirection!=0) {
                character.yVel = -character.jumpStrength;
            }
        }


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(COLOR_BLACK);

            BeginMode2D(camera);
                //draw character
                DrawTexturePro(character.texture, 
                    (Rectangle){0.0f,0.0f,character.dir*character.width,character.height},
                    (Rectangle){character.x, character.y, character.width, character.height},
                    (Vector2){character.width/2.0,character.height/2.0}, curDirection*10.0f, WHITE);

                //draw block
                for (int i=0; i<BLOCKS_WIDTH*BLOCKS_HEIGHT; i++) { blocks[i];
                    // value from 0 to 1 based on distance from player to block
                    float distModifier = distanceModifierCalculate(character, blocks[i]);
                    DrawTexturePro(blockTexture, 
                        (Rectangle){0.0f,0.0f,blockTexture.width,blockTexture.height},
                        (Rectangle){blocks[i].x+blocks[i].randomx*distModifier, 
                            blocks[i].y+blocks[i].randomy*distModifier,
                            blockTexture.width, blockTexture.height},
                        (Vector2){blockTexture.width/2.0,blockTexture.height/2.0}, blocks[i].angle*distModifier, WHITE);
                }
            EndMode2D();

            //DrawFPS(10,10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}