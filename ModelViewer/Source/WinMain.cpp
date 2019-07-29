#include <Windows.h>
#include "SystemMain.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_method) {

	HWND hwnd;
	MSG msg;
	WNDCLASS window_class;

	// �E�B���h�E�N���X��`�E�o�^
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = TEXT("MYCLASS");

	if (RegisterClass(&window_class) == false) {
		return -1;
	}

	// �E�B���h�E�쐬
	int width = 800;
	int height = 600;

	hwnd = CreateWindow(
		TEXT("MYCLASS"),					// �E�B���h�E�N���X��
		TEXT("Breakout"),					// �E�B���h�E��
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// �E�B���h�E�X�^�C��
		CW_USEDEFAULT, CW_USEDEFAULT,		// �����ʒu
		width, height,						// �E�B���h�E�T�C�Y
		NULL,
		NULL,
		instance,
		NULL
	);

	if (hwnd == NULL) {
		return -1;
	}

	SystemMain system;
	if (!system.Initialize(hwnd)) {
		return -1;
	}

	// ���C�����[�v
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  // ���b�Z�[�W�L��
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {  // ���b�Z�[�W����

			// ����
			system.Main();

		}
	}

	return static_cast<int>(msg.wParam);
}


// �E�B���h�E�v���V�[�W���E�E�B���h�E����̃��b�Z�[�W�ɉ����ď�������֐�
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}