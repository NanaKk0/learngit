#include <conio.h>
#include <iostream>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

const int ONE_SECOND = 12000;

int count = 0;

int PLAYERS_LOSE = 2, SOMEONE_WIN = 3;

int Move_a_count = 0, Move_b_count = 0, Move_c_count = 0, Move_d_count = 0;

int Speed[4] = {ONE_SECOND / 2, ONE_SECOND / 2, ONE_SECOND / 2, ONE_SECOND / 2};

const int n = 13, m = 15;

const int FLOOR = 0, MOVEA = 1, MOVEB = 2, MOVEC = 3, MOVED = 4;

int robot1_bomb_time = 2, robot2_bomb_time = 3;

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
    content[n - 2][1] = ' ';
    content[2][m - 1] = ' ';
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
    case 3: //机器人1
        content[x][y] = 'C';
        break;
    case 4: //机器人2
        content[x][y] = 'D';
        break;
    case 5: //炸弹
        content[x][y] = 'O';
        break;
    case 6: //四个方向的光束
        content[x][y] = '<';
        break;
    case 7:
        content[x][y] = '>';
        break;
    case 8:
        content[x][y] = '^';
        break;
    case 9:
        content[x][y] = 'v';
        break;
    case 10:
        content[x][y] = 'X';
        break;
    case 11:
        content[x][y] = 'L'; // Larger,炸弹增强
        break;
    case 12:
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

class Player //玩家类(注:机器人也在其中,仅操作手段不同,但操作相同)
{
private:
    pair<int, int> p;
    char symbol;
    bool has_a_bomb;
    int is_larger;
    int is_quicker;
    int is_lived;
    int point;

public:
    Player(int x, int y, char symbol)
    {
        this->p.first = x;
        this->p.second = y;
        this->symbol = symbol;
        this->has_a_bomb = true;
        this->is_larger = 0;
        this->is_quicker = 0;
        this->is_lived = 3;
        this->point = 0;
    };
    bool update_location(int type);
    pair<int, int> get_location();
    bool has_bomb();
    void bomb_change();
    int get_condition(int type);
    int get_is_lived();
    void update_condition(char c);
    void update_robot_location();
    void update_robot_bomb(int num);
    void change_life();
    bool judge_ifbomb();
    void point_change();
    int point_show();
    int life_show();
    char get_symbol();
};

void Player::update_condition(char c) //更新玩家状态,尚未设置梯度
{
    if (c == 'L' && is_larger < 5) //设置最大梯度是4,即最多吃掉4个道具就不能再增长能力了
        is_larger++;
    else if (c == 'Q' && is_quicker < 5)
    {
        is_quicker++;
        Speed[symbol - 65] /= 2;
    }
}

Player player1(1, 1, 'A');

Player player2(n, m, 'B');

Player robot1(n, 1, 'C');

Player robot2(1, m, 'D');

bool Player::update_location(int type) //玩家更新
{
    bool flag;
    switch (type)
    {
    case 1:
        if (floor.getcontent(p.first, p.second + 1) == ' ' || floor.getcontent(p.first, p.second + 1) == 'L' || floor.getcontent(p.first, p.second + 1) == 'Q')
        {
            p.second++;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first, p.second - 1) != 'O')
                floor.update_content(FLOOR, p.first, p.second - 1);
            flag = true;
        }
        else
            flag = false;
        break;
    case 2:
        if (floor.getcontent(p.first, p.second - 1) == ' ' || floor.getcontent(p.first, p.second - 1) == 'L' || floor.getcontent(p.first, p.second - 1) == 'Q')
        {
            p.second--;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first, p.second + 1) != 'O')
                floor.update_content(FLOOR, p.first, p.second + 1);
            flag = true;
        }
        else
            flag = false;
        break;
    case 3:
        if (floor.getcontent(p.first - 1, p.second) == ' ' || floor.getcontent(p.first - 1, p.second) == 'L' || floor.getcontent(p.first - 1, p.second) == 'Q')
        {
            p.first--;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first + 1, p.second) != 'O')
                floor.update_content(FLOOR, p.first + 1, p.second);
            flag = true;
        }
        else
            flag = false;
        break;
    case 4:
        if (floor.getcontent(p.first + 1, p.second) == ' ' || floor.getcontent(p.first + 1, p.second) == 'L' || floor.getcontent(p.first + 1, p.second) == 'Q')
        {
            p.first++;
            update_condition(floor.getcontent(p.first, p.second));
            floor.update_content((int)symbol - 64, p.first, p.second);
            if (floor.getcontent(p.first - 1, p.second) != 'O')
                floor.update_content(FLOOR, p.first - 1, p.second);
            flag = true;
        }
        else
            flag = false;
        break;
    }
    display();
    return flag;
}

void Player::update_robot_location() //机器人更新
{
    if (floor.getcontent(p.first - 1, p.second) != ' ' && floor.getcontent(p.first - 1, p.second) != 'L' && floor.getcontent(p.first - 1, p.second) != 'Q' && floor.getcontent(p.first + 1, p.second) != ' ' && floor.getcontent(p.first + 1, p.second) != 'L' && floor.getcontent(p.first + 1, p.second) != 'Q' && floor.getcontent(p.first, p.second - 1) != ' ' && floor.getcontent(p.first, p.second - 1) != 'L' && floor.getcontent(p.first, p.second - 1) != 'Q' && floor.getcontent(p.first, p.second + 1) != ' ' && floor.getcontent(p.first, p.second + 1) != 'L' && floor.getcontent(p.first, p.second + 1) != 'Q')
        return;
    int temp = rand() % 4 + 1;
    while (!update_location(temp))
    {
        temp = rand() % 4 + 1;
    }
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

int Player::get_condition(int type)
{
    if (type == 1)
        return is_larger;
    else if (type == 2)
        return is_quicker;
}

int Player::get_is_lived()
{
    return is_lived;
}

void Player::change_life()
{
    if (is_lived > 0)
    {
        is_lived--;
        if (is_lived == 0)
        {
            if (symbol == 'A' || symbol == 'B')
                PLAYERS_LOSE--;
            SOMEONE_WIN--;
        }
    }
}

bool Player::judge_ifbomb()
{
    if (floor.getcontent(p.first - 1, p.second - 1) == ' ')
        return true;
    if (floor.getcontent(p.first + 1, p.second + 1) == ' ')
        return true;
    if (floor.getcontent(p.first + 1, p.second - 1) == ' ')
        return true;
    if (floor.getcontent(p.first - 1, p.second + 1) == ' ')
        return true;
    if (p.first > 1 && floor.getcontent(p.first - 2, p.second) == ' ')
        return true;
    if (p.first < n && floor.getcontent(p.first + 2, p.second) == ' ')
        return true;
    if (p.second > 1 && floor.getcontent(p.first, p.second - 2) == ' ')
        return true;
    if (p.second < m && floor.getcontent(p.first, p.second + 2) == ' ')
        return true;
    return false;
}

void Player::point_change()
{
    point += 5;
}

int Player::point_show()
{
    return point;
}

int Player::life_show()
{
    return is_lived;
}

char Player::get_symbol()
{
    return symbol;
}

class Bomb //炸弹光束类
{
private:
    pair<int, int> p;
    int type;
    int time;
    Player *master;

public:
    Bomb(int x, int y, Player *Master)
    {
        this->p.first = x;
        this->p.second = y;
        this->time = 0;
        this->type = 0;
        this->master = Master;
    };
    void update_bomb();
    void create_bomb(pair<int, int> p0, int kind);
    int get_bomb_time();
};

void Bomb::create_bomb(pair<int, int> p0, int kind)
{
    p = p0;
    type = kind;
    time = 5;
}

bool turn_light_into(int x, int y) //软墙炸开后,七分之一掉落Larger,七分之一掉落Quicker
{
    int temp;
    if (floor.getcontent(x, y) != '#')
    {
        if (prefloor.getcontent(x, y) == '*')
        {
            temp = rand() % 7;
            if (temp == 5)
                floor.update_content(11, x, y);
            else if (temp == 6)
                floor.update_content(12, x, y);
            else
                floor.update_content(0, x, y);
        }
        else
            floor.update_content(0, x, y);
        return true;
    }
    else
        return false;
}

void Bomb::update_bomb() //炸弹更新,还需更新光束将人炸死
{
    if (time == 5)
    {
        floor.update_content(5, p.first, p.second);
        display();
        time--;
    }
    else if (time == 2)
    {
        prefloor.load_floor(floor);
        for (int i = 0; i <= type; i++)
        {
            if (p.first >= i + 2)
            {
                if (floor.getcontent(p.first - i - 1, p.second) != '#')
                {
                    if (floor.getcontent(p.first - i - 1, p.second) == 'A')
                    {
                        player1.change_life();
                        if (master->get_symbol() != 'A')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first - i - 1, p.second) == 'B')
                    {
                        player2.change_life();
                        if (master->get_symbol() != 'B')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first - i - 1, p.second) == 'C')
                    {
                        robot1.change_life();
                        if (master->get_symbol() != 'C')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first - i - 1, p.second) == 'D')
                    {
                        robot2.change_life();
                        if (master->get_symbol() != 'D')
                            master->point_change();
                    }
                    floor.update_content(8, p.first - i - 1, p.second);
                }
                else
                    break;
            }
        }
        for (int i = 0; i <= type; i++)
        {
            if (p.first <= n - i - 1)
            {
                if (floor.getcontent(p.first + i + 1, p.second) != '#')
                {
                    if (floor.getcontent(p.first + i + 1, p.second) == 'A')
                    {
                        player1.change_life();
                        if (master->get_symbol() != 'A')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first + i + 1, p.second) == 'B')
                    {
                        player2.change_life();
                        if (master->get_symbol() != 'B')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first + i + 1, p.second) == 'C')
                    {
                        robot1.change_life();
                        if (master->get_symbol() != 'C')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first + i + 1, p.second) == 'D')
                    {
                        robot2.change_life();
                        if (master->get_symbol() != 'D')
                            master->point_change();
                    }
                    floor.update_content(9, p.first + i + 1, p.second);
                }
                else
                    break;
            }
        }
        for (int i = 0; i <= type; i++)
        {
            if (p.second >= i + 2)
            {
                if (floor.getcontent(p.first, p.second - i - 1) != '#')
                {
                    if (floor.getcontent(p.first, p.second - i - 1) == 'A')
                    {
                        player1.change_life();
                        if (master->get_symbol() != 'A')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second - i - 1) == 'B')
                    {
                        player2.change_life();
                        if (master->get_symbol() != 'B')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second - i - 1) == 'C')
                    {
                        robot1.change_life();
                        if (master->get_symbol() != 'C')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second - i - 1) == 'D')
                    {
                        robot2.change_life();
                        if (master->get_symbol() != 'D')
                            master->point_change();
                    }
                    floor.update_content(6, p.first, p.second - i - 1);
                }
                else
                    break;
            }
        }
        for (int i = 0; i <= type; i++)
        {
            if (p.second <= m - i - 1)
            {
                if (floor.getcontent(p.first, p.second + i + 1) != '#')
                {
                    if (floor.getcontent(p.first, p.second + i + 1) == 'A')
                    {
                        player1.change_life();
                        if (master->get_symbol() != 'A')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second + i + 1) == 'B')
                    {
                        player2.change_life();
                        if (master->get_symbol() != 'B')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second + i + 1) == 'C')
                    {
                        robot1.change_life();
                        if (master->get_symbol() != 'C')
                            master->point_change();
                    }
                    else if (floor.getcontent(p.first, p.second + i + 1) == 'D')
                    {
                        robot2.change_life();
                        if (master->get_symbol() != 'D')
                            master->point_change();
                    }
                    floor.update_content(7, p.first, p.second + i + 1);
                }
                else
                    break;
            }
        }
        if (p == player1.get_location())
            player1.change_life();
        if (p == player2.get_location())
            player2.change_life();
        if (p == robot1.get_location())
            robot1.change_life();
        if (p == robot2.get_location())
            robot2.change_life();
        floor.update_content(10, p.first, p.second);
        time--;
        display();
    }
    else if (time == 1)
    {
        for (int i = 0; i <= type; i++)
        {
            if (!turn_light_into(p.first - i - 1, p.second))
                break;
        }
        for (int i = 0; i <= type; i++)
        {
            if (!turn_light_into(p.first + i + 1, p.second))
                break;
        }
        for (int i = 0; i <= type; i++)
        {
            if (!turn_light_into(p.first, p.second - i - 1))
                break;
        }
        for (int i = 0; i <= type; i++)
        {
            if (!turn_light_into(p.first, p.second + i + 1))
                break;
        }
        floor.update_content(0, p.first, p.second);
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

Bomb bomb1(-1, -1, &player1);

Bomb bomb2(-1, -1, &player2);

Bomb bomb3(-1, -1, &robot1);

Bomb bomb4(-1, -1, &robot2);

void Player::update_robot_bomb(int num)
{
    if (num == 1)
    {
        bomb3.create_bomb(p, get_condition(1));
        bomb3.update_bomb();
    }
    else if (num == 2)
    {
        bomb4.create_bomb(p, get_condition(1));
        bomb4.update_bomb();
    }
}

void gotoxy(int x, int y)
{
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

void display() //刷新屏幕
{
    gotoxy(0, 0);
    floor.print();
    cout << "A's point:" << player1.point_show() << "  A's lives:" << player1.life_show() << endl;
    cout << "B's point:" << player2.point_show() << "  B's lives:" << player2.life_show() << endl;
    cout << "C's point:" << robot1.point_show() << "  C's lives:" << robot1.life_show() << endl;
    cout << "D's point:" << robot2.point_show() << "  D's lives:" << robot2.life_show() << endl;
}

void init() //初始化
{
    floor.update_content(MOVEA, 1, 1);
    floor.update_content(MOVEB, n, m);
    floor.update_content(MOVEC, n, 1);
    floor.update_content(MOVED, 1, m);
    return;
}

void deal_with_timer() //处理定时事件:炸弹更新与机器人定时释放炸弹
{
    if (bomb1.get_bomb_time() > 0)
        bomb1.update_bomb();
    else
        player1.bomb_change();
    if (bomb2.get_bomb_time() > 0)
        bomb2.update_bomb();
    else
        player2.bomb_change();
    if (robot1_bomb_time > 0)
        robot1_bomb_time--;
    else if (robot1.get_is_lived() > 0 && robot1.judge_ifbomb())
    {
        robot1_bomb_time = 6;
        robot1.update_robot_bomb(1);
    }
    if (robot2_bomb_time > 0)
        robot2_bomb_time--;
    else if (robot2.get_is_lived() > 0 && robot2.judge_ifbomb())
    {
        robot2_bomb_time = 6;
        robot2.update_robot_bomb(2);
    }
    if (bomb3.get_bomb_time() > 0)
        bomb3.update_bomb();
    if (bomb4.get_bomb_time() > 0)
        bomb4.update_bomb();
    return;
}

void deal_with_input() //处理输入/角色行为
{
    char ch;
    if (_kbhit())
    {
        ch = _getch();
        cout << ch;
        if (Move_a_count <= 0 && player1.get_is_lived() > 0)
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
            Move_a_count = Speed[0];
        }
        if (_kbhit())
            ch = _getch();
        if (ch == ' ' && player1.has_bomb() && player1.get_is_lived() > 0)
        {
            bomb1.create_bomb(player1.get_location(), player1.get_condition(1));
            bomb1.update_bomb();
            player1.bomb_change();
        }
        if (Move_b_count <= 0 && player2.get_is_lived() > 0)
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
            Move_b_count = Speed[1];
        }
        if (ch == 13 && player2.has_bomb() && player2.get_is_lived() > 0)
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
    srand(time(0));
    while (1 && PLAYERS_LOSE > 0 && SOMEONE_WIN > 0)
    {
        if (Move_c_count == 0 && robot1.get_is_lived() > 0)
        {
            Move_c_count = Speed[2];
            robot1.update_robot_location();
            display();
        }
        if (Move_d_count == 0 && robot2.get_is_lived() > 0)
        {
            Move_d_count = Speed[3];
            robot2.update_robot_location();
            display();
        }
        deal_with_input();
        count++;
        if (Move_a_count > 0)
            Move_a_count--;
        if (Move_b_count > 0)
            Move_b_count--;
        if (Move_c_count > 0)
            Move_c_count--;
        if (Move_d_count > 0)
            Move_d_count--;
        if (count == ONE_SECOND)
        {
            deal_with_timer();
            count = 0;
        }
    }
    system("cls");
    if (PLAYERS_LOSE == 0)
    {
        cout << "Players Lose";
    }
    else
    {
        if (player1.get_is_lived() > 0)
            cout << "Player1 Win";
        else if (player2.get_is_lived() > 0)
            cout << "Player2 Win";
        else
            cout << "Players Lose";
    }
    Sleep(10000);
}