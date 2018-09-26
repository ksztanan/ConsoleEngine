#pragma once

#include "types.h"

class Serializable;

class SaveSystem
{
public:
	SaveSystem();
	~SaveSystem();

	void Save( SaveDataType type, Serializable* object );
	void Load( SaveDataType type, Serializable* object );

private:
	Vector2 m_playerPos;
	Vector2 m_mapPos;
};