#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>
#include "../board/Board.h"
#include "Move.h"

using std::vector;

class move_generator {

public:
    vector<Move> generate_all_moves(Board &board, bool whiteTurn);

private:
    void generate_pawn_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);
    void generate_rook_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);
    void generate_knight_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);
    void generate_bishop_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);
    void generate_queen_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);
    void generate_king_moves(Board &board, int pos, bool whiteTurn, vector<Move> &moves);

    bool is_white_piece(int piece);
    bool is_black_piece(int piece);
};

#endif
