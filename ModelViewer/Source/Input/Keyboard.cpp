#include <Windows.h>
#include "Keyboard.h"

Keyboard * Keyboard::keyboard_ = nullptr;

Keyboard::Keyboard() {

	SecureZeroMemory(keyboard_state_, NUM_KEY);
	SecureZeroMemory(previous_keyboard_state_, NUM_KEY);
}

/*!
* @brief インスタンス生成
*/
void Keyboard::Create() {

	if (keyboard_ != nullptr) {
		return;
	}

	keyboard_ = new Keyboard();
}

/*!
* @brief インスタンス破棄
*/
void Keyboard::Delete() {

	delete keyboard_;
	keyboard_ = nullptr;
}

/*!
* @brief インスタンスへのポインタ取得
*/
Keyboard * Keyboard::GetInstance() {

	return keyboard_;
}

/*!
* @brief キー状態の更新
*/
void Keyboard::Update() {

	// 前フレームのキー状態の保持
	memcpy(previous_keyboard_state_, keyboard_state_, NUM_KEY);

	// キー状態の取得
	GetKeyboardState(keyboard_state_);
}

/*!
* @brief 任意のキーが押し下げられているか
* @param virtual_key	仮想キーコード 
*/
bool Keyboard::IsPushKey(int virtual_key) const {

	// 最上位ビットが1のとき押し下げ状態・仮想キーコードで任意のキーを指定
	if (keyboard_state_[virtual_key] & 0x80) {
		return true;
	}
	else {
		return false;
	}
}

/*!
* @brief 任意のキーが押し下げられたか
* @param virtual_key	仮想キーコード
*/
bool Keyboard::IsDownKey(int virtual_key) const {

	// 1フレーム前のキー状態から押し下げた瞬間を判断
	if (previous_keyboard_state_[virtual_key] & 0x80) {
		return false;
	}

	if (keyboard_state_[virtual_key] & 0x80) {
		return true;
	}
	else {
		return false;
	}
}

/*!
* @brief 任意のキーが離されたか
* @param virtual_key	仮想キーコード
*/
bool Keyboard::IsUpKey(int virtual_key) const {

	// 前フレームのキー状態から上げた瞬間を判断
	if (!(previous_keyboard_state_[virtual_key] & 0x80)) {
		return false;
	}

	if (!(keyboard_state_[virtual_key] & 0x80)) {
		return true;
	}
	else {
		return false;
	}
}
