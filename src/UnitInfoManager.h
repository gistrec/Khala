#pragma once

#include "Common.h"
#include "UnitData.h"
#include "BaseLocation.h"
#include "Unit.h"

class Bot;
class UnitInfoManager 
{
    Bot &           m_bot;

    std::map<Player, UnitData> m_unitData; 
    std::map<Player, std::vector<Unit>> m_units;

    void                    updateUnit(const Unit & unit);
    void                    updateUnitInfo();
    bool                    isValidUnit(const Unit & unit);
    
    const UnitData &        getUnitData(Player player) const;

	void checkPositions(const int player);

    void drawSelectedUnitDebugInfo();

public:

    UnitInfoManager(Bot & bot);

    void                    onFrame();
    void                    onStart();

    const std::vector<Unit> & getUnits(Player player) const;

    size_t                  getUnitTypeCount(Player player, UnitType type, bool completed = true) const;
	size_t                  getUnitTypeCount(Player player, std::vector<UnitType> types, bool completed = true) const;

    void                    getNearbyForce(std::vector<UnitInfo> & unitInfo, Position p, int player, float radius) const;

    const std::map<Unit, UnitInfo> & getUnitInfoMap(Player player) const;

    //bool                  enemyHasCloakedUnits() const;
    void                    drawUnitInformation() const;

};