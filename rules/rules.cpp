#include "rules.h"
#include <vector>

using namespace std;

//////////////////////// SQUARE ATTACK DETECTION ////////////////////////

bool rules::is_square_attacked(Board &board, int square, bool byWhite){

    move_generator generator;

    // generate all moves for attacking side
    vector<Move> moves = generator.generate_all_moves(board, byWhite);

    for(auto &m : moves){
        if(m.to == square)
            return true;
    }

    return false;
}


//////////////////////// KING CHECK DETECTION ////////////////////////

bool rules::is_king_in_check(Board &board, bool whiteKing){

    int kingSquare = -1;

    // find the king
    for(int i = 0; i < 64; i++){

        int piece = board.get_piece(i);

        if(whiteKing && piece == 5){
            kingSquare = i;
            break;
        }

        if(!whiteKing && piece == -5){
            kingSquare = i;
            break;
        }
    }

    if(kingSquare == -1)
        return false;

    // check if enemy attacks king square
    return is_square_attacked(board, kingSquare, !whiteKing);
}


//////////////////////// LEGAL MOVE FILTER ////////////////////////

vector<Move> rules::filter_legal_moves(Board &board,
                                       vector<Move> &pseudoMoves,
                                       bool whiteTurn){

    vector<Move> legalMoves;

    for(auto &m : pseudoMoves){

        int captured = board.get_piece(m.to);

        // make move
        board.make_move(m.from, m.to);

        // check if own king is in check
        bool kingInCheck = is_king_in_check(board, whiteTurn);

        // undo move
        board.undo_move(m.from, m.to, captured);

        if(!kingInCheck)
            legalMoves.push_back(m);
    }

    return legalMoves;
}