#pragma once

#include "Common.h"
#include "UnitType.h"

class Bot;

class Unit
{
    mutable Bot * m_bot;
    UnitID    m_unitID;
    UnitType    m_unitType;

    const sc2::Unit * m_unit;

public:

    Unit();

    Unit(const sc2::Unit * unit, Bot & bot);
    const sc2::Unit * getUnitPtr() const;
    const sc2::UnitTypeID & getAPIUnitType() const;

    bool operator < (const Unit & rhs) const;
    bool operator == (const Unit & rhs) const;

    const UnitType & getType() const;

    Position getPosition() const;
    TilePosition getTilePosition() const;
	Health getTotalHealth() const;
    Health getHitPoints() const;
    Health getShields() const;
    Health getEnergy() const;
    Player getPlayer() const;
    UnitID getID() const;
    float getBuildPercentage() const;
    int getWeaponCooldown() const;
    bool isCompleted() const;
    bool isBeingConstructed() const;
    bool isCloaked() const;
    bool isFlying() const;
    bool isAlive() const;
    bool isPowered() const;
    bool isIdle() const;
    bool isBurrowed() const;
    bool isValid() const;
    bool isTraining() const;
    bool isConstructing(const UnitType & type) const;
	bool isPosVisible() const;
	bool isVisible() const;

    void stop           () const;
    void attackUnit     (const Unit & target) const;
    void attackMove     (const Position & targetPosition) const;
    void move           (const Position & targetPosition) const;
    void move           (const TilePosition & targetTilePosition) const;
    void rightClick     (const Unit & target) const;
    void repair         (const Unit & target) const;
    void build          (const UnitType & buildingType, TilePosition pos) const;
    void buildTarget    (const UnitType & buildingType, const Unit & target) const;
    void train          (const UnitType & buildingType) const;
    void morph          (const UnitType & type) const;
};
