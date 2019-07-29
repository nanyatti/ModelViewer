#pragma once
#include "Graphic/DirectX11.h"

/*!
* @brief DirectX11‚ð—˜—p‚µ‚½ƒ‚ƒfƒ‹•`‰æ
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