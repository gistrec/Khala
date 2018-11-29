#pragma once

#include "BotAssert.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <fstream>
#include <streambuf>
#include <string>
#include <array>
#include "json/json.hpp"
#include "sc2api/sc2_api.h"

using json = nlohmann::json;

typedef sc2::Point2D        Position;
typedef sc2::Point2DI       TilePosition;
typedef sc2::Color          Color;
typedef sc2::UpgradeID      UpgradeID;
typedef sc2::Tag            UnitID;
typedef sc2::Race           Race;
typedef float               Health;
typedef float               PositionType;

typedef size_t Player;

namespace Players {
	enum {
		Self = 0u, Enemy = 1u, Neutral = 2u, Size = 3u, None = 4u
	};
}