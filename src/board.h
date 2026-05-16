#pragma once
#include "types.h"
#include <random>

Board CreateBoard();
bool InBounds(int x, int y);
bool IsAlreadyShot(Cell cell);

bool CanPlaceShip(const Board&, Point, int, bool);
void PlaceShip(Board&, Point, int, bool);
void PlaceShipsRandom(Board&, std::mt19937&);

Result Shoot(Board&, Point);
int CountAliveDecks(const Board&);
void CollectShipCells(const Board&, int, int, std::vector<std::vector<bool>>&, std::vector<Point>&);
bool IsShipSunk(const Board&, Point);