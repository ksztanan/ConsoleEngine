#include "saveSystem.h"
#include "entity.h"
#include "types.h"
#include "config.h"
#include "map.h"

#include <iostream>

SaveSystem::SaveSystem()
{
}

SaveSystem::~SaveSystem()
{
}

void SaveSystem::Save( const SaveData& data )
{
	m_save = data;
	m_save.m_valid = true;
}

bool SaveSystem::Load( SaveData& data )
{
	if( m_save.m_valid )
	{
		data = m_save;
		return true;
	}

	return false;
}

void SaveSystem::DrawDebug() const
{
	std::cout << "SaveSystem Debug:\n";
	std::cout << "--------------------------------------\n";

	if( !m_save.m_valid )
	{
		return;
	}

	std::cout << "SAVED DATA" << "\t\t\t" << "[Vec2]" << "\n";
	std::cout << "Player Local Pos:" << "\t\t" << "[" << m_save.m_playerPos.X << "," << m_save.m_playerPos.Y << "]\n";
	std::cout << "Map LocalToWorld:" << "\t\t" << "[" << m_save.m_mapPos.X << "," << m_save.m_mapPos.Y << "]\n";
	std::cout << "Destroyed entities count:" << "\t" << "[" << m_save.m_destroyedEntities.size() << "]\n";
	for( const Vector2& pos : m_save.m_destroyedEntities )
	{
		std::cout << "Destroyed entities:" << "\t\t" << "[" << pos.X << "," << pos.Y << "]\n";
	}
	std::cout << "\n";
}
