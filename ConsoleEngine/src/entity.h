#pragma once

#include "types.h"

class Serializable
{
public:
	virtual ~Serializable() {}

	virtual void OnLoad() {}
};

class Entity
{
public:
	virtual ~Entity() {}

	virtual bool IsWalkable() const = 0;
	virtual InteractionResult Interact( Entity* ent ) = 0;
	virtual char GetIcon() const = 0;
};

class Terrain : public Entity
{
public:
	Terrain();
	Terrain( const TerrainType type );
	~Terrain();

	virtual bool IsWalkable() const override;
	virtual InteractionResult Interact( Entity* ent ) override;
	virtual char GetIcon() const override;

	TerrainType GetType() const;
	void Destroy();

private:
	TerrainType m_type;
};

class Player : public Entity
{
public:
	Player();
	~Player();

	virtual bool IsWalkable() const override;
	virtual InteractionResult Interact( Entity* ent ) override;
	virtual char GetIcon() const override;

	void SetPos( const Vector2& pos );
	const Vector2& GetPos() const;

	void MoveInDir( Dir dir );

private:
	Vector2 m_pos;
};