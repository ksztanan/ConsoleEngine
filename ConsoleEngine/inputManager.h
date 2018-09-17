#pragma once

enum Dir;

class InputManager
{
public:
	InputManager();

	void Update();

	Dir GetMovementDir() const;
	unsigned char GetLastInput() const;

private:
	unsigned char m_lastInput;
};