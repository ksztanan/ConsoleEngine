#pragma once

enum Dir;

enum InputAction
{
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,	
	Interact,
	SaveGame,
	RestartGame,
	QuitGame,

	InvalidAction
};

class InputManager
{
public:
	InputManager();

	void Update();

	Dir GetMovementDir() const;
	bool IsActionActive( const InputAction action ) const;
	unsigned char GetLastInput() const;

private:
	unsigned char InputActionToButton( const InputAction action ) const;

private:
	unsigned char m_lastInput;
};