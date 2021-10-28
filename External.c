#include "External.h"

int Max(int n1,int n2){
   return n1 > n2 ? n1 : n2;
}

void ShowsCursor(bool visible){
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = visible;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void SetPosition(int16_t X, int16_t Y){
    HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position= { X, Y };
    SetConsoleCursorPosition(Screen, Position);
}

void cPrint(int16_t y, int16_t x, char c){
    SetPosition(x,y);
    printf("%c",c);
}

void SetColor(int8_t ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)){
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

void cColoredPrint(int16_t y,int16_t x,char c,Color color){
    SetColor(color);
    SetPosition(x,y);
    printf("%c",c);
    SetColor(WHITE);
}

void sPrint(int16_t y,int16_t x, char* s,Color color){
    SetColor(color);
    SetPosition(x,y);
    printf("%s",s);
    SetColor(WHITE);
}

void Print(const Object object){
    SetColor(object.color);
    cPrint(object.y,object.x,object.c);
    SetColor(WHITE);
}
