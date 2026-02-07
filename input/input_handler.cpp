#include "input_handler.h"
#include <iostream>

using namespace std;

Move input_handler::get_move_from_user(Board &board) {

    int from, to;

    cout << "Enter FROM square (0-63): ";
    cin >> from;

    cout << "Enter TO square (0-63): ";
    cin >> to;

    Move move;
    move.from = from;
    move.to = to;
    move.captured_piece = board.get_piece(to);

    return move;
}
