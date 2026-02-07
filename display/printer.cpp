#include "Printer.h"
#include <iostream>
#include "../board/Board.h"
#include <string>
//printer.cpp

using namespace std;

void Printer::print_board(Board &board){

    cout<<"Current board position: "<<endl;

    for (int i=0; i<=7; i++){
        for (int j=0; j<=7; j++){

            int index = i*8+j;
            int piece = board.get_piece(index);

            if (piece>0){
               //white:
               if (piece==1) printf("WR     ");
               if (piece==2) printf("WN     ");
               if (piece==3) printf("WB     ");
               if (piece==4) printf("WK     ");
               if (piece==5) printf("WQ     ");
               if (piece==6) printf("WP     ");
            }

            else if (piece<0){
                //black:
               if (piece==-1) printf("BR     ");
               if (piece==-2) printf("BN     ");
               if (piece==-3) printf("BB     ");
               if (piece==-4) printf("BK     ");
               if (piece==-5) printf("BQ     ");
               if (piece==-6) printf("BP     ");

            }

            else{
                printf("_      ");
            }

        }
        cout<<endl;
    }

}

void Printer::print_piece(int piece){

    cout<<"Piece is: ";
    if (piece>0){
            //white:
           if (piece==1) printf("WR     ");
           if (piece==2) printf("WN     ");
           if (piece==3) printf("WB     ");
           if (piece==4) printf("WK     ");
           if (piece==5) printf("WQ     ");
           if (piece==6) printf("WP     ");
        }

    else{
            //black:
           if (piece==-1) printf("BR     ");
           if (piece==-2) printf("BN     ");
           if (piece==-3) printf("BB     ");
           if (piece==-4) printf("BK     ");
           if (piece==-5) printf("BQ     ");
           if (piece==-6) printf("BP     ");

        }

}