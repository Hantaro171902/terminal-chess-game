#include <iostream>
#include <string>
#include <algorithm>

#include "gameLogic.h"
#include "piece.h"
#include "board.h"

using namespace std;

int main() {
    initializeBoard();
    
    while (true) {
        clearTerminalScreen(); // Clear terminal before printing
        printBoard();          // Display the current board

        // Display current turn
        cout << (isWhiteTurn ? "White's turn" : "Black's turn") << ". Enter move (e.g. e2 e4): ";
        string fromStr, toStr;
        cin >> fromStr >> toStr;

        int fromIndex = convertPosToIndex(fromStr);
        int toIndex = convertPosToIndex(toStr);

        // Input validation
        if (fromIndex == -1 || toIndex == -1) {
            clearTerminalScreen();
            printBoard();
            cout << "Invalid input format or coordinates. Please use format like 'e2 e4'.\n";
            cin.ignore(); // Clear invalid input
            cin.get();    // Wait for user to press enter
            continue;
        }
        
        // Check if there's a piece at the source square
        if (board[fromIndex] == nullptr) {
            clearTerminalScreen();
            printBoard();
            cout << "No piece at the source square '" << fromStr << "'.\n";
            cin.ignore();
            cin.get();
            continue;
        }

        // Check if it's the correct turn for the selected piece
        if ((isWhiteTurn && !board[fromIndex]->isWhite) || (!isWhiteTurn && board[fromIndex]->isWhite)) {
            clearTerminalScreen();
            printBoard();
            cout << "It's " << (isWhiteTurn ? "White's" : "Black's") << " turn. You selected an opponent's piece.\n";
            cin.ignore();
            cin.get();
            continue;
        }

        // Get possible moves for the selected piece
        highlightSquares = board[fromIndex]->getPossibleMoves(fromIndex);

        // Check if the destination is a valid move
        bool isValidMove = false;
        // Using find for checking if 'toIndex' is in 'highlightSquares'
        if (find(highlightSquares.begin(), highlightSquares.end(), toIndex) != highlightSquares.end()) {
            isValidMove = true;
        }

        if (isValidMove) {
            executeMove(fromIndex, toIndex); // Perform the move (now includes capture logic)
            isWhiteTurn = !isWhiteTurn;      // Switch turns
            highlightSquares.clear();        // Clear highlights after move
            // Pause briefly to let the "captured" message be seen before screen clears
            cin.ignore();
            cout << "Press Enter to continue...";
            cin.get(); 
        } else {
            clearTerminalScreen();
            printBoard(); // Re-print board with highlights for clarity
            cout << "Invalid move for " << board[fromIndex]->symbol << " from " << fromStr << " to " << toStr << ".\n";
            cout << "Possible moves are highlighted in green.\n";
            cin.ignore();
            cin.get(); // Wait for user to press enter
            highlightSquares.clear(); // Clear highlights if move is invalid
        }
    }

    // Clean up allocated memory (though the infinite loop prevents reaching here in typical game flow)
    // This part should technically be called when the game ends (e.g., checkmate or stalemate)
    // For now, it's here for completeness if the loop were to break.
    for (int i = 0; i < 64; ++i) {
        if (board[i] != nullptr) {
            delete board[i];
        }
    }
    for (Piece* p : capturedWhitePieces) {
        delete p;
    }
    for (Piece* p : capturedBlackPieces) {
        delete p;
    }

    return 0;
}
