#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {57, 255, 20, 255};

int cellSize = 30;
int cellCount = 25;


class Snake 
{
    public :
        deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        Vector2 direction = {1,0};

        void Draw() {
            for (unsigned int i = 0; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = {x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, green);
            }
                
        }

        void Update()
        {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
};

class Food 
{
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
    Snake snake = Snake();
    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        food.Draw();
        snake.Draw();



        EndDrawing();
    };


    CloseWindow(); // Close window and OpenGL context

    return 0;
}