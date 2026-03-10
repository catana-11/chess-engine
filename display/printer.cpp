#include "Printer.h"
#include <iostream>
#include "../board/Board.h"
#include <string>
//printer.cpp

using namespace std;

void Printer::print_board(Board &board){

    cout << "\n    A   B   C   D   E   F   G   H\n";

    for(int row = 7; row >= 0; row--){

        cout << "  +---+---+---+---+---+---+---+---+\n";

        cout << row+1 << " |";

        for(int col = 0; col < 8; col++){

            int index = row * 8 + col;
            int piece = board.get_piece(index);

            print_piece(piece);

            cout << "|";
        }

        cout << " " << row+1 << endl;
    }

    cout << "  +---+---+---+---+---+---+---+---+\n";

    cout << "    A   B   C   D   E   F   G   H\n\n";
}

void Printer::print_piece(int piece){

    if(piece == 0){
        cout << " . ";
        return;
    }

    if(piece > 0){
        if(piece == 1) cout << "WR ";
        if(piece == 2) cout << "WN ";
        if(piece == 3) cout << "WB ";
        if(piece == 4) cout << "WK ";
        if(piece == 5) cout << "WQ ";
        if(piece == 6) cout << "WP ";
    }
    else{
        if(piece == -1) cout << "BR ";
        if(piece == -2) cout << "BN ";
        if(piece == -3) cout << "BB ";
        if(piece == -4) cout << "BK ";
        if(piece == -5) cout << "BQ ";
        if(piece == -6) cout << "BP ";
    }
}