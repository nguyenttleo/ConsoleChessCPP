#include <iostream>

enum pieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY};
enum Color { WHITE, BLACK, NONE};
enum posConversion { A = 1,
			  B = 2,
			  C = 3,
			  D = 4,
			  E = 5,
			  F = 6,
			  G = 7,
			  H = 8};

class Piece {
	pieceType type;
	Color color;
	int value;
	bool firstMove;
public:
	Piece(pieceType inputType, Color inputColor) {
		this->type = inputType;
		this->color = inputColor;
		this->firstMove = true;

		switch(this->type) {
			case pieceType::PAWN:
				this->value = 1;
				break;
			case pieceType::KNIGHT:
				this->value = 3;
				break;
			case pieceType::BISHOP:
				this->value = 3;
				break;
			case pieceType::ROOK:
				this->value = 5;
				break;
			case pieceType::QUEEN:
				this->value = 9;
				break;
			default:
				break;
		}
	}

	Piece(pieceType inputType) {
		this->type = inputType;
	}
}

struct Square {
	Piece piece;
	Color color;
	int x, y;

public:
	Square* getUp(Square* currSquare) {
		if ((currSquare->y - 1) < 0) {
			return currSquare;
		}
		return &board[y-1][x];
	}

	Square* getDown(Square* currSquare) {
		if ((currSquare->y + 1) > 7) {
			return currSquare;
		}
		return &board[y+1][x];
	}

	Square* getLeft(Square* currSquare) {
		if ((currSquare->x - 1) < 0) {
			return currSquare;
		}
		return &board[y][x-1];
	}

	Square* getRight(Square* currSquare) {
		if ((currSquare->x + 1) > 7) {
			return currSquare;
		}
		return &board[y][x+1];
	}

	Square* getUpLeft(Square* currSquare) {
		if ((currSquare->x - 1) < 0 || (currSquare->y - 1) < 0) {
			return currSquare;
		}
		return &board[y-1][x-1];
	}

	Square* getUpRight(Square* currSquare) {
		if ((currSquare->x + 1) > 7 || (currSquare->y - 1) < 0) {
			return currSquare;
		}
		return &board[y-1][x+1];
	}

	Square* getDownLeft(Square* currSquare) {
		if ((currSquare->x - 1) < 0 || (currSquare->y + 1) > 7) {
			return currSquare;
		}
		return &board[y+1][x-1];
	}

	Square* getDownRight(Square* currSquare) {
		if ((currSquare->x + 1) > 7 || (currSquare->y + 1) > 7) {
			return currSquare;
		}
		return &board[y+1][x+1];
	}

};

