#pragma once

#include "types.h"
#include "config.h"
#include "entity.h"
#include "entityFactory.h"

#include <vector>

class Map : public Serializable
{
	using Uint8 = unsigned char;

public:
	Map();
	~Map();

	void MoveLocalMap( Dir dir );

	void SetLocalToWorld( const Vector2& l2w );
	void SetDestroyedEntities( const std::vector< Vector2 >& entities );
	const Vector2& GetLocalToWorld() const;
	const std::vector< Vector2 >& GetDestroyedEntities() const;
	Entity* GetEntityAt( const Vector2& pos ) const;
	void DestroyEntityAt( const Vector2& pos );

	virtual void OnLoad() override;
	
private:
	void LoadWorld( const char* fileName );
	void CreateLocalMap();	
	void DeleteLocalMap();
	void HandleStreaming( Dir dir );

	Entity* CreateObjectAtPos( EntityFactory& factory, const Vector2& pos );

private:
	Uint8 m_worldMap[ config::WORLD_MAP_SIZE_X ][ config::WORLD_MAP_SIZE_Y ];
	Entity* m_localMap[ config::LOC_MAP_SIZE_X ][ config::LOC_MAP_SIZE_Y ];
	Vector2 m_localToWorld;
	std::vector< Vector2 > m_destroyedEntities;
};