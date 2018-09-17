#pragma once

#include "types.h"

namespace utility
{
	void DirToOffset( Dir dir, Vector2& offset );
	char TypeToIcon( const TerrainType type );
	TerrainType IconToType( char icon );
}