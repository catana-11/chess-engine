#include "move_generator.h"

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

        else if(absPiece == 4)
            generate_king_moves(board, i, white_turn, moves);

        else if(absPiece == 5)
            generate_queen_moves(board, i, white_turn, moves);
    }

    return moves;
}

//////////////////////// PAWN ////////////////////////

void move_generator::generate_pawn_moves(Board &board, int pos, bool white_turn, vector<Move> &moves){

    int dir = white_turn ? 8 : -8; 

    int forward = pos + dir;

    // single forward move
    if(forward >= 0 && forward < 64 && board.get_piece(forward) == 0){

        moves.push_back({pos, forward, 0});

        // double forwarding
        int start_rank = white_turn ? 1 : 6;
        int row = pos / 8;

        int double_forward = pos + 2*dir;

        if(row == start_rank &&
           double_forward >= 0 && double_forward < 64 &&
           board.get_piece(double_forward) == 0){

            moves.push_back({pos, double_forward, 0});
        }
    }

    // diagonal capture squares
    int diag1 = pos + dir + 1;
    int diag2 = pos + dir - 1;

    int old_row = pos / 8;
    int old_col = pos % 8;

    // capture right
    if(diag1 >= 0 && diag1 < 64){

        int new_col = diag1 % 8;

        if(abs(new_col - old_col) == 1){

            int target = board.get_piece(diag1);

            if(white_turn && is_black_piece(target))
                moves.push_back({pos, diag1, target});

            if(!white_turn && is_white_piece(target))
                moves.push_back({pos, diag1, target});
        }
    }

    // capture left
    if(diag2 >= 0 && diag2 < 64){

        int new_col = diag2 % 8;

        if(abs(new_col - old_col) == 1){

            int target = board.get_piece(diag2);

            if(white_turn && is_black_piece(target))
                moves.push_back({pos, diag2, target});

            if(!white_turn && is_white_piece(target))
                moves.push_back({pos, diag2, target});
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

        int cur = pos + jumps[i]; // try each jump
        int prev = pos;

        if(cur<0 ||cur>=64) continue; 

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

                    continue; // if my own piece, then that direction is blocked, so break 
                }
            }

            // if horizontal movement:
            else if ((abs(jumps[i])==1 && new_row==old_row)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    continue; // if my own piece, then that direction is blocked, so break 
                }
            }

            // same for vertical
            else if ((abs(jumps[i])==8 && new_col == old_col)){

                int piece = board.get_piece(cur);

                if(piece == 0){
                    moves.push_back({pos,cur,0}); // empty square, can go ahead
                }
                else{
                    if(white_turn && is_black_piece(piece))
                        moves.push_back({pos,cur,piece}); // capture

                    if(!white_turn && is_white_piece(piece)) //capture
                        moves.push_back({pos,cur,piece});

                    continue; // if my own piece, then that direction is blocked, so break 
                }
            }
            else continue;
}
}