#include "entity.h"
#include "utility.h"
#include "config.h"

Terrain::Terrain()
	: m_type( TerrainType::Invalid )
{}

Terrain::Terrain( const TerrainType type )
	: m_type( type )
{}

bool Terrain::IsWalkable() const
{
	if( m_type == TerrainType::Ground || m_type == TerrainType::Forest )
	{
		return true;
	}

	return false;
}

void Terrain::Interact()
{}

char Terrain::GetIcon() const
{
	return utility::TypeToIcon( m_type );
}

//--------------------------------------------------------------------------------------

Player::Player()
{
	m_pos = Vector2( config::PLAYER_SPAWN_X, config::PLAYER_SPAWN_Y );
}

bool Player::IsWalkable() const
{
	return false;
}

void Player::Interact()
{
}

char Player::GetIcon() const
{
	return '@';
}

Vector2& Player::GetPos()
{
	return m_pos;
}

void Player::MoveInDir( Dir dir )
{
	Vector2 offset;
	utility::DirToOffset( dir, offset );
	m_pos += offset;
}