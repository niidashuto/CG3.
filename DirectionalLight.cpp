#include "DirectionalLight.h"

using namespace DirectX;

ID3D12Device* DirectionalLight::device = nullptr;

void DirectionalLight::StaticInitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!DirectionalLight::device);
	//nullptrチェック
	assert(device);
	//静的メンバ変数のセット
	DirectionalLight::device = device;
}

DirectionalLight* DirectionalLight::Create()
{
	//3Dオブジェクトのインスタンスを生成
	DirectionalLight* instance = new DirectionalLight();
	//初期化
	instance->Initialize();
	//生成したインスタンスを返す	
	return instance;
}

void DirectionalLight::Initialize()
{
	//ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(constBuff) + 0xff) & ~0xff;//頂点データ全体のサイズ
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	HRESULT result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{
	//正規化してセット
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}

void DirectionalLight::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
