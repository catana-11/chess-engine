#include "Board.h"
#include "../moves/move.h"

#include <iostream>

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
            board[3] = 4; // white queen WQ at D1
            board[4] = 5; // white king
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
            board[57] = -2; // black knight BN at B8
            board[58] = -3; // black bishop BB at C8
            board[59] = -4; // black queen BQ at D8
            board[60] = -5; // black king
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

void Board::make_move(Move &m){

    m.moved_piece = board[m.from];
    m.captured_piece = board[m.to];

    m.prev_en_passant = en_passant_square;

    m.prev_whiteKingMoved = whiteKingMoved;
    m.prev_blackKingMoved = blackKingMoved;
    m.prev_whiteRookAMoved = whiteRookAMoved;
    m.prev_whiteRookHMoved = whiteRookHMoved;
    m.prev_blackRookAMoved = blackRookAMoved;
    m.prev_blackRookHMoved = blackRookHMoved;

    int piece = m.moved_piece;

    // track rook movement
    if(piece == 1){ // white rook
        if(m.from == 0) whiteRookAMoved = true;
        if(m.from == 7) whiteRookHMoved = true;
    }
    if(piece == -1){ // black rook
        if(m.from == 56) blackRookAMoved = true;
        if(m.from == 63) blackRookHMoved = true;
    }

    // ALSO track if rook gets captured
    if(m.captured_piece == 1){
        if(m.to == 0) whiteRookAMoved = true;
        if(m.to == 7) whiteRookHMoved = true;
    }
    if(m.captured_piece == -1){
        if(m.to == 56) blackRookAMoved = true;
        if(m.to == 63) blackRookHMoved = true;
    }

    // en passant
    if(abs(piece) == 6 && m.to == en_passant_square){
        m.is_en_passant = true;
        int dir = (piece > 0) ? -8 : 8;
        m.captured_piece = board[m.to + dir];
        board[m.to + dir] = 0;
    }

    en_passant_square = -1;

    // set it
    if(abs(piece) == 6 && abs(m.to - m.from) == 16){
        en_passant_square = (m.from + m.to) / 2;
    }

    // castling long and short
    if(piece == 5){
        whiteKingMoved = true;

        if(m.from == 4 && m.to == 6){
            m.is_castling = true;
            board[5] = board[7]; board[7] = 0;
        }
        if(m.from == 4 && m.to == 2){
            m.is_castling = true;
            board[3] = board[0]; board[0] = 0;
        }
    }

    if(piece == -5){
        blackKingMoved = true;

        if(m.from == 60 && m.to == 62){
            m.is_castling = true;
            board[61] = board[63]; board[63] = 0;
        }
        if(m.from == 60 && m.to == 58){
            m.is_castling = true;
            board[59] = board[56]; board[56] = 0;
        }
    }

    // pawn promotion
    int row = m.to / 8;
    if(abs(piece) == 6){
        if((piece > 0 && row == 7) || (piece < 0 && row == 0)){
            m.is_promotion = true;
            piece = (piece > 0) ? 4 : -4;
        }
    }

    board[m.to] = piece;
    board[m.from] = 0;
}

void Board::undo_move(Move &m){

    board[m.from] = m.moved_piece;

    // undo en passant

    if(m.is_en_passant){

        int dir = (m.moved_piece > 0) ? -8 : 8;
        board[m.to + dir] = m.captured_piece;
        board[m.to] = 0;
    }
    else{

        board[m.to] = m.captured_piece;
    }

    // undo castling
    if(m.is_castling){

        if(m.to == 6){ board[7] = board[5]; board[5] = 0; }
        if(m.to == 2){ board[0] = board[3]; board[3] = 0; }
        if(m.to == 62){ board[63] = board[61]; board[61] = 0; }
        if(m.to == 58){ board[56] = board[59]; board[59] = 0; }
    }

    // restore state
    en_passant_square = m.prev_en_passant;

    whiteKingMoved = m.prev_whiteKingMoved;
    blackKingMoved = m.prev_blackKingMoved;
    whiteRookAMoved = m.prev_whiteRookAMoved;
    whiteRookHMoved = m.prev_whiteRookHMoved;
    blackRookAMoved = m.prev_blackRookAMoved;
    blackRookHMoved = m.prev_blackRookHMoved;
}