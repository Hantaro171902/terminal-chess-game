#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm> // Required for std::find

// Define color codes for terminal output
std::string redBg = "\033[41m";    // Red background
std::string whiteBg = "\033[47m";  // White background
std::string blackBg = "\033[40m";  // Black background
std::string greenBg = "\033[42m";  // Green background for possible moves
std::string reset = "\033[0m";     // Reset formatting

// Forward declaration for Piece class (needed for board declaration)
class Piece;

// Global board representation using pointers to Piece objects
std::vector<Piece*> board(64, nullptr);
// List of squares to highlight (possible moves)
std::vector<int> highlightSquares;
// Track whose turn it is (true for White, false for Black)
bool isWhiteTurn = true;

// New: Lists to store captured pieces
std::vector<Piece*> capturedWhitePieces;
std::vector<Piece*> capturedBlackPieces;

// --- Base Piece Class and Derived Classes ---
// Abstract base class for all chess pieces
class Piece {
public:
    bool isWhite;
    std::string symbol;

    Piece(bool isWhite, std::string symbol) : isWhite(isWhite), symbol(symbol) {}
    // Virtual function to get all possible moves for a piece from a given index
    virtual std::vector<int> getPossibleMoves(int index) const = 0;
    // Virtual destructor for proper memory deallocation
    virtual ~Piece() {}
};

// Pawn class
class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite, isWhite ? "♙" : "♟︎") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;
        int direction = isWhite ? -1 : 1; // White moves up (row decreases), Black moves down (row increases)

        // Single step forward
        int newRow = row + direction;
        int newIndex = newRow * 8 + col;
        if (newRow >= 0 && newRow < 8 && board[newIndex] == nullptr) {
            moves.push_back(newIndex);
        }

        // Double step forward on first move
        if (isWhite && row == 6 && board[newIndex] == nullptr && board[(row - 2) * 8 + col] == nullptr) {
            moves.push_back((row - 2) * 8 + col);
        } else if (!isWhite && row == 1 && board[newIndex] == nullptr && board[(row + 2) * 8 + col] == nullptr) {
            moves.push_back((row + 2) * 8 + col);
        }

        // Captures (diagonal)
        int captureCols[] = {col - 1, col + 1};
        for (int c : captureCols) {
            if (newRow >= 0 && newRow < 8 && c >= 0 && c < 8) {
                int captureIndex = newRow * 8 + c;
                if (board[captureIndex] != nullptr && board[captureIndex]->isWhite != isWhite) {
                    moves.push_back(captureIndex);
                }
            }
        }
        return moves;
    }
};

// Rook class
class Rook : public Piece {
public:
    Rook(bool isWhite) : Piece(isWhite, isWhite ? "♖" : "♜") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;

        // Directions: Up, Down, Left, Right
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int r = row + dr[i];
            int c = col + dc[i];
            while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                int currentIdx = r * 8 + c;
                if (board[currentIdx] == nullptr) { // Empty square
                    moves.push_back(currentIdx);
                } else { // Occupied square
                    if (board[currentIdx]->isWhite != isWhite) { // Opponent's piece
                        moves.push_back(currentIdx);
                    }
                    break; // Stop if square is occupied (cannot jump over)
                }
                r += dr[i];
                c += dc[i];
            }
        }
        return moves;
    }
};

// Knight class
class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite, isWhite ? "♘" : "♞") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;

        // All 8 possible L-shaped moves for a knight
        int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
        int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

        for (int i = 0; i < 8; ++i) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                if (board[newIndex] == nullptr || board[newIndex]->isWhite != isWhite) {
                    moves.push_back(newIndex);
                }
            }
        }
        return moves;
    }
};

// Bishop class
class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite, isWhite ? "♗" : "♝") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;

        // Directions: All 4 diagonals
        int dr[] = {-1, -1, 1, 1};
        int dc[] = {-1, 1, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int r = row + dr[i];
            int c = col + dc[i];
            while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                int currentIdx = r * 8 + c;
                if (board[currentIdx] == nullptr) { // Empty square
                    moves.push_back(currentIdx);
                } else { // Occupied square
                    if (board[currentIdx]->isWhite != isWhite) { // Opponent's piece
                        moves.push_back(currentIdx);
                    }
                    break; // Stop if square is occupied
                }
                r += dr[i];
                c += dc[i];
            }
        }
        return moves;
    }
};

// Queen class
class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite, isWhite ? "♕" : "♛") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;

        // Queen moves like a Rook (horizontal/vertical)
        int rook_dr[] = {-1, 1, 0, 0};
        int rook_dc[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int r = row + rook_dr[i];
            int c = col + rook_dc[i];
            while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                int currentIdx = r * 8 + c;
                if (board[currentIdx] == nullptr) {
                    moves.push_back(currentIdx);
                } else {
                    if (board[currentIdx]->isWhite != isWhite) {
                        moves.push_back(currentIdx);
                    }
                    break;
                }
                r += rook_dr[i];
                c += rook_dc[i];
            }
        }

        // Queen moves like a Bishop (diagonal)
        int bishop_dr[] = {-1, -1, 1, 1};
        int bishop_dc[] = {-1, 1, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int r = row + bishop_dr[i];
            int c = col + bishop_dc[i];
            while (r >= 0 && r < 8 && c >= 0 && c < 8) {
                int currentIdx = r * 8 + c;
                if (board[currentIdx] == nullptr) {
                    moves.push_back(currentIdx);
                } else {
                    if (board[currentIdx]->isWhite != isWhite) {
                        moves.push_back(currentIdx);
                    }
                    break;
                }
                r += bishop_dr[i];
                c += bishop_dc[i];
            }
        }
        return moves;
    }
};

// King class
class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite, isWhite ? "♔" : "♚") {}

    std::vector<int> getPossibleMoves(int index) const override {
        std::vector<int> moves;
        int row = index / 8;
        int col = index % 8;

        // All 8 surrounding squares
        int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < 8; ++i) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                int newIndex = newRow * 8 + newCol;
                if (board[newIndex] == nullptr || board[newIndex]->isWhite != isWhite) {
                    moves.push_back(newIndex);
                }
            }
        }
        return moves;
    }
};

// --- Game Logic Functions ---

// Initializes the chessboard with pieces in their starting positions
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

// Checks if a given index is one of the highlighted squares
bool isSquareHighlighted(int index) {
    // Use std::find for better readability and potentially performance
    return std::find(highlightSquares.begin(), highlightSquares.end(), index) != highlightSquares.end();
}

// Clears the terminal screen
void clearTerminalScreen() {
    std::cout << "\033[2J\033[1H";
}

// Prints the current state of the board to the terminal
void printBoard() {
    // Display captured pieces on the side
    std::cout << "Captured Black Pieces: ";
    for (Piece* p : capturedBlackPieces) {
        std::cout << p->symbol << " ";
    }
    std::cout << std::endl;

    for (int row = 0; row <= 7; ++row) { // Iterate from row 8 down to 1
        std::cout << row + 1 << " "; // Print row number
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;
            // Determine square color (alternating)
            bool isWhiteSquare = (row + col) % 2 == 0;
            std::string bgColor = isWhiteSquare ? whiteBg : blackBg;

            // Highlight possible moves
            if (isSquareHighlighted(index)) {
                bgColor = greenBg;
            }
            
            // Print the square with its background and piece symbol
            std::cout << bgColor << " " << (board[index] ? board[index]->symbol : " ") << " " << reset;
        }
        std::cout << std::endl; // New line after each row
    }
    // Print column labels
    std::cout << "   a  b  c  d  e  f  g  h" << std::endl;

    std::cout << "Captured White Pieces: ";
    for (Piece* p : capturedWhitePieces) {
        std::cout << p->symbol << " ";
    }
    std::cout << std::endl;
}

// Converts a chess position string (e.g., "e2") to a board index (0-63)
int convertPosToIndex(const std::string& pos) {
    if (pos.length() != 2) {
        return -1; // Invalid format
    }
    int col = pos[0] - 'a'; // 'a' to 'h' -> 0 to 7
    int row = pos[1] - '1'; // '1' to '8' -> 0 to 7

    if (col < 0 || col > 7 || row < 0 || row > 7) {
        return -1; // Out of bounds
    }
    // Adjust row to match 0-based indexing from top (rank 8) to bottom (rank 1)
    // Board is stored from 0 (a8) to 63 (h1)
    return (7 - row) * 8 + col;
}

// Executes a move on the board
void executeMove(int fromIndex, int toIndex) {
    // If there's a piece at the destination, it means a capture
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

// --- Main Game Loop ---
int main() {
    initializeBoard(); // Set up the initial board

    while (true) {
        clearTerminalScreen(); // Clear terminal before printing
        printBoard();          // Display the current board

        // Display current turn
        std::cout << (isWhiteTurn ? "White's turn" : "Black's turn") << ". Enter move (e.g. e2 e4): ";
        std::string fromStr, toStr;
        std::cin >> fromStr >> toStr;

        int fromIndex = convertPosToIndex(fromStr);
        int toIndex = convertPosToIndex(toStr);

        // Input validation
        if (fromIndex == -1 || toIndex == -1) {
            clearTerminalScreen();
            printBoard();
            std::cout << "Invalid input format or coordinates. Please use format like 'e2 e4'.\n";
            std::cin.ignore(); // Clear invalid input
            std::cin.get();    // Wait for user to press enter
            continue;
        }
        
        // Check if there's a piece at the source square
        if (board[fromIndex] == nullptr) {
            clearTerminalScreen();
            printBoard();
            std::cout << "No piece at the source square '" << fromStr << "'.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        // Check if it's the correct turn for the selected piece
        if ((isWhiteTurn && !board[fromIndex]->isWhite) || (!isWhiteTurn && board[fromIndex]->isWhite)) {
            clearTerminalScreen();
            printBoard();
            std::cout << "It's " << (isWhiteTurn ? "White's" : "Black's") << " turn. You selected an opponent's piece.\n";
            std::cin.ignore();
            std::cin.get();
            continue;
        }

        // Get possible moves for the selected piece
        highlightSquares = board[fromIndex]->getPossibleMoves(fromIndex);

        // Check if the destination is a valid move
        bool isValidMove = false;
        // Using std::find for checking if 'toIndex' is in 'highlightSquares'
        if (std::find(highlightSquares.begin(), highlightSquares.end(), toIndex) != highlightSquares.end()) {
            isValidMove = true;
        }

        if (isValidMove) {
            executeMove(fromIndex, toIndex); // Perform the move (now includes capture logic)
            isWhiteTurn = !isWhiteTurn;      // Switch turns
            highlightSquares.clear();        // Clear highlights after move
            // Pause briefly to let the "captured" message be seen before screen clears
            std::cin.ignore();
            std::cout << "Press Enter to continue...";
            std::cin.get(); 
        } else {
            clearTerminalScreen();
            printBoard(); // Re-print board with highlights for clarity
            std::cout << "Invalid move for " << board[fromIndex]->symbol << " from " << fromStr << " to " << toStr << ".\n";
            std::cout << "Possible moves are highlighted in green.\n";
            std::cin.ignore();
            std::cin.get(); // Wait for user to press enter
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