#pragma once
#include "Graphic/DirectX11.h"

/*!
* @brief DirectX11�𗘗p�������f���`��
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