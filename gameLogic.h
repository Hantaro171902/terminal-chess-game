#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Forward declaration of Piece class
class Piece;

// Global game state variables (defined in gameLogic.cpp)
extern vector<Piece*> board;  
extern vector<int> highlightSquares; 
extern bool isWhiteTurn;
extern vector<Piece*> capturedWhitePieces; 
extern vector<Piece*> capturedBlackPieces; 

// Function declarations
void initializeBoard();
int convertPosToIndex(const string& pos);
void executeMove(int fromIndex, int toIndex);

#endif // GAME_LOGIC_H