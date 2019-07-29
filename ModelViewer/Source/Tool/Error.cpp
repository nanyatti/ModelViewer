#include <Windows.h>
#include <stdio.h>
#include "Error.h"

/*!
* @brief �G���[�̃��b�Z�[�W�{�b�N�X�\��
* @param message	�G���[���b�Z�[�W
* @param func_name	�Ăяo�����̊֐���
* @param line		�Ăяo�����̍s��
*/
void Error::Show(const char * message, const char * func_name, int line) {

	char output[1024];
	sprintf_s(output, "%s\n%s(%d)", message, func_name, line);

	MessageBoxA(NULL, output, "�G���[", MB_OK);

	exit(EXIT_FAILURE);
}
