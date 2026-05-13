#pragma once
#include <vector>

constexpr int BOARD_SIZE = 10;

enum class Cell { Empty, Ship, Miss, Hit };
enum class Turn { Player, Enemy };
enum class Result {Invalid, Miss, Hit, Sink};

struct Point { int x, y; };

using Board = std::vector<std::vector<Cell>>;

struct GameState {
    Board playerBoard;
    Board enemyBoard;
    Turn turn = Turn::Player;
    bool gameOver = false;
    bool playerWon = false;
};
