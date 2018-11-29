#pragma once

#include "Common.h"
#include "DistanceMap.h"
#include "Unit.h"
#include <map>
#include <vector>

class Bot;

class BaseLocation
{
    Bot &                     m_bot;
    DistanceMap                 m_distanceMap;

    TilePosition              m_depotPosition;
    Position                  m_centerOfResources;
    std::vector<Unit>           m_geysers;
    std::vector<Unit>           m_minerals;

    std::vector<Position>     m_mineralPositions;
    std::vector<Position>     m_geyserPositions;

    std::map<Player, bool>    m_isPlayerOccupying;
    std::map<Player, bool>    m_isPlayerStartLocation;
        
    int                         m_baseID;
    PositionType              m_left;
    PositionType              m_right;
    PositionType              m_top;
    PositionType              m_bottom;
    bool                        m_isStartLocation;
    
public:

    BaseLocation(Bot & bot, int baseID, const std::vector<Unit> & resources);
    
    int getGroundDistance(const Position & pos) const;
    int getGroundDistance(const TilePosition & pos) const;
    bool isStartLocation() const;
    bool isPlayerStartLocation(Player player) const;
    bool isMineralOnly() const;
    bool containsPosition(const Position & pos) const;
    const TilePosition & getDepotPosition() const;
    const Position & getPosition() const;
    const std::vector<Unit> & getGeysers() const;
    const std::vector<Unit> & getMinerals() const;
    bool isOccupiedByPlayer(Player player) const;
    bool isExplored() const;
    bool isInResourceBox(int x, int y) const;

    void setPlayerOccupying(Player player, bool occupying);

    const std::vector<TilePosition> & getClosestTiles() const;

    void draw();
};
