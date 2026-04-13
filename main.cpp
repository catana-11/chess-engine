// swap commented blocks for ucl version

#include "board/Board.h"
#include "ai/engine.h"
#include <iostream>
#include <sstream>

using namespace std;

Board board;
engine ai;

bool whiteTurn = true;

//////////////////////////////////////////////////
// helpers
//////////////////////////////////////////////////

int notation_to_index(string s){
    int file = s[0] - 'a';
    int rank = s[1] - '1';
    return rank * 8 + file;
}

string index_to_notation(int idx){
    char file = 'a' + (idx % 8);
    char rank = '1' + (idx / 8);
    return string(1, file) + string(1, rank);
}

void apply_move(string moveStr){

    int from = notation_to_index(moveStr.substr(0,2));
    int to   = notation_to_index(moveStr.substr(2,2));

    Move m{};
    m.from = from;
    m.to = to;

    m.captured_piece = board.get_piece(to);

    // optional promotion support
    if(moveStr.length() == 5){
        m.is_promotion = true;
    }

    board.make_move(m);
}

//////////////////////////////////////////////////
// main UCI loop
//////////////////////////////////////////////////

int main(){

    string line;

    while(getline(cin, line)){

        ////////////////////////////////////////////////
        if(line == "uci"){
            cout << "id name MyChessEngine\n";
            cout << "uciok\n";
        }

        ////////////////////////////////////////////////
        else if(line == "isready"){
            cout << "readyok\n";
        }

        ////////////////////////////////////////////////
        else if(line == "ucinewgame"){
            board = Board();
            whiteTurn = true;
        }

        ////////////////////////////////////////////////
        // POSITION COMMAND
        ////////////////////////////////////////////////
        else if(line.find("position") != string::npos){

            board = Board();
            whiteTurn = true;

            stringstream ss(line);
            string token;

            ss >> token; // "position"
            ss >> token;

            if(token == "startpos"){
                board = Board();
            }
            else if(token == "fen"){

                string fen, temp;

                for(int i=0;i<6;i++){
                    ss >> temp;
                    fen += temp + " ";
                }

                stringstream fenStream(fen);

                string boardPart;
                fenStream >> boardPart;

                int idx = 56;

                for(char c : boardPart){

                    if(c == '/'){
                        idx -= 16;
                    }
                    else if(isdigit(c)){
                        idx += (c - '0');
                    }
                    else{
                        int piece = 0;

                        if(c=='P') piece=6;
                        if(c=='N') piece=2;
                        if(c=='B') piece=3;
                        if(c=='R') piece=1;
                        if(c=='Q') piece=4;
                        if(c=='K') piece=5;

                        if(c=='p') piece=-6;
                        if(c=='n') piece=-2;
                        if(c=='b') piece=-3;
                        if(c=='r') piece=-1;
                        if(c=='q') piece=-4;
                        if(c=='k') piece=-5;

                        board.set_piece(idx, piece);
                        idx++;
                    }
                }

                string turn;
                fenStream >> turn;
                whiteTurn = (turn == "w");
            }

            int moveCount = 0;

            while(ss >> token){
                if(token == "moves") continue;

                apply_move(token);
                moveCount++;
            }

            whiteTurn = (moveCount % 2 == 0);
        }

        ////////////////////////////////////////////////
        // GO COMMAND
        ////////////////////////////////////////////////
        else if(line.find("go") != string::npos){

            int depth = 7;

            stringstream ss(line);
            string token;

            while(ss >> token){
                if(token == "depth"){
                    ss >> depth;
                }
            }

            Move best = ai.find_best_move(board, whiteTurn, depth);

            if(best.from == -1){
                cout << "bestmove 0000" << endl;
                continue;
            }

            string moveStr =
                index_to_notation(best.from) +
                index_to_notation(best.to);

            cout << "bestmove " << moveStr << endl;

            apply_move(moveStr);
            whiteTurn = !whiteTurn;
        }

        ////////////////////////////////////////////////
        else if(line == "quit"){
            break;
        }
    }

    return 0;
}

/*
#include "board/Board.h"
#include "display/Printer.h"
#include "input/input_handler.h"
#include "moves/move_generator.h"
#include "rules/rules.h"
#include "ai/engine.h"
#include <iostream>

using namespace std;

string index_to_notation(int idx){
    char file = 'a' + (idx % 8);
    char rank = '1' + (idx / 8);
    return string(1,file) + string(1,rank);
}

int main(){

    Board board;
    Printer printer;
    input_handler input;
    move_generator generator;
    rules rule_engine;
    engine ai;

    bool whiteTurn = true;

    while(true){

        printer.print_board(board);

        vector<Move> pseudoMoves = generator.generate_all_moves(board, whiteTurn);

        vector<Move> legalMoves = rule_engine.filter_legal_moves(board, pseudoMoves, whiteTurn);

        // game over logic
        if(legalMoves.empty()){

            if(rule_engine.is_king_in_check(board, whiteTurn)){
                cout << "\nCHECKMATE!\n";
                cout << (whiteTurn ? "Black wins!\n" : "White wins!\n");

            } else {
                cout << "\nSTALEMATE!\n";
            }

            break;
        }

        cout << "\nAvailable moves: " << legalMoves.size() << endl;
        cout << (whiteTurn ? "White's turn\n" : "Black's turn\n");

        // human
        if(whiteTurn){

            Move move;
            bool validMove = false;

            while(!validMove){

                move = input.get_move_from_user(board);

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

            board.make_move(move);
        }

        // bot
        else{
            cout << "\nBot thinking...\n";

            Move best = ai.find_best_move(board, whiteTurn, 5);

            if(best.from == -1){
                cout << "No moves available!\n";
                break;
            }

            cout << "Bot played: "
            << index_to_notation(best.from)
            << " -> "
            << index_to_notation(best.to)
            << endl;

            board.make_move(best);
        }

        // is check
        if(rule_engine.is_king_in_check(board, !whiteTurn)){
            cout << "\nCHECK!\n";
        }

        whiteTurn = !whiteTurn;
    }

    return 0;
}
    */