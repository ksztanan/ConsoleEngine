#include "entityFactory.h"
#include "utility.h"

Entity* EntityFactory::CreateObject( const char icon )
{
	return new Terrain( utility::IconToType( icon ) );
}