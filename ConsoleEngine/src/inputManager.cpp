#include "inputManager.h"
#include "types.h"

#include <conio.h>

InputManager::InputManager()
{}

void InputManager::Update()
{
	m_lastInput = getch();
}

Dir InputManager::GetMovementDir() const
{
	Dir dir = Dir::Undefined;

	if( m_lastInput == InputActionToButton( InputAction::MoveUp ) )
	{
		dir = Dir::Up;
	}
	else if( m_lastInput == InputActionToButton( InputAction::MoveDown ) )
	{
		dir = Dir::Down;
	}
	else if( m_lastInput == InputActionToButton( InputAction::MoveLeft ) )
	{
		dir = Dir::Left;
	}
	else if( m_lastInput == InputActionToButton( InputAction::MoveRight ) )
	{
		dir = Dir::Right;
	}

	return dir;
}

bool InputManager::IsActionActive( const InputAction action ) const
{
	return m_lastInput == InputActionToButton( action );
}

unsigned char InputManager::GetLastInput() const
{
	return m_lastInput;
}

unsigned char InputManager::InputActionToButton( const InputAction action ) const
{
	switch( action )
	{
		case MoveUp:			return 'w';
		case MoveDown:			return 's';
		case MoveLeft:			return 'a';
		case MoveRight:			return 'd';
		case Interact:			return ' ';
		case SaveGame:			return 'p';
		case RestartGame:		return 'r';
		case QuitGame:			return 'q';		
	}

	return '?';
}
