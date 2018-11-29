#pragma once

#include "Common.h"
#include "UnitType.h"

namespace MetaTypes
{
    enum { Unit, Upgrade, Buff, Tech, Ability, None };
}

class Bot;
class MetaType
{
    Bot *         m_bot;
    size_t          m_type;
    std::string     m_name;
    Race          m_race;
    UnitType        m_unitType;
    UpgradeID       m_upgrade;

    
public:

    MetaType();
    MetaType(const std::string & name, Bot & bot);
    MetaType(const UnitType & unitType, Bot & bot);
    MetaType(const UpgradeID & upgradeType, Bot & bot);

	bool operator ==(const MetaType &b) const;

	bool operator!=(const MetaType & b) const;

    bool    isUnit()        const;
    bool    isUpgrade()     const;
    bool    isTech()        const;
    bool    isBuilding()    const;

    const size_t &          getMetaType()  const;
    const std::string &     getName()       const;
    const Race &          getRace()       const;
    const UnitType &        getUnitType() const;
    const UpgradeID &       getUpgrade()  const;

    std::vector<UnitType>   whatBuilds;

};


