#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {57, 255, 20, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime  = 0;

bool eventTriggered(double interval) 
{
    double currentTime = GetTime();
    if ((currentTime - lastUpdateTime) >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake 
{
    public :
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};

        void Draw() 
        {
            for (unsigned int i = 0; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
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

        if (eventTriggered(0.2))
        {
            snake.Update();
        }

        if (IsKeyPressed(KEY_UP))
        {
            snake.direction = {0, -1};
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            snake.direction = {0, 1};
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            snake.direction = {-1, 0};
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            snake.direction = {1, 0};
        }

       
        ClearBackground(BLACK);
        food.Draw();
        snake.Draw();



        EndDrawing();
    };


    CloseWindow(); // Close window and OpenGL context

    return 0;
}