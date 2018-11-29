#include "MapTools.h"
#include "Util.h"
#include "Bot.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <array>

const size_t LegalActions = 4;
const int actionX[LegalActions] ={1, -1, 0, 0};
const int actionY[LegalActions] ={0, 0, 1, -1};

typedef std::vector<std::vector<bool>> vvb;
typedef std::vector<std::vector<int>>  vvi;
typedef std::vector<std::vector<float>>  vvf;

    #define HALF_TILE 0.5f

// constructor for MapTools
MapTools::MapTools(Bot & bot)
    : m_bot     (bot)
    , m_width   (0)
    , m_height  (0)
    , m_maxZ    (0.0f)
    , m_frame   (0)
{

}

void MapTools::onStart()
{
    m_width  = m_bot.Observation()->GetGameInfo().width;
    m_height = m_bot.Observation()->GetGameInfo().height;

    m_walkable       = vvb(m_width, std::vector<bool>(m_height, true));
    m_buildable      = vvb(m_width, std::vector<bool>(m_height, false));
    m_depotBuildable = vvb(m_width, std::vector<bool>(m_height, false));
    m_lastSeen       = vvi(m_width, std::vector<int>(m_height, 0));
    m_sectorNumber   = vvi(m_width, std::vector<int>(m_height, 0));
    m_terrainHeight  = vvf(m_width, std::vector<float>(m_height, 0.0f));

    // Set the boolean grid data from the Map
    for (int x(0); x < m_width; ++x)
    {
        for (int y(0); y < m_height; ++y)
        {
            m_buildable[x][y]       = canBuild(x, y);
            m_depotBuildable[x][y]  = canBuild(x, y);
            m_walkable[x][y]        = m_buildable[x][y] || canWalk(x, y);
            m_terrainHeight[x][y]   = terrainHeight(Position((PositionType)x, (PositionType)y));
        }
    }

    for (auto & unit : m_bot.Observation()->GetUnits())
    {
        m_maxZ = std::max(unit->pos.z, m_maxZ);
    }

    // set tiles that static resources are on as unbuildable
    for (auto & resource : m_bot.GetUnits())
    {
        if (!resource.getType().isMineral() && !resource.getType().isGeyser())
        {
            continue;
        }

        int width = resource.getType().tileWidth();
        int height = resource.getType().tileHeight();
        int tileX = static_cast<int>(std::floor(resource.getPosition().x) - (width / 2.0));
        int tileY = static_cast<int>(std::floor(resource.getPosition().y) - (height / 2.0));

        if (!isVisible(resource.getTilePosition().x, resource.getTilePosition().y))
        {
        }

        for (int x=tileX; x<tileX+width; ++x)
        {
            for (int y=tileY; y<tileY+height; ++y)
            {
                m_buildable[x][y] = false;

                // depots can't be built within 3 tiles of any resource
                for (int rx=-3; rx<=3; rx++)
                {
                    for (int ry=-3; ry<=3; ry++)
                    {
                        // sc2 doesn't fill out the corners of the mineral 3x3 boxes for some reason
                        if (std::abs(rx) + std::abs(ry) == 6) { continue; }
                        if (!isValidTile(TilePosition(x+rx, y+ry))) { continue; }

                        m_depotBuildable[x+rx][y+ry] = false;
                    }
                }
            }
        }
    }
    computeConnectivity();
}

void MapTools::onFrame()
{
    m_frame++;

    for (int x=0; x<m_width; ++x)
    {
        for (int y=0; y<m_height; ++y)
        {
            if (isVisible(x, y))
            {
                m_lastSeen[x][y] = m_frame;
            }
        }
    }

    draw();
}

void MapTools::computeConnectivity()
{
    // the fringe data structe we will use to do our BFS searches
    std::vector<std::array<int, 2>> fringe;
    fringe.reserve(m_width*m_height);
    int sectorNumber = 0;

    // for every tile on the map, do a connected flood fill using BFS
    for (int x=0; x<m_width; ++x)
    {
        for (int y=0; y<m_height; ++y)
        {
            // if the sector is not currently 0, or the map isn't walkable here, then we can skip this tile
            if (getSectorNumber(x, y) != 0 || !isWalkable(x, y))
            {
                continue;
            }

            // increase the sector number, so that walkable tiles have sectors 1-N
            sectorNumber++;

            // reset the fringe for the search and add the start tile to it
            fringe.clear();
            fringe.push_back({x,y});
            m_sectorNumber[x][y] = sectorNumber;

            // do the BFS, stopping when we reach the last element of the fringe
            for (size_t fringeIndex=0; fringeIndex<fringe.size(); ++fringeIndex)
            {
                auto & tile = fringe[fringeIndex];

                // check every possible child of this tile
                for (size_t a=0; a<LegalActions; ++a)
                {
                    int nextX = tile[0] + actionX[a];
                    int nextY = tile[1] + actionY[a];

                    // if the new tile is inside the map bounds, is walkable, and has not been assigned a sector, add it to the current sector and the fringe
                    if (isValidTile(nextX, nextY) && isWalkable(nextX, nextY) && (getSectorNumber(nextX, nextY) == 0))
                    {
                        m_sectorNumber[nextX][nextY] = sectorNumber;
                        fringe.push_back({nextX, nextY});
                    }
                }
            }
        }
    }
}

bool MapTools::isExplored(const TilePosition & pos) const
{
    return isExplored(pos.x, pos.y);
}

bool MapTools::isExplored(const Position & pos) const
{
    return isExplored(Util::GetTilePosition(pos));
}

bool MapTools::isExplored(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY)) { return false; }

    sc2::Visibility vis = m_bot.Observation()->GetVisibility(Position(tileX + HALF_TILE, tileY + HALF_TILE));
    return vis == sc2::Visibility::Fogged || vis == sc2::Visibility::Visible;
}
bool MapTools::isVisible(const TilePosition & tile) const
{
	return isVisible(tile.x, tile.y);
}

bool MapTools::isVisible(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY)) { return false; }

    return m_bot.Observation()->GetVisibility(Position(tileX + HALF_TILE, tileY + HALF_TILE)) == sc2::Visibility::Visible;
}

bool MapTools::isPowered(int tileX, int tileY) const
{
    for (auto & powerSource : m_bot.Observation()->GetPowerSources())
    {
        if (Util::Dist(Position(tileX + HALF_TILE, tileY + HALF_TILE), powerSource.position) < powerSource.radius)
        {
            return true;
        }
    }

    return false;
}

float MapTools::terrainHeight(float x, float y) const
{
    return m_terrainHeight[(int)x][(int)y];
}

//int MapTools::getGroundDistance(const Position & src, const Position & dest) const
//{
//    return (int)Util::Dist(src, dest);
//}

int MapTools::getGroundDistance(const Position & src, const Position & dest) const
{
    if (m_allMaps.size() > 50)
    {
        m_allMaps.clear();
    }

    return getDistanceMap(dest).getDistance(src);
}

const DistanceMap & MapTools::getDistanceMap(const Position & pos) const
{
    return getDistanceMap(Util::GetTilePosition(pos));
}

const DistanceMap & MapTools::getDistanceMap(const TilePosition & tile) const
{
    std::pair<int,int> pairTile(tile.x, tile.y);

    if (m_allMaps.find(pairTile) == m_allMaps.end())
    {
        m_allMaps[pairTile] = DistanceMap();
        m_allMaps[pairTile].computeDistanceMap(m_bot, tile);
    }

    return m_allMaps[pairTile];
}

int MapTools::getSectorNumber(int x, int y) const
{
    if (!isValidTile(x, y))
    {
        return 0;
    }

    return m_sectorNumber[x][y];
}

bool MapTools::isValidTile(int tileX, int tileY) const
{
    return tileX >= 0 && tileY >= 0 && tileX < m_width && tileY < m_height;
}

bool MapTools::isValidTile(const TilePosition & tile) const
{
    return isValidTile(tile.x, tile.y);
}

bool MapTools::isValidPosition(const Position & pos) const
{
    return isValidTile(Util::GetTilePosition(pos));
}

void MapTools::drawLine(PositionType x1, PositionType y1, PositionType x2, PositionType y2, const Color & color) const
{
    m_bot.Debug()->DebugLineOut(sc2::Point3D(x1, y1, terrainHeight(x1, y1) + 0.2f), sc2::Point3D(x2, y2, terrainHeight(x2, y2) + 0.2f), color);
}

void MapTools::drawLine(const Position & p1, const Position & p2, const Color & color) const
{
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void MapTools::drawTile(int tileX, int tileY, const Color & color) const
{
    PositionType px = Util::TileToPosition((float)tileX) + Util::TileToPosition(0.1f);
    PositionType py = Util::TileToPosition((float)tileY) + Util::TileToPosition(0.1f);
    PositionType d  = Util::TileToPosition(0.8f);

    drawLine(px,     py,     px + d, py,     color);
    drawLine(px + d, py,     px + d, py + d, color);
    drawLine(px + d, py + d, px,     py + d, color);
    drawLine(px,     py + d, px,     py,     color);
}

void MapTools::drawBox(PositionType x1, PositionType y1, PositionType x2, PositionType y2, const Color & color) const
{
    m_bot.Debug()->DebugBoxOut(sc2::Point3D(x1, y1, m_maxZ + 2.0f), sc2::Point3D(x2, y2, m_maxZ-5.0f), color);
}

void MapTools::drawBox(const Position & tl, const Position & br, const Color & color) const
{
    m_bot.Debug()->DebugBoxOut(sc2::Point3D(tl.x, tl.y, m_maxZ + 2.0f), sc2::Point3D(br.x, br.y, m_maxZ-5.0f), color);
}

void MapTools::drawCircle(const Position & pos, PositionType radius, const Color & color) const
{
    m_bot.Debug()->DebugSphereOut(sc2::Point3D(pos.x, pos.y, terrainHeight(pos)), radius, color);
}

void MapTools::drawCircle(PositionType x, PositionType y, PositionType radius, const Color & color) const
{
    m_bot.Debug()->DebugSphereOut(sc2::Point3D(x, y, terrainHeight(x,y)), radius, color);
}


void MapTools::drawText(const Position & pos, const std::string & str, const Color & color) const
{
    m_bot.Debug()->DebugTextOut(str, sc2::Point3D(pos.x, pos.y, terrainHeight(pos)), color);
}

void MapTools::drawTextScreen(float xPerc, float yPerc, const std::string & str, const Color & color) const
{
    m_bot.Debug()->DebugTextOut(str, Position(xPerc, yPerc), color);
}

bool MapTools::isConnected(int x1, int y1, int x2, int y2) const
{
    if (!isValidTile(x1, y1) || !isValidTile(x2, y2))
    {
        return false;
    }

    int s1 = getSectorNumber(x1, y1);
    int s2 = getSectorNumber(x2, y2);

    return s1 != 0 && (s1 == s2);
}

bool MapTools::isConnected(const TilePosition & p1, const TilePosition & p2) const
{
    return isConnected(p1.x, p1.y, p2.x, p2.y);
}

bool MapTools::isConnected(const Position & p1, const Position & p2) const
{
    return isConnected(Util::GetTilePosition(p1), Util::GetTilePosition(p2));
}

bool MapTools::isBuildable(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_buildable[tileX][tileY];
}

bool MapTools::canBuildTypeAtPosition(int tileX, int tileY, const UnitType & type) const
{
    return m_bot.Query()->Placement(m_bot.Data(type).buildAbility, Position((float)tileX, (float)tileY));
}

bool MapTools::isBuildable(const TilePosition & tile) const
{
    return isBuildable(tile.x, tile.y);
}

void MapTools::printMap()
{
    std::stringstream ss;
    for (int y(0); y < m_height; ++y)
    {
        for (int x(0); x < m_width; ++x)
        {
            ss << isWalkable(x, y);
        }

        ss << "\n";
    }

    std::ofstream out("map.txt");
    out << ss.str();
    out.close();
}

bool MapTools::isDepotBuildableTile(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_depotBuildable[tileX][tileY];
}

bool MapTools::isWalkable(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_walkable[tileX][tileY];
}

bool MapTools::isWalkable(const TilePosition & tile) const
{
    return isWalkable(tile.x, tile.y);
}

int MapTools::width() const
{
    return m_width;
}

int MapTools::height() const
{
    return m_height;
}

const std::vector<TilePosition> & MapTools::getClosestTilesTo(const TilePosition & pos) const
{
    return getDistanceMap(pos).getSortedTiles();
}

TilePosition MapTools::getLeastRecentlySeenTile() const
{
    int minSeen = std::numeric_limits<int>::max();
    TilePosition leastSeen;
    const BaseLocation * baseLocation = m_bot.Bases().getPlayerStartingBaseLocation(Players::Self);

    for (auto & tile : baseLocation->getClosestTiles())
    {
        BOT_ASSERT(isValidTile(tile), "How is this tile not valid?");

        int lastSeen = m_lastSeen[tile.x][tile.y];
        if (lastSeen < minSeen)
        {
            minSeen = lastSeen;
            leastSeen = tile;
        }
    }

    return leastSeen;
}

bool MapTools::canWalk(int tileX, int tileY) 
{
    auto & info = m_bot.Observation()->GetGameInfo();
    sc2::Point2DI pointI(tileX, tileY);
    if (pointI.x < 0 || pointI.x >= info.width || pointI.y < 0 || pointI.y >= info.width)
    {
        return false;
    }

    assert(info.pathing_grid.data.size() == info.width * info.height);
    unsigned char encodedPlacement = info.pathing_grid.data[pointI.x + ((info.height - 1) - pointI.y) * info.width];
    bool decodedPlacement = encodedPlacement == 255 ? false : true;
    return decodedPlacement;
}

bool MapTools::canBuild(int tileX, int tileY) 
{
    auto & info = m_bot.Observation()->GetGameInfo();
    sc2::Point2DI pointI(tileX, tileY);
    if (pointI.x < 0 || pointI.x >= info.width || pointI.y < 0 || pointI.y >= info.width)
    {
        return false;
    }

    assert(info.placement_grid.data.size() == info.width * info.height);
    unsigned char encodedPlacement = info.placement_grid.data[pointI.x + ((info.height - 1) - pointI.y) * info.width];
    bool decodedPlacement = encodedPlacement == 255 ? true : false;
    return decodedPlacement;
}

float MapTools::terrainHeight(const Position & point) const
{
    auto & info = m_bot.Observation()->GetGameInfo();
    sc2::Point2DI pointI((int)point.x, (int)point.y);
    if (pointI.x < 0 || pointI.x >= info.width || pointI.y < 0 || pointI.y >= info.width)
    {
        return 0.0f;
    }

    assert(info.terrain_height.data.size() == info.width * info.height);
    unsigned char encodedHeight = info.terrain_height.data[pointI.x + ((info.height - 1) - pointI.y) * info.width];
    float decodedHeight = -100.0f + 200.0f * float(encodedHeight) / 255.0f;
    return decodedHeight;
}


void MapTools::draw() const
{
    Position camera = m_bot.Observation()->GetCameraPos();
    int sx = (int)(camera.x - 12.0f);
    int sy = (int)(camera.y - 8);
    int ex = sx + 24;
    int ey = sy + 20;

    for (int x = sx; x < ex; ++x)
    {
        for (int y = sy; y < ey; y++)
        {
            if (!isValidTile((int)x, (int)y))
            {
                continue;
            }

            if (m_bot.Config().DrawWalkableSectors)
            {
                std::stringstream ss;
                ss << getSectorNumber(x, y);
                drawText(Position(Util::TileToPosition(x + 0.5f), Util::TileToPosition(y + 0.5f)), ss.str());
            }

            if (m_bot.Config().DrawTileInfo)
            {
                Color color = isWalkable(x, y) ? Color(0, 255, 0) : Color(255, 0, 0);
                if (isWalkable(x, y) && !isBuildable(x, y)) { color = Color(255, 255, 0); }
                if (isBuildable(x, y) && !isDepotBuildableTile(x, y)) { color = Color(127, 255, 255); }
                drawTile(x, y, color);
            }
        }
    }
}
