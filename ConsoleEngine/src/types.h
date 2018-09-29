#pragma once

enum TerrainType
{
	Ground,
	Forest,
	Water,
	Rock,
	Destroyed,

	Invalid
};

enum Dir
{
	Up,
	Right,
	Down,
	Left,

	Undefined
};

enum InteractionResult
{
	TerrainDestroyed,

	None
};

struct Vector2
{
	Vector2()
		: X( 0 )
		, Y( 0 )
	{}

	Vector2( int x, int y )
		: X( x )
		, Y( y )
	{}

	const Vector2 operator+( const Vector2& rhs ) 
	{ 
		return Vector2( X + rhs.X, Y + rhs.Y ); 
	}

	Vector2& operator+=( const Vector2& rhs ) 
	{ 
		X += rhs.X;
		Y += rhs.Y;
		return *this;
	}

	bool operator==( const Vector2& rhs ) const { return ( X == rhs.X ) && ( Y == rhs.Y ); }
	bool operator!=( const Vector2& rhs ) const { return ( X != rhs.X ) || ( Y != rhs.Y ); }

	int X;
	int Y;
};