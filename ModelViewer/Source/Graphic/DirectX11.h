#pragma once
#pragma comment(lib,"d3d11.lib")
#include <d3d11_2.h>
#include <DirectXMath.h>

using namespace DirectX;

class DirectX11 {

public:

	struct Vertex {
		float position[3];
		float color[4];
	};

	struct ConstantBuffer {
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

public:

	DirectX11();
	~DirectX11();
	void Release();
	void Clear();
	void Render(UINT num_index);
	void Present();
	HRESULT CreateDeviceAndSwapChain(HWND hwnd);
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencilView();
	HRESULT CreateVertexBuffer(Vertex * g_VertexList, UINT num_vertex);
	HRESULT CreateIndexBuffer(UINT * g_IndexList, UINT num_index);
	HRESULT CreateConstantBuffer();
	HRESULT CreateVertexShader(const char * path);
	HRESULT CreatePixelShader(const char * path);
	HRESULT CreateSamplerState();
	void SetViewPort();

private:
	
	template <typename Type>
	void SafeRelease(Type & val){
		if (val != nullptr) {
			val->Release();
			val = nullptr;
		}
	}

public:

	RECT rect;
	ID3D11Device*           device_;
	ID3D11DeviceContext*    device_context_;
	IDXGISwapChain*         swap_chain_;

	ID3D11RenderTargetView* render_target_view_;

	ID3D11Texture2D*        depth_stencil_texture_;
	ID3D11DepthStencilView* depth_stencil_view_;
	ID3D11ShaderResourceView* ds_shader_resource_view_;
	ID3D11DepthStencilState* depth_stencil_state_;

	ID3D11Buffer*           vertex_buffer_;
	ID3D11Buffer*           index_buffer_;
	ID3D11Buffer*           constant_buffer_;

	ID3D11InputLayout*      input_layout_;
	ID3D11VertexShader*     vertex_shader_;
	ID3D11PixelShader*      pixel_shader_;

	ID3D11SamplerState*		sampler_state_;


	D3D11_VIEWPORT          viewport_;

};