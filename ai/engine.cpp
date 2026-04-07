#include "engine.h"
#include <limits>

using namespace std;

int engine::minimax(Board &board, int depth, bool maximizing){

    move_generator generator;
    rules rule_engine;
    eval evaluator;

    // base case
    if(depth == 0){
        return evaluator.evaluate(board);
    }

    vector<Move> pseudoMoves = generator.generate_all_moves(board, maximizing);
    vector<Move> legalMoves =
        rule_engine.filter_legal_moves(board, pseudoMoves, maximizing);

    // no moves → checkmate or stalemate
    if(legalMoves.size() == 0){

        if(rule_engine.is_king_in_check(board, maximizing)){
            return maximizing ? -100000 : 100000;
        }
        else{
            return 0; // stalemate
        }
    }

    if(maximizing){

        int maxEval = numeric_limits<int>::min();

        for(auto &m : legalMoves){

            int captured = board.get_piece(m.to);

            board.make_move(m.from, m.to);

            int eval = minimax(board, depth - 1, false);

            board.undo_move(m.from, m.to, captured);

            maxEval = max(maxEval, eval);
        }

        return maxEval;
    }
    else{

        int minEval = numeric_limits<int>::max();

        for(auto &m : legalMoves){

            int captured = board.get_piece(m.to);

            board.make_move(m.from, m.to);

            int eval = minimax(board, depth - 1, true);

            board.undo_move(m.from, m.to, captured);

            minEval = min(minEval, eval);
        }

        return minEval;
    }
}


Move engine::find_best_move(Board &board, bool whiteTurn, int depth){

    move_generator generator;
    rules rule_engine;

    vector<Move> pseudoMoves = generator.generate_all_moves(board, whiteTurn);
    vector<Move> legalMoves =
        rule_engine.filter_legal_moves(board, pseudoMoves, whiteTurn);

    Move bestMove = legalMoves[0];

    int bestValue = whiteTurn ? numeric_limits<int>::min()
                             : numeric_limits<int>::max();

    for(auto &m : legalMoves){

        int captured = board.get_piece(m.to);

        board.make_move(m.from, m.to);

        int eval = minimax(board, depth - 1, !whiteTurn);

        board.undo_move(m.from, m.to, captured);

        if(whiteTurn){
            if(eval > bestValue){
                bestValue = eval;
                bestMove = m;
            }
        }
        else{
            if(eval < bestValue){
                bestValue = eval;
                bestMove = m;
            }
        }
    }

    return bestMove;
}