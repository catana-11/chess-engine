#ifndef ENGINE_H
#define ENGINE_H

#include "../board/Board.h"
#include "../moves/Move.h"
#include "../moves/move_generator.h"
#include "../rules/rules.h"
#include "../eval/eval.h"
#include <vector>

using std::vector;

class engine {

public:
    Move find_best_move(Board &board, bool whiteTurn, int depth);

private:
    int minimax(Board &board, int depth, bool maximizing);
};

#endif