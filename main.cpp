#include <iostream>
#include <string>
#include <algorithm>

#include "gameLogic.h"
#include "piece.h"
#include "board.h"

using namespace std;

int main() {
    initializeBoard(); // Set up the initial board

    while (true) {
        clearTerminalScreen(); // Clear terminal before printing
        printBoard();          // Display the current board

        // Display current turn
        cout << (isWhiteTurn ? "White's turn" : "Black's turn") << ". Enter move (e.g. e2 e4): ";
        string fromStr, toStr;
        cin >> fromStr >> toStr;

        // Convert input to board indices
        int fromIndex = convertPosToIndex(fromStr);
        int toIndex = convertPosToIndex(toStr);

        // Validate move
        if (fromIndex == -1 || toIndex == -1) {
            clearTerminalScreen();
            printBoard();
            cout << "Invalid input format or coordinates. Please use format like 'e2 e4'.\n" << endl;
            cin.ignore();
            cin.get();
            continue;
        }

        if (board[fromIndex] == nullptr) {
        	clearTerminalScreen();
        	printBoard();
        	cout << "No piece at the source square ''" << fromStr << "'.\n";
            cin.ignore();
            cin.get();
            continue;
		}

        if ((isWhiteTurn && !board[fromIndex]->isWhite) || (!isWhiteTurn && board[fromIndex]->isWhite)) {
            clearTerminalScreen();
            printBoard();
            cout << "It's " << (isWhiteTurn ? "White's" : "Black's") << " turn. You selected an opponent's piece.\n";
            cin.ignore();
            cin.get();
            continue;
        }

        highlightSquares = board[fromIndex]->getPossibleMoves(fromIndex);

        bool isValidMove = false;
        if (find(highlightSquares.begin(), highlightSquares.end(), toIndex) != highlightSquares.end()) {
            isValidMove = true;
        }

        if (isValidMove) {
            executeMove(fromIndex, toIndex);
            isWhiteTurn = !isWhiteTurn; // Switch turns
            highlightSquares.clear(); 
            cin.ignore(); 
            cout << "Press Enter to continue...";
            cin.get(); 
        } else {
            clearTerminalScreen();
            printBoard();
            cout << "Invalid move from " << board[fromIndex]->symbol << " from " << fromStr << " to " << toStr << ". Try again.\n";
            cout << "Possible moves are highlighted in green.\n";
            cin.ignore();
            cin.get();
            highlightSquares.clear(); // Clear highlights after invalid move
        }

        // Clean up allocated memory when the game ends (or program exits)
        for (int i = 0; i < 64; ++i) {
            delete board[i]; 
            board[i] = nullptr; 
        }
        for (Piece* p : capturedWhitePieces) {
            delete p; 
        }

        for (Piece* p : capturedBlackPieces) {
            delete p; 
        }

        return 0; 
    }

}
