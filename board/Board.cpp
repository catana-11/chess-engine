#include "Board.h"
#include <iostream>
//Board.cpp

using namespace std;

Board::Board(){
            // ye constructor board pe pieces set karega
            for (int i=0; i<64; i++){ board[i] = 0;}

            /*
            should be like:
            ...
            WP  WP...
            8   9   10  11  12  13  14  15
            WR  WN  WB  WK  WQ  WB  WN  WR
            0   1   2   3   4   5   6   7  --> white king row
            */

            // white side-->
            board[0] = 1; // white rook WR at A1
            board[1] = 2; // white knight WN at B1
            board[2] = 3; // white bishop WB at C1
            board[3] = 4; // white king WK at D1
            board[4] = 5; // white queen
            board[5] = 3; // bishop again
            board[6] = 2; board[7] = 1;

            board[8] = 6; // 6 for white pawns
            board[9] = 6; 
            board[10] = 6; 
            board[11] = 6; 
            board[12] = 6;
            board[13] = 6; 
            board[14] = 6; board[15] = 6;


            // black side -->
            board[56] = -1; // black rook BR at A8
            board[57] = -2; // black knight WN at B8
            board[58] = -3; // black bishop WB at C8
            board[59] = -4; // black king WK at D8
            board[60] = -5; // black queen
            board[61] = -3; // bishop again
            board[62] = -2; board[63] = -1;

            board[48] = -6; // -6 for blk pawns
            board[49] = -6; 
            board[50] = -6; 
            board[51] = -6; 
            board[52] = -6;
            board[53] = -6; 
            board[54] = -6; board[55] = -6;
        }

int Board::get_piece(int index){
            return board[index];
        }

void Board::set_piece(int index, int val){
            board[index]=val;
        }

void Board::make_move(int from_index, int to_index){
            board[to_index] = board[from_index];
            board[from_index] = 0;
        }

void Board::undo_move(int from_index, int to_index, int captured_piece){
    board[from_index] = board[to_index];
    board[to_index] = captured_piece;
}

