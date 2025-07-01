#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Color codes for terminal output
extern string redBg;    
extern string whiteBg;  
extern string blackBg;  
extern string greenBg;  
extern string reset;     

// Forward declaration of global variables used in board display
extern vector<class Piece*> board;  
extern vector<int> highlightSquares; 
extern vector<class Piece*> capturedWhitePieces;
extern vector<class Piece*> capturedBlackPieces;

// Function declarations
void clearTerminalScreen();
void printBoard();
bool isSquareHighlighted(int index);

#endif // BOARD_DISPLAY_H