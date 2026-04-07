#include "board/Board.h"
#include "display/Printer.h"
#include "input/input_handler.h"
#include "moves/move_generator.h"
#include "rules/rules.h"
#include "ai/engine.h"
#include <iostream>

using namespace std;

int main(){

    Board B;
    Printer P;
    input_handler input;
    move_generator generator;
    rules rule_engine;

    engine ai;

    bool whiteTurn = true;

    while(true){

        P.print_board(B);

        vector<Move> pseudoMoves = generator.generate_all_moves(B, whiteTurn);

        vector<Move> legalMoves =
            rule_engine.filter_legal_moves(B, pseudoMoves, whiteTurn);

        if(legalMoves.size() == 0){

            if(rule_engine.is_king_in_check(B, whiteTurn)){
                cout << "\nCHECKMATE!\n";

                if(whiteTurn)
                    cout << "Black wins!\n";
                else
                    cout << "White wins!\n";
            }
            else{
                cout << "\nSTALEMATE!\n";
            }

            break;
        }

        cout << "\nAvailable moves: " << legalMoves.size() << endl;

        if(whiteTurn)
            cout << "White's turn\n";
        else
            cout << "Black's turn\n";

        if(whiteTurn){

            // HUMAN (WHITE)
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
        }
        else{

            // AI (BLACK)
            cout << "\nAI thinking...\n";

            Move bestMove = ai.find_best_move(B, whiteTurn, 3);

            cout << "AI played: "
                << bestMove.from << " -> " << bestMove.to << endl;

            B.make_move(bestMove.from, bestMove.to);
        }

        bool opponentInCheck = rule_engine.is_king_in_check(B, !whiteTurn);
        if(opponentInCheck){cout << "\nCHECK!\n";}

        whiteTurn = !whiteTurn;
    }

    return 0;
}
