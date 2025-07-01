#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Forward declaration of the board for Piece's getPossibleMoves
// This is necessary because Piece methods need to know about the board structure
// but we want to avoid circular dependencies by including GameLogic.h here.
// The actual board definition will be in GameLogic.cpp.

extern vector<class Piece*> board;  // Declare board as an external variable

class Piece {
public:
    bool isWhite;  
    string symbol; 

    Piece(bool isWhite, string symbol);

    // getPossibleMoves now takes the current board state as an argument
    virtual vector<int> getPossibleMoves(int index) const = 0;
    virtual ~Piece() {}
};

// Pawn class
class Pawn : public Piece {
public:
    Pawn(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
};

// Rook class
class Rook : public Piece {
public:
    Rook(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
};

// Knight class
class Knight : public Piece {
public:
    Knight(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
}; 

// Bishop class
class Bishop : public Piece {
public:
    Bishop(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
};

// Queen class
class Queen : public Piece {
public:
    Queen(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
};

// King class
class King : public Piece {
public:
    King(bool isWhite);
    vector<int> getPossibleMoves(int index) const override;
};

#endif // PIECE_H