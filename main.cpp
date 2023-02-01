// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TT7L
// Names: Azri Syahmi bin Azhar | Aiman Faris bin Aidi Zamri | Muhammad Izzul Iqbal bin Ismail
// IDs: 1211103115 | 1211101303 | 1211103148
// Emails: 1211103115@student.mmu.edu.my | 1211101303@student.mmu.edu.my | 1211103148@student.mmu.edu.my
// Phones: 0108336713 | 0136811083 | 01158861607
// *********************************************************

// TODO:
// 1. Game Characters:
//     a) Atrributes: Life, Attack, Range
//     b) Movement and attack

// 2. Game Object
//     a) Arrow
//     b) Health
//     c) Pod
//     d) Rock
//     e) Empty
//     f) Trail

// 3. Game Controls:
//     a) Commands
//     b) Saving and loading

// 4. Additional feature

#include <iostream>
#include <cstdlib>
#include <iomanip> //for setw()
#include <string>
#include <vector>
#include <ctime>  //for time() in srand( time(NULL) );
#include <limits> //for numeric_limits (Clear Input Stream)
using namespace std;

int ClearScreen();
int Pause();
void clearInputStream();
void base(int &rows, int &columns, int &zombie, char characters[8]);
int chooseMenu();
// void menuStartGame();
// void menuLoadGame();
// void menuInstructions();
// void menuCredits();
void customSetting(int &rows, int &columns, int &zombie);
void checkOddRows(int &rows);
void checkOddColumns(int &columns);
void checkIntZombie(int &zombie);
void board(int &rows, int &columns, char characters[8], int &zombie);

int main()
{

    // default setting
    int rows = 5;
    int columns = 5;
    int zombie = 2;
    char characters[8] = {'^', 'v', '>', '<', 'h', 'p', 'r', ' '};
    base(rows, columns, zombie, characters);

    cout << endl;

    return 0;
}

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

void base(int &rows, int &columns, int &zombie, char characters[8])
{
    int menuChoice;
    bool done = false;
    do
    {
        menuChoice = chooseMenu();
        switch (menuChoice)
        {
        case 1:
            customSetting(rows, columns, zombie); // nanti mula dengan startgame()
            board(rows, columns, characters, zombie);
            Pause();
            break;
        case 2:
            cout << "Option 2 Not yet implemented." << endl;
            Pause();
            // menuLoadGame();
            break;
        case 3:
            cout << "Option 3 Not yet implemented." << endl;
            Pause();
            // menuInstructions();
            break;
        case 4:
            cout << "Option 4 Not yet implemented." << endl;
            Pause();
            // menuCredits();
            break;
        case 5:
            done = true; // nanti tambah confirmation
            break;
        default:
            cout << "Invalid selection, try again!" << endl;
            cout << endl;
            break;
        }
    } while (!done);

    cout << "Goodbye" << endl; // nanti tukar proper
    Pause();
    exit(0);
}

int chooseMenu()
{
    int menuChoice;

    ClearScreen();
    cout << endl;
    cout << "+---------------------------------------------+" << endl;
    cout << "|              Alien VS Zombies               |" << endl;
    cout << "|=============================================|" << endl;
    cout << "| Select:                                     |" << endl;
    cout << "| 1 => Start New Game                         |" << endl;
    cout << "| 2 => Load game                              |" << endl;
    cout << "| 3 => Instructions                           |" << endl;
    cout << "| 4 => Credits                                |" << endl;
    cout << "| 5 => Quit                                   |" << endl;
    cout << "+---------------------------------------------+" << endl;
    cout << endl;
    cout << "Command => ";
    cin >> menuChoice;
    clearInputStream();

    return menuChoice;
}

void customSetting(int &rows, int &columns, int &zombie)
{
    cout << "-----------------------" << endl;
    cout << " Default Game Settings " << endl;
    cout << "-----------------------" << endl;
    cout << " " << endl;
    cout << "Board Rows: " << rows << endl;
    cout << "Board Columns: " << columns << endl;
    cout << "Zombie Count: " << zombie << endl
         << endl;

    char choice;
    cout << "Do you wish to change default setting? (y/n) => ";
    cin >> choice; // nanti tambah check input

    // check if user wants to change default setting
    if (tolower(choice) == 'y') // tolower() is to convert input to lowercase
    {
        checkOddRows(rows); // Ask user no of rows and check if valid

        checkOddColumns(columns); // Ask user no of columns and check if valid

        checkIntZombie(zombie); // Ask user no of zombies and check if valid
    }                           // nanti tambah if no
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

void checkOddColumns(int &columns)
{
    while (true)
    {
        cout << "Enter number of columns: ";
        cin >> columns;
        if (cin.fail())
        {
            clearInputStream();
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        else if (columns % 2 == 0 || columns > 99)
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
        else if (zombie > 10)
        {
            cout << " " << endl;
            cout << "Number of zombie must below than 10" << endl
                 << endl;
            continue;
        }
        else
        {
            break;
        }
    }
}

void board(int &rows, int &columns, char characters[8], int &zombie)
{
    cout << '\n';
    srand(time(0));

    // calculate the middle row and column
    int midRow = (rows + 1) / 2;
    int midCol = (columns + 1) / 2;
    int zombiePlaced = 0;

    // print column number on top of the grid
    cout << "   ";
    for (int i = 1; i <= columns; i++)
    {
        cout << setw(3) << i << " ";
    }
    cout << endl;

    //print top grid
    cout << setw(4);
    for (int i = 0; i < (columns * 4) + 1; i++)
    {
        cout << "-";
    }
    cout << endl;

    //print rows with objects and "|" and row number
    for (int i = 1; i <= rows; i++)
    {
        cout << setw(2) << i << " |";   //row number
        for (int j = 1; j <= columns; j++)  //Object
        {
            if (i == midRow && j == midCol) //Alien
            {
                cout << setw(2) << "A" <<setw(2)
                     << "|";
            }
            else    //Other objects
            {
                int randomNum = rand() % 8;
                if (randomNum == 1)
                {
                    cout << setw(2) << "r"<<setw(2)
                         << "|";
                }
                else if (randomNum == 0 && zombiePlaced < zombie)
                {
                    cout << setw(2) << "Z"<<setw(2)
                         << "|";
                    zombiePlaced++;
                }
                else if (randomNum == 2)
                {
                    cout << setw(2) << "^"<<setw(2)
                         << "|";
                }
                else if (randomNum == 3)
                {
                    cout << setw(2) << ">"<<setw(2)
                         << "|";
                }
                else if (randomNum == 4)
                {
                    cout << setw(2) << "<"<<setw(2)
                         << "|";
                }
                else if (randomNum == 5)
                {
                    cout << setw(2) << "v"<<setw(2)
                         << "|";
                }
                else if (randomNum == 6)
                {
                    cout << setw(2) << "p"<<setw(2)
                         << "|";
                }
                else if (randomNum == 7)
                {
                    cout << setw(2) << "h"<<setw(2)
                         << "|";
                }
                else
                {
                    cout << setw(2) << "  "<<setw(2)
                         << "|";
                }
            }
        }
        cout << endl;

        //print bottom grid
        cout << setw(4);
        for (int i = 0; i < (columns * 4) + 1; i++)
        {
            cout << "-";
        }
        cout << endl;
    }
}