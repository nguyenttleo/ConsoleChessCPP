//
// Created by Leo Nguyen on 4/22/23.
//

#include "board.hpp"
typedef std::bitset<64> bigNumba;

// size 64 bitset representations of all piece boards
bigNumba bPawn;
bigNumba wPawn;
bigNumba bKnight;
bigNumba wKnight;
bigNumba bRook;
bigNumba wRook;
bigNumba bBishop;
bigNumba wBishop;
bigNumba bQueen;
bigNumba wQueen;
bigNumba bKing;
bigNumba wKing;

// pointers to all piece boards
bigNumba *blackPawn;
bigNumba *whitePawn;
bigNumba *blackKnight;
bigNumba *whiteKnight;
bigNumba *blackRook;
bigNumba *whiteRook;
bigNumba *whiteBishop;
bigNumba *blackBishop;
bigNumba *blackQueen;
bigNumba *blackKing;
bigNumba *whiteKing;
bigNumba *whiteQueen;

// bitmasks to determine possible and legal moves
bigNumba whitePawnAttackMask[64];
bigNumba blackPawnAttackMask[64];
bigNumba knightMask[64];
bigNumba diagonalMasks[64];
bigNumba straightMasks[64];
bigNumba queenMasks[64];
bigNumba kingMask[64];

// automatically generated bitmasks for a given index
bigNumba verticalMask[15];
bigNumba horizontalMask[15];

// piece counters
int numBPawns;
int numWPawns;
int numBKnights;
int numWKnights;
int numBBishops;
int numWBishops;
int numBRooks;
int numWRooks;
int numBQueen;
int numWQueen;

// array of possible pieces giving check
int checkingPieces[2];

//std::bitset<64> doublePawnMoves = 0ULL;
//std::bitset<64>* dPawnMoves = &doublePawnMoves;

// create abitrary values to track pieces without having to use strings: black pieces are odd, white even
enum {
    WHITE = 0,
    BLACK = 1,
    PawnVal = 10,
    KnightVal = 30,
    BishopVal = 32,
    RookVal = 50,
    QueenVal = 90,
    KingVal= 900
};

// returns the number of "on" bits in a given bitset: used to update piece counters
int getCount (std::bitset<64>* currBitset) {
    return (int) currBitset->count();
}

// generates a bitmask of all possible pawn moves: not excluding illegal moves (legality is determined in movePawn())
void genPawnAttackMasks() {
    for (int i = 0; i < 64; i++) {
        if (i + 1 % 8 != 0) {whitePawnAttackMask[i] |= (1Ull << (i + 7)); blackPawnAttackMask[i] |= (1Ull << (i - 9));}
        if (i % 8 != 0) {whitePawnAttackMask[i] |= (1ULL << (i + 9)); blackPawnAttackMask[i] |= (1Ull << (i - 7));}
    }
}

// generates a bitmask of all possible knight moves: not excluding illegal moves (legality is determined in moveKnight())
void genKnightBitmask() {
    for (int i = 0; i < 64; ++i) {
        if (i + 6 < 64) {knightMask[i] |= (1Ull << (i + 6));}
        if (i - 6 >= 0) {knightMask[i] |= ((1Ull << (i - 6)));}
        if (i + 10 < 64) {knightMask[i] |= ((1Ull << (i + 10)));}
        if (i - 10 >= 0) {knightMask[i] |= ((1Ull << (i - 10)));}
        if (i + 15 < 64) {knightMask[i] |= ((1Ull << (i + 15)));}
        if (i - 15 >= 0) {knightMask[i] |= ((1Ull << (i - 15)));}
        if (i + 17 < 64) {knightMask[i] |= ((1Ull << (i + 17)));}
        if (i - 17 >= 0) {knightMask[i] |= ((1Ull << (i - 17)));}

        if (i % 8 == 0) {
            knightMask[i] &= verticalMask[10];
        } else if (i % 8 == 1) {
            knightMask[i] &= verticalMask[11];
        } else if ((i - 7) % 8 == 0) {
            knightMask[i] &= verticalMask[2];
        } else if ((i - 7) % 8 == 7) {
            knightMask[i] &= verticalMask[3];
        } else {
            knightMask[i] &= 0x1F1F1F1F1F1F1F1FULL << ((i % 8) - 2);
        }
    }
}

// generates diagonalMasks (array of all diagonal bits from given index)
void genDiagonalBitmask() {
    for (int i = 0; i < 64; ++i) {
        for (int j = i + 9; j < 64 && j % 8 != 0; j += 9) {
            diagonalMasks[i] |= (1ULL << j);
        }
        for (int j = i + 7; j < 64 && j % 8 != 7; j += 7) {
            diagonalMasks[i] |= (1ULL << j);
        }
        for (int j = i - 9; j >= 0 && j % 8 != 7; j -= 9) {
            diagonalMasks[i] |= (1ULL << j);
        }
        for (int j = i - 7; j >= 0 && j % 8 != 0; j -= 7) {
            diagonalMasks[i] |= (1ULL << j);
        }
    }
}

// generates straightMasks (array of all straight bits from given index)
void genStraightBitmask() {
    for (int i = 0; i < 64; ++i) {
        for (int j = i + 1; j < 64 && (j & 7) != 0; ++j) {
            straightMasks[i] |= (1ULL << j);
        }
        for (int j = i + 8; j < 64; j += 8) {
            straightMasks[i] |= (1ULL << j);
        }
        for (int j = i - 1; j >= 0 && (j & 7) != 7; --j) {
            straightMasks[i] |= (1ULL << j);
        }
        for (int j = i - 8; j >= 0; j -= 8) {
            straightMasks[i] |= (1ULL << j);
        }
    }
}

// generates queenMasks: array of all possible queen moves from given index (not excluding illegal moves)
void genQueenBitmask() {
    for (int i = 0; i < 64; i++) {
        queenMasks[i] = straightMasks[i] | diagonalMasks[i];
    }
}

// generates kingMask (array of all bits directly neighboring bit at index)
void genKingBitmask() {
    for (int i = 0; i < 64; ++i) {
        if (((i + 1) & 7) != 0 && (i + 1) < 64) {
            kingMask[i] |= (1ULL << (i + 1));
        }
        if ((i + 8) < 64) {
            kingMask[i] |= (1ULL << (i + 8));
        }
        if (((i - 1) & 7) != 7 && (i - 1) >= 0) {
            kingMask[i] |= (1ULL << (i - 1));
        }
        if ((i - 8) >= 0) {
            kingMask[i] |= (1ULL << (i - 8));
        }
        if ((i + 9) % 8 != 0 && (i + 9) < 64) {
            kingMask[i] |= (1ULL << (i + 9));
        }
        if ((i + 7) % 8 != 7 && (i + 7)  < 64) {
            kingMask[i] |= (1ULL << (i + 7));
        }
        if ((i - 9) % 8 != 7 && (i - 9) >= 0) {
            kingMask[i] |= (1ULL << (i - 9));
        }
        if ((i - 7) % 8 != 0 && (i - 7) >= 0) {
            kingMask[i] |= (1ULL << (i - 7));
        }
    }
}

// initialize above variables: starting board representations, default bitmasks, piece counters
void initBoards() {

    // default starting piece boards
    bPawn = 0xFF000000000000ULL;
    wPawn = 0xFF00ULL;
    bKnight = 0x2400000000000000ULL;
    wKnight = 0x24ULL;
    bRook = 0x8100000000000000ULL;
    wRook = 0x81ULL;
    bBishop = 0x4200000000000000ULL;
    wBishop = 0x42ULL;
    bQueen = 0x1000000000000000ULL;
    wQueen = 0x10ULL;
    bKing = 0x800000000000000ULL;
    wKing = 0x8ULL;

    // pointers to above baords
    blackPawn = &bPawn;
    whitePawn = &wPawn;
    blackKnight = &bKnight;
    whiteKnight = &wKnight;
    blackRook = &bRook;
    whiteRook = &wRook;
    whiteBishop = &wBishop;
    blackBishop = &bBishop;
    blackQueen = &bQueen;
    blackKing = &bKing;
    whiteKing = &wKing;
    whiteQueen = &wQueen;

// 7 masks starting from left then full board then 7 from right
    verticalMask[0] = 0x8080808080808080ULL;
    verticalMask[1] = 0xC0C0C0C0C0C0C0C0ULL;
    verticalMask[2] = 0xE0E0E0E0E0E0E0E0ULL;
    verticalMask[3] = 0xF0F0F0F0F0F0F0F0ULL;
    verticalMask[4] = 0xF8F8F8F8F8F8F8F8ULL;
    verticalMask[5] = 0xFCFCFCFCFCFCFCFCULL;
    verticalMask[6] = 0xFEFEFEFEFEFEFEFEULL;
    verticalMask[7] = 0xFFFFFFFFFFFFFFFFULL;
    verticalMask[8] = 0x8080808080808080ULL >> 7;
    verticalMask[9] = 0xC0C0C0C0C0C0C0C0ULL >> 6;
    verticalMask[10] = 0xE0E0E0E0E0E0E0E0ULL >> 5;
    verticalMask[11] = 0xF0F0F0F0F0F0F0F0ULL >> 4;
    verticalMask[12] = 0xF8F8F8F8F8F8F8F8ULL >> 3;
    verticalMask[13] = 0xFCFCFCFCFCFCFCFCULL >> 2;
    verticalMask[14] = 0xFEFEFEFEFEFEFEFEULL >> 1;

// 7 masks starting from top down then full board then 7 from bottom
    horizontalMask[0] = 0xFF00000000000000ULL;
    horizontalMask[1] = 0xFFFF000000000000ULL;
    horizontalMask[2] = 0xFFFFFF0000000000ULL;
    horizontalMask[3] = 0xFFFFFFFF00000000ULL;
    horizontalMask[4] = 0xFFFFFFFFFF000000ULL;
    horizontalMask[5] = 0xFFFFFFFFFFFF0000ULL;
    horizontalMask[6] = 0xFFFFFFFFFFFFFF00ULL;
    horizontalMask[7] = 0xFFFFFFFFFFFFFFFFULL;
    horizontalMask[8] = 0xFF00000000000000ULL >> 56;
    horizontalMask[9] = 0xFFFF000000000000ULL >> 48;
    horizontalMask[10] = 0xFFFFFF0000000000ULL >> 40;
    horizontalMask[11] = 0xFFFFFFFF00000000ULL >> 32;
    horizontalMask[12] = 0xFFFFFFFFFF000000ULL >> 24;
    horizontalMask[13] = 0xFFFFFFFFFFFF0000ULL >> 16;
    horizontalMask[14] = 0xFFFFFFFFFFFFFF00ULL >> 8;

    // default starting piece counts
    numBPawns = 8;
    numWPawns = 8;
    numBKnights = 2;
    numWKnights = 2;
    numBBishops = 2;
    numWBishops = 2;
    numBRooks = 2;
    numWRooks = 2;
    numBQueen = 1;
    numWQueen = 1;

    // generate bitmasks
    genKnightBitmask();
    genDiagonalBitmask();
    genStraightBitmask();
    genKingBitmask();
}


// elementary piece move function: moves bit in currBitset from currIndex to newIndex
std::bitset<64> movePiece(std::bitset<64>* currBitset, int currIndex, int newIndex) {
//    if (currBitset == 0 || currIndex > 63 || currIndex < 0 || newIndex > 63 || newIndex < 0) {
//        throw std::invalid_argument("Invalid argument.");
//    }
    try {
        currBitset->set(currIndex, false);
        currBitset->set(newIndex, true);
        return *currBitset;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid argument." << std::endl;
        return *currBitset;
    } catch (std::out_of_range&) {
        std::cout << "Argument out of bounds." << std::endl;
        return *currBitset;
    }
}

// shifts bit in currBitset at currIndex forward by steps steps
std::bitset<64> moveForward(std::bitset<64> currBitset, int currIndex, int steps) {
    return movePiece(&currBitset, currIndex, currIndex + (8*steps));
}
// shifts bit in currBitset at currIndex backwards by steps steps
std::bitset<64> moveBack(std::bitset<64> currBitset, int currIndex, int steps) {
    return movePiece(&currBitset, currIndex, currIndex - (8*steps));
}
// shifts bit in currBitset at currIndex right by steps steps
std::bitset<64> moveRight(std::bitset<64> currBitset, int currIndex, int steps) {
    return movePiece(&currBitset, currIndex, currIndex - steps);
}
// shifts bit in currBitset at currIndex left by steps steps
std::bitset<64> moveLeft(std::bitset<64> currBitset, int currIndex, int steps) {
    return movePiece(&currBitset, currIndex, currIndex + steps);
}

// To implement piece movement use bitset.set(index of bit, boolean value to set to)
// Single bits can be flipped using bitset.flip(index of bit)

// shifts bit in currBitset at currIndex forward by forwardSteps steps and left by leftSteps
std::bitset<64> moveForwardLeft(std::bitset<64> currBitset, int currIndex, int forwardSteps, int leftSteps) {
	return movePiece(&currBitset, currIndex, (currIndex + (8*forwardSteps) + leftSteps));
}
// shifts bit in currBitset at currIndex forward by forwardSteps steps and right by rightSteps
std::bitset<64> moveForwardRight(std::bitset<64> currBitset, int currIndex, int forwardSteps, int rightSteps) {
    return movePiece(&currBitset, currIndex, (currIndex + (8*forwardSteps) - rightSteps));
}
// shifts bit in currBitset at currIndex backwards by backSteps steps and left by leftSteps
std::bitset<64> moveBackLeft(std::bitset<64> currBitset, int currIndex, int backSteps, int leftSteps) {
    return movePiece(&currBitset, currIndex, (currIndex - (8*backSteps) + leftSteps));
}
// shifts bit in currBitset at currIndex backwards by backSteps steps and right by rightSteps
std::bitset<64> moveBackRight(std::bitset<64> currBitset, int currIndex, int backSteps, int rightSteps) {
    return movePiece(&currBitset, currIndex, (currIndex - (8*backSteps) - rightSteps));
}

// return printable representation of given bitboard by separating into 8 divisions of 8 chars with a newline char between them
std::string printBitboard(std::bitset<64>* currBitset) {
    std::string cout;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout += currBitset->to_string().substr(i * 8 + j, 1) + "  ";
        }
        cout += "\n";
    }
    return "\n" + cout + "\n";
}

// bitshift a << b (a is pushed left b spaces)
// bitshift a >> b (a is pushed right b spaces)

// checks all piece boards for bit at given index
int checkAllBoards(int index) {
    if (bPawn[index]) {return BLACK + PawnVal;}
    if (wPawn[index]) {return PawnVal;}
    if (bKnight[index]) {return BLACK + KnightVal;}
    if (wKnight[index]) {return KnightVal;}
    if (bBishop[index]) {return BLACK + BishopVal;}
    if (wBishop[index]) {return BishopVal;}
    if (bRook[index]) {return BLACK + RookVal;}
    if (wRook[index]) {return RookVal;}
    if (bQueen[index]) {return BLACK + QueenVal;}
    if (wQueen[index]) {return QueenVal;}
    if (bKing[index]) {return BLACK + KingVal;}
    if (wKing[index]) {return KingVal;}

    return 0;
}

/* determines whether king of color "color" at referenceIndex is in check and 
puts checking pieces in checkingPieces array. if no check, returns nullptr, if 
check, returns pointer to first element in checkingPieces[]*/
int* detectCheck(int color, int referenceIndex) {
    int checkIdx = 0;
    if (color == WHITE) {
        for (int i = 0; i < 63; i++) {
            switch(checkAllBoards(referenceIndex)) {
                case (PawnVal + BLACK):
                    if (blackPawnAttackMask[i][referenceIndex] && wKing[referenceIndex]) {
                        checkingPieces[checkIdx] = PawnVal + BLACK;
                        checkIdx++;
                    }
                case (KnightVal + BLACK):
                    if (knightMask[i][referenceIndex] && wKing[referenceIndex]) {
                        checkingPieces[checkIdx] = KnightVal + BLACK;
                        checkIdx++;
                    }
                case (BishopVal + BLACK):
                    if (diagonalMasks[i][referenceIndex] && wKing[referenceIndex]) {
                        checkingPieces[checkIdx] = BishopVal + BLACK;
                        checkIdx++;
                    }
                case (RookVal + BLACK):
                    if (straightMasks[i][referenceIndex] && wKing[referenceIndex]) {
                        checkingPieces[checkIdx] = RookVal + BLACK;
                        checkIdx++;
                    }
                case (QueenVal + BLACK):
                    if (queenMasks[i][referenceIndex] && wKing[referenceIndex]) {
                        checkingPieces[checkIdx] = QueenVal + BLACK;
                        checkIdx++;
                    }
            }
        }
    } else if (color == BLACK) {
        for (int i = 0; i < 63; i++) {
            switch(checkAllBoards(referenceIndex)) {
                case (PawnVal):
                    if (blackPawnAttackMask[i][referenceIndex] && bKing[referenceIndex]) {
                        checkingPieces[checkIdx] = PawnVal;
                        checkIdx++;
                    }
                case (KnightVal):
                    if (knightMask[i][referenceIndex] && bKing[referenceIndex]) {
                        checkingPieces[checkIdx] = KnightVal;
                        checkIdx++;
                    }
                case (BishopVal):
                    if (diagonalMasks[i][referenceIndex] && bKing[referenceIndex]) {
                        checkingPieces[checkIdx] = BishopVal;
                        checkIdx++;
                    }
                case (RookVal):
                    if (straightMasks[i][referenceIndex] && bKing[referenceIndex]) {
                        checkingPieces[checkIdx] = RookVal;
                        checkIdx++;
                    }
                case (QueenVal):
                    if (queenMasks[i][referenceIndex] && bKing[referenceIndex]) {
                        checkingPieces[checkIdx] = QueenVal;
                        checkIdx++;
                    }
            }
        }
    }
    if (checkIdx == 0) {
        return nullptr;
    } else {
        return &checkingPieces[0];
    }
}

// Returns the bit at the given index in currBitset
bool getBit(std::bitset<64>* currBitset, int index) {
    if (index < 0 || index > 63) {
        throw std::out_of_range("Argument out of bounds.");
    }
    return (*currBitset)[index];
}

// Returns the bit above the current bit given by index in currBitset
bool getForwardBit(std::bitset<64>* currBitset, int index, int step) {
    if ((index + 8 * step) >= 64 || (index + 8 * step) < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return (*currBitset)[index + (step*8)];
}

// Returns bit forward of bit at index by "steps" steps
int getFwdBitIdx(int index, int step) {return index + 8*step;}

// Returns bit backwards of bit at index by "steps" steps
bool getBackBit(std::bitset<64>* currBitset, int index, int step) {
    if ((index - 8 * step) >= 64 || (index - 8 * step) < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return ((*currBitset)[index - 8*step]);
}

// Returns index of bit backwards of bit at index by "steps" steps
int getBackBitIdx(int index, int step) {return index - 8*step;}

// Returns the bit to the left of the current bit given by index by "steps" steps
bool getLeftBit(std::bitset<64>* currBitset, int index, int step) {
    if ((step + (index % 8)) >= 8 || (step + index) > 64) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return ((*currBitset)[index + step]);
}

// Returns index of bit left of bit at index by "steps" steps
int getLeftBitIdx(int index, int step) {
    if ((step + (index % 8)) >= 8 || (step + index) > 64) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return index + step;
}

// Returns the bit to the right of the current bit given by index by "steps" steps
bool getRightBit(std::bitset<64>* currBitset, int index, int step) {
    if (((index % 8) - step) < 0|| (index - step) < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return ((*currBitset)[index - step]);
}

// Returns index of bit right of bit at index by "steps" steps
int getRightBitIdx(int index, int step) {
    if (((index % 8) - step) < 0|| (index - step) < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    return index - step;
}