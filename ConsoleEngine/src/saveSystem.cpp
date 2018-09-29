#include "saveSystem.h"
#include "entity.h"
#include "types.h"
#include "config.h"
#include "map.h"

#include <iostream>
#include <fstream>
#include <string>

//#define DBG_SHOW_ENTITIES

SaveSystem::SaveSystem()
{
	if( config::USE_SAVED_GAME )
	{
		LoadFromFile();
	}
}

SaveSystem::~SaveSystem()
{
}

void SaveSystem::LoadFromFile()
{
	std::ifstream file( "data/savegame.txt", std::ifstream::in );

	if( file.is_open() )
	{
		std::string line;
		std::getline( file, line );
		m_save.m_playerPos.X = std::stoi( line, nullptr, 0 );
		std::getline( file, line );
		m_save.m_playerPos.Y = std::stoi( line, nullptr, 0 );
		std::getline( file, line );
		m_save.m_mapPos.X = std::stoi( line, nullptr, 0 );
		std::getline( file, line );
		m_save.m_mapPos.Y = std::stoi( line, nullptr, 0 );
		std::getline( file, line );
		int entSize = std::stoi( line, nullptr, 0 );


		for( int i = 0; i < entSize; ++i )
		{
			Vector2 pos;
			std::getline( file, line );
			pos.X = std::stoi( line, nullptr, 0 );
			std::getline( file, line );
			pos.Y = std::stoi( line, nullptr, 0 );

			m_save.m_destroyedEntities.push_back( pos );
		}

		m_save.m_valid = true;

		file.close();
	}
}

void SaveSystem::SaveToFile()
{
	if( m_save.m_valid )
	{
		std::ofstream file( "data/savegame.txt" );

		file << m_save.m_playerPos.X << std::endl;
		file << m_save.m_playerPos.Y << std::endl;
		file << m_save.m_mapPos.X << std::endl;
		file << m_save.m_mapPos.Y << std::endl;
		file << m_save.m_destroyedEntities.size() << std::endl;

		for( auto& ent : m_save.m_destroyedEntities )
		{
			file << ent.X << std::endl;
			file << ent.Y << std::endl;
		}

		file.close();
	}
}

void SaveSystem::Save( const SaveData& data )
{
	m_save = data;
	m_save.m_valid = true;

	SaveToFile();
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
	std::cout << "\n";

#ifdef DBG_SHOW_ENTITIES
	std::cout << "Destroyed entities:\n\n";
	for( const Vector2& pos : m_save.m_destroyedEntities )
	{
		std::cout << "World Pos:" << "\t\t\t" << "[" << pos.X << "," << pos.Y << "]\n";
	}

	std::cout << "\n";
#endif // DBG_SHOW_ENTITIES
}