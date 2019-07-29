#include <Windows.h>
#include "SystemMain.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_method) {

	HWND hwnd;
	MSG msg;
	WNDCLASS window_class;

	// ウィンドウクラス定義・登録
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

	// ウィンドウ作成
	int width = 800;
	int height = 600;

	hwnd = CreateWindow(
		TEXT("MYCLASS"),					// ウィンドウクラス名
		TEXT("Breakout"),					// ウィンドウ名
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// ウィンドウスタイル
		CW_USEDEFAULT, CW_USEDEFAULT,		// 初期位置
		width, height,						// ウィンドウサイズ
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

	// メインループ
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  // メッセージ有り
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {  // メッセージ無し

			// 処理
			system.Main();

		}
	}

	return static_cast<int>(msg.wParam);
}


// ウィンドウプロシージャ・ウィンドウからのメッセージに応じて処理する関数
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