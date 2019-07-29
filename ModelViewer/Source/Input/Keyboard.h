#pragma once
#include <Windows.h>

/*!
* @brief �L�[���͏�Ԃ��Ǘ�����E�V���O���g��
*/
class Keyboard {

private:

	static const int NUM_KEY = 256;

private:

	Keyboard();
	~Keyboard() {};

public:

	static void Create();
	static void Delete();
	static Keyboard * GetInstance();
	void Update();
	bool IsPushKey(int virtual_key) const;
	bool IsDownKey(int virtual_key) const;
	bool IsUpKey(int virtual_key) const;

private:

	static Keyboard * keyboard_;			// Keyboard�C���X�^���X�ւ̃|�C���^
	byte keyboard_state_[NUM_KEY];			// ���t���[���̃L�[���
	byte previous_keyboard_state_[NUM_KEY]; // �O�t���[���̃L�[���
};