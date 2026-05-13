#pragma once
#include "types.h"

struct Bot{
    Point lastHit = {-1, -1};
    std::vector<Point> huntTargets;
    int huntDirection = 0;
    bool hunting = false;

};

Point GetBotShot(Bot& bot, const Board& playerBoard);
void SetBotResult(Bot& bot, Result result, Point shot);