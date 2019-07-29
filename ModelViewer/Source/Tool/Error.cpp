#include <Windows.h>
#include <stdio.h>
#include "Error.h"

/*!
* @brief エラーのメッセージボックス表示
* @param message	エラーメッセージ
* @param func_name	呼び出し元の関数名
* @param line		呼び出し元の行数
*/
void Error::Show(const char * message, const char * func_name, int line) {

	char output[1024];
	sprintf_s(output, "%s\n%s(%d)", message, func_name, line);

	MessageBoxA(NULL, output, "エラー", MB_OK);

	exit(EXIT_FAILURE);
}
