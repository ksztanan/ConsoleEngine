#pragma once

#include "types.h"

#include <vector>

class Serializable;

struct SaveData
{
	SaveData()
		: m_valid( false )
		, m_playerPos( Vector2( -1, -1 ) )
		, m_mapPos( Vector2( -1, -1 ) )
	{}

	bool m_valid;
	Vector2 m_playerPos;
	Vector2 m_mapPos;
	std::vector< Vector2 > m_destroyedEntities;
};

class SaveSystem
{
public:
	SaveSystem();
	~SaveSystem();

	void Save( const SaveData& data );
	bool Load( SaveData& data );

	void DrawDebug() const;

private:
	SaveData m_save;
};