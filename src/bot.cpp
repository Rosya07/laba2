#include "bot.h"
#include "board.h"
#include <random>
#include <chrono>

static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
static std::uniform_int_distribution<int> dist(0, BOARD_SIZE - 1);

Point GetBotShot(Bot& bot, const Board& playerBoard) {
    if (!bot.huntTargets.empty()) {
        Point p = bot.huntTargets.back();
        bot.huntTargets.pop_back();
        return p;
    }
    
    return Point{dist(rng), dist(rng)};
}

void SetBotResult(Bot& bot, Result result, Point shot) {
    if (result == Result::Hit || result == Result::Sink) {
        if (result == Result::Hit) {
            Point neighbors[] = {
                {shot.x + 1, shot.y}, {shot.x - 1, shot.y},
                {shot.x, shot.y + 1}, {shot.x, shot.y - 1}
            };
            
            for (Point p : neighbors) {
                if (InBounds(p.x, p.y)) {
                    bool alreadyInQueue = false;
                    for (Point q : bot.huntTargets) {
                        if (q.x == p.x && q.y == p.y) {
                            alreadyInQueue = true;
                            break;
                        }
                    }
                    if (!alreadyInQueue) {
                        bot.huntTargets.push_back(p);
                    }
                }
            }
        }
        
        if (result == Result::Sink) {
            bot.huntTargets.clear();
            bot.hunting = false;
        }
    }
}