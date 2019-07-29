#include <Windows.h>
#include "Keyboard.h"

Keyboard * Keyboard::keyboard_ = nullptr;

Keyboard::Keyboard() {

	SecureZeroMemory(keyboard_state_, NUM_KEY);
	SecureZeroMemory(previous_keyboard_state_, NUM_KEY);
}

/*!
* @brief �C���X�^���X����
*/
void Keyboard::Create() {

	if (keyboard_ != nullptr) {
		return;
	}

	keyboard_ = new Keyboard();
}

/*!
* @brief �C���X�^���X�j��
*/
void Keyboard::Delete() {

	delete keyboard_;
	keyboard_ = nullptr;
}

/*!
* @brief �C���X�^���X�ւ̃|�C���^�擾
*/
Keyboard * Keyboard::GetInstance() {

	return keyboard_;
}

/*!
* @brief �L�[��Ԃ̍X�V
*/
void Keyboard::Update() {

	// �O�t���[���̃L�[��Ԃ̕ێ�
	memcpy(previous_keyboard_state_, keyboard_state_, NUM_KEY);

	// �L�[��Ԃ̎擾
	GetKeyboardState(keyboard_state_);
}

/*!
* @brief �C�ӂ̃L�[�������������Ă��邩
* @param virtual_key	���z�L�[�R�[�h 
*/
bool Keyboard::IsPushKey(int virtual_key) const {

	// �ŏ�ʃr�b�g��1�̂Ƃ�����������ԁE���z�L�[�R�[�h�ŔC�ӂ̃L�[���w��
	if (keyboard_state_[virtual_key] & 0x80) {
		return true;
	}
	else {
		return false;
	}
}

/*!
* @brief �C�ӂ̃L�[������������ꂽ��
* @param virtual_key	���z�L�[�R�[�h
*/
bool Keyboard::IsDownKey(int virtual_key) const {

	// 1�t���[���O�̃L�[��Ԃ��牟���������u�Ԃ𔻒f
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
* @brief �C�ӂ̃L�[�������ꂽ��
* @param virtual_key	���z�L�[�R�[�h
*/
bool Keyboard::IsUpKey(int virtual_key) const {

	// �O�t���[���̃L�[��Ԃ���グ���u�Ԃ𔻒f
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
