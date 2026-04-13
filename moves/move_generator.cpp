#include "move_generator.h"

#include "../rules/rules.h"
rules rule_engine;

bool move_generator::is_white_piece(int p){return p > 0;}

bool move_generator::is_black_piece(int p){return p < 0;}

vector<Move> move_generator::generate_all_moves(Board &board, bool white_turn){

    vector<Move> moves;

    for(int i=0;i<64;i++){

        int piece = board.get_piece(i); // gets the piece id on that square

        if(piece == 0) continue;

        if(white_turn && !is_white_piece(piece)) continue;
        if(!white_turn && !is_black_piece(piece)) continue;

        int absPiece = abs(piece);

        if(absPiece == 6)  // pawn 
            generate_pawn_moves(board,i,white_turn,moves);

        else if(absPiece == 1) // rook
            generate_rook_moves(board,i,white_turn,moves);

        else if(absPiece == 2) // knight
            generate_knight_moves(board,i,white_turn,moves);

        else if(absPiece == 3) // bishop
            generate_bishop_moves(board, i, white_turn, moves);

        else if(absPiece == 5)
            generate_king_moves(board, i, white_turn, moves);

        else if(absPiece == 4)
            generate_queen_moves(board, i, white_turn, moves);
    }

    return moves;
}

//////////////////////// PAWN ////////////////////////

void move_generator::generate_pawn_moves(Board &board, int pos, bool white_turn, vector<Move> &moves) {

    int dir = white_turn ? 8 : -8; 
    int row = pos / 8;
    int col = pos % 8;
    int forward = pos + dir;

    // single forward

    if (forward >= 0 && forward < 64 && board.get_piece(forward) == 0) {
        
        int toRow = forward / 8;
        bool isPromoting = (white_turn && toRow == 7) || (!white_turn && toRow == 0);

        Move m = {pos, forward, 0};
        m.is_promotion = isPromoting;
        moves.push_back(m);

        // double forward (only possible if single step was empty)
        int start_rank = white_turn ? 1 : 6;
        int double_forward = pos + 2 * dir;

        if (row == start_rank && board.get_piece(double_forward) == 0) {
            // double steps can never be promotions, so no flag needed
            moves.push_back({pos, double_forward, 0});
        }
    }

    // diagonal caps

    int diags[2] = {pos + dir + 1, pos + dir - 1};

    for (int diag : diags) {
        if (diag >= 0 && diag < 64) {
            int new_col = diag % 8;
            
            // prevent horizontal wrap around
            if (abs(new_col - col) == 1) {
                int target = board.get_piece(diag);
                int toRow = diag / 8;
                bool isPromoting = (white_turn && toRow == 7) || (!white_turn && toRow == 0);

                // capture
                if ((white_turn && is_black_piece(target)) || 
                    (!white_turn && is_white_piece(target))) {
                    
                    Move m = {pos, diag, target};
                    m.is_promotion = isPromoting;
                    moves.push_back(m);
                }

                // en passant capture
                else if (diag == board.en_passant_square) {
                    Move m = {pos, diag, 0}; // captured_piece will be set in board.make_move
                    m.is_en_passant = true;
                    moves.push_back(m);
                }
            }
        }
    }
}

//////////////////////// ROOK ////////////////////////

void move_generator::generate_rook_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int directions[4] = {8,-8,1,-1}; // upar, niche, right, left

    for(int d=0; d<4; d++){ // try sliding in each direction one by one

        int cur = pos;
        int prev = pos;

        while(true){

            cur += directions[d];

            if(cur <0 || cur>=64) break;

            int old_row = prev/8; int old_col = prev%8;
            int new_row = cur/8; int new_col = cur%8;

            // rook moves horizontally and checking if we accidently exceed previous row
            if ((abs(directions[d])==1 && new_row==old_row)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }

            // same for vertical
            else if ((abs(directions[d])==8 && new_col == old_col)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }
            else break;

            prev = cur;
        } // while will keep appending moves in that direction
    }
}

//////////////////////// KNIGHT ////////////////////////

void move_generator::generate_knight_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int jumps[8] = {15,17,6,10,-15,-17,-6,-10}; // ideal case => 8 moves possible

    for(int i=0;i<8;i++){

        int to = pos + jumps[i]; // try each jump

        if(to<0 || to>=64) continue; 

        int new_row = to/8; int new_col = to%8;
        int old_row = pos/8; int old_col = pos%8;

        if ((abs(new_row - old_row) == 2 && abs(new_col - old_col) == 1) || (abs(new_row - old_row) == 1 && abs(new_col - old_col) == 2)){

            int piece = board.get_piece(to);

            if(piece==0){
                moves.push_back({pos,to,0});
            }
            else{
                if(white_turn && is_black_piece(piece))
                    moves.push_back({pos,to,piece});
                if(!white_turn && is_white_piece(piece))
                    moves.push_back({pos,to,piece});
            }
        }
    }
}


//////////////////////// BISHOP ////////////////////////

void move_generator::generate_bishop_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int directions[4] = {+7,+9,-7,-9}; 

    for(int d=0; d<4; d++){ // try sliding in each direction one by one

        int cur = pos;
        int prev = pos;

        while(true){

            cur += directions[d];

            if(cur <0 || cur>=64) break;
            
            int new_row = cur/8; int new_col = cur%8;
            int old_row = prev/8; int old_col = prev%8;

            if (abs(new_row-old_row)==1 && abs(new_col-old_col)==1){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }
            else break;
            prev = cur;
        } // while will keep appending moves in that direction
    }
}

//////////////////////// QUEEN ////////////////////////

void move_generator::generate_queen_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int directions[8] = {+7,+9,-7,-9, 8,-8,1,-1}; 

    for(int d=0; d<8; d++){ // try sliding in each direction one by one

        int cur = pos;
        int prev = pos;

        while(true){

            cur += directions[d];

            if(cur <0 || cur>=64) break;
            
            int new_row = cur/8; int new_col = cur%8;
            int old_row = prev/8; int old_col = prev%8;

            // if diagonal movement:
            if (abs(new_row-old_row)==1 && abs(new_col-old_col)==1){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }

            // if horizontal movement:
            else if ((abs(directions[d])==1 && new_row==old_row)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }

            // same for vertical
            else if ((abs(directions[d])==8 && new_col == old_col)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    break; // if my own piece, then that direction is blocked, so break 
                }
            }
            else break;
            prev = cur;
        } // while will keep appending moves in that direction
    }
}

//////////////////////// KING ////////////////////////

void move_generator::generate_king_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int jumps[8] = {-1, +1, +8, -8, +7, -7, +9, -9}; 

    for(int i=0;i<8;i++){

        int cur = pos + jumps[i];
        if(cur < 0 || cur >= 64) continue;

        int new_row = cur/8, new_col = cur%8;
        int old_row = pos/8, old_col = pos%8;

        if(abs(new_row-old_row)<=1 && abs(new_col-old_col)<=1){

            int piece = board.get_piece(cur);

            if(piece == 0 ||
               (white_turn && is_black_piece(piece)) ||
               (!white_turn && is_white_piece(piece))){

                moves.push_back({pos, cur, piece});
            }
        }
    }

    // castling
    if(white_turn){
        if(!board.whiteKingMoved){

            // king must NOT be in check
            if(!rule_engine.is_square_attacked(board, 4, false)){

                // kingside (e1 -> g1)
                if(!board.whiteRookHMoved &&
                   board.get_piece(5)==0 && board.get_piece(6)==0 &&
                   !rule_engine.is_square_attacked(board, 5, false) &&
                   !rule_engine.is_square_attacked(board, 6, false)){

                    moves.push_back({pos, 6, 0});
                }

                // queenside (e1 -> c1)
                if(!board.whiteRookAMoved &&
                   board.get_piece(1)==0 && board.get_piece(2)==0 && board.get_piece(3)==0 &&
                   !rule_engine.is_square_attacked(board, 3, false) &&
                   !rule_engine.is_square_attacked(board, 2, false)){

                    moves.push_back({pos, 2, 0});
                }
            }
        }
    }
    else{
        if(!board.blackKingMoved){

            // king must NOT be in check
            if(!rule_engine.is_square_attacked(board, 60, true)){

                // kingside (e8 -> g8)
                if(!board.blackRookHMoved &&
                   board.get_piece(61)==0 && board.get_piece(62)==0 &&
                   !rule_engine.is_square_attacked(board, 61, true) &&
                   !rule_engine.is_square_attacked(board, 62, true)){

                    moves.push_back({pos, 62, 0});
                }

                // queenside (e8 -> c8)
                if(!board.blackRookAMoved &&
                   board.get_piece(57)==0 && board.get_piece(58)==0 && board.get_piece(59)==0 &&
                   !rule_engine.is_square_attacked(board, 59, true) &&
                   !rule_engine.is_square_attacked(board, 58, true)){

                    moves.push_back({pos, 58, 0});
                }
            }
        }
    }
}