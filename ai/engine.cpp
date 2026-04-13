
#include "engine.h"
#include <limits>

#include "../moves/move.h"

#include <algorithm>

using namespace std;

static long long nodes = 0; // gotta keep track of how many nodes it checks per move

int engine::minimax(Board &board, int depth, int alpha, int beta, bool maximizing){

    nodes++;

    if(depth == 0){
        return evaluator.evaluate(board);
    }

    if(depth >= 4) cout << "Searching 4 and/or above\n";

    vector<Move> pseudoMoves = generator.generate_all_moves(board, maximizing);

    vector<Move> legalMoves = rule_engine.filter_legal_moves(board, pseudoMoves, maximizing);

    if(legalMoves.empty()){

        if(rule_engine.is_king_in_check(board, maximizing))
            return maximizing ? -100000 : 100000;

        return 0;
    }

    // move ordering (captures first)

    sort(legalMoves.begin(), legalMoves.end(), [](Move &a, Move &b){

        return abs(a.captured_piece) > abs(b.captured_piece);
    });

    if(maximizing){

        int maxEval = numeric_limits<int>::min();

        for(auto &m : legalMoves){

            board.make_move(m);

            int eval = minimax(board, depth - 1, alpha, beta, !maximizing);

            board.undo_move(m);

            maxEval = max(maxEval, eval);

            alpha = max(alpha, eval);

            if(beta <= alpha) break;
        }

        return maxEval;
    }
    else{

        int minEval = numeric_limits<int>::max();

        for(auto &m : legalMoves){

            board.make_move(m);

            int eval = minimax(board, depth - 1, alpha, beta, !maximizing);

            board.undo_move(m);

            minEval = min(minEval, eval);

            beta = min(beta, eval);

            if(beta <= alpha) break;
        }

        return minEval;
    }
}


Move engine::find_best_move(Board &board, bool whiteTurn, int depth){

    vector<Move> pseudoMoves = generator.generate_all_moves(board, whiteTurn);

    vector<Move> legalMoves = rule_engine.filter_legal_moves(board, pseudoMoves, whiteTurn);

    if(legalMoves.empty()){

        return Move{-1, -1, 0};
    }

    // move ordering
    sort(legalMoves.begin(), legalMoves.end(), [](Move &a, Move &b){
        return abs(a.captured_piece) > abs(b.captured_piece);
    });

    Move bestMove = legalMoves[0];

    int bestValue = whiteTurn ? numeric_limits<int>::min(): numeric_limits<int>::max();

    for(auto &m : legalMoves){

        board.make_move(m);

        int eval = minimax(board, depth - 1,
                        numeric_limits<int>::min(),
                        numeric_limits<int>::max(),
                        !whiteTurn);

        board.undo_move(m);

        if(whiteTurn){
            if(eval > bestValue){
                bestValue = eval;
                bestMove = m;
            }
        } else {
            if(eval < bestValue){
                bestValue = eval;
                bestMove = m;
            }
        }
    }

    cout << "Nodes searched: " << nodes << endl;
    nodes = 0; //  for next move

    return bestMove;
}