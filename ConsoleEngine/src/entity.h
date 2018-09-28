#pragma once

#include "types.h"

class Serializable
{
public:
	virtual ~Serializable() {}

	virtual void OnSave() {}
	virtual void OnLoad() {}
};

class Entity : public Serializable
{
public:
	virtual ~Entity() {}

	virtual bool IsWalkable() const = 0;
	virtual void Interact( Entity* ent ) = 0;
	virtual char GetIcon() const = 0;
};

class Terrain : public Entity
{
public:
	Terrain();
	Terrain( const TerrainType type );
	~Terrain();

	virtual bool IsWalkable() const override;
	virtual void Interact( Entity* ent ) override;
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
	virtual void Interact( Entity* ent ) override;
	virtual char GetIcon() const override;

	virtual void OnSave() override;
	virtual void OnLoad() override;

	void SetPos( const Vector2& pos );
	Vector2& GetPos();

	void MoveInDir( Dir dir );

private:
	Vector2 m_pos;
};