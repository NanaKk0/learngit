#include <conio.h>
#include <iostream>

using namespace std;

class floor //地图/地板类,尚未实现更新状态
{
private:
    char content;

public:
    floor(char content)
    {
        this->content = content;
    };
    void update_content(int type); //尚未定义
    char getcontent();
    void print();
};

char floor::getcontent()
{
    return content;
}

void floor::print()
{
    putchar(content);
}

void deal_with_input()
{
    char ch;
    if (_kbhit())
    {
        ch = _getch();
        cout << ch;
    }
}

void display()
{
    system("cls");
}

class Player //玩家类,尚未实现更新状态
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
    void update_location(int type); //尚未定义
    pair<int, int> get_location();
}

pair<int, int>
Player::get_location()
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
    void update_time();
}

void
Bomb::update_time()

    int main()
{
}