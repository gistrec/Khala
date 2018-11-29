#pragma once

#include "Common.h"
#include "Squad.h"
#include "SquadData.h"

class Bot;

class CombatCommander
{
    Bot &         m_bot;

    SquadData       m_squadData;
    std::vector<Unit>  m_combatUnits;
    bool            m_initialized;
    bool            m_attackStarted;

    void            updateScoutDefenseSquad();
    void            updateDefenseSquads();
    void            updateAttackSquads();
    void            updateIdleSquad();
    bool            isSquadUpdateFrame();

    Unit            findClosestDefender(const Squad & defenseSquad, const Position & pos);
    Unit            findClosestWorkerTo(std::vector<Unit> & unitsToAssign, const Position & target);

    Position      getMainAttackLocation();

    void            updateDefenseSquadUnits(Squad & defenseSquad, const size_t & flyingDefendersNeeded, const size_t & groundDefendersNeeded);
    bool            shouldWeStartAttacking();

public:

    CombatCommander(Bot & bot);


    void onStart();
    void onFrame(const std::vector<Unit> & combatUnits);

    void drawSquadInformation();
};

