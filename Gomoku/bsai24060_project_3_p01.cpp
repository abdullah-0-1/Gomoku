#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include<string>
#include<time.h>

using namespace std;


void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

// ******************************************

void Initialization(int& Dim, int& NOP, int& Turn, int& WinCon, char*& Psym, string*& Names, char**& Board)
{
    cout << "Enter Dimension: ";
    cin >> Dim;
    Board = new char* [Dim];
    cout << "Enter Number of Players: ";
    cin >> NOP;
    int seed = time(0);
    int n = 0, sum = 0;
    srand(seed);
    Turn = rand() % NOP;
    cout << "Enter Win Condition (5 for gomoku and 3 for Tic Tac Toe): ";
    cin >> WinCon;
    Psym = new char[NOP];
    Names = new string[NOP];
    int c = 1;
    cout << "Enter Player name and its symbol: \n";
    for (int i = 0; i < NOP; i++)
    {
        cout << c << ") ";
        cin >> Names[i] >> Psym[i];
        c++;
    }
    // Board
    for (int ri = 0; ri < Dim; ri++)
    {
        Board[ri] = new char[Dim];
        for (int ci = 0; ci < Dim; ci++)
        {
            Board[ri][ci] = '-';
        }
    }
    system("cls");
}

void PrintBoard(char** Board, int Dim)
{
    for (int ri = 0; ri < Dim; ri++)
    {
        for (int ci = 0; ci < Dim; ci++)
        {
            cout << Board[ri][ci];
        }
        cout << endl;
    }
}

void TurnDisplay(string Aname)
{
    cout << "\"" << Aname << "\"" << "Take Your Turn...\n" << "Click on \"-\" to select\n";
}

void PlayerTurn(int& r, int& c)
{
    getRowColbyLeftClick(r, c);
}

bool IsValid(char** Board, int r, int c, int Dim)
{
    return((r >= 0 && c >= 0) && (r < Dim && c < Dim) && (Board[r][c] == '-'));
}

void BoardUpdate(char**& Board, int r, int c, char Asym)
{
    Board[r][c] = Asym;
}

void TurnChange(int& turn, int NOP)
{
    turn++;
    turn = turn % NOP;
}

bool IsDraw(char** Board, int Dim)
{
    for (int ri = 0; ri < Dim; ri++)
    {
        for (int ci = 0; ci < Dim; ci++)
        {
            if (Board[ri][ci] == '-')
            {
                return false;
            }
        }
    }
    return true;
}

bool HWin(char** Board, int Dim, int ri, int ci, int WinCon, char Asym)
{
    int count = 0;

    if (ci + (WinCon - 1) >= Dim)
    {
        return false;
    }

    for (int i = 0; i < WinCon; i++)
    {
        if (Board[ri][ci + i] == Asym)
        {
            count++;
        }
    }
    return (count == WinCon);
}

bool VWin(char** Board, int Dim, int ri, int ci, int WinCon, char Asym)
{
    int count = 0;

    if (ri + (WinCon - 1) >= Dim)
    {
        return false;
    }
    for (int i = 0; i < WinCon; i++)
    {
        if (Board[ri + i][ci] == Asym)
        {
            count++;
        }
    }
    return (count == WinCon);
}

bool DWin(char** Board, int Dim, int ri, int ci, int WinCon, char Asym)
{
    int count = 0;
    if (ci + (WinCon - 1) >= Dim || ri + (WinCon - 1) >= Dim)
    {
        return false;
    }
    for (int i = 0; i < WinCon; i++)
    {
        if (Board[ri + i][ci + i] == Asym)
        {
            count++;
        }
    }
    return (count == WinCon);
}

bool DRWin(char** Board, int Dim, int ri, int ci, int WinCon, char Asym)
{
    int count = 0;

    if (ci + (WinCon - 1) >= Dim || ri - (WinCon - 1) < 0)
    {
        return false;
    }

    for (int i = 0; i < WinCon; i++)
    {
        if (Board[ri - i][ci + i] == Asym)
        {
            count++;
        }
    }
    return (count == WinCon);
}

bool AmIWinner(char** Board, int Dim, int ri, int ci, int WinCon, char Asym)
{
    return(HWin(Board, Dim, ri, ci, WinCon, Asym) || VWin(Board, Dim, ri, ci, WinCon, Asym) ||
        DWin(Board, Dim, ri, ci, WinCon, Asym) || DRWin(Board, Dim, ri, ci, WinCon, Asym));
}


bool IsWinner(char** Board, int Dim, char Asym, int WinCon)
{
    for (int ri = 0; ri < Dim; ri++)
    {
        for (int ci = 0; ci < Dim; ci++)
        {
            if (AmIWinner(Board, Dim, ri, ci, WinCon, Asym))
            {
                return true;
            }
        }
    }
    return false;
}


/*
    Recommended Compiler settings:
    Font size: atleast 36
    Font: Cascadia Code
*/

int main()
{
    int Dim, NOP, Turn, WinCon, r, c;
    char* Psym;
    string* Names;
    char** Board;
    bool W = false;
    Initialization(Dim, NOP, Turn, WinCon, Psym, Names, Board);
    PrintBoard(Board, Dim);

    do
    {
        TurnDisplay(Names[Turn]);
        do
        {
            PlayerTurn(r, c);
        } while (!IsValid(Board, r, c, Dim));
        BoardUpdate(Board, r, c, Psym[Turn]);
        system("cls");
        PrintBoard(Board, Dim);
        if (IsWinner(Board, Dim, Psym[Turn], WinCon))
        {
            W = true;
            break;
        }


        TurnChange(Turn, NOP);
    } while (!IsDraw(Board, Dim));

    if (W == true)
    {
        cout << "Winner of This Match Is " << Names[Turn];
    }
    else
    {
        cout << "Match Ended as a DRAW...";
    }

    for (int i = 0; i < Dim; i++)
    {
        delete[] Board[i];
    }
    delete[] Board;
    delete[] Psym;
    delete[] Names;
    return 0;
}