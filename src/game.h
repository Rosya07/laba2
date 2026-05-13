#pragma once
#include "types.h"
#include <random>

void ResetGame(GameState&, std::mt19937&);
void HandlePlayerTurn(GameState&);
void HandleEnemyTurn(GameState&, std::mt19937&);
bool MouseToCell(int,int,int,int,Point&);
