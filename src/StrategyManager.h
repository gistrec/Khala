#pragma once

#include "Common.h"
#include "BuildOrder.h"
#include "Condition.h"

typedef std::pair<UnitType, size_t> UnitPair;
typedef std::vector<UnitPair>       UnitPairVector;

class Bot;

struct Strategy
{
    std::string m_name;
    Race      m_race;
    int         m_wins;
    int         m_losses;
    BuildOrder  m_buildOrder;
    Condition   m_scoutCondition;
    Condition   m_attackCondition;

    Strategy();
    Strategy(const std::string & name, const Race & race, const BuildOrder & buildOrder, const Condition & scoutCondition, const Condition & attackCondition);
};

class StrategyManager
{
    Bot & m_bot;

    Race                          m_selfRace;
    Race                          m_enemyRace;
    std::map<std::string, Strategy> m_strategies;
    int                             m_totalGamesPlayed;
    const BuildOrder                m_emptyBuildOrder;

    bool  shouldExpandNow() const;
    const UnitPairVector getProtossBuildOrderGoal() const;
    const UnitPairVector getTerranBuildOrderGoal() const;
    const UnitPairVector getZergBuildOrderGoal() const;

public:

    StrategyManager(Bot & bot);

    const Strategy & getCurrentStrategy() const;
    bool scoutConditionIsMet() const;
    bool attackConditionIsMet() const;
    void onStart();
    void onFrame();
    void onEnd(const bool isWinner);
    void addStrategy(const std::string & name, const Strategy & strategy);
    const UnitPairVector getBuildOrderGoal() const;
    const BuildOrder & getOpeningBookBuildOrder() const;
    void readStrategyFile(const std::string & str);
};
