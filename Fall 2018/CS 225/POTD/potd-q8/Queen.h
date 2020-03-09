#ifndef _QUEEN_H
#define _QUEEN_H
#include <iostream>
using std::string;
#include "Piece.h"

class Queen : public Piece {
    public:
        string getType();
};

#endif
