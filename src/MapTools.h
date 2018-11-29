#pragma once

#include <vector>
#include "DistanceMap.h"
#include "UnitType.h"

class Bot;

class MapTools
{
    Bot & m_bot;
    int     m_width;
    int     m_height;
    float   m_maxZ;
    int     m_frame;
    

    // a cache of already computed distance maps, which is mutable since it only acts as a cache
    mutable std::map<std::pair<int,int>, DistanceMap>   m_allMaps;   

    std::vector<std::vector<bool>>  m_walkable;         // whether a tile is buildable (includes static resources)
    std::vector<std::vector<bool>>  m_buildable;        // whether a tile is buildable (includes static resources)
    std::vector<std::vector<bool>>  m_depotBuildable;   // whether a depot is buildable on a tile (illegal within 3 tiles of static resource)
    std::vector<std::vector<int>>   m_lastSeen;         // the last time any of our units has seen this position on the map
    std::vector<std::vector<int>>   m_sectorNumber;     // connectivity sector number, two tiles are ground connected if they have the same number
    std::vector<std::vector<float>> m_terrainHeight;        // height of the map at x+0.5, y+0.5
    
    void computeConnectivity();

    int getSectorNumber(int x, int y) const;
        
    void printMap();

    float   terrainHeight(const Position & point) const;
    bool    canBuild(int tileX, int tileY);
    bool    canWalk(int tileX, int tileY);

public:

    MapTools(Bot & bot);

    void    onStart();
    void    onFrame();
    void    draw() const;

    int     width() const;
    int     height() const;
    float   terrainHeight(float x, float y) const;

    void    drawLine(PositionType x1, PositionType y1, PositionType x2, PositionType y2, const Color & color = Color(255, 255, 255)) const;
    void    drawLine(const Position & p1, const Position & p2, const Color & color = Color(255, 255, 255)) const;
    void    drawTile(int tileX, int tileY, const Color & color = Color(255, 255, 255)) const;
    void    drawBox(PositionType x1, PositionType y1, PositionType x2, PositionType y2, const Color & color = Color(255, 255, 255)) const;
    void    drawBox(const Position & tl, const Position & br, const Color & color = Color(255, 255, 255)) const;
    void    drawCircle(PositionType x1, PositionType x2, PositionType radius, const Color & color = Color(255, 255, 255)) const;
    void    drawCircle(const Position & pos, PositionType radius, const Color & color = Color(255, 255, 255)) const;
    void    drawText(const Position & pos, const std::string & str, const Color & color = Color(255, 255, 255)) const;
    void    drawTextScreen(float xPerc, float yPerc, const std::string & str, const Color & color = Color(255, 255, 255)) const;
    
    bool    isValidTile(int tileX, int tileY) const;
    bool    isValidTile(const TilePosition & tile) const;
    bool    isValidPosition(const Position & pos) const;
    bool    isPowered(int tileX, int tileY) const;
    bool    isExplored(int tileX, int tileY) const;
	bool	isVisible(const TilePosition & tile) const;
    bool    isExplored(const Position & pos) const;
    bool    isExplored(const TilePosition & pos) const;
    bool    isVisible(int tileX, int tileY) const;
    bool    canBuildTypeAtPosition(int tileX, int tileY, const UnitType & type) const;

    const   DistanceMap & getDistanceMap(const TilePosition & tile) const;
    const   DistanceMap & getDistanceMap(const Position & tile) const;
    int     getGroundDistance(const Position & src, const Position & dest) const;
    bool    isConnected(int x1, int y1, int x2, int y2) const;
    bool    isConnected(const TilePosition & from, const TilePosition & to) const;
    bool    isConnected(const Position & from, const Position & to) const;
    bool    isWalkable(int tileX, int tileY) const;
    bool    isWalkable(const TilePosition & tile) const;
    
    bool    isBuildable(int tileX, int tileY) const;
    bool    isBuildable(const TilePosition & tile) const;
    bool    isDepotBuildableTile(int tileX, int tileY) const;
    
    TilePosition getLeastRecentlySeenTile() const;

    // returns a list of all tiles on the map, sorted by 4-direcitonal walk distance from the given position
    const std::vector<TilePosition> & getClosestTilesTo(const TilePosition & pos) const;
};

