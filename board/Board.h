
// this should have: the board, initialise it, get and set pieces, make move
//Board.h
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
using namespace std;

class Board{

    int board[64];

    public:

        Board();

        int get_piece(int index);
        void set_piece(int index, int val);
        void make_move(int from_index, int to_index);


};

#endif