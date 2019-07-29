#include <stdio.h>
#include "DirectX11.h"
#include "../Tool/Error.h"

DirectX11::DirectX11() {

	Release();
}

DirectX11::~DirectX11() {

	Release();
}

/*!
* @brief 各パラメータの解放
*/
void DirectX11::Release() {

	if (device_context_ != nullptr) {
		device_context_->ClearState();
		device_context_->Flush();
	}

	SafeRelease(sampler_state_);

	SafeRelease(pixel_shader_);
	SafeRelease(vertex_shader_);
	SafeRelease(input_layout_);

	SafeRelease(constant_buffer_);
	SafeRelease(index_buffer_);
	SafeRelease(vertex_buffer_);
	
	SafeRelease(ds_shader_resource_view_);
	SafeRelease(depth_stencil_view_);
	SafeRelease(depth_stencil_texture_);
	
	SafeRelease(render_target_view_);
	SafeRelease(swap_chain_);
	SafeRelease(device_);
}

/*!
* @brief バックバッファ・デプスバッファのクリア
*/
void DirectX11::Clear() {

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	device_context_->ClearRenderTargetView(render_target_view_, clearColor);
	device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/*!
* @brief バックバッファへの描画
* @param num_index	描画時に利用する頂点インデックス数
*/
void DirectX11::Render(UINT num_index) {

	XMMATRIX worldMatrix = XMMatrixTranslation( 0.0f, 0.0f, 0.0f );
         
	float radius = 3.0f;
	static float deg = -90.0f;
	float rad = XMConvertToRadians(deg += 0.01f);

    XMVECTOR eye         = XMVectorSet( radius * cosf(rad), 1.0f, radius * sinf(rad), 0.0f );
    XMVECTOR focus       = XMVectorSet( 0.0f, 0.0f,  0.0f, 0.0f );
    XMVECTOR up          = XMVectorSet( 0.0f, 1.0f,  0.0f, 0.0f );
    XMMATRIX viewMatrix  = XMMatrixLookAtLH( eye, focus, up );
 
    float    fov         = XMConvertToRadians( 45.0f );
    float    aspect      = viewport_.Width / viewport_.Height;
    float    nearZ       =   0.1f;
    float    farZ        = 100.0f;
    XMMATRIX projMatrix  = XMMatrixPerspectiveFovLH( fov, aspect, nearZ, farZ );
 
    ConstantBuffer cb;
    XMStoreFloat4x4( &cb.world,      XMMatrixTranspose( worldMatrix ) );
    XMStoreFloat4x4( &cb.view,       XMMatrixTranspose( viewMatrix ) );
    XMStoreFloat4x4( &cb.projection, XMMatrixTranspose( projMatrix ) );
    device_context_->UpdateSubresource( constant_buffer_, 0, nullptr, &cb, 0, 0 );
 
 
    UINT strides = sizeof( Vertex );
    UINT offsets = 0;
	device_context_->IASetInputLayout( input_layout_ );
    device_context_->IASetVertexBuffers( 0, 1, &vertex_buffer_, &strides, &offsets );
    device_context_->IASetIndexBuffer( index_buffer_, DXGI_FORMAT_R32_UINT, 0 );
    device_context_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    device_context_->VSSetConstantBuffers( 0, 1, &constant_buffer_ );
    device_context_->VSSetShader( vertex_shader_, nullptr, 0 );
    
	device_context_->RSSetViewports( 1, &viewport_ );
    
	device_context_->PSSetShaderResources(0, 1, &ds_shader_resource_view_);
	device_context_->PSSetSamplers(0, 1, &sampler_state_);
	device_context_->PSSetShader( pixel_shader_, nullptr, 0 );

	device_context_->OMSetDepthStencilState(depth_stencil_state_, 0);
	device_context_->OMSetRenderTargets( 1, &render_target_view_, depth_stencil_view_ );

    device_context_->DrawIndexed( num_index, 0, 0 );
 
}

/*!
* @brief フロントバッファへの表示
*/
void DirectX11::Present() {

	swap_chain_->Present(0, 0);
}

/*!
* @brief スワップチェインの作成
* @param hwnd	ハンドルウィンドウ・ウィンドウサイズ取得
*/
HRESULT DirectX11::CreateDeviceAndSwapChain(HWND hwnd) {

	HRESULT  hresult;

	UINT flags = 0;
	D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL level;

	DXGI_SWAP_CHAIN_DESC sc_desc;
	GetClientRect(hwnd, &rect);
	ZeroMemory(&sc_desc, sizeof(sc_desc));
	sc_desc.BufferCount = 1;
	sc_desc.BufferDesc.Width = rect.right;
	sc_desc.BufferDesc.Height = rect.bottom;
	sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc_desc.OutputWindow = hwnd;
	sc_desc.SampleDesc.Count = 1;
	sc_desc.SampleDesc.Quality = 0;
	sc_desc.Windowed = TRUE;

	hresult = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		pLevels,
		1,
		D3D11_SDK_VERSION,
		&sc_desc,
		&swap_chain_,
		&device_,
		&level,
		&device_context_);

	if (FAILED(hresult)) {
		SHOW_ERROR("スワップチェインの作成失敗");
	}

	return hresult;
}

/*!
* @brief レンダーターゲットビューの作成
*/
HRESULT DirectX11::CreateRenderTargetView() {

	HRESULT  hresult;

	ID3D11Texture2D* back_buffer = nullptr;

	hresult = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
	if (FAILED(hresult)) {
		SHOW_ERROR("スワップチェインからバックバッファの取得失敗");
		return hresult;
	}

	hresult = device_->CreateRenderTargetView(back_buffer, nullptr, &render_target_view_);
	back_buffer->Release();
	if (FAILED(hresult)) {
		SHOW_ERROR("レンダーターゲットビューの作成失敗");
	}

	return hresult;
}

/*!
* @brief デプスステンシルビューの作成
*/
HRESULT DirectX11::CreateDepthStencilView() {

	HRESULT hresult;

	D3D11_TEXTURE2D_DESC tx_desc;
	ZeroMemory(&tx_desc, sizeof(tx_desc));
	tx_desc.Width = rect.right;
	tx_desc.Height = rect.bottom;
	tx_desc.MipLevels = 1;
	tx_desc.ArraySize = 1;
	tx_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tx_desc.SampleDesc.Count = 1;
	tx_desc.SampleDesc.Quality = 0;
	tx_desc.Usage = D3D11_USAGE_DEFAULT;
	tx_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tx_desc.CPUAccessFlags = 0;
	tx_desc.MiscFlags = 0;
	hresult = device_->CreateTexture2D(&tx_desc, nullptr, &depth_stencil_texture_);

	if (FAILED(hresult)) {
		SHOW_ERROR("デプスステンシルテクスチャの作成失敗");
		return hresult;
	}
	

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hresult = device_->CreateDepthStencilView(depth_stencil_texture_, &dsv_desc, &depth_stencil_view_);
	if (FAILED(hresult)) {
		SHOW_ERROR("デプスステンシルビューの作成失敗");
		return hresult;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;
	hresult = device_->CreateShaderResourceView(depth_stencil_texture_, &srv_desc, &ds_shader_resource_view_);
	if (FAILED(hresult)) {
		SHOW_ERROR("デプスバッファのシェーダリソースビューの作成失敗");
		return hresult;
	}


	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ZeroMemory(&ds_desc, sizeof(ds_desc));
	ds_desc.DepthEnable = true;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	hresult = device_->CreateDepthStencilState(&ds_desc, &depth_stencil_state_);
	if (FAILED(hresult)) {
		SHOW_ERROR("デプスステンシルステートの作成失敗");
	}

	return hresult;
}

/*!
* @brief 頂点バッファの作成
* @param g_VertexList	頂点データの配列
*/
HRESULT DirectX11::CreateVertexBuffer(Vertex * g_VertexList, UINT num_vertex) {

	HRESULT hresult;

	D3D11_BUFFER_DESC vb_desc;
	ZeroMemory(&vb_desc, sizeof(vb_desc));
	vb_desc.ByteWidth = sizeof(Vertex) * num_vertex;
	vb_desc.Usage = D3D11_USAGE_DEFAULT;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vr_data;
	ZeroMemory(&vr_data, sizeof(vr_data));
	vr_data.pSysMem = g_VertexList;
	vr_data.SysMemPitch = 0;
	vr_data.SysMemSlicePitch = 0;

	hresult = device_->CreateBuffer(&vb_desc, &vr_data, &vertex_buffer_);
	if (FAILED(hresult)) {
		SHOW_ERROR("頂点バッファの作成失敗");
	}

	return hresult;
}

/*!
* @brief インデックスバッファの作成
* @param g_IndexList	頂点インデックスの配列
*/
HRESULT DirectX11::CreateIndexBuffer(UINT * g_IndexList, UINT num_index) {

	HRESULT hresult;

	D3D11_BUFFER_DESC ib_desc;
	ZeroMemory(&ib_desc, sizeof(ib_desc));
	ib_desc.ByteWidth = sizeof(UINT) * num_index;
	ib_desc.Usage = D3D11_USAGE_DEFAULT;
	ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib_desc.CPUAccessFlags = 0;
	ib_desc.MiscFlags = 0;
	ib_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ir_data;
	ZeroMemory(&ir_data, sizeof(ir_data));
	ir_data.pSysMem = g_IndexList;
	ir_data.SysMemPitch = 0;
	ir_data.SysMemSlicePitch = 0;

	hresult = device_->CreateBuffer(&ib_desc, &ir_data, &index_buffer_);
	if (FAILED(hresult)) {
		SHOW_ERROR("インデックスバッファの作成失敗");
	}
	
	return hresult;
}

/*!
* @brief 定数バッファの作成
*/
HRESULT DirectX11::CreateConstantBuffer() {

	HRESULT hresult;

	//定数バッファ作成
	D3D11_BUFFER_DESC cb_desc;
	ZeroMemory(&cb_desc, sizeof(cb_desc));
	cb_desc.ByteWidth = sizeof(ConstantBuffer);
	cb_desc.Usage = D3D11_USAGE_DEFAULT;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = 0;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	hresult = device_->CreateBuffer(&cb_desc, nullptr, &constant_buffer_);
	if (FAILED(hresult)) {
		SHOW_ERROR("定数バッファの作成失敗");
	}
	
	return hresult;
}

/*!
* @brief 頂点シェーダ・入力レイアウトの作成
* @param path	頂点シェーダ（.cso）のパス 
*/
HRESULT DirectX11::CreateVertexShader(const char * path) {

	HRESULT hresult;

	errno_t error;
	FILE* fp;
	error = fopen_s(&fp, path, "rb");

	if (error != 0) {
		SHOW_ERROR("頂点シェーダファイルの読込失敗");
		return E_FAIL;
	}

	fseek(fp, 0, SEEK_END);
	UINT vs_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	UCHAR * vs_buffer = new UCHAR[vs_size];
	fread(vs_buffer, sizeof(UCHAR), vs_size, fp);

	hresult = device_->CreateVertexShader(vs_buffer, vs_size, nullptr, &vertex_shader_);
	if (FAILED(hresult)) {
		SHOW_ERROR("頂点シェーダの作成失敗");
		delete[] vs_buffer;
		fclose(fp);
		return hresult;
	}

	D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hresult = device_->CreateInputLayout(g_VertexDesc, ARRAYSIZE(g_VertexDesc), vs_buffer, vs_size, &input_layout_);
	if (FAILED(hresult)) {
		SHOW_ERROR("入力レイアウトの作成失敗");
	}

	delete[] vs_buffer;
	fclose(fp);
	
	return hresult;
}

/*!
* @brief ピクセルシェーダの作成
* @param path	ピクセルシェーダ（.cso）のパス
*/
HRESULT DirectX11::CreatePixelShader(const char * path) {

	HRESULT hresult;

	errno_t error;
	FILE* fp;
	error = fopen_s(&fp, path, "rb");

	if (error != 0) {
		SHOW_ERROR("ピクセルシェーダファイルの読込失敗");
		return E_FAIL;
	}

	fseek(fp, 0, SEEK_END);
	UINT px_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	UCHAR * px_buffer = new UCHAR[px_size];
	fread(px_buffer, sizeof(UCHAR), px_size, fp);

	hresult = device_->CreatePixelShader(px_buffer, px_size, nullptr, &pixel_shader_);
	if (FAILED(hresult)) {
		SHOW_ERROR("ピクセルシェーダの作成失敗");
	}

	delete[] px_buffer;
	fclose(fp);

	return hresult;
}

/*!
* @brief サンプラーの作成
*/
HRESULT DirectX11::CreateSamplerState() {

	HRESULT hresult;

	D3D11_SAMPLER_DESC smp_desc;
	ZeroMemory(&smp_desc, sizeof(smp_desc));
	smp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smp_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smp_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smp_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smp_desc.MinLOD = 0;
	smp_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hresult = device_->CreateSamplerState(&smp_desc, &sampler_state_);
	if (FAILED(hresult)) {
		SHOW_ERROR("サンプラーの作成失敗");
	}
	
	return hresult;
}

/*!
* @brief ビューポートの設定
*/
void DirectX11::SetViewPort() {

	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.Width = static_cast<float>(rect.right);
	viewport_.Height = static_cast<float>(rect.bottom);
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}
