#include <conio.h>
#include <iostream>
#include <utility>

using namespace std;

const int ONE_SECOND = 1000;

int count = 0;

const int n = 10, m = 10;

const int FLOOR = 0, MOVEA = 1, MOVEB = 2;

void display();

class Floor //地图/地板类
{
private:
    char content[n + 2][m + 2];

public:
    Floor();
    void update_content(int type, int x, int y);
    char getcontent(int x, int y);
    void print();
};

Floor::Floor()
{
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            content[i][j] = ' ';
}

void Floor::update_content(int type, int x, int y)
{
    switch (type)
    {
    case 0:
        content[x][y] = ' ';
        break;
    case 1:
        content[x][y] = 'A';
        break;
    case 2:
        content[x][y] = 'B';
        break;
    }
}

char Floor::getcontent(int x, int y)
{
    return content[x][y];
}

void Floor::print()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
            putchar(content[i][j]);
        putchar('\n');
    }
}

Floor floor;

class Player //玩家类
{
private:
    pair<int, int> p;
    char symbol;

public:
    Player(int x, int y, char symbol)
    {
        this->p.first = x;
        this->p.second = y;
        this->symbol = symbol;
    };
    void update_location(int type);
    pair<int, int> get_location();
};

Player player1(1, 1, 'A');

void Player::update_location(int type)
{
    switch (type)
    {
    case 1:
        if (p.second <= m)
        {
            p.second++;
            floor.update_content(MOVEA, p.first, p.second);
            floor.update_content(FLOOR, p.first, p.second - 1);
        }
        break;
    case 2:
        if (p.second > 1)
        {
            p.second--;
            floor.update_content(MOVEA, p.first, p.second);
            floor.update_content(FLOOR, p.first, p.second + 1);
        }
        break;
    case 3:
        if (p.first > 1)
        {
            p.first--;
            floor.update_content(MOVEA, p.first, p.second);
            floor.update_content(FLOOR, p.first + 1, p.second);
        }
        break;
    case 4:
        if (p.first <= n)
        {
            p.first++;
            floor.update_content(MOVEA, p.first, p.second);
            floor.update_content(FLOOR, p.first - 1, p.second);
        }
        break;
    }
    display();
    return;
}

pair<int, int> Player::get_location()
{
    return p;
}

class Bomb //炸弹光束类,尚未实现更新时间以及变成光的状态
{
private:
    pair<int, int> p;
    int type;
    int time;

public:
    Bomb(int x, int y, int type)
    {
        this->p.first = x;
        this->p.second = y;
        this->type = type;
        this->time = 4;
    };
    // void update_time();
};

void display()
{
    system("cls");
    floor.print();
}

void init()
{
    floor.update_content(MOVEA, 1, 1);
    return;
}

void deal_with_timer()
{
    return;
}

void deal_with_input()
{
    char ch;
    if (_kbhit())
    {
        ch = _getch();
        cout << ch;
        switch (ch)
        {
        case 'w':
            player1.update_location(3);
            break;
        case 'W':
            player1.update_location(3);
            break;
        case 's':
            player1.update_location(4);
            break;
        case 'S':
            player1.update_location(4);
            break;
        case 'a':
            player1.update_location(2);
            break;
        case 'A':
            player1.update_location(2);
            break;
        case 'd':
            player1.update_location(1);
            break;
        case 'D':
            player1.update_location(1);
            break;
        }
    }
}

int main()
{
    init();
    display();
    while (1)
    {
        deal_with_input();
        count++;
        if (count == ONE_SECOND)
        {
            deal_with_timer();
            count = 0;
        }
    }
}