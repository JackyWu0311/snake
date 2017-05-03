#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std ;

const int BORDER = 27;  //場景X長度  ,周長為170

struct Point {
    int x,y;
    void set(int X, int Y) {x=X; y=Y;}
};

HANDLE hIn, hOut;                          //I/O 控制器
Point  body[32], apple, v = {-2,0};        //貪食蛇身體, 蘋果, 移動向量,
char   room[BORDER][BORDER*2+4];           //場景大小
int    delay_time = 100;                    //延遲時間
int    x1=2,y1=1,x2=(BORDER-1)*2,y2=BORDER-1, len=2, score=0;  //場景邊界, 身長, 起始分數
bool   Exit = false;                       //判斷是否持續遊戲

void gotoxy (int x, int y)   //選取xy座標
{
    static COORD c;
    c.X = x; c.Y = y;
    SetConsoleCursorPosition (hOut, c);
}

void draw (int x, int y, char* s)
{
    gotoxy (x,y);
    cout << s;
}

void drawList (char* a, char* b, char* c, int w, int h=1)
{
    static char* p = room[0];
    for (int i; h--; *p++=*c, *p++=c[1], *p++=0, p++)
        for (*p++='\n', *p++=*a, *p++=a[1], *p++=a[2],
            i=w-2; i--;) *p++=*b, *p++=b[1];
}
void put_apple ()
{
    apple.x = 3 + rand()%(BORDER-2) * 2;
    apple.y = 2 + rand()%(BORDER-3);

}

void init()    //初始化場景
{
    srand (time(0));
    hOut = GetStdHandle (STD_OUTPUT_HANDLE);
    hIn  = GetStdHandle (STD_INPUT_HANDLE);
    HANDLE err = INVALID_HANDLE_VALUE;
    if (hIn == err || hOut == err) {
        puts ("handle failed");
        getch ();
        exit (1);
    }
    drawList (" ┌","─","┐", BORDER);
    drawList (" │","  ","│", BORDER, BORDER-2);
    drawList (" └","─","┘", BORDER);

    gotoxy (0,0);
    for (int i=0; i<BORDER; i++) cout << room[i];
    put_apple ();

    body[0].set ((x2-x1)/2, (y2-y1)/2);
    body[1].set (body[0].x+1, body[0].y);
}

void key_control()
{
    static DWORD count;
    static INPUT_RECORD ir;

    ReadConsoleInput (hIn, &ir, 1, &count);
    if (!ir.Event.KeyEvent.bKeyDown) return;
    switch (ir.Event.KeyEvent.wVirtualKeyCode) {
        case VK_UP   : if (v.y !=  1) v.set(0,-1); break;
        case VK_DOWN : if (v.y != -1) v.set(0, 1); break;
        case VK_LEFT : if (v.x !=  2) v.set(-2,0); break;
        case VK_RIGHT: if (v.x != -2) v.set( 2,0); break;
        case VK_ESCAPE: Exit = true;
    }
}

void move_snack()
{
    int i;
    int& x = body[0].x;
    int& y = body[0].y;
    for (i=1; i<len; i++)                   //檢查有無碰撞自己
        if (body[0].x == body[i].x &&
            body[0].y == body[i].y ) break;

    if (i!=len || x<=x1 || x>x2 || y<=y1 || y>y2) {
        draw (BORDER/2,BORDER/2,"失 敗");
        getch(); Exit = true;
        return;
    }
    if (x == apple.x && y == apple.y) {   //吃到蘋果
        delay_time -= 2;
        score++;
        len++;
        if (score == 15) {                //吃到15顆蘋果即獲勝
        	gotoxy(60, 12);
		    printf("總共吃了15顆蘋果");
            draw (BORDER/2,BORDER/2, "恭 喜 獲 勝 !");
            getch(); Exit = true;
            return;
        }put_apple();
    }
    else draw (body[len-1].x, body[len-1].y, "  ");

    for (i=len-1; i>0; --i)
        body[i] = body[i-1];
    x += v.x;
    y += v.y;
    for (i=0; i<len; ++i)
        draw (body[i].x, body[i].y, "█");
}


int main ()
{
    init();
    while (!Exit)
    {
        Sleep (delay_time); if (kbhit()) key_control();
        draw (apple.x, apple.y, "◎");
        move_snack();
        Sleep (delay_time); if (kbhit()) key_control();
        
        gotoxy(60,5);
        printf("方向鍵上下左右控制蛇的行動");
        
        gotoxy(60, 12);
		printf("總共吃了%d顆蘋果", score);

		gotoxy(60, 10);
		printf("吃到30顆蘋果即獲勝!");

		gotoxy(60, 14);
		printf("目前蘋果位置: (%2d,%2d)  貪食蛇位置: (%2d,%2d)",
			apple.x, apple.y, body[0].x, body[0].y);
    }
}
