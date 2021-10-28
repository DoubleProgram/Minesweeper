#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "External.h"

typedef enum CellState{
    Flagged,Questioned,Open,Closed,WrongBomb,BombHit
}CellState;

typedef enum CellKind{
    ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,BOMB,EMPTY
}CellKind;

typedef struct Cell{
    Object object;
    CellKind kind;
    CellState state;
}Cell;

void PrintMap(bool show);
void AssignDrawMap(int8_t XSize, int8_t YSize, int8_t bombs);
Cell* GetCell(int8_t y, int8_t x);
void Player(int8_t bombs);
bool isOutside(int8_t y, int8_t x);
bool isCellBomb(int8_t y, int8_t x);
void AssignCell(int8_t n, int8_t y, int8_t x);
void OpenCells(int8_t y, int8_t x);
void TryOpen(int8_t y, int8_t x);
void PrintBombsLeft(int8_t bombs, int16_t flagged);
void PrintCell(int8_t y, int8_t x);
#endif
