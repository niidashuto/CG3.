#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include <wrl.h>

class LightGroup
{
private:
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://�萔
	static const int DirLightNum = 3;
	//�_�����̐�
	static const int PointLightNum = 3;
public://�T�u�N���X
	
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
	};
private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
public://�ÓI�����o�֐�
	static void StaticInitialize(ID3D12Device* device);

	static LightGroup* Create();
private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�����̐F
	XMFLOAT3 ambientColor = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�[�e�B�t���O
	bool dirty = false;

	D3D12_HEAP_PROPERTIES cbHeapProp{};

	D3D12_RESOURCE_DESC cbResourceDesc{};
public://�����o�֐�
	void Initialize();

	void TransferConstBuffer();

	//�����̃��C�g�F�̃Z�b�g
	void SetAmbientColor(const XMFLOAT3& color);
	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index, bool active);
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	//���s�����̃��C�g�F���Z�b�g
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	//�W���̃��C�g�ݒ�
	void DefaultLightSetting();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList*cmdList,UINT rootParameterIndex);
};

