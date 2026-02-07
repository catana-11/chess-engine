#include "board/Board.h"
#include "display/Printer.h"
#include "input/input_handler.h"
#include "moves/move_generator.h"
#include <iostream>

using namespace std;

int main(){

    Board B;
    Printer P;
    input_handler input;
    move_generator generator;

    bool whiteTurn = true;

    while(true){

        P.print_board(B);

        vector<Move> legalMoves = generator.generate_all_moves(B, whiteTurn);

        cout << "\nAvailable moves: " << legalMoves.size() << endl;

        if(whiteTurn)
            cout << "White's turn\n";
        else
            cout << "Black's turn\n";

        Move move = input.get_move_from_user(B);

        B.make_move(move.from, move.to);

        whiteTurn = !whiteTurn;
    }

    return 0;
}
