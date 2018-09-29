#pragma once

namespace config
{
	static constexpr int WORLD_MAP_SIZE_X = 64;
	static constexpr int WORLD_MAP_SIZE_Y = 40;
	static constexpr int LOC_MAP_SIZE_X = 15;
	static constexpr int LOC_MAP_SIZE_Y = 9;
	static constexpr int PLAYER_SPAWN_X = LOC_MAP_SIZE_X / 2;
	static constexpr int PLAYER_SPAWN_Y = LOC_MAP_SIZE_Y / 2;

	static constexpr bool USE_SAVED_GAME = false;
}