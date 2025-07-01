#include <"piece.h"
#include <algorithm> // For find, if need in future piece logic

// Global board definition (declared extern in Piece.h)
// This is defined here because Piece methods need to access it.
// It's better to pass it as an argument to getPossibleMoves for cleaner design,
// but for now, to keep the current structure, we'll use extern.
// A more robust design would involve passing the board as a const reference to getPossibleMoves.
// For simplicity and to match previous code, we'll keep the extern approach for now.

// Base Piece class implementation

using namespace std;

// Pawn class implementation
Pawn::Pawn(bool isWhite) : Piece(isWhite, isWhite ? "♙" : "♟︎") {}
vector<int> Pawn::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;
    int direction = isWhite ? -1 : 1;

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

// Rook class implementation
Rook::Rook(bool isWhite) : Piece(isWhite, isWhite ? "♖" : "♜") {}
vector<int> Rook::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int r = row + dr[i];
        int c = col + dc[i];
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
            r += dr[i];
            c += dc[i];
        }
    }
    return moves;
}

// Knight class implementation
Knight::Knight(bool isWhite) : Piece(isWhite, isWhite ? "♘" : "♞") {}
vector<int> Knight::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;

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

// Bishop class implementation
Bishop::Bishop(bool isWhite) : Piece(isWhite, isWhite ? "♗" : "♝") {}
vector<int> Bishop::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;

    int dr[] = {-1, -1, 1, 1};
    int dc[] = {-1, 1, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int r = row + dr[i];
        int c = col + dc[i];
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
            r += dr[i];
            c += dc[i];
        }
    }
    return moves;
}

// Queen class implementation
Queen::Queen(bool isWhite) : Piece(isWhite, isWhite ? "♕" : "♛") {}
vector<int> Queen::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;

    // Rook-like moves
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

    // Bishop-like moves
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

// King class implementation
King::King(bool isWhite) : Piece(isWhite, isWhite ? "♔" : "♚") {}
vector<int> King::getPossibleMoves(int index) const {
    vector<int> moves;
    int row = index / 8;
    int col = index % 8;

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