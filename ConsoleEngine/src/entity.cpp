#include "entity.h"
#include "engine.h"
#include "utility.h"
#include "config.h"

Terrain::Terrain()
	: m_type( TerrainType::Invalid )
{}

Terrain::Terrain( const TerrainType type )
	: m_type( type )
{}

Terrain::~Terrain()
{}

bool Terrain::IsWalkable() const
{
	if( m_type == TerrainType::Destroyed || m_type == TerrainType::Ground || m_type == TerrainType::Forest )
	{
		return true;
	}

	return false;
}

InteractionResult Terrain::Interact( Entity* ent )
{
	return InteractionResult::None;
}

char Terrain::GetIcon() const
{
	return utility::TypeToIcon( m_type );
}

TerrainType Terrain::GetType() const
{
	return m_type;
}

void Terrain::Destroy()
{
	m_type = TerrainType::Destroyed;
}

//--------------------------------------------------------------------------------------

Player::Player()
{
	m_pos = Vector2( config::PLAYER_SPAWN_X, config::PLAYER_SPAWN_Y );
}

Player::~Player()
{
}

bool Player::IsWalkable() const
{
	return false;
}

InteractionResult Player::Interact( Entity* ent )
{
	if( Terrain* terrain = (Terrain*)ent )
	{
		if( terrain->GetType() == TerrainType::Forest )
		{
			terrain->Destroy();
			return InteractionResult::TerrainDestroyed;
		}
	}

	return InteractionResult::None;
}

char Player::GetIcon() const
{
	return '@';
}

void Player::SetPos( const Vector2& pos )
{
	m_pos = pos;
}

const Vector2& Player::GetPos() const
{
	return m_pos;
}

void Player::MoveInDir( Dir dir )
{
	Vector2 offset;
	utility::DirToOffset( dir, offset );
	m_pos += offset;
}