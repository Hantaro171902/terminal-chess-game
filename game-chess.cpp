#include <iostream>
#include <iomanip> // for setw
#include <string>
using namespace std;

const string whiteBg = "\033[47m";
const string blackBg = "\033[100m";
const string reset = "\033[0m";

// ASCII piece representation: white = upper, black = lower
string board[64];

void initBoard()
{
    // Emoji chess pieces: white = row 7, black = row 0
    string whiteBackRow[8] = {"🏰", "🐴", "🧙", "👸", "👑", "🧙", "🐴", "🏰"};
    string blackBackRow[8] = {"🏯", "🐎", "🧛", "🧝", "🤴", "🧛", "🐎", "🏯"};

    for (int i = 8; i < 16; ++i)
        board[i] = "🚶"; // Black pawn
    for (int i = 48; i < 56; ++i)
        board[i] = "🧍"; // White pawn

    // Black back row
    for (int i = 0; i < 8; ++i)
        board[i] = blackBackRow[i];

    // Empty
    for (int i = 16; i < 48; ++i)
        board[i] = " ";

    // White back row
    for (int i = 56; i < 64; ++i)
        board[i] = whiteBackRow[i - 56];
}

void printBoard()
{
    for (int row = 7; row >= 0; --row)
    {
        cout << row + 1 << " ";
        for (int col = 0; col < 8; ++col)
        {
            int index = row * 8 + col;
            bool isWhiteSquare = (row + col) % 2 == 0;
            string bg = isWhiteSquare ? whiteBg : blackBg;

            cout << bg << " " << setw(2) << board[index] << " " << reset;
        }
        cout << endl;
    }

    // Column labels
    cout << "   ";
    for (char c = 'a'; c <= 'h'; ++c)
        cout << " " << setw(2) << c << " ";
    cout << endl;
}

int main()
{
    initBoard();
    printBoard();
    return 0;
}
