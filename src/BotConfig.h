#pragma once

#include "Common.h"

/**
 * Конфиг бота
 * TODO: Добавить описание
 */
class BotConfig {

public:
	bool UsingAutoObserver = true;
    
    std::string BotName = "Khala";
    std::string Authors = "Alex Frost";
    bool PrintInfoOnStart = false;

    int SetLocalSpeed = 10;
    int SetFrameSkip = 0;
    bool UserInput = true;
    bool CompleteMapInformation = false;
    
    bool DrawGameInfo  = true;
    bool DrawTileInfo  = false;
	bool DrawSquadInfo = false;
    bool DrawResourceInfo = false;
    bool DrawScoutInfo    = false;
    bool DrawWorkerInfo   = false;
    bool DrawModuleTimers = false;
	bool DrawBuildingInfo = false;
	bool DrawSelfUnitInfo = false;
	bool DrawEnemyUnitInfo   = false;
	bool DrawUnitTargetInfo  = false;
	bool DrawProductionInfo  = false;
	bool DrawWalkableSectors = false;
	bool DrawLastSeenTileInfo = false;
	bool DrawBaseLocationInfo = false;
    bool DrawReservedBuildingTile = false;
    
    Color ColorLineTarget       = Color(255, 255, 255);
    Color ColorLineMineral      = Color(0, 128, 128);
    Color ColorUnitNearEnemy    = Color(255, 0, 0);
    Color ColorUnitNotNearEnemy = Color(0, 255, 0);
    
    bool KiteWithRangedUnits = true;    
    bool ScoutHarassEnemy    = true;
    
    int WorkersPerRefinery = 3;
    int BuildingSpacing    = 1;
    int PylonSpacing       = 3;
 
    BotConfig() = default;
};