// SNAKE GAME
// by AJP
//
// v 1.2.0

#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;
#define RESET   "\033[0m"


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

    //color settings
    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);

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
            else if (i == y && j == x){ // printing the head
                attron(COLOR_PAIR(1));
                mvprintw(i,j,"O");
                attroff(COLOR_PAIR(1));
            }
            else if (i == fruitY && j == fruitX) // printing the fruit
            {    
                attron(COLOR_PAIR(2));
                mvprintw(i,j,"@");
                attroff(COLOR_PAIR(2));
            }    
            else                                     // printing the tail section
                for (int k = 0; k < nTail; k++)
                {
                    if (TailX[k] == j && TailY[k] == i){
                        attron(COLOR_PAIR(1));
                        mvprintw(i,j,"o");
                        attroff(COLOR_PAIR(1));
                    }    
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
    // create prevX and prevY, and 2X and 2Y, set current TailX and TailY to head position
    int prevX = TailX[0]; 
    int prevY = TailY[0]; 
    int prev2X, prev2Y;
    TailX[0] = x; // the first tail piece will be placed on the start position once it's printed
    TailY[0] = y;

    for (int i = 1; i < nTail; i++) // loop that builds a chain, interate for every tail piece nTail, starting with pos 1
    {
        prev2X = TailX[i]; // prev2X becomes equal to TailX[1]
        prev2Y = TailY[i]; // prev2Y becomes euqla to TailY[1]

        TailX[i] = prevX; // TailX[1] position is assigned to prevX
        TailY[i] = prevY; // TailY[1] position is assigned to prevY

        prevX = prev2X; // increases prevX to the next [i]
        prevY = prev2Y; // increases prevX to the next [i]
    } // 

    switch(dir) // logic for controls; note that up is negative and down is positive since it reads from top left to bottom right.
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

    if (x > width || x < 1 || y > height || y < 1) // if x reaches boundaries, game over
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

void registration() // GAME OVER SCREEN
{
        system("clear");

        string ruserName; // define a userName, output a game over prompt and ask for name for high score screen
        cout << "Your score was " << score << endl;
        cout << "\t Enter the username : ";
        cin >> ruserName;

        ofstream f1("highscore.txt", ios::app); //append the file directory - ios::app
        f1 << ruserName << ' ' << score << endl;

        system("clear");
        clear();
}

void ScoreScreen(); // calling this now so it can be used in main()

int main()
{
    
    int d;
    cout << R"(
  _________ _______      _____   ____  __.___________
 /   _____/ \      \    /  _  \ |    |/ _|\_   _____/
 \_____  \  /   |   \  /  /_\  \|      <   |    __)_ 
 /        \/    |    \/    |    \    |  \  |        \
/_______  /\____|__  /\____|__  /____|__ \/_______  /
        \/         \/         \/        \/        \/ 
-----------------------------------------------------
#####                                           #####
#####           WELCOME TO SNAKE GAME           #####
#####                                           #####
#####                                           #####
#####            ENTER 1 to PLAY                #####
#####            ENTER 2 to SEE SCORES          #####
#####            ENTER 3 to EXIT                #####
#####                                           #####
-----------------------------------------------------
)";

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
        system("clear");
        cout << R"(
  ________    _____      _____  ___________
 /  _____/   /  _  \    /     \ \_   _____/
/   \  ___  /  /_\  \  /  \ /  \ |    __)_
\    \_\  \/    |    \/    Y    \|        \
 \______  /\____|__  /\____|__  /_______  /
        \/         \/         \/        \/
____________   _________________________
\_____  \   \ /   /\_   _____/\______   \
 /   |   \   Y   /  |    __)_  |       _/
/    |    \     /   |        \ |    |   \
\_______  /\___/   /_______  / |____|_  /
        \/                 \/         \/

        )";
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

    string line; // this variable is our line
    ifstream f("highscore.txt");

    if (f.is_open()) { // read the file
        vector<pair<int, string> > score_vector; //create a vector pair called score_vector - int for score and string for name
        string name; // variable for name
        int score; // variable for score

        while (f >> name >> score) { // while loop using our file f - >> "name" >> "score" means our vector will read the first column's data as name and second as score
            score_vector.push_back(make_pair(score, name)); // this fills score_vector with 
            cout << line << '\n';
        }

        f.close(); // closes the file 

        system("clear");
        clear();

        cout << R"(
  __________________  ________ _____________________ _________
 /   _____/\_   ___ \ \_____  \\______   \_   _____//   _____/
 \_____  \ /    \  \/  /   |   \|       _/|    __|_ \_____  \
 /        \\     \____/    |    \    |   \|        \/        \
/_______  / \______  /\_______  /____|_  /_______  /_______  /
        \/         \/         \/       \/        \/        \/
        )";            
        cout << endl;                                                       

        cout << "#" << "    " << "Name" << "\t\t" << "Score" << endl;
        cout << "-----------------------------" << endl;

        sort(score_vector.begin(), score_vector.end()); // sort the vector from beginning to end (descending)
        reverse(score_vector.begin(), score_vector.end()); // reverse the order of the sort (ascending)

        int place = 1;
        int topTen = 0;
        for(auto it = score_vector.begin(); topTen < 10; ++it){ // starting at the first score, keep going as long as you're not at the end, +1 to it
            cout << place << "    " << it->second << "\t\t" << it->first << endl; // Print 
            place++;
            topTen++;
        }
    }
    else
        cout << "Unable to open text";
  
    int e = 0;
    cout << "\n ENTER 1 to go back to the menu.\n";
    cout << "\n ENTER 2 to exit Snake." << endl;
    cout << " ";
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