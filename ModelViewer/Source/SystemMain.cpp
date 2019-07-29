#include "SystemMain.h"
#include "Tool/Error.h"

namespace {

	UINT test_num_vertex = 24;
	DirectX11::Vertex test_vertex_array[]{
	{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },

	{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },

	{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },

	{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

	{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },

	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	};

	UINT test_num_index = 36;
	UINT test_index_array[]{
		 0,  1,  2,     3,  2,  1,
		 4,  5,  6,     7,  6,  5,
		 8,  9, 10,    11, 10,  9,
		12, 13, 14,    15, 14, 13,
		16, 17, 18,    19, 18, 17,
		20, 21, 22,    23, 22, 21,
	};

	const char * vertex_shader_path = "Source/Graphic/Shader/VertexShader.cso";
	const char * pixel_shader_path = "Source/Graphic/Shader/PixelShader.cso";
}


/*!
* @brief DirectX�̏�����
*/
bool SystemMain::Initialize(HWND hwnd) {

	directx.CreateDeviceAndSwapChain(hwnd);
	directx.CreateRenderTargetView();
	directx.CreateDepthStencilView();
	directx.CreateVertexBuffer(test_vertex_array, test_num_vertex);
	directx.CreateIndexBuffer(test_index_array, test_num_index);
	directx.CreateConstantBuffer();
	directx.CreateVertexShader(vertex_shader_path);
	directx.CreatePixelShader(pixel_shader_path);
	directx.CreateSamplerState();
	directx.SetViewPort();

	return true;
}

/*!
* @brief �I������
*/
void SystemMain::Finalize() const {

}

/*!
* @brief ���C������
*/
void SystemMain::Main() {

	// ��ʃN���A
	directx.Clear();

	// �X�V
	directx.Render(test_num_index);

	// �t���[���o�b�t�@�̍X�V
	directx.Present();
}
