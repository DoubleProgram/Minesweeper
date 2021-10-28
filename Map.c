#include "Map.h"

static uint8_t ySize,xSize;
Cell* map;

void PrintMap(bool show){
    for (int i = 0; i < ySize*xSize; i++){
        if (!show || map[i].state == BombHit)
            PrintCell(map[i].object.y,map[i].object.x);
        else Print(map[i].object);
    }
}

void AssignDrawMap(int8_t YSize, int8_t XSize, int8_t bombs){
    ySize = YSize; xSize = XSize;

    map = (Cell*)malloc(sizeof(Cell)*(ySize * xSize));
    for (int i = 0; i < ySize*xSize; i++) map[i].kind = EMPTY;
    /*Setting bombs*/
    int8_t randomY;
    int8_t randomX;
    for (int i = 0; i<bombs;){
        randomY = rand() % ySize;
        randomX = rand() % xSize;

        if (GetCell(randomY,randomX)->kind != BOMB){
            GetCell(randomY,randomX)->kind = BOMB;
            GetCell(randomY,randomX)->object = (Object){MAGENTA,'X',randomY,randomX};
            i++;
        }
    }
	/*counting the surrounded bombs around each cell*/
    Cell* cell;
    int8_t n = 0;
    for (int8_t y = 0; y < ySize; y++)
        for (int8_t x = 0; x < xSize; x++){
            GetCell(y,x)->state = Closed;
            if (GetCell(y,x)->kind != BOMB){
                if (isCellBomb(y+1,x)) n++;
                if (isCellBomb(y,x+1)) n++;

                if (isCellBomb(y-1,x)) n++;
                if (isCellBomb(y,x-1)) n++;

                if (isCellBomb(y+1,x-1)) n++;
                if (isCellBomb(y-1,x+1)) n++;

                if (isCellBomb(y-1,x-1)) n++;
                if (isCellBomb(y+1,x+1)) n++;

                AssignCell(n,y,x);
                n = 0;
            }
        }
    PrintBombsLeft(bombs,0);
    PrintMap(false);
}

void OpenCells(int8_t y, int8_t x){
    if (GetCell(y,x)->kind != EMPTY) return;
    TryOpen(y+1,x);
    TryOpen(y,x+1);
    TryOpen(y-1,x);
    TryOpen(y,x-1);
    TryOpen(y+1,x-1);
    TryOpen(y-1,x+1);
    TryOpen(y-1,x-1);
    TryOpen(y+1,x+1);
}

void TryOpen(int8_t y, int8_t x){
    Cell *cell;
    if (!isOutside(y,x)) {
        cell = GetCell(y,x);
        if (cell->state == Open) return;
        cell->state = Open;

        PrintCell(y,x);
        if (cell->kind == EMPTY) OpenCells(y,x);
    }
}

void AssignCell(int8_t n, int8_t y, int8_t x){
    Cell* cell = GetCell(y,x);
    switch (n){
        case 0: cell->kind = EMPTY; cell->object = (Object){WHITE,' ',y,x}; break;
        case 1: cell->kind = ONE; cell->object = (Object){BLUE,'1',y,x}; break;
        case 2: cell->kind = TWO; cell->object = (Object){GREEN,'2',y,x}; break;
        case 3: cell->kind = THREE; cell->object = (Object){RED,'3',y,x}; break;
        case 4: cell->kind = FOUR; cell->object = (Object){DARKBLUE,'4',y,x}; break;
        case 5: cell->kind = FIVE; cell->object = (Object){DARKRED,'5',y,x}; break;
        case 6: cell->kind = SIX; cell->object = (Object){CYAN,'6',y,x}; break;
        case 7: cell->kind = SEVEN; cell->object = (Object){BLACK,'7',y,x}; break;
        case 8: cell->kind = EIGHT; cell->object = (Object){GRAY,'8',y,x}; break;
    }
}

Cell* GetCell(int8_t y, int8_t x){
    return &map[x+y * xSize];
}

bool isCellBomb(int8_t y, int8_t x){
    if (isOutside(y,x)) return false;
    return (GetCell(y,x)->kind == BOMB);
}

void Player(int8_t bombs){
    Object player = (Object){DARKMAGENTA,'P',0,0};
    int tempy = 0,tempx = 0;
    bool gameover = false;
    bool showMap = false;
    int16_t flagged = 0;
	int8_t bombsFlagged = 0, questioned = 0;
	while (!gameover){
        switch (getch()){
            case 'w': case 'W': if (!isOutside(player.y-1,player.x)) player.y--; break;
            case 's': case 'S': if (!isOutside(player.y+1,player.x)) player.y++; break;
            case 'd': case 'D': if (!isOutside(player.y,player.x+1)) player.x++; break;
            case 'a': case 'A': if (!isOutside(player.y,player.x-1)) player.x--; break;
            case 'o': case 'O': //open cell
                GetCell(player.y,player.x)->state = Open;
                OpenCells(player.y,player.x);
                if (GetCell(player.y,player.x)->kind == BOMB){
                    GetCell(player.y,player.x)->state = BombHit;
                    sPrint(ySize+2,Max(xSize/2 - 5,0),"Game Over!",DARKRED);
                    gameover = true;
                }
                break;
            case 'f': case 'F': //flag the cell
                if (GetCell(player.y,player.x)->state == Open) break;
                if (GetCell(player.y,player.x)->state == Questioned) questioned--;

                if (GetCell(player.y,player.x)->state == Flagged){
                    flagged--;
                    PrintBombsLeft(bombs,flagged);
                    GetCell(player.y,player.x)->state = Closed;
                    if (GetCell(player.y,player.x)->kind == BOMB) bombsFlagged--;
                    break;
                }
                flagged++;
                PrintBombsLeft(bombs,flagged);
                GetCell(player.y,player.x)->state = Flagged;
                if (GetCell(player.y,player.x)->kind == BOMB) bombsFlagged++;
                break;
            case 'q': case 'Q': //set a question to current cell
                if (GetCell(player.y,player.x)->state == Open) break;
                if (GetCell(player.y,player.x)->state == Flagged){
                    if (GetCell(player.y,player.x)->kind == BOMB) bombsFlagged--;
                    flagged--; PrintBombsLeft(bombs,flagged);
                }
                if (GetCell(player.y,player.x)->state == Questioned) {
                    questioned--;
                    GetCell(player.y,player.x)->state = Closed;
                    break;
                }
                GetCell(player.y,player.x)->state = Questioned;
                questioned++;
                break;
            case 'c': case 'C': PrintMap(showMap = !showMap); break; //cheat
            case 'l': case 'L': PrintMap(true); return; //leave
        }
        if (bombsFlagged == bombs && flagged == bombs && questioned == 0){ //if all bombs flagged, game won
            sPrint(ySize+2,Max(xSize/2 - 5,0)," Game Won!",DARKGREEN);
            gameover = true;
        }
        showMap == false ? PrintCell(tempy,tempx) : Print(GetCell(tempy,tempx)->object);
        tempy = player.y; tempx = player.x;
        Print(player);
    }
    PrintMap(true);
}

void PrintBombsLeft(int8_t bombs, int16_t flagged){
    sPrint(ySize+1,Max(xSize/2 - 6,0),"Bombs Left: ",WHITE);
    printf("%d ",bombs-flagged);
}

void PrintCell(int8_t y, int8_t x){
   Object o = (Object){DARKYELLOW,'#',y,x};
   switch (GetCell(y,x)->state){
        case BombHit: o = (Object){DARKMAGENTA ,'X',y,x}; break;
        case Flagged: o = (Object){DARKRED,'F',y,x}; break;
        case Questioned: o = (Object){WHITE,'?',y,x}; break;
        case Open: o = GetCell(y,x)->object; break;
        default: break;
   }
   Print(o);
}

bool isOutside(int8_t y, int8_t x){
   return (y < 0 || y >= ySize || x < 0 || x >= xSize);
}
