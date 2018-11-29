#include "Bot.h"
#include "Util.h"

Bot::Bot()
    : m_map(*this)
    , m_bases(*this)
    , m_unitInfo(*this)
    , m_workers(*this)
    , m_gameCommander(*this)
    , m_strategy(*this)
    , m_techTree(*this)
{
    
}

void Bot::OnGameStart() 
{
    m_config.readConfigFile();

    // add all the possible start locations on the map
    for (auto & loc : Observation()->GetGameInfo().enemy_start_locations)
    {
        m_baseLocations.push_back(loc);
    }
    m_baseLocations.push_back(Observation()->GetStartLocation());
    
    setUnits();
    m_techTree.onStart();
    m_strategy.onStart();
    m_map.onStart();
    m_unitInfo.onStart();
    m_bases.onStart();
    m_workers.onStart();

    m_gameCommander.onStart();
}

void Bot::OnStep()
{
    setUnits();
    m_map.onFrame();
    m_unitInfo.onFrame();
    m_bases.onFrame();
    m_workers.onFrame();
    m_strategy.onFrame();

    m_gameCommander.onFrame();

#ifndef LADDEREXE
    Debug()->SendDebug();
#endif
}

void Bot::setUnits()
{
    m_allUnits.clear();
    Control()->GetObservation();
    for (auto & unit : Observation()->GetUnits())
    {
        m_allUnits.push_back(Unit(unit, *this));    
    }

}

Race Bot::GetPlayerRace(int player) const
{
    auto playerID = Observation()->GetPlayerID();
    for (auto & playerInfo : Observation()->GetGameInfo().player_info)
    {
        if (playerInfo.player_id == playerID)
        {
            return playerInfo.race_actual;
        }
    }

    BOT_ASSERT(false, "Didn't find player to get their race");
    return sc2::Race::Random;
}

BotConfig & Bot::Config()
{
     return m_config;
}

const MapTools & Bot::Map() const
{
    return m_map;
}

const StrategyManager & Bot::Strategy() const
{
    return m_strategy;
}

const BaseLocationManager & Bot::Bases() const
{
    return m_bases;
}

const UnitInfoManager & Bot::UnitInfo() const
{
    return m_unitInfo;
}

int Bot::GetCurrentFrame() const
{
    return (int)Observation()->GetGameLoop();
}

const TypeData & Bot::Data(const UnitType & type) const
{
    return m_techTree.getData(type);
}

const TypeData & Bot::Data(const Unit & unit) const
{
    return m_techTree.getData(unit.getType());
}

const TypeData & Bot::Data(const UpgradeID & type) const
{
    return m_techTree.getData(type);
}

const TypeData & Bot::Data(const MetaType & type) const
{
    return m_techTree.getData(type);
}

WorkerManager & Bot::Workers()
{
    return m_workers;
}

int Bot::GetCurrentSupply() const
{
    return Observation()->GetFoodUsed();
}

int Bot::GetMaxSupply() const
{
    return Observation()->GetFoodCap();
}

int Bot::GetMinerals() const
{
    return Observation()->GetMinerals();
}

int Bot::GetGas() const
{
    return Observation()->GetVespene();
}

Unit Bot::GetUnit(const UnitID & tag) const
{
    return Unit(Observation()->GetUnit(tag), *(Bot *)this);
}

const std::vector<Unit> & Bot::GetUnits() const
{
    return m_allUnits;
}

Position Bot::GetStartLocation() const
{
    return Observation()->GetStartLocation();
}

const std::vector<Position> & Bot::GetStartLocations() const
{
    return m_baseLocations;
}

void Bot::OnError(const std::vector<sc2::ClientError> & client_errors, const std::vector<std::string> & protocol_errors)
{
    
}