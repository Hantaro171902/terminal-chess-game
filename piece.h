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