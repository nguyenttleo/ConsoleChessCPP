//
// Created by Leo Nguyen on 5/4/23.
//
#include <iostream>
#include <bitset>
#include <string>
#include <stdexcept>

#ifndef HENGINE_BOARD_HPP
#define HENGINE_BOARD_HPP

#endif //HENGINE_BOARD_HPP

typedef std::bitset<64> bigNumba;

extern bigNumba bPawn;
extern bigNumba wPawn;
extern bigNumba bKnight;
extern bigNumba wKnight;
extern bigNumba bRook;
extern bigNumba wRook;
extern bigNumba bBishop;
extern bigNumba wBishop;
extern bigNumba bQueen;
extern bigNumba wQueen;
extern bigNumba bKing;
extern bigNumba wKing;

extern bigNumba verticalMask[15];
extern bigNumba horizontalMask[15];

extern bigNumba knightMask[64];
extern bigNumba diagonalMasks[64];
extern bigNumba straightMasks[64];
extern bigNumba kingMask[64];

extern bigNumba *blackPawn;
extern bigNumba *whitePawn;
extern bigNumba *blackKnight;
extern bigNumba *whiteKnight;
extern bigNumba *blackRook;
extern bigNumba *whiteRook;
extern bigNumba *whiteBishop;
extern bigNumba *blackBishop;
extern bigNumba *blackQueen;
extern bigNumba *blackKing;
extern bigNumba *whiteKing;
extern bigNumba *whiteQueen;

extern int numBPawns;
extern int numWPawns;
extern int numBKnights;
extern int numWKnights;
extern int numBBishops;
extern int numWBishops;
extern int numBRooks;
extern int numWRooks;
extern int numBQueen;
extern int numWQueen;

extern int promotionChoice;

extern int checkingPieces[2];

void initBoards();

int getCount (std::bitset<64>* currBitset);

void genPawnAttackMasks();
void genKnightBitmask();
void genDiagonalBitmask();
void genStraightBitmask();
void genQueenBitmask();
void genKingBitmask();

std::bitset<64> movePiece(std::bitset<64>* currBitset, int currIndex, int newIndex);
std::bitset<64> moveForward(std::bitset<64> currBitset, int currIndex, int steps);
std::bitset<64> moveBack(std::bitset<64> currBitset, int currIndex, int steps);
std::bitset<64> moveRight(std::bitset<64> currBitset, int currIndex, int steps);
std::bitset<64> moveLeft(std::bitset<64> currBitset, int currIndex, int steps);
std::bitset<64> moveForwardLeft(std::bitset<64> currBitset, int currIndex, int forwardSteps, int leftSteps);
std::bitset<64> moveForwardRight(std::bitset<64> currBitset, int currIndex, int forwardSteps, int rightSteps);
std::bitset<64> moveBackLeft(std::bitset<64> currBitset, int currIndex, int backSteps, int leftSteps);
std::bitset<64> moveBackRight(std::bitset<64> currBitset, int currIndex, int backSteps, int rightSteps);

std::string printBitboard(std::bitset<64>* currBitset);

int checkAllBoards(int index);
int* detectCheck(int color, int referenceIndex);

bool getBit(std::bitset<64>* currBitset, int index);
bool getForwardBit(std::bitset<64>* currBitset, int index, int step);
bool getBackBit(std::bitset<64>* currBitset, int index, int step);
bool getLeftBit(std::bitset<64>* currBitset, int index, int step);
bool getRightBit(std::bitset<64>* currBitset, int index, int step);

int getFwdBitIdx(int index, int step);
int getBackBitIdx(int index, int step);
int getLeftBitIdx(int index, int step);
int getRightBitIdx(int index, int step);
