#pragma once

#define SHOW_ERROR(str) Error::Show(str, __FUNCTION__, __LINE__)

/*!
* @brief �G���[�̃��b�Z�[�W�{�b�N�X�\��
*/
class Error {

public:

	static void Show(const char * message, const char * func_name, int line);
};