#include <iostream>
#include <raylib.h>

using namespace std;

Color green = {255, 57, 255, 255};
Color red = {255, 0, 0, 255};

int cellSize = 30;
int cellCount = 25;

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food() {
        Image image = LoadImage("Graphics/Apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition();
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        float scale = (float)cellSize / texture.width;
        DrawTextureEx(texture, Vector2{position.x * cellSize, position.y * cellSize}, 0.0f, scale, WHITE);
    }

    Vector2 GenerateRandomPosition() {
        int x = GetRandomValue(0, cellCount - 1);
        int y = GetRandomValue(0, cellCount - 1);
        return Vector2{(float)x, (float)y};
    }

    void Respawn() {
        position = GenerateRandomPosition();
    }
};


int main () {

    cout << "Strting the game..." << endl;
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake Game");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    Food food = Food();
    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        food.Draw();



        EndDrawing();
    };


    CloseWindow(); // Close window and OpenGL context

    return 0;
}