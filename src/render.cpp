#include "render.h"
#include "board.h"
#include "raylib.h"

#include <algorithm>
#include <vector>

constexpr int CELL = 32;

constexpr int PX = 160;
constexpr int PY = 100;

constexpr int EX = 690;
constexpr int EY = 100;

static void DrawBoard(const Board& b, int sx, int sy, bool hide)
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            int px = sx + x * CELL;
            int py = sy + y * CELL;

            Color c = LIGHTGRAY;
            auto cell = b[y][x];

            if (cell == Cell::Ship) c = hide ? LIGHTGRAY : DARKPURPLE;
            if (cell == Cell::Miss) c = DARKBLUE;
            if (cell == Cell::Hit)  c = RED;

            DrawRectangle(px, py, CELL - 1, CELL - 1, c);
        }
    }
}

static void CollectShipCells(const Board& b,
                             int x,
                             int y,
                             std::vector<std::vector<bool>>& used,
                             std::vector<Cell>& ship)
{
    if (!InBounds(x, y)) return;
    if (used[y][x]) return;

    if (b[y][x] != Cell::Ship && b[y][x] != Cell::Hit)
        return;

    used[y][x] = true;
    ship.push_back(b[y][x]);

    CollectShipCells(b, x + 1, y, used, ship);
    CollectShipCells(b, x - 1, y, used, ship);
    CollectShipCells(b, x, y + 1, used, ship);
    CollectShipCells(b, x, y - 1, used, ship);
}

static std::vector<std::vector<Cell>> GetShips(const Board& b)
{
    std::vector<std::vector<Cell>> ships;
    std::vector<std::vector<bool>> used(
        BOARD_SIZE,
        std::vector<bool>(BOARD_SIZE, false)
    );

    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (!used[y][x] && (b[y][x] == Cell::Ship || b[y][x] == Cell::Hit))
            {
                std::vector<Cell> ship;
                CollectShipCells(b, x, y, used, ship);
                ships.push_back(ship);
            }
        }
    }

    std::sort(
        ships.begin(),
        ships.end(),
        [](const std::vector<Cell>& a, const std::vector<Cell>& b)
        {
            return a.size() > b.size();
        }
    );

    return ships;
}

static bool IsShipSunkInPanel(const std::vector<Cell>& ship)
{
    for (Cell c : ship)
    {
        if (c == Cell::Ship)
            return false;
    }
    return true;
}

static void DrawFleetPanel(const Board& b, int x, int y)
{

    auto ships = GetShips(b);

    int rowY = y + 50;
    int mini = 22;

    for (const auto& ship : ships)
    {
        Color color = IsShipSunkInPanel(ship) ? RED : DARKPURPLE;

        for (int i = 0; i < (int)ship.size(); i++)
        {
            int cellX = x + i * (mini + 4);
            DrawRectangle(cellX, rowY, mini, mini, color);
            DrawRectangleLines(cellX, rowY, mini, mini, BLACK);
        }

        rowY += mini + 12;
    }
}

void DrawUI(const GameState& g)
{
    DrawText("PLAYER", PX + 95, 50, 34, BLACK);
    DrawText("BOT", EX + 125, 50, 34, BLACK);

    DrawBoard(g.playerBoard, PX, PY, false);
    DrawBoard(g.enemyBoard, EX, EY, true);

    DrawFleetPanel(g.playerBoard, 20, 100);
    DrawFleetPanel(g.enemyBoard, 1040, 100);


    if (g.gameOver)
    {
        const char* text = g.playerWon ? "PLAYER WINS!" : "BOT WINS!";
        int fontSize = 56;
        int textWidth = MeasureText(text, fontSize);

        DrawRectangle(320, 440, 640, 80, Fade(RAYWHITE, 0.85f));
        DrawText(text, 640 - textWidth / 2, 452, fontSize,
                 g.playerWon ? DARKGREEN : RED);
    }
}