#pragma once
#include "Graphic/DirectX11.h"

/*!
* @brief DirectX11を利用したモデル描画
*/
class SystemMain {

public:

	SystemMain() {}
	~SystemMain() {}
	bool Initialize(HWND hwnd);
	void Finalize() const;
	void Main();

private:

	DirectX11 directx;
};