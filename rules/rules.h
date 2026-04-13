#ifndef RULES_H
#define RULES_H

#include "../board/Board.h"
#include "../moves/Move.h"
#include "../moves/move_generator.h"
#include <vector>

using std::vector;

class rules {

    public:
        bool is_square_attacked(Board &board, int square, bool byWhite);

        bool is_king_in_check(Board &board, bool whiteKing);

        vector<Move> filter_legal_moves(Board &board,
                                        vector<Move> &pseudoMoves,
                                        bool whiteTurn);
};

#endif