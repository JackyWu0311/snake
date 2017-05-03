#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std ;

const int BORDER = 27;  //����X����  ,�P����170

struct Point {
    int x,y;
    void set(int X, int Y) {x=X; y=Y;}
};

HANDLE hIn, hOut;                          //I/O ���
Point  body[32], apple, v = {-2,0};        //�g���D����, ī�G, ���ʦV�q,
char   room[BORDER][BORDER*2+4];           //�����j�p
int    delay_time = 100;                    //����ɶ�
int    x1=2,y1=1,x2=(BORDER-1)*2,y2=BORDER-1, len=2, score=0;  //�������, ����, �_�l����
bool   Exit = false;                       //�P�_�O�_����C��

void gotoxy (int x, int y)   //���xy�y��
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

void init()    //��l�Ƴ���
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
    drawList (" �z","�w","�{", BORDER);
    drawList (" �x","  ","�x", BORDER, BORDER-2);
    drawList (" �|","�w","�}", BORDER);

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
    for (i=1; i<len; i++)                   //�ˬd���L�I���ۤv
        if (body[0].x == body[i].x &&
            body[0].y == body[i].y ) break;

    if (i!=len || x<=x1 || x>x2 || y<=y1 || y>y2) {
        draw (BORDER/2,BORDER/2,"�� ��");
        getch(); Exit = true;
        return;
    }
    if (x == apple.x && y == apple.y) {   //�Y��ī�G
        delay_time -= 2;
        score++;
        len++;
        if (score == 15) {                //�Y��15��ī�G�Y���
        	gotoxy(60, 12);
		    printf("�`�@�Y�F15��ī�G");
            draw (BORDER/2,BORDER/2, "�� �� �� �� !");
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
        draw (body[i].x, body[i].y, "�i");
}


int main ()
{
    init();
    while (!Exit)
    {
        Sleep (delay_time); if (kbhit()) key_control();
        draw (apple.x, apple.y, "��");
        move_snack();
        Sleep (delay_time); if (kbhit()) key_control();
        
        gotoxy(60,5);
        printf("��V��W�U���k����D�����");
        
        gotoxy(60, 12);
		printf("�`�@�Y�F%d��ī�G", score);

		gotoxy(60, 10);
		printf("�Y��30��ī�G�Y���!");

		gotoxy(60, 14);
		printf("�ثeī�G��m: (%2d,%2d)  �g���D��m: (%2d,%2d)",
			apple.x, apple.y, body[0].x, body[0].y);
    }
}
