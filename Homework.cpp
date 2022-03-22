#include <conio.h>
#include <iostream>
using namespace std ;
void deal_with_input ( )
{  
    char ch ;
    if ( _kbhit ( ) ) {
        ch = _getch ( ) ; 
        cout << ch ;
    }
}
void display()
{
    system("cls");
}
int main ()
{
    
}