#include <conio.h>
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int ONE_SECOND = 12000;

bool DIED = false, LIVED = true;

int count = 0;

int Move_a_count = 0, Move_b_count = 0;

const int n = 13, m = 15;

const int FLOOR = 0, MOVEA = 1, MOVEB = 2;

void display();

class Floor //地图/地板类
{
private:
    char content[n + 2][m + 2];

public:
    Floor();
    void load_floor(Floor floor1);
    void update_content(int type, int x, int y);
    char getcontent(int x, int y);
    void print();
};

Floor::Floor()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (i == 2 || i == 12)
            {
                if ((j >= 2 && j <= 7) || (j >= 9 && j <= 14))
                    content[i][j] = '#';
                else
                    content[i][j] = ' ';
            }
            else if (i == 3 || i == 11)
            {
                if (j == 1 || j == 2 || (j >= 4 && j <= 15))
                    content[i][j] = '*';
                else
                    content[i][j] = ' ';
            }
            else if (i == 4 || i == 10)
            {
                if ((j >= 1 && j <= 2) || (j == 4) || (j >= 6 && j <= 10) || (j == 12) || (j >= 14 && j <= 15))
                    content[i][j] = '#';
                else
                    content[i][j] = '*';
            }
            else if (i == 5 || i == 9)
            {
                if (j == 4 || j == 12)
                    content[i][j] = '#';
                else
                    content[i][j] = '*';
            }
            else if (i == 6 || i == 8)
            {
                if (j == 2 || j == 4 || j == 5 || j == 7 || j == 8 || j == 9 || j == 11 || j == 12 || j == 14)
                    content[i][j] = '#';
                else
                    content[i][j] = '*';
            }
            else if (i == 7)
            {
                if (j == 2 || (j >= 7 && j <= 9) || j == 14)
                    content[i][j] = '#';
                else if (j == 1 || j == 6 || j == 13)
                    content[i][j] = ' ';
                else
                    content[i][j] = '*';
            }
            else if (j == 3 || j == 4 || j == 6 || j == 7 || (j >= 9 && j <= 12))
                content[i][j] = '*';
            else
                content[i][j] = ' ';
        }
    }
    for (int i = 0; i <= n + 1; i++)
        content[i][0] = '#';
    for (int i = 0; i <= n + 1; i++)
        content[i][m + 1] = '#';
    for (int j = 1; j <= m; j++)
        content[0][j] = '#';
    for (int j = 1; j <= m; j++)
        content[n + 1][j] = '#';
}

char Floor::getcontent(int x, int y)
{
    return content[x][y];
}

void Floor::load_floor(Floor floor)
{
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            content[i][j] = floor.getcontent(i, j);
}

Floor floor;

Floor prefloor;

void Floor::update_content(int type, int x, int y) //地图更新
{
    switch (type)
    {
    case 0: //地板
        content[x][y] = ' ';
        break;
    case 1: //角色1
        content[x][y] = 'A';
        break;
    case 2: //角色2
        content[x][y] = 'B';
        break;
    case 3: //炸弹
        content[x][y] = 'O';
        break;
    case 4: //四个方向的光束
        content[x][y] = '<';
        break;
    case 5:
        content[x][y] = '>';
        break;
    case 6:
        content[x][y] = '^';
        break;
    case 7:
        content[x][y] = 'v';
        break;
    case 8:
        content[x][y] = 'X';
        break;
    case 9:
        content[x][y] = 'L'; // Larger,炸弹增强
        break;
    case 10:
        content[x][y] = 'Q'; // Quicker,速度增加
        break;
    }
}

void Floor::print()
{
    for (int i = 0; i <= n + 1; i++)
    {
        for (int j = 0; j <= m + 1; j++)
            putchar(content[i][j]);
        putchar('\n');
    }
}

class Player //玩家类
{
private:
    pair<int, int> p;
    char symbol;
    bool has_a_bomb;
    bool is_larger;
    bool is_quicker;

public:
    Player(int x, int y, char symbol)
    {
        this->p.first = x;
        this->p.second = y;
        this->symbol = symbol;
        this->has_a_bomb = true;
        this->is_larger = false;
        this->is_quicker = false;
    };
    void update_location(int type);
    pair<int, int> get_location();
    bool has_bomb();
    void bomb_change();
    bool get_condition(int type);
    void update_condition(char c);
};

void Player::update_condition(char c) //更新玩家状态,尚未设置梯度
{
    if (c == 'L')
        is_larger = true;
    else if (c == 'Q')
        is_quicker = true;
}

Player player1(1, 1, 'A');

Player player2(n, m, 'B');

void Player::update_location(int type) //玩家更新
{
    switch (type) //人目前不能走到光柱上,待调整
    {
    case 1:
        if (floor.getcontent(p.first, p.second + 1) == ' ' || floor.getcontent(p.first, p.second + 1) == 'L' || floor.getcontent(p.first, p.second + 1) == 'Q')
        {
            p.second++;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first, p.second - 1) != 'O')
                floor.update_content(FLOOR, p.first, p.second - 1);
        }
        break;
    case 2:
        if (floor.getcontent(p.first, p.second - 1) == ' ' || floor.getcontent(p.first, p.second - 1) == 'L' || floor.getcontent(p.first, p.second - 1) == 'Q')
        {
            p.second--;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first, p.second + 1) != 'O')
                floor.update_content(FLOOR, p.first, p.second + 1);
        }
        break;
    case 3:
        if (floor.getcontent(p.first - 1, p.second) == ' ' || floor.getcontent(p.first - 1, p.second) == 'L' || floor.getcontent(p.first - 1, p.second) == 'Q')
        {
            p.first--;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first + 1, p.second) != 'O')
                floor.update_content(FLOOR, p.first + 1, p.second);
        }
        break;
    case 4:
        if (floor.getcontent(p.first + 1, p.second) == ' ' || floor.getcontent(p.first + 1, p.second) == 'L' || floor.getcontent(p.first + 1, p.second) == 'Q')
        {
            p.first++;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first - 1, p.second) != 'O')
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

bool Player::has_bomb()
{
    return has_a_bomb;
}

void Player::bomb_change()
{
    if (has_a_bomb)
        has_a_bomb = false;
    else
        has_a_bomb = true;
}

bool Player::get_condition(int type)
{
    if (type == 1)
        return is_larger;
    else if (type == 2)
        return is_quicker;
}

class Bomb //炸弹光束类
{
private:
    pair<int, int> p;
    bool type;
    int time;

public:
    Bomb(int x, int y);
    void update_bomb();
    void create_bomb(pair<int, int> p0, bool kind);
    int get_bomb_time();
};

Bomb::Bomb(int x, int y)
{
    p.first = x;
    p.second = y;
    time = 0;
    type = false;
}

void Bomb::create_bomb(pair<int, int> p0, bool kind)
{
    p = p0;
    type = kind;
    time = 5;
}

void turn_light_into(int x, int y) //软墙炸开后,百分之二十掉落Larger,百分之二十掉落Quicker,百分之六十不掉道具
{
    srand(time(0));
    int temp;
    if (floor.getcontent(x, y) != '#')
    {
        if (prefloor.getcontent(x, y) == '*')
        {
            temp = rand() % 5;
            if (temp == 3)
                floor.update_content(9, x, y);
            else if (temp == 4)
                floor.update_content(10, x, y);
            else
                floor.update_content(0, x, y);
        }
        else
            floor.update_content(0, x, y);
    }
}

void Bomb::update_bomb() //炸弹更新,还需更新光束将人炸死
{
    if (time == 5)
    {
        floor.update_content(3, p.first, p.second);
        display();
        time--;
    }
    else if (time == 2) //尚未维护光束将人炸死的情况
    {
        prefloor.load_floor(floor);
        if (floor.getcontent(p.first - 1, p.second) != '#')
        {
            floor.update_content(6, p.first - 1, p.second);
            if ((type) && p.first > 1)
            {
                if (floor.getcontent(p.first - 2, p.second) != '#')
                    floor.update_content(6, p.first - 2, p.second);
            }
        }
        if (floor.getcontent(p.first + 1, p.second) != '#')
        {
            floor.update_content(7, p.first + 1, p.second);
            if ((type) && p.first < n)
            {
                if (floor.getcontent(p.first + 2, p.second) != '#')
                    floor.update_content(7, p.first + 2, p.second);
            }
        }
        if (floor.getcontent(p.first, p.second - 1) != '#')
        {
            floor.update_content(4, p.first, p.second - 1);
            if ((type) && p.second > 1)
            {
                if (floor.getcontent(p.first, p.second - 2) != '#')
                    floor.update_content(4, p.first, p.second - 2);
            }
        }
        if (floor.getcontent(p.first, p.second + 1) != '#')
        {
            floor.update_content(5, p.first, p.second + 1);
            if ((type) && p.second < m)
            {
                if (floor.getcontent(p.first, p.second + 2) != '#')
                    floor.update_content(5, p.first, p.second + 2);
            }
        }
        floor.update_content(8, p.first, p.second);
        time--;
        display();
    }
    else if (time == 1)
    {
        turn_light_into(p.first - 1, p.second);
        turn_light_into(p.first + 1, p.second);
        turn_light_into(p.first, p.second - 1);
        turn_light_into(p.first, p.second + 1);
        turn_light_into(p.first, p.second);
        if (type)
        {
            if (p.first > 1 && floor.getcontent(p.first - 1, p.second) != '#')
                turn_light_into(p.first - 2, p.second);
            if (p.first < n && floor.getcontent(p.first + 1, p.second) != '#')
                turn_light_into(p.first + 2, p.second);
            if (p.second > 1 && floor.getcontent(p.first, p.second - 1) != '#')
                turn_light_into(p.first, p.second - 2);
            if (p.second < m && floor.getcontent(p.first, p.second + 1) != '#')
                turn_light_into(p.first, p.second + 2);
        }
        display();
        time--;
    }
    else
        time--;
}

int Bomb::get_bomb_time()
{
    return time;
}

Bomb bomb1(-1, -1);

Bomb bomb2(-1, -1);

void display() //刷新屏幕
{
    system("cls");
    floor.print();
}

void init() //初始化
{
    floor.update_content(MOVEA, 1, 1);
    floor.update_content(MOVEB, n, m);
    return;
}

void deal_with_timer() //处理定时事件
{
    if (bomb1.get_bomb_time() > 0)
        bomb1.update_bomb();
    else
        player1.bomb_change();
    if (bomb2.get_bomb_time() > 0)
        bomb2.update_bomb();
    else
        player2.bomb_change();
    return;
}

void deal_with_input() //处理输入/角色行为
{
    char ch;
    if (_kbhit())
    {
        ch = _getch();
        cout << ch;
        if (Move_a_count <= 0)
        {
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
            if (player1.get_condition(2))
                Move_a_count = ONE_SECOND / 4;
            else
                Move_a_count = ONE_SECOND / 2;
        }
        if (ch == ' ' && player1.has_bomb())
        {
            bomb1.create_bomb(player1.get_location(), player1.get_condition(1));
            bomb1.update_bomb();
            player1.bomb_change();
        }
        if (Move_b_count <= 0)
        {
            switch (ch)
            {
            case 'i':
                player2.update_location(3);
                break;
            case 'I':
                player2.update_location(3);
                break;
            case 'k':
                player2.update_location(4);
                break;
            case 'K':
                player2.update_location(4);
                break;
            case 'j':
                player2.update_location(2);
                break;
            case 'J':
                player2.update_location(2);
                break;
            case 'l':
                player2.update_location(1);
                break;
            case 'L':
                player2.update_location(1);
                break;
            }
            if (player2.get_condition(2))
                Move_b_count = ONE_SECOND / 4;
            else
                Move_b_count = ONE_SECOND / 2;
        }
        if (ch == 13 && player2.has_bomb())
        {
            bomb2.create_bomb(player2.get_location(), player2.get_condition(1));
            bomb2.update_bomb();
            player2.bomb_change();
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
        if (Move_a_count > 0)
            Move_a_count--;
        if (Move_b_count > 0)
            Move_b_count--;
        if (count == ONE_SECOND)
        {
            deal_with_timer();
            count = 0;
        }
    }
}