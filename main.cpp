// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TT7L
// Names: Azri Syahmi bin Azhar | Aiman Faris bin Aidi Zamri | Muhammad Izzul Iqbal bin Ismail
// IDs: 1211103115 | 1211101303 | 1211103148
// Emails: 1211103115@student.mmu.edu.my | 1211101303@student.mmu.edu.my | 1211103148@student.mmu.edu.my
// Phones: 0108336713 | 0136811083 | 01158861607
// *********************************************************

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>       // for system()
#include <ctime>         // for time() in srand( time(NULL) );
#include <iomanip>       // for setw()
#include <limits>        // for numeric_limits (Clear Input Stream)
#include <windows.h>     // for Sleep()
#include <bits/stdc++.h> // for transform
#include <fstream>       // for save and load

using namespace std;

struct Alien
{
    int health = 150;
    int attack = 0;
    int shield = 0;
};
Alien myAlien;

struct Zombie
{
    int health;
    int attack;
    int attack_range;
    int col;
    int row;
    int alive;
};

int ClearScreen();
int Pause();
void clearInputStream();
void typingText(string, int);

void base(int &rows, int &columns, int &zombie); // navigation to each page
int chooseMenu();                                // display main menu page
void menuStartGame(int &rows, int &columns, int &zombie);
void menuInstructions();
void menuCredits();

int getCols(); // return total number of columns
int getRows(); // return total number of rows
char getObject(int cols, int rows);
void setObject(int cols, int rows, char ch);
bool insideBoard(int row, int col);

// Global Variable //
int cols, rows, zombie;
int dimX_, dimY_;
int alX, alY; // x & y coordinate of Alien (keep updating)
int zombiesAlive;
int distance_1, distance_2, distance_3, distance_4, distance_5, distance_6, distance_7, distance_8, distance_9; // distance each zombie and alien

vector<vector<char>> map_; // convention to put trailing underscore
void display();            // print gameboard
void printGameStatus();    // print game status

void customSetting(int &rows, int &cols, int &zombie); // set game setting based on player
void checkOddRows(int &rows);                          // check row odd
void checkOddColumns(int &cols);                       // check columns odd
void checkIntZombie(int &zombie);                      // check zombie between 1-9
void init(int cols, int rows);                         // set character for each coordinate
void zombieCount(int zombie);                          // initialize Zombies
void randomizeZombieAttributes();

void posAlien();     // initialize Alien position at the center
void initialAlien(); // get initial position of Alien

vector<Zombie> zombies;
void zombieTurn();
void zombieMove(int zombieIndex);
void zombieAttack(int zombieIndex);

void rock(); // reveal object under rock
void arrow();
void zombloc(); // distance each zombie from alien
void pod();
void healthpack();

void command();
void move(string movement); // input movement choice
void up();                  // move up
void down();                // move down
void right();               // move right
void left();                // move left
void arrowDirection();
void help();
void untrail(); // set object to trail
void save();
void load();

void won();
void lost();

/////////////// START GAME ///////////////

int main()
{
    // srand(1); // use this for fixed map during testing
    srand(time(NULL)); // try this for random map
    // initial value
    rows = 5;
    cols = 5;
    zombie = 2;

    base(rows, cols, zombie);

    return 0;
}

/////////////// TOOLS ///////////////
int ClearScreen()
{
#if defined(_WIN32)
    return system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    return system("clear");
#endif
}

int Pause()
{
#if defined(_WIN32)
    return system("pause");
#elif defined(__linux__) || defined(__APPLE__)
    return system(R"(read -p "Press any key to continue . . . " dummy)");
#endif
}

void clearInputStream()
{
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void typingText(string text, int typeSpeed = 100)
{
    for (int i = 0; i < text.length(); i++)
    {
        cout << text[i];
        Sleep(typeSpeed);
    }
}

/////////////// MAIN MENU ///////////////
void base(int &rows, int &cols, int &zombie)
{
    int menuChoice;
    bool done = false;
    do
    {
        menuChoice = chooseMenu();
        switch (menuChoice)
        {
        case 1:
            menuStartGame(rows, cols, zombie);
            break;
        case 2:
            menuInstructions();
            break;
        case 3:
            menuCredits();
            break;
        case 4:
            done = true; // nanti tambah confirmation
            break;
        default:
            cout << "Invalid selection, try again!" << endl;
            cout << endl;
            break;
        }
    } while (!done);

    char answer;
    cout << "Are you sure you want to quit? Any unsaved progress will be lost. (y/n): \n";
    cout << "Command => ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        cout << "  ______ __ __  __ ____  _    _    __  \n"
             << " |                                   | \n"
             << " |  THANK YOU FOR PLAYING OUR GAME!  | \n"
             << " |__  ___ _ __ ____ _   _    ___   __| \n\n";

        cout << "Exiting game..." << endl;
        Pause();
        exit(0);
    }
    else if (answer == 'n' || answer == 'N')
    {
        base(rows, cols, zombie);
    }
}

int chooseMenu()
{
    int menuChoice;

    ClearScreen(); // comment this out during testing
    cout << endl;
    cout << "+---------------------------------------------+" << endl;
    cout << "|              Alien VS Zombies               |" << endl;
    cout << "|=============================================|" << endl;
    cout << "| Select:                                     |" << endl;
    cout << "| 1 => Start New Game                         |" << endl;
    cout << "| 2 => Instructions                           |" << endl;
    cout << "| 3 => Credits                                |" << endl;
    cout << "| 4 => Quit                                   |" << endl;
    cout << "+---------------------------------------------+" << endl;
    cout << endl;
    cout << "Command => ";
    cin >> menuChoice;
    clearInputStream();

    return menuChoice;
}

void menuStartGame(int &rows, int &cols, int &zombie)
{
    customSetting(rows, cols, zombie);
    init(cols, rows);
    initialAlien();
    posAlien();
    zombieCount(zombie);
    randomizeZombieAttributes();
    display();
    printGameStatus();
    while (zombiesAlive > 0 && myAlien.health > 0)
    {
        command();
    }
    if (zombiesAlive == 0)
    {
        display();
        printGameStatus();
        won();
    }
    else if (myAlien.health <= 0)
    {
        lost();
    }
}

void menuInstructions()
{
    int instructChoice;
    ClearScreen();
    while (true)
    {
        string instructionMenuText = "\n~ Instructions ~\n"
                                     "1 => Goal / Gameplay Controls\n"
                                     "2 => Game Objects\n"
                                     "3 => Return to the Main Menu\n\n"
                                     "Your inquiry? => ";
        typingText(instructionMenuText, 20);

        cin >> instructChoice;

        if (instructChoice == 1)
        {
            string goalText = "\nGOAL:\n"
                              "The invasion was swift, the zombies destroyed your hives and devastated your home. "
                              "Retreating to your ancestral planet, your people have been granted ancient powers to reclaim your lush home world. "
                              "Prepare your path and smash your way through your enemies to halt the invasion. "
                              "You are the final hope to protect the value of life across your planet!\n"

                              "Defeat all the zombies!\n"
                              "You can collect power ups along the way to help you in this quest!\n\n"

                              "BASIC CONTROLS    :\n"
                              "up       => Alien to move up\n"
                              "down     => Alien to move down\n"
                              "left     => Alien to move left\n"
                              "right    => Alien to move right\n"
                              "arrow    => Switch the direction of an arrow object in the game board\n"
                              "help     => List and describe the commands that you can use in the game\n"
                              "save     => Save the current game\n"
                              "load     => Load a saved game\n"
                              "quit     => Quit the game\n"
                              "If you would like to know more about each game object, please input 2\n"
                              "on the instructions menu screen!\n\n";
            typingText(goalText, 15);
        }
        else if (instructChoice == 2)
        {
            string bufferText;
            cout << '^';
            bufferText = " : Arrow (up)    => Changes Alien\'s direction of movement to up and adds 20 attack power. Use it wisely!\n";
            typingText(bufferText, 15);

            cout << 'v';
            bufferText = " : Arrow (down)  => Changes Alien\'s direction of movement to down and adds 20 attack power. Use it wisely!\n";
            typingText(bufferText, 15);

            cout << '<';
            bufferText = " : Arrow (left)  => Changes Alien\'s direction of movement to left and adds 20 attack power. Use it wisely!\n";
            typingText(bufferText, 15);

            cout << '>';
            bufferText = " : Arrow (right) => Changes Alien\'s direction of movement to right and adds 20 attack power. Use it wisely!\n";
            typingText(bufferText, 15);

            cout << 'h';
            bufferText = " : Health Pack   => Good as new! This magical elixir adds an extra 20 health point!\n";
            typingText(bufferText, 15);

            cout << 'p';
            bufferText = " : Pod           => Small and mysterious! When hit by your alien, they inflict 10 points of damage to the nearest enemy. Watch out for them!\n";
            typingText(bufferText, 15);

            cout << 'r';
            bufferText = " : Rock          => These heavy boulders hides valuable game objects! Hit them to reveal the hidden object! But be careful, hitting them will stop your alien!\n";
            typingText(bufferText, 15);

            cout << 's';
            bufferText = " : Shield        => This vibranium shield gives extra protection to Alien. Immune to 1 zombie attack!\n";
            typingText(bufferText, 15);

            cout << '.';
            bufferText = " : Trail         => Alien leaves a trail behind, which transforms into another game object at the end of the turn!\n\n";
            typingText(bufferText, 15);
        }
        else if (instructChoice == 3)
        {
            cout << endl
                 << "Returning to base..." << endl;

            clearInputStream();
            Pause();
            ClearScreen();
            break;
        }
        else
        {
            cout << endl
                 << "Incorrect input. Try again!" << endl;
        }

        clearInputStream();
        Pause();
        ClearScreen();
    }
}

void menuCredits()
{
    ClearScreen();
    string creditText = "\nTCP1101 Programming Fundamentals \nAssignment Trimester 1 22/23 (T2215) \nLecture Class TC1V \nTutorial Class TT7L \n\n"
                        "Team Members: \n\tAzri Syahmi bin Azhar \n\tAiman Faris bin Aidi Zamri \n\tMuhammad Izzul Iqbal bin Ismail \n\n"
                        "Special Thanks to: \n\tDr Wong Ya Ping \n\tMr William Ban Kar Weng \n\tMs Pang Nyuk Khee @ Angeline Pang\n\n";
    typingText(creditText, 40);
    Pause();
}

/////////////// GAMEBOARD FUNCTIONS ///////////////
int getCols()
{
    return dimX_;
}

int getRows()
{
    return dimY_;
}

void setObject(int cols, int rows, char ch)
{
    map_[dimY_ - rows][cols - 1] = ch;
}

char getObject(int cols, int rows)
{
    return map_[dimY_ - rows][cols - 1];
}

bool insideBoard(int col, int row)
{
    int x = getCols();
    int y = getRows();
    return row <= y && row >= 1 && col <= x && col >= 1;
}

/////////////// GAMEBOARD DISPLAY FUNCTION ///////////////
void display()
{
    // comment this out during testing
    ClearScreen();

    cout << " -------------------------------" << endl;
    cout << " =       Alien VS Zombie       =" << endl;
    cout << " -------------------------------" << endl;
    cout << endl;

    // display upper border of the row
    cout << setw(6) << "+";
    for (int i = 0; i < dimX_; i++)
    {
        cout << "---+";
    }
    cout << endl;

    // for each row
    for (int i = 0; i < dimY_; ++i)
    {

        // display row number
        cout << setw(4) << (dimY_ - i);

        // display cell content and border of each column
        for (int j = 0; j < dimX_; ++j)
        {
            cout << " |" << map_[i][j] << " ";
        }
        cout << " |" << endl;

        // display border between each row
        cout << setw(6) << "+";
        for (int j = 0; j < dimX_; j++)
        {
            cout << "---+";
        }
        cout << endl;
    }

    // Print column numbers below grid
    cout << setw(7);
    for (int i = 1; i <= dimX_; i++)
    {
        if (i < 10)
            cout << " " << i << "  ";
        else
            cout << " " << i << " ";
    }
    cout << endl;

    cout << endl;
}

void printGameStatus()
{
    // Alien Status
    cout << "Alien"
         << "    - Life: " << setw(4) << myAlien.health
         << ", Attack: " << setw(3) << myAlien.attack
         << ", Shield: " << setw(2) << myAlien.shield
         << endl;

    // Zombie Status
    for (int i = 0; i < zombie; i++)
    {
        if (zombies[i].alive == 1)
        {
            cout << "Zombie " << i + 1
                 << " - Life: " << setw(4) << zombies[i].health
                 << ", Attack: " << setw(3) << zombies[i].attack
                 << ", Range: " << setw(3) << zombies[i].attack_range
                 << endl;
        }
        else if (zombies[i].alive == 0)
        {
            cout << "Zombie " << i + 1
                 << " - Life: " << setw(4) << '-'
                 << ", Attack: " << setw(3) << '-'
                 << ", Range: " << setw(3) << '-'
                 << endl;
        }
    }
    cout << endl;
}

/////////////// NEW GAME FUNCTION ///////////////
void customSetting(int &rows, int &cols, int &zombie)
{
    ClearScreen();
    cout << "-----------------------" << endl;
    cout << " Default Game Settings " << endl;
    cout << "-----------------------" << endl;
    cout << " " << endl;
    cout << "Board Rows: " << rows << endl;
    cout << "Board Columns: " << cols << endl;
    cout << "Zombie Count: " << zombie << endl
         << endl;

    char choice;
    cout << "Do you wish to change default setting? (y/n) => ";
    cin >> choice; // nanti tambah check input

    // check if user wants to change default setting
    if (tolower(choice) == 'y') // tolower() is to convert input to lowercase
    {
        checkOddRows(rows); // Ask user no of rows and check if valid

        checkOddColumns(cols); // Ask user no of columns and check if valid

        checkIntZombie(zombie); // Ask user no of zombies and check if valid

        cout << endl;
        cout << "Setting Updated." << endl;
    }
    else if (tolower(choice) == 'n') // nanti tambah if no
    {
        cout << "No settings were changed." << endl;
    }
    cout << endl;
    zombies.resize(zombie);
    zombiesAlive = zombie;

    Pause();
}

void checkOddRows(int &rows)
{
    while (true)
    {
        cout << "Enter number of rows: ";
        cin >> rows;
        if (cin.fail())
        {
            clearInputStream();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        else if (rows % 2 == 0 || rows > 99)
        {
            cout << "Please enter an odd number less than or equal to 99." << endl
                 << endl;
            continue;
        }
        else
        {
            break;
        }
    }
}

void checkOddColumns(int &cols)
{
    while (true)
    {
        cout << "Enter number of columns: ";
        cin >> cols;
        if (cin.fail())
        {
            clearInputStream();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        else if (cols % 2 == 0 || cols > 99)
        {
            cout << "Please enter an odd number less than or equal to 99." << endl
                 << endl;
            continue;
        }
        else
        {
            break;
        }
    }
}

void checkIntZombie(int &zombie)
{
    while (true)
    {
        cout << "Enter number of zombies (1 ~ 9): ";
        cin >> zombie;
        if (cin.fail())
        {
            clearInputStream();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        else if (zombie > 9 || zombie < 1)
        {
            cout << " " << endl;
            cout << "Number of zombies must be between 1 and 9." << endl
                 << endl;
            continue;
        }
        else
        {
            break;
        }
    }
}

void init(int cols, int rows) // set character for each coordinate
{
    dimX_ = cols;
    dimY_ = rows;
    char objects[] = {'^', 'v', '<', '>', 'p', 'h', 'r', 's', ' ', ' '};
    int noOfObjects = 10; // number of objects in the objects array

    // create dynamic 2D array using vector
    map_.resize(dimY_); // create empty rows
    for (int i = 0; i < dimY_; ++i)
    {
        map_[i].resize(dimX_); // resize each row
    }

    // put random characters into the vector array
    for (int i = 0; i < dimY_; ++i)
    {
        for (int j = 0; j < dimX_; ++j)
        {
            int objNo = rand() % noOfObjects;
            map_[i][j] = objects[objNo];
        }
    }
}

void zombieCount(int zombie)
{
    int zombieNum = 1;
    int midRows = dimY_ / 2; // mid rows
    int midCols = dimX_ / 2; // mid cols

    for (int i = 0; i < zombie; ++i)
    {
        int row = rand() % dimY_;
        int col = rand() % dimX_;
        while ((row == midRows && col == midCols) || (map_[row][col] >= '1' && map_[row][col] <= '9')) // check existing zombie
        {
            row = rand() % dimY_;
            col = rand() % dimX_;
        }
        map_[row][col] = char(zombieNum + '0'); // convert int to char using ASCII
        ++zombieNum;
        zombies[i].col = col;
        zombies[i].row = row;
    }
}

void randomizeZombieAttributes()
{
    // Seed the random number generator with the current time
    srand(time(0));

    for (int i = 0; i < zombies.size(); i++)
    {
        zombies[i].health = rand() % 151 + 50;    // Random health between 50-200
        zombies[i].attack = rand() % 16 + 5;      // Random attack between 5-20
        zombies[i].attack_range = rand() % 3 + 1; // Random attack range between 1-3
        zombies[i].alive = 1;
    }
}

/////////////// ALIEN'S ///////////////
void posAlien()
{
    int midRows = dimY_ / 2; // mid rows
    int midCols = dimX_ / 2; // mid cols
    char ch = 'A';
    map_[midRows][midCols] = ch;
}

void initialAlien()
{
    alX = (dimX_ / 2) + 1;
    alY = (dimY_ / 2) + 1;
}

/////////////// ZOMBIE'S ///////////////
void zombieTurn()
{
    for (int i = 0; i < zombie; i++)
    {
        if (zombies[i].health <= 0)
        {
            // Skip turn if zombie is dead
            continue;
        }

        zombieMove(i);
        zombieAttack(i);
    }
}

void zombieMove(int zombieIndex)
{ // zombie's position
    int x = (zombies[zombieIndex].col + 1);
    int y = (dimY_ - zombies[zombieIndex].row);
    int direction;
    do
    {
        direction = rand() % 4;
    } while (
        (direction == 0 && (y == dimY_ || getObject(x, y + 1) == 'A' ||
                            getObject(x, y + 1) >= '1' && getObject(x, y + 1) <= '9')) ||
        (direction == 1 && (y == 1 || getObject(x, y - 1) == 'A' ||
                            getObject(x, y - 1) >= '1' && getObject(x, y - 1) <= '9')) ||
        (direction == 2 && (x == dimX_ || getObject(x + 1, y) == 'A' ||
                            getObject(x + 1, y) >= '1' && getObject(x + 1, y) <= '9')) ||
        (direction == 3 && (x == 1 || getObject(x - 1, y) == 'A' ||
                            getObject(x - 1, y) >= '1' && getObject(x - 1, y) <= '9')));

    string zombieMovement;

    if (direction == 0) // up
    {
        zombieMovement = "up";
        setObject(x, y, ' ');
        zombies[zombieIndex].row -= 1;
        y += 1;
        setObject(x, y, '0' + zombieIndex + 1);
    }
    else if (direction == 1) // down
    {
        zombieMovement = "down";
        setObject(x, y, ' ');
        zombies[zombieIndex].row += 1;
        y -= 1;
        setObject(x, y, '0' + zombieIndex + 1);
    }
    else if (direction == 2) // right
    {
        zombieMovement = "right";
        setObject(x, y, ' ');
        zombies[zombieIndex].col += 1;
        x += 1;
        setObject(x, y, '0' + zombieIndex + 1);
    }
    else if (direction == 3) // left
    {
        zombieMovement = "left";
        setObject(x, y, ' ');
        zombies[zombieIndex].col -= 1;
        x -= 1;
        setObject(x, y, '0' + zombieIndex + 1);
    }
    cout << "Zombie " << zombieIndex + 1 << " moves " << zombieMovement << ".\n\n";
    Pause();
    display();
    printGameStatus();
}

void zombieAttack(int zombieIndex)
{

    int x = zombies[zombieIndex].col + 1;
    int y = dimY_ - zombies[zombieIndex].row;
    int distanceAZ = abs(alX - x) + abs(alY - y);
    if (distanceAZ <= zombies[zombieIndex].attack_range)
    {
        if (myAlien.shield > 0)
        {
            cout << "Zombie " << zombieIndex + 1 << " attacks Alien.\n";
            cout << "It broke Alien\'s shield. Shield -1!\n\n";
            myAlien.shield -= 1;
            Pause();
            display();
            printGameStatus();
        }
        else
        {
            myAlien.health -= zombies[zombieIndex].attack;
            cout << "Zombie " << zombieIndex + 1 << " attacks Alien.\n";
            cout << "Alien receives a damage of " << zombies[zombieIndex].attack << ".\n";
            if (myAlien.health > 0)
            {
                cout << "Alien is still alive.\n\n";
                Pause();
                display();
                printGameStatus();
            }
            else if (myAlien.health <= 0)
            {
                myAlien.health = 0;
                myAlien.attack = 0;
                myAlien.shield = 0;
                cout << "Alien is defeated!\n\n";
                Pause();
                display();
                printGameStatus();
                lost();
            }
        }
    }
    else if (distanceAZ > zombies[zombieIndex].attack_range)
    {
        // zombie dont attack *out of range
        cout << "Zombie " << zombieIndex + 1 << " is unable to attack Alien.\n";
        cout << "Alien is too far.\n\n";
        Pause();
        display();
        printGameStatus();
    }
}

/////////////// GAME OBJECTS ///////////////
void rock()
{
    cout << "Alien stumbles upon a rock.\n";
    char objects[] = {
        '^',
        'v',
        '<',
        '>',
        'p',
        'h',
    };
    int noOfObjects = 6;
    int objNo = rand() % noOfObjects;

    setObject(alX, alY, objects[objNo]);
    string hiddenObject;
    if (getObject(alX, alY) == '^')
    {
        hiddenObject = "an arrow (up)";
    }
    else if (getObject(alX, alY) == 'v')
    {
        hiddenObject = "an arrow (down)";
    }
    else if (getObject(alX, alY) == '<')
    {
        hiddenObject = "an arrow (left)";
    }
    else if (getObject(alX, alY) == '>')
    {
        hiddenObject = "an arrow (right)";
    }
    else if (getObject(alX, alY) == 'p')
    {
        hiddenObject = "a pod";
    }
    else if (getObject(alX, alY) == 'h')
    {
        hiddenObject = "a health pack";
    }
    cout << "Alien discovers " << hiddenObject << " beneath the rock.\n\n";
    Pause();
}

void arrow()
{
    cout << "Alien finds an arrow.\n";
    cout << "Alien's attack is increased by 20.\n\n";
    Pause();
    myAlien.attack += 20;
    setObject(alX, alY, 'A');
    display();
    printGameStatus();
}

void zombloc()
{
    distance_1 = 100, // distance each zombie and alien
        distance_2 = 100,
    distance_3 = 100,
    distance_4 = 100,
    distance_5 = 100,
    distance_6 = 100,
    distance_7 = 100,
    distance_8 = 100,
    distance_9 = 100;
    int x = getCols();
    int y = getRows();

    int cY = 1;
    while (cY <= y)
    {
        int cX = 1;
        while (cX <= x)
        {
            getObject(cX, cY);
            if (getObject(cX, cY) == '1')
            {
                distance_1 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '2')
            {
                distance_2 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '3')
            {
                distance_3 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '4')
            {
                distance_4 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '5')
            {
                distance_5 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '6')
            {
                distance_6 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '7')
            {
                distance_7 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '8')
            {
                distance_8 = abs(cX - alX) + abs(cY - alY);
            }
            else if (getObject(cX, cY) == '9')
            {
                distance_9 = abs(cX - alX) + abs(cY - alY);
            }
            ++cX;
        }
        ++cY;
    }
}

void pod()
{
    cout << "Alien finds a pod." << endl;
    zombloc();
    int smallest = distance_1;
    int i = 0;
    while (i < 9)
    {
        if (distance_2 < smallest)
        {
            smallest = distance_2;
        }
        else if (distance_3 < smallest)
        {
            smallest = distance_3;
        }
        else if (distance_4 < smallest)
        {
            smallest = distance_4;
        }
        else if (distance_5 < smallest)
        {
            smallest = distance_5;
        }
        else if (distance_6 < smallest)
        {
            smallest = distance_6;
        }
        else if (distance_7 < smallest)
        {
            smallest = distance_7;
        }
        else if (distance_8 < smallest)
        {
            smallest = distance_8;
        }
        else if (distance_9 < smallest)
        {
            smallest = distance_9;
        }
        ++i;
    }

    int zombieHitted;
    if (smallest == distance_1)
    {
        zombies[0].health -= 10;
        if (zombies[0].health <= 0)
        {
            zombies[0].health = 0;
            setObject(zombies[0].col, zombies[0].row, ' ');
            zombieHitted = 1;
        }
        else
        {
            zombieHitted = 1;
        }
    }
    else if (smallest == distance_2)
    {
        zombies[1].health -= 10;
        if (zombies[1].health <= 0)
        {
            zombies[1].health = 0;
            setObject(zombies[1].col, zombies[1].row, ' ');
            zombieHitted = 2;
        }
        else
        {
            zombieHitted = 2;
        }
    }
    else if (smallest == distance_3)
    {
        zombies[2].health -= 10;
        zombies[2].health -= 10;
        if (zombies[2].health <= 0)
        {
            zombies[2].health = 0;
            setObject(zombies[2].col, zombies[2].row, ' ');
            zombieHitted = 3;
        }
        else
        {
            zombieHitted = 3;
        }
    }
    else if (smallest == distance_4)
    {
        zombies[3].health -= 10;
        if (zombies[3].health <= 0)
        {
            zombies[3].health = 0;
            setObject(zombies[3].col, zombies[3].row, ' ');
            zombieHitted = 4;
        }
        else
        {
            zombieHitted = 4;
        }
    }
    else if (smallest == distance_5)
    {
        zombies[4].health -= 10;
        if (zombies[4].health <= 0)
        {
            zombies[4].health = 0;
            setObject(zombies[4].col, zombies[4].row, ' ');
            zombieHitted = 5;
        }
        else
        {
            zombieHitted = 5;
        }
    }
    else if (smallest == distance_6)
    {
        if (zombies[5].health <= 0)
        {
            zombies[5].health = 0;
            setObject(zombies[5].col, zombies[5].row, ' ');
            zombieHitted = 6;
        }
        else
        {
            zombieHitted = 6;
        }
    }
    else if (smallest == distance_7)
    {
        zombies[6].health -= 10;
        if (zombies[6].health <= 0)
        {
            zombies[6].health = 0;
            setObject(zombies[6].col, zombies[6].row, ' ');
            zombieHitted = 7;
        }
        else
        {
            zombieHitted = 7;
        }
    }
    else if (smallest == distance_8)
    {
        zombies[7].health -= 10;
        if (zombies[7].health <= 0)
        {
            zombies[7].health = 0;
            setObject(zombies[7].col, zombies[7].row, ' ');
            zombieHitted = 8;
        }
        else
        {
            zombieHitted = 8;
        }
    }
    else if (smallest == distance_9)
    {
        zombies[8].health -= 10;
        if (zombies[8].health <= 0)
        {
            zombies[8].health = 0;
            setObject(zombies[8].col, zombies[8].row, ' ');
            zombieHitted = 9;
        }
        else
        {
            zombieHitted = 9;
        }
    }

    cout << "Zombie " << zombieHitted << " receives a damage of 10." << endl;
    if (zombies[zombieHitted - 1].health > 0)
    {
        cout << "Zombie " << zombieHitted << " is still alive.\n\n";
    }
    else if (zombies[zombieHitted - 1].health == 0)
    {
        cout << "Zombie " << zombieHitted << " is defeated!\n\n";
        zombiesAlive--;
    }
    Pause();
    display();
    printGameStatus();
    if (zombiesAlive == 0)
    {
        won();
    }
    else if (myAlien.health <= 0)
    {
        lost();
    }
}

void shield()
{
    cout << "Alien finds a shield.\n"
         << "Alien's shield is increased by 1.\n\n";
    Pause();
    myAlien.shield += 1;
    display();
    printGameStatus();
}

void healthpack()
{
    cout << "Alien finds a health pack.\n"
         << "Alien's life is increased by 20.\n\n";
    Pause();

    myAlien.health += 20;
    if (myAlien.health < 150)
    {
    }
    else if (myAlien.health >= 150)
    {
        myAlien.health = 150;
    }

    display();
    printGameStatus();
}

/////////////// COMMAND & MOVEMENTS ////////////////
void command()
{
    string userInput;
    cout << "Command => ";
    cin >> userInput;
    transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

    if (userInput == "up" || userInput == "down" || userInput == "left" || userInput == "right")
    {
        move(userInput);
        myAlien.attack = 0;
        untrail();
        zombieTurn();
    }
    else if (userInput == "arrow")
    {
        arrowDirection();
    }
    else if (userInput == "help")
    {
        help();
    }
    else if (userInput == "save")
    {
        save();
        Pause();
    }
    else if (userInput == "load")
    {
        load();
        Pause();
    }
    else if (userInput == "quit")
    {
        char answer;
        cout << "Are you sure you want to quit? Any unsaved progress will be lost. (y/n): \n";
        cout << "Command => ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            cout << "  ______ __ __  __ ____  _    _    __  \n"
                 << " |                                   | \n"
                 << " |  THANK YOU FOR PLAYING OUR GAME!  | \n"
                 << " |__  ___ _ __ ____ _   _    ___   __| \n\n";

            cout << "Exiting game..." << endl;
            Pause();
            exit(0);
        }
        else if (answer == 'n' || answer == 'N')
        {
            command();
        }
    }
    else
    {
        cout << "Command could not be accepted" << endl;
        cout << "Please reenter command" << endl
             << endl;
        command();
    }
}

void move(string movement)
{
    if (movement == "right")
    {
        right();
    }
    else if (movement == "left")
    {
        left();
    }
    else if (movement == "up")
    {
        up();
    }
    else if (movement == "down")
    {
        down();
    }
}

void up()
{
    int x = getCols();
    int y = getRows();

    while (alY < y)
    {
        setObject(alX, alY, '.');
        ++alY;
        getObject(alX, alY);
        if (getObject(alX, alY) == 'r')
        {
            rock();
            alY -= 1;
            setObject(alX, alY, 'A');
            display();
            printGameStatus();
            break;
        }
        else if (getObject(alX, alY) == '1' ||
                 getObject(alX, alY) == '2' ||
                 getObject(alX, alY) == '3' ||
                 getObject(alX, alY) == '4' ||
                 getObject(alX, alY) == '5' ||
                 getObject(alX, alY) == '6' ||
                 getObject(alX, alY) == '7' ||
                 getObject(alX, alY) == '8' ||
                 getObject(alX, alY) == '9')
        {
            char c = getObject(alX, alY);
            cout << "Alien attacks Zombie " << c << "." << endl;
            cout << "Zombie " << c << " receives a damage of " << myAlien.attack << "." << endl;
            int i = int(c) - int('0') - 1;
            if (zombies[i].health > myAlien.attack)
            {
                zombies[i].health -= myAlien.attack;
                cout << "Zombie " << c << " is still alive.\n\n";

                alY -= 1;
                setObject(alX, alY, 'A');
                Pause();
                display();
                printGameStatus();

                break;
            }
            else if (zombies[i].health <= myAlien.attack)
            {
                zombies[i].health = 0;
                zombies[i].alive = 0;
                setObject(alX, alY, 'A');
                cout << "Zombie " << c << " is defeated!\n\n";
                zombiesAlive--;
            }
            Pause();
            display();
            printGameStatus();
            if (zombiesAlive == 0)
            {
                won();
            }
            else if (myAlien.health <= 0)
            {
                lost();
            }
        }
        else if (getObject(alX, alY) == '^')
        {
            arrow();
            up();
            break;
        }
        else if (getObject(alX, alY) == '>')
        {
            arrow();
            right();
            break;
        }
        else if (getObject(alX, alY) == '<')
        {
            arrow();
            left();
            break;
        }
        else if (getObject(alX, alY) == 'v')
        {
            arrow();
            down();
            break;
        }
        else if (getObject(alX, alY) == 'p')
        {
            setObject(alX, alY, 'A');
            pod();
        }
        else if (getObject(alX, alY) == 'h')
        {
            setObject(alX, alY, 'A');
            healthpack();
        }
        else if (getObject(alX, alY) == 's')
        {
            setObject(alX, alY, 'A');
            shield();
        }
        else
        {
            setObject(alX, alY, 'A');
            cout << "Alien finds an empty space.\n\n";
            Pause();
            display();
            printGameStatus();
        }
    }
}

void down()
{
    int x = getCols();
    int y = getRows();

    while (alY > 1)
    {
        setObject(alX, alY, '.');
        --alY;
        getObject(alX, alY);
        if (getObject(alX, alY) == 'r')
        {
            rock();
            alY += 1;
            setObject(alX, alY, 'A');
            display();
            printGameStatus();
            break;
        }
        else if (getObject(alX, alY) == '1' ||
                 getObject(alX, alY) == '2' ||
                 getObject(alX, alY) == '3' ||
                 getObject(alX, alY) == '4' ||
                 getObject(alX, alY) == '5' ||
                 getObject(alX, alY) == '6' ||
                 getObject(alX, alY) == '7' ||
                 getObject(alX, alY) == '8' ||
                 getObject(alX, alY) == '9')
        {
            char c = getObject(alX, alY);
            cout << "Alien attacks Zombie " << c << "." << endl;
            cout << "Zombie " << c << " receives a damage of " << myAlien.attack << "." << endl;
            int i = int(c) - int('0') - 1;
            if (zombies[i].health > myAlien.attack)
            {
                zombies[i].health -= myAlien.attack;
                cout << "Zombie " << c << " is still alive.\n\n";

                alY += 1;
                setObject(alX, alY, 'A');
                Pause();
                display();
                printGameStatus();
                break;
            }
            else if (zombies[i].health <= myAlien.attack)
            {
                zombies[i].health = 0;
                zombies[i].alive = 0;
                setObject(alX, alY, 'A');
                cout << "Zombie " << c << " is defeated!\n\n";
                zombiesAlive--;
            }
            Pause();
            display();
            printGameStatus();
            if (zombiesAlive == 0)
            {
                won();
            }
            else if (myAlien.health <= 0)
            {
                lost();
            }
        }
        else if (getObject(alX, alY) == '^')
        {
            arrow();
            up();
            break;
        }
        else if (getObject(alX, alY) == '>')
        {
            arrow();
            right();
            break;
        }
        else if (getObject(alX, alY) == '<')
        {
            arrow();
            left();
            break;
        }
        else if (getObject(alX, alY) == 'v')
        {
            arrow();
            down();
            break;
        }
        else if (getObject(alX, alY) == 'p')
        {
            setObject(alX, alY, 'A');
            pod();
        }
        else if (getObject(alX, alY) == 'h')
        {
            setObject(alX, alY, 'A');
            healthpack();
        }
        else if (getObject(alX, alY) == 's')
        {
            setObject(alX, alY, 'A');
            shield();
        }
        else
        {
            setObject(alX, alY, 'A');
            cout << "Alien finds an empty space.\n\n";
            Pause();
            display();
            printGameStatus();
        }
    }
}

void right()
{
    int x = getCols();
    int y = getRows();

    while (alX < x)
    {
        setObject(alX, alY, '.');
        ++alX;
        getObject(alX, alY);
        if (getObject(alX, alY) == 'r')
        {
            rock();
            alX -= 1;
            setObject(alX, alY, 'A');
            display();
            printGameStatus();
            break;
        }
        else if (getObject(alX, alY) == '1' ||
                 getObject(alX, alY) == '2' ||
                 getObject(alX, alY) == '3' ||
                 getObject(alX, alY) == '4' ||
                 getObject(alX, alY) == '5' ||
                 getObject(alX, alY) == '6' ||
                 getObject(alX, alY) == '7' ||
                 getObject(alX, alY) == '8' ||
                 getObject(alX, alY) == '9')
        {
            char c = getObject(alX, alY);
            cout << "Alien attacks Zombie " << c << "." << endl;
            cout << "Zombie " << c << " receives a damage of " << myAlien.attack << "." << endl;
            int i = int(c) - int('0') - 1;
            if (zombies[i].health > myAlien.attack)
            {
                zombies[i].health -= myAlien.attack;
                cout << "Zombie " << c << " is still alive.\n\n";

                alX -= 1;
                setObject(alX, alY, 'A');
                Pause();
                display();
                printGameStatus();

                break;
            }
            else if (zombies[i].health <= myAlien.attack)
            {
                zombies[i].health = 0;
                zombies[i].alive = 0;
                setObject(alX, alY, 'A');
                cout << "Zombie " << c << " is defeated!\n\n";
                zombiesAlive--;
            }
            Pause();
            display();
            printGameStatus();
            if (zombiesAlive == 0)
            {
                won();
            }
            else if (myAlien.health <= 0)
            {
                lost();
            }
        }
        else if (getObject(alX, alY) == '^')
        {
            arrow();
            up();
            break;
        }
        else if (getObject(alX, alY) == '>')
        {
            arrow();
            right();
            break;
        }
        else if (getObject(alX, alY) == '<')
        {
            arrow();
            left();
            break;
        }
        else if (getObject(alX, alY) == 'v')
        {
            arrow();
            down();
            break;
        }
        else if (getObject(alX, alY) == 'p')
        {
            setObject(alX, alY, 'A');
            pod();
        }
        else if (getObject(alX, alY) == 'h')
        {
            setObject(alX, alY, 'A');
            healthpack();
        }
        else if (getObject(alX, alY) == 's')
        {
            setObject(alX, alY, 'A');
            shield();
        }
        else
        {
            setObject(alX, alY, 'A');
            cout << "Alien finds an empty space.\n\n";
            Pause();
            display();
            printGameStatus();
        }
    }
}

void left()
{
    int x = getCols();
    int y = getRows();

    while (alX > 1)
    {
        setObject(alX, alY, '.');
        --alX;
        getObject(alX, alY);
        if (getObject(alX, alY) == 'r')
        {
            rock();
            alX += 1;
            setObject(alX, alY, 'A');
            display();
            printGameStatus();
            break;
        }
        else if (getObject(alX, alY) == '1' ||
                 getObject(alX, alY) == '2' ||
                 getObject(alX, alY) == '3' ||
                 getObject(alX, alY) == '4' ||
                 getObject(alX, alY) == '5' ||
                 getObject(alX, alY) == '6' ||
                 getObject(alX, alY) == '7' ||
                 getObject(alX, alY) == '8' ||
                 getObject(alX, alY) == '9')
        {
            char c = getObject(alX, alY);
            cout << "Alien attacks Zombie " << c << "." << endl;
            cout << "Zombie " << c << " receives a damage of " << myAlien.attack << "." << endl;
            int i = int(c) - int('0') - 1;
            if (zombies[i].health > myAlien.attack)
            {
                zombies[i].health -= myAlien.attack;
                cout << "Zombie " << c << " is still alive.\n\n";

                alX += 1;
                setObject(alX, alY, 'A');
                Pause();
                display();
                printGameStatus();
                break;
            }
            else if (zombies[i].health <= myAlien.attack)
            {
                zombies[i].health = 0;
                zombies[i].alive = 0;
                setObject(alX, alY, 'A');
                cout << "Zombie " << c << " is defeated!\n\n";
                zombiesAlive--;
            }
            Pause();
            display();
            printGameStatus();
            if (zombiesAlive == 0)
            {
                won();
            }
            else if (myAlien.health <= 0)
            {
                lost();
            }
        }
        else if (getObject(alX, alY) == '^')
        {
            arrow();
            up();
            break;
        }
        else if (getObject(alX, alY) == '>')
        {
            arrow();
            right();
            break;
        }
        else if (getObject(alX, alY) == '<')
        {
            arrow();
            left();
            break;
        }
        else if (getObject(alX, alY) == 'v')
        {
            arrow();
            down();
            break;
        }
        else if (getObject(alX, alY) == 'p')
        {
            setObject(alX, alY, 'A');
            pod();
        }
        else if (getObject(alX, alY) == 'h')
        {
            setObject(alX, alY, 'A');
            healthpack();
        }
        else if (getObject(alX, alY) == 's')
        {
            setObject(alX, alY, 'A');
            shield();
        }
        else
        {
            setObject(alX, alY, 'A');
            cout << "Alien finds an empty space.\n\n";
            Pause();
            display();
            printGameStatus();
        }
    }
}

void arrowDirection()
{
    int row, column;
    string direction;
    char oldDirection, newDirection;
    do
    {
        cout << "Enter row => ";
        cin >> row;
        if (cin.fail() || row < 1 || row > dimY_)
        {
            clearInputStream();
            cout << "Invalid row number. Please enter a number between 1 and " << dimY_ << "." << endl;
        }
    } while (cin.fail() || row < 1 || row > dimY_);

    do
    {
        cout << "Enter column => ";
        cin >> column;
        if (cin.fail() || column < 1 || column > dimX_)
        {
            clearInputStream();
            cout << "Invalid column number. Please enter a number between 1 and " << dimX_ << "." << endl;
        }
    } while (cin.fail() || column < 1 || column > dimX_);

    char obj = getObject(column, row);
    if (obj != '^' && obj != 'v' && obj != '<' && obj != '>')
    {
        cout << "There is no arrow at row " << row << " and column " << column << ". Please try again." << endl
             << endl;
        arrowDirection(); // Prompt user to enter new values
    }

    do
    {
        cout << "Enter direction => ";
        cin >> direction;

        // Convert direction to lowercase
        transform(direction.begin(), direction.end(), direction.begin(), ::tolower);
        if (cin.fail() || (direction != "right" && direction != "left" && direction != "up" && direction != "down"))
        {
            clearInputStream();
            cout << "Invalid direction. Please enter one of the following: right, left, up, down." << endl;
        }
    } while (cin.fail() || (direction != "right" && direction != "left" && direction != "up" && direction != "down"));

    oldDirection = getObject(column, row);

    if (direction == "right")
    {
        newDirection = '>';
    }
    else if (direction == "left")
    {
        newDirection = '<';
    }
    else if (direction == "up")
    {
        newDirection = '^';
    }
    else if (direction == "down")
    {
        newDirection = 'v';
    }

    setObject(column, row, newDirection);

    cout << "Arrow " << oldDirection << " is switched to " << newDirection << "." << endl
         << endl;
    Pause();
    display();
    printGameStatus();
}

void help()
{
    cout << endl;
    cout << "Commands:" << endl;
    cout << "1. up      - Move up" << endl;
    cout << "2. down    - Move down" << endl;
    cout << "3. left    - Move left" << endl;
    cout << "4. right   - Move Right" << endl;
    cout << "5. arrow   - Change the direction of the arrow" << endl;
    cout << "6. help    - Display user commands" << endl;
    cout << "7. save    - Save game" << endl;
    cout << "8. load    - Load game" << endl;
    cout << "9. quit    - Quit the game" << endl;
    cout << endl;
    Pause();
}

void untrail()
{
    int x = getCols();
    int y = getRows();

    int cY = 1;
    while (cY <= y)
    {
        int cX = 1;
        while (cX <= x)
        {
            getObject(cX, cY);
            if (getObject(cX, cY) == '.')
            {
                char objects[] = {'^', 'v', '<', '>', 'p', 'h', 'r', ' ', ' ', 's'};
                int noOfObjects = 10;
                int objNo = rand() % noOfObjects;

                setObject(cX, cY, objects[objNo]);
                ++cX;
                continue;
            }
            else
            {
                ++cX;
                continue;
            }
        }
        ++cY;
    }
    cout << "Alien's turn ends. The trail is reset.\n\n";
    Pause();
    display();
    printGameStatus();
}

void won()
{
    cout << "Congratulations!\n"
         << "---------------------- YOU WIN!----------------------\n"
         << "You've defeated all your enemies and save your land!\n";
    cout << endl;
    cout << endl;
    cout << "Do you want to play again? (y/n)" << endl;
    char playagain;
    cout << "Command => ";
    cin >> playagain;
    if (tolower(playagain) == 'y')
    {
        map_.clear();
        zombies.clear();
        menuStartGame(rows, cols, zombie);
    }
    else if (tolower(playagain) == 'n')
    {
        map_.clear();
        zombies.clear();
        base(rows, cols, zombie);
    }
}

void lost()
{
    cout << "---------------- YOU LOSE !-----------------\n";
    cout << "Mission failed! You'll get 'em next time!" << endl;
    cout << endl;
    cout << endl;
    cout << "Do you want to play again? (y/n)" << endl;
    char playagain;
    cout << "Command => ";
    cin >> playagain;
    if (tolower(playagain) == 'y')
    {
        map_.clear();
        zombies.clear();
        menuStartGame(rows, cols, zombie);
    }
    else if (tolower(playagain) == 'n')
    {
        map_.clear();
        zombies.clear();
        base(rows, cols, zombie);
    }
}

char sep = ',';
void save()
{
    string filename;
    cout << "Enter file name to save: ";
    cin >> filename;
    ofstream file(filename);

    // Save map dimensions
    file << dimX_ << sep
         << dimY_ << sep;

    // Save map contents
    for (int i = 0; i < dimY_; i++)
    {
        for (int j = 0; j < dimX_; j++)
        {
            file << map_[i][j] << sep;
        }
    }

    // Save alien position and attributes
    file << alX << sep << alY << sep
         << myAlien.health << sep << myAlien.attack << sep;

    // Save zombie positions and attributes
    for (int i = 0; i < zombies.size(); i++)
    {
        file << zombies[i].row << sep << zombies[i].col << sep
             << zombies[i].health << sep << zombies[i].attack << sep << zombies[i].attack_range << sep;
    }

    file.close();
    cout << "Game saved to " << filename << endl;
}

void load()
{
    string filename;
    cout << "Enter file name to load: ";
    cin >> filename;

    ifstream file(filename);
    if (file.fail())
    {
        cout << "Failed to open file " << filename << endl;
        return;
    }

    string data;
    getline(file, data, sep);
    dimX_ = stoi(data);

    getline(file, data, sep);
    dimY_ = stoi(data);

    map_.clear();
    map_.resize(dimY_);
    for (int i = 0; i < dimY_; i++)
    {
        map_[i].resize(dimX_);
        for (int j = 0; j < dimX_; j++)
        {
            getline(file, data, sep);
            map_[i][j] = data.c_str()[0];
        }
    }

    getline(file, data, sep);
    alX = stoi(data);

    getline(file, data, sep);
    alY = stoi(data);

    getline(file, data, sep);
    myAlien.health = stoi(data);

    getline(file, data, sep);
    myAlien.attack = stoi(data);

    zombies.clear();
    for (int i = 0; i < zombie; i++)
    {
        Zombie zombie;

        getline(file, data, sep);
        zombie.col = stoi(data);

        getline(file, data, sep);
        zombie.row = stoi(data);

        getline(file, data, sep);
        zombies[i].health = stoi(data);

        getline(file, data, sep);
        zombies[i].attack = stoi(data);

        getline(file, data, sep);
        zombies[i].attack_range = stoi(data);

        zombies.push_back(zombie);
    }

    file.close();
    cout << "Game loaded from " << filename << endl;
}
