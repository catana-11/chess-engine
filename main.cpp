#include "board/Board.h"
#include "display/Printer.h"
#include "input/input_handler.h"
#include "moves/move_generator.h"
#include "rules/rules.h"
#include <iostream>

using namespace std;

int main(){

    Board B;
    Printer P;
    input_handler input;
    move_generator generator;
    rules rule_engine;

    bool whiteTurn = true;

    while(true){

        P.print_board(B);

        vector<Move> pseudoMoves = generator.generate_all_moves(B, whiteTurn);

        vector<Move> legalMoves =
            rule_engine.filter_legal_moves(B, pseudoMoves, whiteTurn);

        cout << "\nAvailable moves: " << legalMoves.size() << endl;

        if(whiteTurn)
            cout << "White's turn\n";
        else
            cout << "Black's turn\n";

        Move move;

        bool validMove = false;

        while(!validMove){

            move = input.get_move_from_user(B);

            for(auto &m : legalMoves){

                if(m.from == move.from && m.to == move.to){
                    validMove = true;
                    break;
                }
            }

            if(!validMove){
                cout << "Illegal move! Try again.\n";
            }
        }

        B.make_move(move.from, move.to);

        whiteTurn = !whiteTurn;
    }

    return 0;
}
