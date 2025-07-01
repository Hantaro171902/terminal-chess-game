#include "board.h"
#include "piece.h" // To access Piece members like symbol
#include <iostream>
#include <iomanip> // For std::setw
#include <algorithm> // For std::find

using namespace std;

// Define color codes (declared in board.h)
string redBg = "\033[41m";    
string whiteBg = "\033[47m";  
string blackBg = "\033[40m";  
string greenBg = "\033[42m";  
string reset = "\033[0m"; 

// Clear the terminal screen
void clearTerminalScreen() {
    cout << "\033[2J\033[1H"; // ANSI escape codes to clear the screen and move the cursor to the top left
}

// Check if a given index is one of the highlighted squares
bool isSquareHighlighted(int index) {
    return find(highlightSquares.begin(), highlightSquares.end(), index) != highlightSquares.end();
}

// Print the current state of the board to the terminal
void printBoard() {
    // Display captured pieces on the side
    cout << "Captured Black Pieces: ";
    for (Piece* p : capturedBlackPieces) {
        cout << p->symbol << " ";
    }
    cout << endl;

    for (int row = 0; row <= 7; ++row) { // Iterate from row 8 down to 1
        cout << row + 1 << " "; // Print row number
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            // Determine square color (alternating)
            bool isWhiteSquare = (row + col) % 2 == 0;
            string bgColor = isWhiteSquare ? whiteBg : blackBg;

            // Highlight possible moves
            if (isSquareHighlighted(index)) {
                bgColor = greenBg;
            }
            
            // Print the square with its background and piece symbol
            cout << bgColor << " " << (board[index] ? board[index]->symbol : " ") << " " << reset;
        }
        cout << endl; // New line after each row
    }
    // Print column labels
    cout << "   a  b  c  d  e  f  g  h" << endl;

    cout << "Captured White Pieces: ";
    for (Piece* p : capturedWhitePieces) {
        cout << p->symbol << " ";
    }
    cout << endl;
}



