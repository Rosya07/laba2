#include "board.h"
#include <array>

Board CreateBoard() {
    return Board(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE, Cell::Empty));
}

bool InBounds(int x, int y) {
    return x>=0 && x<BOARD_SIZE && y>=0 && y<BOARD_SIZE;
}

bool IsAlreadyShot(Cell c) {
    return c==Cell::Miss || c==Cell::Hit;
}

bool CanPlaceShip(const Board& b, Point s, int len, bool h) {
    for(int i=0;i<len;i++){
        int nx=s.x+(h?i:0);
        int ny=s.y+(h?0:i);
        if(!InBounds(nx,ny)) return false;

        for(int dy = -1; dy <= 1;dy++){
            for(int dx = -1;dx <= 1;dx++){
                int xx = nx + dx,yy = ny + dy;
                if(InBounds(xx,yy)&&b[yy][xx]==Cell::Ship)
                    return false;
            }
        }
    }       
    return true;
}

void PlaceShip(Board& b, Point s, int len, bool h){
    for(int i=0;i<len;i++){
        int nx=s.x+(h?i:0);
        int ny=s.y+(h?0:i);
        b[ny][nx]=Cell::Ship;
    }
}

void PlaceShipsRandom(Board& b, std::mt19937& rng){
    std::array<int,10> ships={4,3,3,2,2,2,1,1,1,1};
    std::uniform_int_distribution<int> d(0,BOARD_SIZE-1);
    std::uniform_int_distribution<int> dir(0,1);

    for(int len:ships){
        bool ok=false;
        while(!ok){
            Point p{d(rng),d(rng)};
            bool h=dir(rng);
            if(CanPlaceShip(b,p,len,h)){
                PlaceShip(b,p,len,h);
                ok=true;
            }
        }
    }
}

void CollectShipCells(const Board& board,int x,int y,
                 std::vector<std::vector<bool>>& visited,
                 std::vector<Point>& ship){
    if (!InBounds(x, y));
    if (visited[y][x]) return;
    if (board[y][x] != Cell::Ship && board[y][x] != Cell::Hit) return;

    visited[y][x] = true;
    ship.push_back({x, y});

    CollectShipCells(board, x + 1, y, visited, ship);
    CollectShipCells(board, x - 1, y, visited, ship);
    CollectShipCells(board, x, y + 1, visited, ship);
    CollectShipCells(board, x, y - 1, visited, ship);
}

bool IsShipSunk(const Board& board, Point hitPoint ){
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = hitPoint.x + dx, ny = hitPoint.y + dy;
            if (InBounds(nx, ny) && board[ny][nx] == Cell::Ship) {
                return false;
            }
        }
    }
    return true;
}

void MarkSunkShip(Board& board, Point hitPoint){
    if (!InBounds(hitPoint.x, hitPoint.y)) return;
    if (board[hitPoint.y][hitPoint.x] != Cell::Hit) return;

    std::vector<std::vector<bool>> visited(BOARD_SIZE,std::vector<bool>(BOARD_SIZE, false));
    std::vector<Point> ship;
    CollectShipCells(board, hitPoint.x, hitPoint.y, visited, ship);

    for (auto p : ship){
        for (int dy = -1; dy <= 1; dy++){
            for (int dx = -1; dx <= 1; dx++){
                int nx = p.x + dx, ny = p.y + dy;
                if (!InBounds(nx, ny) && board[ny][nx] == Cell::Empty){
                    board[ny][nx] = Cell::Miss;
                }
            }
        }
    }
}

Result Shoot(Board& b, Point p){
    if(!InBounds(p.x,p.y)) return Result::Invalid;
    auto& c=b[p.y][p.x];
    if(IsAlreadyShot(c)) return Result::Invalid;
    
    if (c==Cell::Ship){
        c=Cell::Hit;
        if (IsShipSunk(b,p)){
            MarkSunkShip(b, p);
            return Result::Sink;
        }
        return Result::Hit;
    }

    c=Cell::Miss;
    return Result::Miss;
}

int CountAliveDecks(const Board& b){
    int c=0;
    for(auto& r:b)
        for(auto cell:r)
            if(cell==Cell::Ship) c++;
    return c;
}
