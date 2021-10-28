#include <stdio.h>
#include <stdlib.h>
#include "External.h"
#include "Map.h"
#include <time.h>
bool Play();
extern Cell* map;
int main(){
    srand(time(NULL));
    ShowsCursor(false);
    SetConsoleTitle("Minesweeper");
    int8_t ylength = 0,xlength = 0,bombs = 0;
    
    do{
        sPrint(1,40/2 - 17,"Which level do you want to play?",GREEN);
        sPrint(2,40/2 - 20,"[1 = Easy, 2 = Intermediate, 3 = Hard]",WHITE);
        switch (getch()){
            case '2': ylength = 16; xlength = 16; bombs = 40; break;
            case '3': ylength = 16; xlength = 30; bombs = 99; break;
            default: ylength = 9; xlength = 9; bombs = 10; break;
        }
        system("cls");
        AssignDrawMap(ylength,xlength,bombs);
        Player(bombs);
        sPrint(ylength+3,Max(xlength/2 - 11,0),"Do you want to replay?",WHITE);
        sPrint(ylength+4,Max(xlength/2 - 3,0)," [Y/N]",WHITE);
        free(map);
        map = NULL;
    } while (Play());

    sPrint(ylength+5,Max(xlength/2 - 10,0),"Thanks for playing! ",MAGENTA);
    Sleep(2500);
    SetPosition(0,ylength+5);

    return 0;
}
bool Play(){
    while (true){
        switch (getch()){
            case 'y': case 'Y': system("cls"); return true;
            case 'n': case 'N': return false;
            default: break;
        }
    }
}







