#pragma once

enum Dir;

class InputManager
{
public:
	InputManager();

	void Update();

	Dir GetMovementDir() const;
	bool GetRestartRequest() const;
	bool GetExitRequest() const;
	bool GetSaveRequest() const;
	unsigned char GetLastInput() const;

private:
	unsigned char m_lastInput;
};