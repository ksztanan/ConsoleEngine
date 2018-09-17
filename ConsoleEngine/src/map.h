#pragma once

#include "types.h"
#include "config.h"

class Entity;

class Map
{
	using Uint8 = unsigned char;

public:
	Map();
	~Map();

	void Initialize();
	void MoveLocalMap( Dir dir );

	const Vector2& GetLocalToWorld() const;
	const Entity* GetEntityAt( const Vector2& pos ) const;
	
private:
	void LoadWorld( const char* fileName );
	void CreateLocalMap( const Vector2& offset );
	void RefreshLocalMap( const Vector2& offset );
	void DeleteLocalMap();

private:
	Uint8 m_worldMap[ config::WORLD_MAP_SIZE_X ][ config::WORLD_MAP_SIZE_Y ];
	Entity* m_localMap[ config::LOC_MAP_SIZE_X ][ config::LOC_MAP_SIZE_Y ];
	Vector2 m_localToWorld;
};