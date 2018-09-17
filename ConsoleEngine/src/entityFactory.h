#pragma once

#include "entity.h"

class EntityFactory
{
public:
	Entity* CreateObject( const char icon );
};