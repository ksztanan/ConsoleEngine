#pragma once

#include "types.h"

class Entity
{
public:
	virtual ~Entity() {}

	virtual bool IsWalkable() const = 0;
	virtual void Interact() = 0;
	virtual char GetIcon() const = 0;
};

// todo - put derived classes in separate file 
class Terrain : public Entity
{
public:
	Terrain();
	Terrain( const TerrainType type );

	virtual bool IsWalkable() const override;
	virtual void Interact() override;
	virtual char GetIcon() const override;

private:
	TerrainType m_type;
};

class Player : public Entity
{
public:
	Player();

	virtual bool IsWalkable() const override;
	virtual void Interact() override;
	virtual char GetIcon() const override;

	Vector2& GetPos();

	void MoveInDir( Dir dir );

private:
	Vector2 m_pos;
};