#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {57, 255, 20, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

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
        bool addSegment = false;

        void Draw() 
        {
            for (unsigned int i = 0; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, green);
            }
                
        }

        void Update()
        {

            body.push_front(Vector2Add(body[0], direction));

            if (addSegment){

                addSegment = false;

            } else {

                body.pop_back();

            }

        }

        void reset()
        {
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction = {1, 0};
            addSegment = false;
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
        DrawTextureEx(texture, Vector2{offset + position.x * cellSize, offset + position.y * cellSize}, 0.0f, scale, WHITE);
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
        bool running = true;
        int score = 0;

        void Update()
        {
            // Game update logic goes here
            if (running) {
                snake.Update();
                CheckFoodCollision();
                CheckCollisionWithEdges();
                checkSelfCollision();
            }
            
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
                snake.addSegment = true;
                score += 1;
            }
        }

        void CheckCollisionWithEdges(){

            if(snake.body[0].x == cellCount || snake.body[0].x == -1) {
                GameOver();
            }
            if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
                GameOver();
            }
        }

        void GameOver(){
            cout << "Game Over! Your score: " << snake.body.size() - 3 << endl;
            snake.reset();
            food.position = food.GenerateRandomPosition(snake.body);
            running = false;
            score = 0;
        }

        void checkSelfCollision(){
            deque<Vector2> headlessBody = snake.body;
            headlessBody.pop_front();
            if(elementinDeque(snake.body[0], headlessBody)){
                GameOver();
            }
        }
};

int main () {

    cout << "Strting the game..." << endl;
    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Snake Game");
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
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

       
        ClearBackground(BLACK);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, green);
        DrawText("Snake" ,offset-5, 20, 40, green);
        DrawText(TextFormat("Score: %d", game.score), offset-5, offset+cellCount*cellSize+10, 40, green);
        game.Draw();

        if (!game.running) {
            DrawText("Press arrow keys to start! of escape to quit :(", offset + 10, offset + cellCount * cellSize / 2 - 20, 30, GREEN);
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            break; // Exit the game loop
        }


        EndDrawing();
    };


    CloseWindow(); // Close window and OpenGL context

    return 0;
}