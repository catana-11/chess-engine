#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../moves/Move.h"   
#include "../board/Board.h"

class input_handler {

public:
    Move get_move_from_user(Board &board);
};

#endif
