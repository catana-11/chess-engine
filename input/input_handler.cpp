#include "input_handler.h"
#include <iostream>

using namespace std;

Move input_handler::get_move_from_user(Board &board){

    string from, to;

    cout << "Enter move (example: e2 e4): ";
    cin >> from >> to;

    int fromIndex = notation_to_index(from);
    int toIndex   = notation_to_index(to);

    Move move;
    move.from = fromIndex;
    move.to   = toIndex;
    move.captured_piece = board.get_piece(toIndex);

    return move;
}


int input_handler::notation_to_index(string notation){

    char fileChar = notation[0];   // e.g. 'E'
    char rankChar = notation[1];   // e.g. '2'

    int file = fileChar - 'a';     // 'a' → 0, 'b' → 1 ...
    int rank = rankChar - '1';     // '1' → 0, '2' → 1 ...

    return rank * 8 + file;
}
