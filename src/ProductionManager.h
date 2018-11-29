#pragma once

#include "Common.h"
#include "BuildOrder.h"
#include "BuildingManager.h"
#include "BuildOrderQueue.h"

class Bot;

class ProductionManager
{
    Bot &       m_bot;

    BuildingManager m_buildingManager;
    BuildOrderQueue m_queue;

    Unit    getClosestUnitToPosition(const std::vector<Unit> & units, Position closestTo);
    bool    meetsReservedResources(const MetaType & type);
    bool    canMakeNow(const Unit & producer, const MetaType & type);
    bool    detectBuildOrderDeadlock();
    void    setBuildOrder(const BuildOrder & buildOrder);
    void    create(const Unit & producer, BuildOrderItem & item);
    void    manageBuildOrderQueue();
    int     getFreeMinerals();
    int     getFreeGas();

    void    fixBuildOrderDeadlock();

public:

    ProductionManager(Bot & bot);

    void    onStart();
    void    onFrame();
    void    onUnitDestroy(const Unit & unit);
    void    drawProductionInformation();

    Unit getProducer(const MetaType & type, Position closestTo = Position(0, 0));
};
