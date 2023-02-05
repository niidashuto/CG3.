#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include <wrl.h>

class LightGroup
{
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://定数
	static const int DirLightNum = 3;
	//点光源の数
	static const int PointLightNum = 3;
public://サブクラス
	
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
	};
private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
public://静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	static LightGroup* Create();
private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//環境光の色
	XMFLOAT3 ambientColor = { 1,1,1 };
	//平行光源の配列
	DirectionalLight dirLights[DirLightNum];
	//ダーティフラグ
	bool dirty = false;

	D3D12_HEAP_PROPERTIES cbHeapProp{};

	D3D12_RESOURCE_DESC cbResourceDesc{};
public://メンバ関数
	void Initialize();

	void TransferConstBuffer();

	//環境光のライト色のセット
	void SetAmbientColor(const XMFLOAT3& color);
	//平行光源の有効フラグをセット
	void SetDirLightActive(int index, bool active);
	//平行光源のライト方向をセット
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	//平行光源のライト色をセット
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	//標準のライト設定
	void DefaultLightSetting();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList*cmdList,UINT rootParameterIndex);
};

