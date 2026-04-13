
// this should have: the board, init it, get and set pieces, make move

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "../moves/move.h"

using namespace std;

class Board{

    int board[64];

    public:

        Board();

        int get_piece(int index);
        void set_piece(int index, int val);
        void make_move(Move &m);
        void undo_move(Move &m);

        bool whiteKingMoved = false, blackKingMoved = false;
        bool whiteRookAMoved = false, whiteRookHMoved = false;
        bool blackRookAMoved = false, blackRookHMoved = false;

        int en_passant_square = -1;
};

#endif