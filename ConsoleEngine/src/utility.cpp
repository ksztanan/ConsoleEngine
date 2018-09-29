#include "utility.h"

namespace utility
{
	void DirToOffset( Dir dir, Vector2& offset )
	{
		switch( dir )
		{
			case Up:
				offset.X = 0;
				offset.Y = -1;
				break;
			case Right:
				offset.X = 1;
				offset.Y = 0;
				break;
			case Down:
				offset.X = 0;
				offset.Y = 1;
				break;
			case Left:
				offset.X = -1;
				offset.Y = 0;
				break;
			default:
				offset.X = 0;
				offset.Y = 0;
				break;
		}
	}

	char TypeToIcon( const TerrainType type )
	{
		switch( type )
		{
			case Ground:			return ',';
			case Forest:			return 'A';
			case Water:				return 'S';
			case Rock:				return 'O';
			case Destroyed:			return 'x';
		}

		return '?';
	}

	TerrainType IconToType( char icon )
	{
		switch( icon )
		{
			case ',':				return TerrainType::Ground;
			case '^':				return TerrainType::Forest;
			case '~':				return TerrainType::Water;
			case 'O':				return TerrainType::Rock;
			case 'x':				return TerrainType::Destroyed;
		}

		return TerrainType::Invalid;
	}
}