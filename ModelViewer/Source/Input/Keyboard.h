#pragma once
#include <Windows.h>

/*!
* @brief キー入力状態を管理する・シングルトン
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

	static Keyboard * keyboard_;			// Keyboardインスタンスへのポインタ
	byte keyboard_state_[NUM_KEY];			// 現フレームのキー状態
	byte previous_keyboard_state_[NUM_KEY]; // 前フレームのキー状態
};