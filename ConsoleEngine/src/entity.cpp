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
{
}

bool Terrain::IsWalkable() const
{
	if( m_type == TerrainType::Destroyed || m_type == TerrainType::Ground || m_type == TerrainType::Forest )
	{
		return true;
	}

	return false;
}

void Terrain::Interact( Entity* ent )
{}

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

	OnLoad();
}

Player::~Player()
{
}

bool Player::IsWalkable() const
{
	return false;
}

void Player::Interact( Entity* ent )
{
	if( Terrain* terrain = (Terrain*)ent )
	{
		if( terrain->GetType() == TerrainType::Forest )
		{
			terrain->Destroy();
		}
	}
}

char Player::GetIcon() const
{
	return '@';
}

void Player::OnSave()
{
	engine::Engine::Get().GetSaveSystem().Save( SaveDataType::PlayerPos, this );
}

void Player::OnLoad()
{
	engine::Engine::Get().GetSaveSystem().Load( SaveDataType::PlayerPos, this );
}

void Player::SetPos( const Vector2& pos )
{
	m_pos = pos;
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