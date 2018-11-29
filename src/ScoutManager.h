#pragma once

#include "Common.h"
#include "Unit.h"

class Bot;

class ScoutManager
{
    Bot &   m_bot;

    Unit m_scoutUnit;
    std::string     m_scoutStatus;
    int             m_numScouts;
    bool            m_scoutUnderAttack;
    Health        m_previousScoutHP;

    bool            enemyWorkerInRadiusOf(const Position & pos) const;
    Position      getFleePosition() const;
    Unit            closestEnemyWorkerTo(const Position & pos) const;
    void            moveScouts();
    void            drawScoutInformation();


public:

    ScoutManager(Bot & bot);

    void onStart();
    void onFrame();
    void setWorkerScout(const Unit & unit);
};