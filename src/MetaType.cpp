#include "MetaType.h"
#include "Util.h"
#include "Bot.h"

MetaType::MetaType()
    : m_bot         (nullptr)
    , m_type        (MetaTypes::None)
    , m_name        ("MetaType")
    , m_unitType    ()
    , m_upgrade     ()
{
}

MetaType::MetaType(const std::string & name, Bot & bot)
    : MetaType()
{
    m_bot = &bot;
    m_name = name;

    m_unitType = UnitType::GetUnitTypeFromName(m_name, bot);
    if (m_unitType.isValid())
    {
        m_type = MetaTypes::Unit;
        return;
    }

    for (const sc2::UpgradeData & data : bot.Observation()->GetUpgradeData())
    {
        if (name == data.name)
        {
            m_upgrade = data.upgrade_id;
            m_type = MetaTypes::Upgrade;
            return;
        }
    }

    BOT_ASSERT(false, "Could not find MetaType with name: %s", name.c_str());
}


MetaType::MetaType(const UnitType & unitType, Bot & bot)
{
    m_bot           = &bot;
    m_type          = MetaTypes::Unit;
    m_unitType      = unitType;
    m_race          = unitType.getRace();
    m_name          = unitType.getName();
}

MetaType::MetaType(const UpgradeID & upgradeType, Bot & bot)
{
    m_bot           = &bot;
    m_type          = MetaTypes::Upgrade;
    m_upgrade       = upgradeType;

    m_race          = m_bot->GetPlayerRace(Players::Self);
    m_name          = sc2::UpgradeIDToName(upgradeType);
}

bool MetaType::operator==(const MetaType &b) const
{
	return m_unitType == b.m_unitType;
}

bool MetaType::operator!=(const MetaType &b) const
{
	return m_unitType != b.m_unitType;
}

bool MetaType::isBuilding() const
{
    return isUnit() && getUnitType().isBuilding();
}

const size_t & MetaType::getMetaType() const
{
    return m_type;
}

bool MetaType::isUnit() const
{
    return m_type == MetaTypes::Unit;
}

bool MetaType::isUpgrade() const
{
    return m_type == MetaTypes::Upgrade;
}

bool MetaType::isTech() const
{
    return m_type == MetaTypes::Tech;
}

const Race & MetaType::getRace() const
{
    return m_race;
}

const UnitType & MetaType::getUnitType() const
{
    return m_unitType;
}

const UpgradeID & MetaType::getUpgrade() const
{
    return m_upgrade;
}

const std::string & MetaType::getName() const
{
    return m_name;
}