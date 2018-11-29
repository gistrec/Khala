#include "Unit.h"
#include "Bot.h"

Unit::Unit()
    : m_bot(nullptr)
    , m_unit(nullptr)
    , m_unitID(0)
{

}

Unit::Unit(const sc2::Unit * unit, Bot & bot)
    : m_bot(&bot)
    , m_unit(unit)
    , m_unitID(unit->tag)
    , m_unitType(unit->unit_type, bot)
{
    
}

const sc2::Unit * Unit::getUnitPtr() const
{
    return m_unit;
}

const sc2::UnitTypeID & Unit::getAPIUnitType() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->unit_type;
}

bool Unit::operator < (const Unit & rhs) const
{
    return m_unit < rhs.m_unit;
}

bool Unit::operator == (const Unit & rhs) const
{
    return m_unit == rhs.m_unit;
}

const UnitType & Unit::getType() const
{
    return m_unitType;
}


Position Unit::getPosition() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->pos;
}

TilePosition Unit::getTilePosition() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return Util::GetTilePosition(m_unit->pos);
}

Health Unit::getTotalHealth() const
{
	BOT_ASSERT(isValid(), "Unit is not valid");
	return m_unit->health + m_unit->shield;
}

Health Unit::getHitPoints() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->health;
}

Health Unit::getShields() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->shield;
}

Health Unit::getEnergy() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->energy;
}

float Unit::getBuildPercentage() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->build_progress;
}

Player Unit::getPlayer() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    if (m_unit->alliance == sc2::Unit::Alliance::Self) { return 0; }
    else if (m_unit->alliance == sc2::Unit::Alliance::Enemy) { return 1; }
    else { return 2; }
}

UnitID Unit::getID() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    UnitID id = m_unit->tag;

    BOT_ASSERT(id == m_unitID, "Unit ID changed somehow");
    return id;
}

bool Unit::isCompleted() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->build_progress >= 1.0f;
}

bool Unit::isTraining() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->orders.size() > 0;
}

bool Unit::isVisible() const
{
	BOT_ASSERT(isValid(), "Unit is not valid");
	return m_bot->Observation()->GetUnit(m_unit->tag) && m_bot->Observation()->GetUnit(m_unit->tag)->display_type==sc2::Unit::DisplayType::Visible;
}

bool Unit::isBeingConstructed() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return !isCompleted() && m_unit->build_progress > 0.0f;
}

int Unit::getWeaponCooldown() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return (int)m_unit->weapon_cooldown;
}

bool Unit::isCloaked() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->cloak;
}

bool Unit::isFlying() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->is_flying;
}

bool Unit::isAlive() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->is_alive;
}

bool Unit::isPowered() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->is_powered;
}

bool Unit::isIdle() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->orders.empty();
}

bool Unit::isBurrowed() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    return m_unit->is_burrowed;
}

bool Unit::isValid() const
{
    return m_unit != nullptr;
}

void Unit::stop() const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::STOP);
}

void Unit::attackUnit(const Unit & target) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    BOT_ASSERT(target.isValid(), "Target is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::ATTACK_ATTACK, target.getUnitPtr());
}

void Unit::attackMove(const Position & targetPosition) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::ATTACK_ATTACK, targetPosition);
}

void Unit::move(const Position & targetPosition) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::MOVE, targetPosition);
}

void Unit::move(const TilePosition & targetPosition) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::MOVE, Position((float)targetPosition.x, (float)targetPosition.y));
}

void Unit::rightClick(const Unit & target) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, sc2::ABILITY_ID::SMART, target.getUnitPtr());
}

void Unit::repair(const Unit & target) const
{
    rightClick(target);
}

void Unit::build(const UnitType & buildingType, TilePosition pos) const
{
    BOT_ASSERT(m_bot->Map().isConnected(getTilePosition(), pos), "Error: Build Position is not connected to worker");
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, m_bot->Data(buildingType).buildAbility, Util::GetPosition(pos));
}

void Unit::buildTarget(const UnitType & buildingType, const Unit & target) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, m_bot->Data(buildingType).buildAbility, target.getUnitPtr());
}

void Unit::train(const UnitType & type) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, m_bot->Data(type).buildAbility);
}

void Unit::morph(const UnitType & type) const
{
    BOT_ASSERT(isValid(), "Unit is not valid");
    m_bot->Actions()->UnitCommand(m_unit, m_bot->Data(type).buildAbility);
}

bool Unit::isConstructing(const UnitType & type) const
{
    sc2::AbilityID buildAbility = m_bot->Data(type).buildAbility;
    return (getUnitPtr()->orders.size() > 0) && (getUnitPtr()->orders[0].ability_id == buildAbility);
}

bool Unit::isPosVisible() const
{
	return m_bot->Observation()->GetVisibility(getPosition()) == sc2::Visibility::Visible;
}