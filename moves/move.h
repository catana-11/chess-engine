#ifndef MOVE_H
#define MOVE_H

struct Move {
    int from, to;

    int captured_piece;
    int moved_piece;

    int prev_en_passant;

    bool prev_whiteKingMoved;
    bool prev_blackKingMoved;
    bool prev_whiteRookAMoved;
    bool prev_whiteRookHMoved;
    bool prev_blackRookAMoved;
    bool prev_blackRookHMoved;

    // special moves
    bool is_en_passant = false;
    bool is_castling = false;
    bool is_promotion = false;
};
#endif
