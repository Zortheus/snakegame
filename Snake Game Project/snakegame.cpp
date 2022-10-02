#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <string.h>

using namespace std;

void ScoreScreen();
int d, e;

// Creating some universal constants and a boolean for whether the game is over
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score; // creating our variables for positioning
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

// tail program - max tail segments is 100, tail starts at 0 length
int TailX[100], TailY[100];
int nTail = 0;

void Setup()
{
    initscr();   // these functions set the ncurses library settings
    clear();
    noecho();
    cbreak();
    curs_set(0);
    
    gameOver = false;  // the initial game conditions
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = (rand() % width)+1;
    fruitY = (rand() % height)+1;
    nTail = 0;
    score = 0;

}

void Draw()
{
    clear(); // clear the terminal as the game begins

    for (int i = 0; i < width + 2; i++) // 
        mvprintw(0,i,"#");

    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == 21) // printing the ceiling?
                mvprintw(i,j,"#");
            else if (j == 0 || j == 21) // printing the walls?
                mvprintw(i,j,"#");
            else if (i == y && j == x) // printing the head
                mvprintw(i,j,"O");
            else if (i == fruitY && j == fruitX) // printing the fruit
                mvprintw(i,j,"@");
            else                                     // printing the tail section
                for (int k = 0; k < nTail; k++)
                {
                    if (TailX[k] == j && TailY[k] == i)
                        mvprintw(i,j,"o");
                }
        }
    }

    //Draw Sidebar Menu
    mvprintw(3, 29, "---------------------------------");
    mvprintw(4, 29, "|    Snake Game Made by AJP     |");
    mvprintw(5, 29, "---------------------------------");
    mvprintw(7, 29, "Use direction keys to eat fruit");
    mvprintw(9, 43, "   ^ ");
    mvprintw(10, 43, "<  v  >");
    mvprintw(14, 29, "** If you collide with wall or tail, Snake will die");

    mvprintw(17, 29, "Score %d", score);

    refresh();
}

void Input()
{
  
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch(c)
    {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 113:
            gameOver = true;
            break;
    }

}

void Logic()
{

    int prevX = TailX[0];
    int prevY = TailY[0];
    int prev2X, prev2Y;
    TailX[0] = x;
    TailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = TailX[i];
        prev2Y = TailY[i];
        TailX[i] = prevX;
        TailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch(dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x > width || x < 1 || y > height || y < 1)
        gameOver = true;


    if (x == fruitX && y == fruitY) // When you eat a fruit, add one to score, assign a new rand pos to fruit, and increase tail
    {
        score++;
        fruitX = (rand() % width) +1;
        fruitY = (rand() % height) +1;
        nTail++;
    }

    for (int i = 0; i < nTail; i++) // run through all tail sections to check if the coordinates are same as head
        if (TailX[i] == x && TailY[i] == y)
        {
            gameOver = true;
        }
}

void registration()
{
        string ruserName;
        system("clear");
        cout << "Your score was " << score;
        cout << "\t Enter the username : ";
        cin >> ruserName;

        ofstream f1("highscore.txt", ios::app); //append the directory
        f1 << ruserName << ' ' << score << endl;
        system("clear");
        clear();
}

int main()
{
    
    int d;
    cout << "---------------------------------------------------" << endl;
    cout << "#####                                         #####" << endl;
    cout << "#####          WELCOME TO SNAKE GAME          #####" << endl;
    cout << "#####                                         #####" << endl;
    cout << "#####                                         #####" << endl;
    cout << "#####           Enter 1 to PLAY               #####" << endl;
    cout << "#####           Enter 2 to SEE SCORES         #####" << endl;
    cout << "#####           Enter 3 to EXIT               #####" << endl;
    cout << "#####                                         #####" << endl;
    cout << "---------------------------------------------------" << endl;

    cout << "\t   Please enter your choice: ";
    cin >> d;
    cout << endl;

    switch (d)
    {
        case 1:
            Setup();
            break;
        case 3:
            break;
        case 2:
            ScoreScreen();
            break;
        default:
            system("clear"); // clearing a screen on mac is not cls it's "clear"

    }
    
    while(!gameOver)  // the main game loop; draw, take inputs, determine logic, repeat...
    {
        Draw();
        Input();
        Logic();
    }

    getch();
    sleep(1);
    endwin();

    if (d == 3 || d == 2)
    {
        clear();
    } else if (d == 1)
    {
        registration();
        main();
        clear();
    }

    system("clear");

    return 0;
}

void ScoreScreen()
{
    system("clear");
    ifstream f("highscore.txt");

    if (f.is_open()) 
        cout << f.rdbuf() << endl;
  
    int e = 0;
    cout << "\n Enter 1 to go back to the menu or 2 to exit : "; 
    cin >> e;

    switch (e)
    {
        case 1:
            system("clear");
            clear();
            main();
            break;
        case 2:
            system("clear");
            break;
        default:
            system("clear");
    }
}