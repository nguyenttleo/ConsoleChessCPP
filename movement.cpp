//
// Created by Leo Nguyen on 4/27/23.
//
#include <iostream>
#include <bitset>
#include "board.cpp"

// init turn counter
int currTurn = 0;

// upates piece counters for all pieces
void updatePieceCount() {
    numBPawns = blackPawn->count();
    numWPawns = whitePawn->count();
    numBKnights = blackKnight->count();
    numWKnights = whiteKnight->count();
    numBBishops = blackBishop->count();
    numWBishops = whiteBishop->count();
    numBRooks = blackRook->count();
    numWRooks = whiteRook->count();
    numBQueen = blackQueen->count();
    numWQueen = whiteQueen->count();
}

// executes piece capture by flipping bit in bitset of takenPiece and setting index of takenPiece to true in currBitset
std::bitset<64> capturePiece(std::bitset<64> currBitset, int takenPiece, int currIndex, int newIndex) {
    switch (takenPiece) {
        case (PawnVal):
            whitePawn->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (PawnVal + BLACK):
            blackPawn->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (KnightVal):
            whiteKnight->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (KnightVal + BLACK):
            blackKnight->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (BishopVal):
            whiteBishop->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (BishopVal + BLACK):
            blackBishop->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (RookVal):
            whiteRook->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (RookVal + BLACK):
            blackRook->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (QueenVal):
            whiteQueen->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        case (QueenVal + BLACK):
            blackQueen->set(newIndex, false);
            updatePieceCount();
            return movePiece(&currBitset, currIndex, newIndex);
        default:
            return currBitset;
    }
}

/* handles all pawn movement: captures, checks, promotions, en passant, legality
(includes optional promotionChoice parameter, if not included, will default
to queen value) */
std::bitset<64> movePawn(int color, int currIndex, int newIndex, int promotionChoice = 90) {
    if (newIndex > 63 || newIndex < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    if (color == WHITE) {
        // Moving straight forward 1
        if (newIndex == currIndex + 8) {
            if (checkAllBoards(newIndex) == 0) {
                if (newIndex < 64 && newIndex > 55) {
                    switch (promotionChoice) {
                        case KnightVal:
                            wKnight = whiteKnight->set(newIndex, true);
                        case BishopVal:
                            wBishop = whiteBishop->set(newIndex, true);
                        case RookVal:
                            wRook = whiteRook->set(newIndex, true);
                        case QueenVal:
                            wQueen = whiteQueen->set(newIndex, true);
                    }
                    wPawn = whitePawn->set(currIndex, false);
                    return wPawn;
                } else {
                    return wPawn = moveForward(wPawn, currIndex, 1);
                }
            } else if (((bigNumba) checkAllBoards(newIndex))[0] == 0){
                throw std::invalid_argument("Square is occupied by piece of same color.");
            }
        // Moving straight forward 2 only if pawn has not moved
        } else if (newIndex == currIndex + 16 && currIndex < 16 && currIndex > 7) {
            if (checkAllBoards(newIndex) == 0 && checkAllBoards(newIndex - 8) == 0) {
                return wPawn = moveForward(wPawn, currIndex, 2);
            }
        // Moving forward both diagonals 9 = forward left & 7 = forward right
        } else if (newIndex ==  currIndex + 9 || newIndex == currIndex + 7) {
            if (checkAllBoards(newIndex) != 0) {
                wPawn = capturePiece(wPawn, checkAllBoards(newIndex), currIndex, newIndex);
                if (newIndex < 64 && newIndex > 55) {
                    switch (promotionChoice) {
                        case KnightVal:
                            wKnight = whiteKnight->set(newIndex, true);
                        case BishopVal:
                            wBishop = whiteBishop->set(newIndex, true);
                        case RookVal:
                            wRook = whiteRook->set(newIndex, true);
                        case QueenVal:
                            wQueen = whiteQueen->set(newIndex, true);
                    }
                    wPawn = whitePawn->set(currIndex, false);
                    return wPawn;
                }
                // REMEMBER TO UPDATE LENGTH OF EACH BITBOARD AFTER A CAPTURE TO SEE WHAT WAS CAPTURED
            } else if (checkAllBoards(newIndex == 0)) {
                if (((checkAllBoards(currIndex + 1) == PawnVal + BLACK) || (checkAllBoards(currIndex - 1) == PawnVal + BLACK))&& newIndex == currIndex + 7) {
                    blackPawn->set(currIndex + 1, false);
                    return wPawn = movePiece(&wPawn, currIndex, newIndex);
                } else if (checkAllBoards(currIndex - 1) == PawnVal + BLACK && newIndex == currIndex + 9) {
                    blackPawn->set(currIndex - 1, false);
                    return wPawn = movePiece(&wPawn, currIndex, newIndex);
                }
            }
        }
        return wPawn;
    } else if (color == BLACK) {
        // Moving straight forward 1
        if (newIndex == currIndex - 8) {
            if (checkAllBoards(newIndex) == 0) {
                if (newIndex < 8 && newIndex > 0) {
                    switch (promotionChoice) {
                        case KnightVal:
                            bKnight = blackKnight->set(newIndex, true);
                        case BishopVal:
                            bBishop = blackBishop->set(newIndex, true);
                        case RookVal:
                            bRook = blackRook->set(newIndex, true);
                        case QueenVal:
                            bQueen = blackQueen->set(newIndex, true);
                    }
                    return bPawn = blackPawn->set(currIndex, false);;
                } else {
                    return bPawn = moveBack(bPawn, currIndex, 1);
                }
            } else if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                throw std::invalid_argument("Square is occupied by piece of same color.");
            }
            // Moving straight forward 2 only if pawn has not moved
        } else if (newIndex == currIndex - 16 && currIndex < 56 && currIndex > 47) {
            if (checkAllBoards(newIndex) == 0 && checkAllBoards(newIndex - 8) == 0) {
                return bPawn = moveBack(bPawn, currIndex, 2);
            }
            // Moving forward both diagonals 9 = forward left & 7 = forward right
        } else if (newIndex ==  currIndex - 9 || newIndex == currIndex - 7) {
            if (checkAllBoards(newIndex) != 0) {
                bPawn = capturePiece(bPawn, checkAllBoards(newIndex), currIndex, newIndex);
                if (newIndex < 8 && newIndex > 0) {
                    switch (promotionChoice) {
                        case KnightVal:
                            bKnight = blackKnight->set(newIndex, true);
                        case BishopVal:
                            bBishop = blackBishop->set(newIndex, true);
                        case RookVal:
                            bRook = blackRook->set(newIndex, true);
                        case QueenVal:
                            bQueen = blackQueen->set(newIndex, true);
                    }
                    return bPawn = blackPawn->set(newIndex, false);;
                }
                // REMEMBER TO UPDATE LENGTH OF EACH BITBOARD AFTER A CAPTURE TO SEE WHAT WAS CAPTURED
            } else if (checkAllBoards(newIndex == 0)) {
                if (checkAllBoards(currIndex + 1) == PawnVal && newIndex == currIndex - 9) {
                    whitePawn->set(currIndex + 1, false);
                    return bPawn = movePiece(&bPawn, currIndex, newIndex);
                } else if (checkAllBoards(currIndex - 1) == PawnVal && newIndex == currIndex - 7) {
                    whitePawn->set(currIndex - 1, false);
                    return bPawn = movePiece(&bPawn, currIndex, newIndex);
                }
            }
        }
        return bPawn;
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

/* handles all knight movement: captures, legality*/
std::bitset<64> moveKnight(int color, int currIndex, int newIndex) {
    if (knightMask[currIndex][newIndex]) {
        if (color == WHITE) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 0 && checkAllBoards(newIndex) != 0) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return wKnight = capturePiece(wKnight, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return wKnight = movePiece(&wKnight, currIndex, newIndex);
        }  else if (color == BLACK) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return bKnight = capturePiece(bKnight, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return bKnight = movePiece(&bKnight, currIndex, newIndex);
        }
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

/* handles all bishop movement: captures, legality*/
std::bitset<64> moveBishop(int color, int currIndex, int newIndex) {
    if (newIndex > 63 || newIndex < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    if (color == WHITE) {
        if (diagonalMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 0) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return wBishop = capturePiece(wBishop, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return wBishop = movePiece(&wBishop, currIndex, newIndex);
        }
    } else if (color == BLACK) {
        if (diagonalMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return bBishop = capturePiece(bBishop, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return bBishop = movePiece(&bBishop, currIndex, newIndex);
        }
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

/* handles all rook movement: captures, legality*/
std::bitset<64> moveRook(int color, int currIndex, int newIndex) {
    if (newIndex > 63 || newIndex < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    if (color == WHITE) {
        if (straightMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 0) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return wRook = capturePiece(wRook, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return wRook = movePiece(&wRook, currIndex, newIndex);
        }
    } else if (color == BLACK) {
        if (straightMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return bRook = capturePiece(bRook, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return bRook = movePiece(&bRook, currIndex, newIndex);
        }
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

/* handles all queen movement: captures, legality*/
std::bitset<64> moveQueen(int color, int currIndex, int newIndex) {
    if (newIndex > 63 || newIndex < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    if (color == WHITE) {
        if (straightMasks[currIndex][newIndex] | straightMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 0) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return wQueen = capturePiece(wQueen, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return wQueen = movePiece(&wQueen, currIndex, newIndex);
        }
    } else if (color == BLACK) {
        if (straightMasks[currIndex][newIndex] | straightMasks[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return bQueen = capturePiece(bQueen, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return bQueen = movePiece(&bQueen, currIndex, newIndex);
        }
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

/* handles all king movement: captures, legality, checks, checkmate */
std::bitset<64> moveKing(int color, int currIndex, int newIndex) {
    if (newIndex > 63 || newIndex < 0) {
        throw std::out_of_range("Argument out of bounds.");
    }

    if (color == WHITE) {
        if (kingMask[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 0) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return wKing = capturePiece(wKing, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return wKing = movePiece(&wKing, currIndex, newIndex);
        }
    } else if (color == BLACK) {
        if (kingMask[currIndex][newIndex]) {
            if (checkAllBoards(newIndex)) {
                if (((bigNumba) checkAllBoards(newIndex))[0] == 1) {
                    throw std::invalid_argument("Square is occupied by piece of same color.");
                }
                return bKing = capturePiece(bKing, checkAllBoards(newIndex), currIndex, newIndex);
            }
            return bKing = movePiece(&bKing, currIndex, newIndex);
        }
    }
    throw std::invalid_argument("Method call completed without valid arguments.");
}

bool ifCheck(int color) {
    if (color == WHITE) {
        int kingIndex = 0;
        while (wKing != 1) {
            wKing >> 1;
            kingIndex++;
        }
        if (detectCheck(diagonalMasks(kingIndex)))
    }
}

int main() {
    initBoards();
    genKnightBitmask();
    genDiagonalBitmask();
    genStraightBitmask();
    genKingBitmask();
//	std::cout << "black pawn structure is: " << *blackPawn << std::endl;
//	std::cout << "black queen structure is: " << *blackQueen << std::endl;
//	std::cout << "black king structure is: " << *blackKing << std::endl;
//	std::cout << "black rook structure is: " << *blackRook << std::endl;
//	std::cout << "black bishop structure is: " << *blackBishop << std::endl;
//	std::cout << "black knight structure is: " << *blackKnight << std::endl;
//	std::cout << "black pawn structure is: " << *blackPawn << std::endl;
//	std::cout << "white pawn structure is: " << *whitePawn << std::endl;
//	std::cout << "white knight structure is: " << *whiteKnight << std::endl;
//	std::cout << "white bishop structure is: " << *whiteBishop << std::endl;
//	std::cout << "white rook structure is: " << *whiteRook << std::endl;
//	std::cout << "white king structure is: " << *whiteKing << std::endl;
//	std::cout << "white queen structure is: " << *whiteQueen << std::endl;
//    std::cout << *whiteQueen << std::endl;
//    std::cout << moveForwardLeft(*whiteQueen, 1, 1) << std::endl;
//    std::cout << getRightBit(whitePawn, 12, 3) << std::endl;
//    whitePawn->flip(0);
//    std::cout << printBitboard(whitePawn);
//    *whitePawn = moveForwardLeft(*whitePawn, 10, 1, 1);
//    std::cout << printBitboard(whitePawn);
//    std::cout << getBit(whitePawn, 65);
//    std::cout << bRookVal << "  " << wRookVal << std::endl;
    movePawn(BLACK, 55, 39);
    movePawn(BLACK, 39, 31);
    movePawn(BLACK, 31, 23);
    movePawn(BLACK, 23, 14);
    movePawn(BLACK, 14, 5, 32);
    std::cout << printBitboard(&bPawn) << std::endl;
    std::cout << printBitboard(&wPawn) << std::endl;
    std::cout << printBitboard(&wKnight) << std::endl;
    std::cout << printBitboard(&bBishop) << std::endl;
//    std::cout << printBitboard(&verticalMask[4]) << std::endl;
//    std::cout << verticalMask[4][26];
//    for (int i = 0; i < 15; i++) {
//        std::cout << printBitboard(&verticalMask[i]) << std::endl;
//    }

}