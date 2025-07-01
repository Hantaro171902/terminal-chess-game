#include "gameLogic.h"
#include "piece.h"
#include <iostream>

using namespace std;

// Global game state variables definitions (declared in gameLogic.h)
vector<Piece*> board(64, nullptr);
vector<int> highlightSquares; 
bool isWhiteTurn = true; 
vector<Piece*> capturedWhitePieces; 
vector<Piece*> capturedBlackPieces; 

// Initializes the chess board with pieces in their starting positions
void initializeBoard() {
    // Clear any existing pieces and captured pieces
    for (int i = 0; i < 64; ++i) {
        if (board[i] != nullptr) {
            delete board[i];
            board[i] = nullptr;
        }
    }
    for (Piece* p : capturedWhitePieces) {
        delete p;
    }
    capturedWhitePieces.clear();
    for (Piece* p : capturedBlackPieces) {
        delete p;
    }
    capturedBlackPieces.clear();

    // Black pieces
    board[0] = new Rook(false);
    board[1] = new Knight(false);
    board[2] = new Bishop(false);
    board[3] = new Queen(false);
    board[4] = new King(false);
    board[5] = new Bishop(false);
    board[6] = new Knight(false);
    board[7] = new Rook(false);
    for (int i = 8; i < 16; ++i) {
        board[i] = new Pawn(false);
    }

    // White pieces
    for (int i = 48; i < 56; ++i) {
        board[i] = new Pawn(true);
    }
    board[56] = new Rook(true);
    board[57] = new Knight(true);
    board[58] = new Bishop(true);
    board[59] = new Queen(true);
    board[60] = new King(true);
    board[61] = new Bishop(true);
    board[62] = new Knight(true);
    board[63] = new Rook(true);

    isWhiteTurn = true; // Reset turn for a new game
}

// Converts a chess position string (e.g., "e2") to a board index (0-63)
int convertPosToIndex(const string& pos) {
    if (pos.length() != 2) {
        return -1; // Invalid format
    }
    int col = pos[0] - 'a'; // 'a' to 'h' -> 0 to 7
    int row = pos[1] - '1'; // '1' to '8' -> 0 to 7

    if (col < 0 || col > 7 || row < 0 || row > 7) {
        return -1; // Out of bounds
    }

    return (7 - row) * 8 + col;
}

// Executes a move on the board
void executeMove(int fromIndex, int toIndex) {
    if (board[toIndex] != nullptr) {
        // Add the captured piece to the appropriate list
        if (board[toIndex]->isWhite) {
            capturedWhitePieces.push_back(board[toIndex]);
        } else {
            capturedBlackPieces.push_back(board[toIndex]);
        }
        // Inform the player about the capture
        std::cout << (board[fromIndex]->isWhite ? "White's " : "Black's ") << board[fromIndex]->symbol 
                  << " captured " << (board[toIndex]->isWhite ? "White's " : "Black's ") 
                  << board[toIndex]->symbol << "!\n";
    }
    board[toIndex] = board[fromIndex]; // Move the piece
    board[fromIndex] = nullptr;        // Clear the original square
}