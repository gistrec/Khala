#pragma once

#include "Common.h"
#include <map>

class Bot;


class DistanceMap 
{
    int m_width;
    int m_height;
    TilePosition m_startTile;

    // 2D matrix storing distances from the start tile
    std::vector<std::vector<int>> m_dist;

    std::vector<TilePosition> m_sortedTiles;
    
public:
    
    DistanceMap();
    void computeDistanceMap(Bot & m_bot, const TilePosition & startTile);

    int getDistance(int tileX, int tileY) const;
    int getDistance(const TilePosition & pos) const;
    int getDistance(const Position & pos) const;

    // given a position, get the position we should move to to minimize distance
    const std::vector<TilePosition> & getSortedTiles() const;
    const TilePosition & getStartTile() const;

    void draw(Bot & bot) const;
};