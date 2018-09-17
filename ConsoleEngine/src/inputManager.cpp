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

	switch( m_lastInput )
	{
		case 'w':
			dir = Dir::Up;
			break;

		case 's':
			dir = Dir::Down;
			break;

		case 'a':
			dir = Dir::Left;
			break;

		case 'd':
			dir = Dir::Right;
			break;
	}

	return dir;
}

unsigned char InputManager::GetLastInput() const
{
	return m_lastInput;
}