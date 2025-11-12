#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {57, 255, 20, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime  = 0;

bool elementinDeque(Vector2 element, deque<Vector2> deq)
{
    for (unsigned int i = 0; i < deq.size(); i++)
    {
        if (Vector2Equals(element, deq[i]))
        {
            return true;
        }
    }
    return false;
}

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

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("Graphics/Apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition(snakeBody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        float scale = (float)cellSize / texture.width;
        DrawTextureEx(texture, Vector2{position.x * cellSize, position.y * cellSize}, 0.0f, scale, WHITE);
    }

    Vector2 GenerateRandomCell() {
        int x = GetRandomValue(0, cellCount - 1);
        int y = GetRandomValue(0, cellCount - 1);
        return Vector2{(float)x, (float)y};
    }

    Vector2 GenerateRandomPosition(deque<Vector2> snakeBody) {
        
        Vector2 pos = GenerateRandomCell();
        while (elementinDeque(pos, snakeBody)) {
            pos = GenerateRandomCell();
        }

        return pos;
        
    }
};

class Game
{
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);

        void Update()
        {
            // Game update logic goes here
            snake.Update();
            CheckFoodCollision();
        }

        void Draw()
        {
            // Game drawing logic goes here
            food.Draw();
            snake.Draw();   
        }

        void CheckFoodCollision()
        {
            if (Vector2Equals(snake.body[0], food.position))
            {
                // Snake eats the food
                food.position = food.GenerateRandomPosition(snake.body);
            }
        }
};

int main () {

    cout << "Strting the game..." << endl;
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake Game");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    
    Game game = Game();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        if (eventTriggered(0.2))
        {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
        }

       
        ClearBackground(BLACK);
        game.Draw();



        EndDrawing();
    };


    CloseWindow(); // Close window and OpenGL context

    return 0;
}