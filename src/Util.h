#pragma once

#include "Common.h"
#include "UnitType.h"

class Bot;
class Unit;

namespace Util
{
    Race          GetRaceFromString(const std::string & str);
    TilePosition  GetTilePosition(const Position & pos);
    Position      GetPosition(const TilePosition & tile);
    std::string     GetStringFromRace(const Race & race);
    bool            UnitCanMetaTypeNow(const Unit & unit, const UnitType & type, Bot & m_bot);
	std::vector<UnitType> getEquivalentTypes(const UnitType unit, Bot & bot);
    UnitType        GetTownHall(const Race & race, Bot & bot);
    UnitType        GetRefinery(const Race & race, Bot & bot);
    UnitType        GetSupplyProvider(const Race & race, Bot & bot);
    Position      CalcCenter(const std::vector<Unit> & units);
    bool            IsZerg(const Race & race);
    bool            IsProtoss(const Race & race);
    bool            IsTerran(const Race & race);
    PositionType  TileToPosition(float tile);

    sc2::BuffID     GetBuffFromName(const std::string & name, Bot & bot);
    sc2::AbilityID  GetAbilityFromName(const std::string & name, Bot & bot);

    float Dist(const Unit & unit, const Position & p2);
    float Dist(const Unit & unit1, const Unit & unit2);
    float Dist(const Position & p1, const Position & p2);
    PositionType DistSq(const Position & p1, const Position & p2);

	std::string ExePath();
};
