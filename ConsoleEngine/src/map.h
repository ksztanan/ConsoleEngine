#pragma once

#include "types.h"
#include "config.h"
#include "entity.h"

class Map : public Serializable
{
	using Uint8 = unsigned char;

public:
	Map();
	~Map();

	void MoveLocalMap( Dir dir );

	void SetLocalToWorld( const Vector2& l2w );
	const Vector2& GetLocalToWorld() const;
	Entity* GetEntityAt( const Vector2& pos ) const;

	virtual void OnSave() override;
	virtual void OnLoad() override;
	
private:
	void LoadWorld( const char* fileName );
	void CreateLocalMap( const Vector2& offset );	
	void DeleteLocalMap();
	void HandleStreaming( Dir dir );

private:
	Uint8 m_worldMap[ config::WORLD_MAP_SIZE_X ][ config::WORLD_MAP_SIZE_Y ];
	Entity* m_localMap[ config::LOC_MAP_SIZE_X ][ config::LOC_MAP_SIZE_Y ];
	Vector2 m_localToWorld;
};