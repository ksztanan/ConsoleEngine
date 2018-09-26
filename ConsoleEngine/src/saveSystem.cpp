#include "saveSystem.h"
#include "entity.h"
#include "types.h"
#include "config.h"
#include "map.h"

SaveSystem::SaveSystem()
	: m_playerPos( Vector2( config::PLAYER_SPAWN_X, config::PLAYER_SPAWN_Y ) )
	, m_mapPos( Vector2( 0, 0 ) )
{
}

SaveSystem::~SaveSystem()
{
}

void SaveSystem::Save( SaveDataType type, Serializable* object )
{
	switch( type )
	{
		case SaveDataType::PlayerPos:
		{
			if( Player* player = (Player*)object )
			{
				m_playerPos = player->GetPos();
			}
			break;
		}

		case SaveDataType::MapPos:
		{
			if( Map* map = (Map*)object )
			{
				m_mapPos = map->GetLocalToWorld();
			}
			break;
		}
	}
}

void SaveSystem::Load( SaveDataType type, Serializable* object )
{
	switch( type )
	{
		case SaveDataType::PlayerPos:
		{
			if( Player* player = (Player*)object )
			{
				player->SetPos( m_playerPos );
			}
			break;
		}
		case SaveDataType::MapPos:
		{
			if( Map* map = (Map*)object )
			{
				map->SetLocalToWorld( m_mapPos );
			}
			break;
		}
	}
}
