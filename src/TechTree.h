#pragma once

#include "Common.h"
#include "UnitType.h"

class Bot;
class MetaType;

struct TypeData
{
    Race                  race;
    int                     mineralCost     = 0;      // mineral cost of the item
    int                     gasCost         = 0;          // gas cost of the item
    int                     supplyCost      = 0;       // supply cost of the item
    int                     buildTime       = 0;        // build time of the item
    bool                    isUnit          = false;
    bool                    isBuilding      = false;
    bool                    isWorker        = false;
    bool                    isRefinery      = false;
    bool                    isSupplyProvider= false;
    bool                    isResourceDepot = false;
    bool                    isAddon         = false;
    sc2::AbilityID          buildAbility    = 0;     // the ability that creates this item
    sc2::AbilityID          warpAbility     = 0;      // the ability that creates this item via warp-in
    std::vector<UnitType>   whatBuilds;       // any of these units can build the item
    std::vector<UnitType>   requiredUnits;    // owning ONE of these is required to make
    std::vector<UpgradeID>  requiredUpgrades; // having ALL of these is required to make
};

class TechTree
{
    Bot & m_bot;
    std::map<UnitType, TypeData>  m_unitTypeData;
    std::map<UpgradeID, TypeData> m_upgradeData;

    void initUnitTypeData();
    void initUpgradeData();

    void outputJSON(const std::string & filename) const;

public:

    TechTree(Bot & bot);
    void onStart();

    const TypeData & getData(const UnitType & type) const;
    const TypeData & getData(const UpgradeID & type) const;
    const TypeData & getData(const MetaType & type) const;
};