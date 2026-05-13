#include "bot.h"
#include "board.h"
#include <vector>
#include <cstdlib>
#include <ctime>

Point GetBotShot() {
    srand(time(0));
    return Point{rand() % BOARD_SIZE, rand() % BOARD_SIZE};
}
void SetBotResult(Result result) {
    return;
}