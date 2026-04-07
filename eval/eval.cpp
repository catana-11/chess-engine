#include "eval.h"

int eval::evaluate(Board &board){

    int score = 0;

    for(int i = 0; i < 64; i++){

        int piece = board.get_piece(i);

        switch(piece){

            case 6: score += 100; break;   // pawn
            case 2: score += 320; break;   // knight
            case 3: score += 330; break;   // bishop
            case 1: score += 500; break;   // rook
            case 4: score += 900; break;   // queen
            case 5: score += 10000; break; // king

            case -6: score -= 100; break;
            case -2: score -= 320; break;
            case -3: score -= 330; break;
            case -1: score -= 500; break;
            case -4: score -= 900; break;
            case -5: score -= 10000; break;
        }
    }

    return score;
}