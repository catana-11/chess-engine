#include "rules.h"
#include <vector>

using namespace std;

//////////////////////// SQUARE ATTACK DETECTION ////////////////////////

bool rules::is_square_attacked(Board &board, int square, bool byWhite){

    int row = square / 8;
    int col = square % 8;


    // pawns
    if(byWhite){
        int r = row - 1;

        if(r >= 0){
            if(col - 1 >= 0 && board.get_piece(r*8 + col - 1) == 6) return true;
            if(col + 1 < 8 && board.get_piece(r*8 + col + 1) == 6) return true;
        }
    }
    else{
        int r = row + 1;

        if(r < 8){
            if(col - 1 >= 0 && board.get_piece(r*8 + col - 1) == -6) return true;
            if(col + 1 < 8 && board.get_piece(r*8 + col + 1) == -6) return true;
        }
    }
    // knights
    int knightMoves[8] = {15,17,6,10,-15,-17,-6,-10};

    for(int k=0;k<8;k++){
        int to = square + knightMoves[k];
        if(to < 0 || to >= 64) continue;

        int r = to/8, c = to%8;

        if(abs(r - row)==2 && abs(c - col)==1 ||
           abs(r - row)==1 && abs(c - col)==2){

            int p = board.get_piece(to);
            if(byWhite && p == 2) return true;
            if(!byWhite && p == -2) return true;
        }
    }

    // rook/queeen
    int directions[4] = {8,-8,1,-1};

    for(int d=0; d<4; d++){

        int cur = square;

        while(true){
            int prev = cur;
            cur += directions[d];

            if(cur < 0 || cur >= 64) break;

            int old_row = prev/8, old_col = prev%8;
            int new_row = cur/8, new_col = cur%8;

            if((abs(directions[d])==1 && new_row!=old_row) ||
               (abs(directions[d])==8 && new_col!=old_col))
                break;

            int p = board.get_piece(cur);

            if(p != 0){
                if(byWhite && (p == 1 || p == 4)) return true;
                if(!byWhite && (p == -1 || p == -4)) return true;
                break;
            }
        }
    }

    // bishop/queen
    int diag[4] = {7,9,-7,-9};

    for(int d=0; d<4; d++){

        int cur = square;

        while(true){
            int prev = cur;
            cur += diag[d];

            if(cur < 0 || cur >= 64) break;

            int old_row = prev/8, old_col = prev%8;
            int new_row = cur/8, new_col = cur%8;

            if(abs(new_row-old_row)!=1 || abs(new_col-old_col)!=1)
                break;

            int p = board.get_piece(cur);

            if(p != 0){
                if(byWhite && (p == 3 || p == 4)) return true;
                if(!byWhite && (p == -3 || p == -4)) return true;
                break;
            }
        }
    }

    // king
    
    int kingMoves[8] = {-1,1,8,-8,7,-7,9,-9};

    for(int i=0;i<8;i++){
        int to = square + kingMoves[i];
        if(to < 0 || to >= 64) continue;

        int r = to/8, c = to%8;

        if(abs(r-row)<=1 && abs(c-col)<=1){
            int p = board.get_piece(to);
            if(byWhite && p == 5) return true;
            if(!byWhite && p == -5) return true;
        }
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
        board.make_move(m);

        // check if own king is in check
        bool kingInCheck = is_king_in_check(board, whiteTurn);

        // undo move
        board.undo_move(m);

        if(!kingInCheck)
            legalMoves.push_back(m);
    }

    return legalMoves;
}