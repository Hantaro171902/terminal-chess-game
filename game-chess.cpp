#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

string board[64];
string redBg = "\033[41m";   // red background
string whiteBg = "\033[47m"; // white background
string blackBg = "\033[40m"; // black background
string greenBg = "\033[42m"; // green background for moves
string reset = "\033[0m";

vector<int> highlightSquares;

void initBoard()
{
    string whiteBackRow[8] = {"♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"};
    string blackBackRow[8] = {"♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"};

    for (int i = 0; i < 8; ++i)
        board[i] = blackBackRow[i];
    for (int i = 8; i < 16; ++i)
        board[i] = "♟︎";
    for (int i = 16; i < 48; ++i)
        board[i] = " ";
    for (int i = 48; i < 56; ++i)
        board[i] = "♙";
    for (int i = 56; i < 64; ++i)
        board[i] = whiteBackRow[i - 56];
}

bool isHighlighted(int index)
{
    for (int i : highlightSquares)
        if (i == index)
            return true;
    return false;
}

void clearScreen()
{
    cout << "\033[2J\033[1H"; // ANSI escape code to clear the screen
}

void printBoard()
{
    // cout << "   a  b  c  d  e  f  g  h" << endl;
    for (int row = 7; row >= 0; --row)
    {
        cout << row + 1 << " ";
        for (int col = 0; col < 8; ++col)
        {
            int index = row * 8 + col;
            bool isWhiteSquare = (row + col) % 2 == 0;
            string bg = isWhiteSquare ? whiteBg : blackBg;
            if (isHighlighted(index))
                bg = greenBg;
            cout << bg << " " << board[index] << " " << reset;
        }
        cout << " " << row + 1 << endl;
    }
    cout << "   a  b  c  d  e  f  g  h" << endl;
}

int posToIndex(string pos)
{
    if (pos.length() != 2)
        return -1;
    int col = pos[0] - 'a';
    int row = pos[1] - '1';
    if (col < 0 || col > 7 || row < 0 || row > 7)
        return -1;
    row = 7 - row;
    return row * 8 + col;
}

bool isInsideBoard(int row, int col)
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

vector<int> getPossibleMoves(int index)
{
    vector<int> moves;
    string piece = board[index];
    int row = index / 8;
    int col = index % 8;

    if (piece == "♙")
    { // White pawn
        if (isInsideBoard(row - 1, col) && board[(row - 1) * 8 + col] == " ")
            moves.push_back((row - 1) * 8 + col);
    }
    else if (piece == "♖")
    { // Rook
        // Up
        for (int r = row - 1; r >= 0; --r)
        {
            int i = r * 8 + col;
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        // Down
        for (int r = row + 1; r < 8; ++r)
        {
            int i = r * 8 + col;
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        // Left
        for (int c = col - 1; c >= 0; --c)
        {
            int i = row * 8 + c;
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        // Right
        for (int c = col + 1; c < 8; ++c)
        {
            int i = row * 8 + c;
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
    }
    else if (piece == "♗")
    { // Bishop
        // 4 diagonals
        for (int dr = -1, dc = -1; isInsideBoard(row + dr, col + dc); --dr, --dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = -1, dc = 1; isInsideBoard(row + dr, col + dc); --dr, ++dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = 1, dc = -1; isInsideBoard(row + dr, col + dc); ++dr, --dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = 1, dc = 1; isInsideBoard(row + dr, col + dc); ++dr, ++dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
    }
    else if (piece == "♘")
    { // Knight
        int movesKnight[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
        for (auto m : movesKnight)
        {
            int r = row + m[0];
            int c = col + m[1];
            if (isInsideBoard(r, c) && board[r * 8 + c] == " ")
                moves.push_back(r * 8 + c);
        }
    }
    else if (piece == "♕")
    { // Queen
        // Rook + Bishop moves
        highlightSquares.clear();
        vector<int> rookMoves = getPossibleMoves(index); // Call rook code manually or inline
        highlightSquares = rookMoves;
        // Diagonals manually as Bishop
        for (int dr = -1, dc = -1; isInsideBoard(row + dr, col + dc); --dr, --dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = -1, dc = 1; isInsideBoard(row + dr, col + dc); --dr, ++dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = 1, dc = -1; isInsideBoard(row + dr, col + dc); ++dr, --dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
        for (int dr = 1, dc = 1; isInsideBoard(row + dr, col + dc); ++dr, ++dc)
        {
            int i = (row + dr) * 8 + (col + dc);
            if (board[i] == " ")
                moves.push_back(i);
            else
                break;
        }
    }
    else if (piece == "♔")
    { // King
        int drs[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dcs[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int d = 0; d < 8; ++d)
        {
            int r = row + drs[d];
            int c = col + dcs[d];
            if (isInsideBoard(r, c) && board[r * 8 + c] == " ")
                moves.push_back(r * 8 + c);
        }
    }

    return moves;
}

void movePiece(int from, int to)
{
    board[to] = board[from];
    board[from] = " ";
}

int main()
{
    initBoard();

    while (true)
    {
        clearScreen();
        printBoard();
        cout << "It is white's move" << endl;
        cout << "Pick a piece (example e2): ";
        string pick;
        cin >> pick;
        int from = posToIndex(pick);
        if (from == -1)
        {
            cout << "Invalid input, try again.\n";
            continue;
        }

        highlightSquares = getPossibleMoves(from);
        clearScreen();
        printBoard();

        cout << "Move to (example e4): ";
        string destination;
        cin >> destination;
        int to = posToIndex(destination);

        if (to != -1 && isHighlighted(to))
        {
            movePiece(from, to);
        }
        else
        {
            cout << "Invalid move.\n";
            cin.ignore();
            cin.get(); // wait
        }

        highlightSquares.clear();
    }

    return 0;
}
