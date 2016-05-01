#ifndef PUZZLE_CMP_H_INCLUDED
#define PUZZLE_CMP_H_INCLUDED
#include<string>
#include"component.h"

class Puzzle : public Component{
public:
    std::string name;
    bool done;
};

#endif // PUZZLE_CMP_H_INCLUDED
