#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

/// <summary>
/// ライト
/// </summary>
class Light
{
private://エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;//ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor;//ライトの色
	};
private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
public://静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	static Light* Create();
private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff;
	//ライト光線方向(単位ベクトル)
	XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty = false;

	D3D12_HEAP_PROPERTIES cbHeapProp{};

	D3D12_RESOURCE_DESC cbResourceDesc{};
public://メンバ関数

	void Initialize();

	void TransferConstBuffer();

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);
};

