//==========================================================
//
// �|��Ă��鋴 [bridge.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "bridge.h"
#include "objectX.h"

namespace {
	const float ROT_SPEED = (-D3DX_PI * 0.02f);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CBridge::CBridge()
{
	// �l�̃N���A
	m_bClose = false;
	m_pObj = nullptr;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBridge::~CBridge()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CBridge::Init(void)
{
	m_pObj = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\bridge.x");
	m_pObj->SetEnableCollision(false);
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CBridge::Uninit(void)
{
	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	// �J������
	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CBridge::Update(void)
{
	if (!m_bClose) {
		return;
	}

	if (m_pObj == nullptr) {
		return;
	}

	D3DXVECTOR3 rot = m_pObj->GetRotation();
	rot.z += ROT_SPEED;
	bool bEnd = false;

	if (rot.z >= D3DX_PI){
		rot.z = D3DX_PI;
		bEnd = true;
	}
	else if (rot.z <= 0.0f) {
		rot.z = 0.0f;
		bEnd = true;
	}

	m_pObj->SetRotation(rot);

	if (bEnd) {
		m_bClose = false;
		m_pObj->SetEnableCollision(true);
	}
}

//==========================================================
// ����
//==========================================================
CBridge *CBridge::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	CBridge *pBridge = nullptr;

	pBridge = new CBridge;

	if (pBridge != nullptr)
	{
		// ����������
		pBridge->Init();

		// �p�����[�^�ݒ�
		pBridge->SetData(pos, rot);
	}

	return pBridge;
}

void CBridge::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_pObj == nullptr) {
		return;
	}

	D3DXVECTOR3 Rot = rot;
	Rot.z = D3DX_PI * 0.5f;
	m_pObj->SetPosition(pos);
	m_pObj->SetRotation(Rot);
}