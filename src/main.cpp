#include "game.h"
#include "render.h"
#include "raylib.h"
#include <random>

int main(){
    std::mt19937 rng(std::random_device{}());
    InitWindow(1280,560,"SeaBattle");
    SetTargetFPS(60);

    GameState g;
    ResetGame(g,rng);

    while(!WindowShouldClose()){
        if(!g.gameOver){
            if(g.turn==Turn::Player) HandlePlayerTurn(g);
            else HandleEnemyTurn(g,rng);
        }
        else if(IsKeyPressed(KEY_R)){
            ResetGame(g,rng);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if(!g.gameOver)
            DrawText(g.turn == Turn::Player ? "PLAYER TURN" : "BOT TURN", 480, 20, 40, BLACK);
        DrawUI(g);
        EndDrawing();
    }

    CloseWindow();
}
