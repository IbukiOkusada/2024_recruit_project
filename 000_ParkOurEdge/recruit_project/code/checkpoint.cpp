//==========================================================
//
// �`�F�b�N�|�C���g���� [checkpoinyt.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "checkpoint.h"
#include "checkpoint_manager.h"
#include "debugproc.h"
#include "manager.h"

// ���O���
namespace {
	const float COLSIZE = (150.0f);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CCheckPoint::CCheckPoint()
{
	// �l�̃N���A
	m_pos = { 0.0f, 0.0f, 0.0f };

	// ���X�g�ɑ}��
	CCheckPointManager::GetInstance()->ListIn(this);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCheckPoint::~CCheckPoint()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CCheckPoint::Init(void)
{
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CCheckPoint::Uninit(void)
{
	// ���X�g����O��
	CCheckPointManager::GetInstance()->ListOut(this);
	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CCheckPoint::Update(void)
{

}

//===============================================
// �q�b�g����
//===============================================
void CCheckPoint::Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos)
{
	if (pos.x < m_pos.x - COLSIZE || pos.x > m_pos.x + COLSIZE ||
		pos.z < m_pos.z - COLSIZE || pos.z > m_pos.z + COLSIZE)
	{
		return;
	}

	// ���g�̍��W��n���Ĕp��
	CheckPointPos = m_pos;
	Uninit();
}

//===============================================
// ����
//===============================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& pos)
{
	CCheckPoint* pCheckPoint = nullptr;

	pCheckPoint = new CCheckPoint;

	if (pCheckPoint != nullptr)
	{
		// ����������
		pCheckPoint->Init();

		pCheckPoint->SetPosition(pos);
	}

	return pCheckPoint;
}