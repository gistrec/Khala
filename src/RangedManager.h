#pragma once

#include "Common.h"
#include "MicroManager.h"

class Bot;

class RangedManager: public MicroManager
{
public:

    RangedManager(Bot & bot);
    void    executeMicro(const std::vector<Unit> & targets);
    void    assignTargets(const std::vector<Unit> & targets);
    int     getAttackPriority(const Unit & rangedUnit, const Unit & target);
    Unit    getTarget(const Unit & rangedUnit, const std::vector<Unit> & targets);
};
