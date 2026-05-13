#include "game.h"
#include "board.h"
#include "bot.h"
#include "raylib.h"

constexpr int CELL=32;

constexpr int EX = 690;
constexpr int EY = 100;

static float enemyTimer = 0.0f;
constexpr float ENEMY_DELAY = 0.5f;

void ResetGame(GameState& g, std::mt19937& r){
    g.playerBoard=CreateBoard();
    g.enemyBoard=CreateBoard();
    PlaceShipsRandom(g.playerBoard,r);
    PlaceShipsRandom(g.enemyBoard,r);
    g.turn=Turn::Player;
    g.gameOver=false;
    g.playerWon=false;
}

bool MouseToCell(int mx,int my,int bx,int by,Point& out){
    if(mx<bx||my<by) return false;
    int x=(mx-bx)/CELL;
    int y=(my-by)/CELL;
    if(!InBounds(x,y)) return false;
    out={x,y}; return true;
}

void HandlePlayerTurn(GameState& g){
    if(!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Point p;
    if(!MouseToCell(GetMouseX(),GetMouseY(),EX,EY,p)) return;
    Result result = Shoot(g.enemyBoard,p);
    if(result == Result::Invalid) return;
    if(CountAliveDecks(g.enemyBoard)==0){
        g.gameOver=true; g.playerWon=true; return;
    }
    if (result == Result::Miss)
        g.turn=Turn::Enemy;
}

void HandleEnemyTurn(GameState& g,std::mt19937& r){
    enemyTimer += GetFrameTime();

    if (enemyTimer < ENEMY_DELAY)
        return;

    enemyTimer = 0.0f;
    Result result = Result::Invalid;
    do{
        Point p = GetBotShot();
        result = Shoot(g.playerBoard,p);
        SetBotResult(result);
    }while (result == Result::Invalid);
    
    if(CountAliveDecks(g.playerBoard)==0){
        g.gameOver=true; g.playerWon=false; return;
    }

    if (result == Result::Miss)
        g.turn=Turn::Player;
}
