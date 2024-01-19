//==========================================================
//
// �G�̏��� [enemy.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy.h"
#include "enemy_manager.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================
// �R���X�g���N�^
//==========================================================
CEnemy::CEnemy()
{
	// �l�̃N���A
	m_Info.pos = { 0.0f, 0.0f, 0.0f };
	m_Info.rot = { 0.0f, 0.0f, 0.0f };
	m_Info.move = { 0.0f, 0.0f, 0.0f };
	m_Info.posOld = { 0.0f, 0.0f, 0.0f };
	m_nLife = 3;

	// ���X�g�ɑ}��
	CEnemyManager::GetInstance()->ListIn(this);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CEnemy::~CEnemy()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CEnemy::Init(void)
{
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CEnemy::Uninit(void)
{
	// ���X�g����O��
	CEnemyManager::GetInstance()->ListOut(this);
	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CEnemy::Update(void)
{
	// �}�g���b�N�X�ݒ�
	SetMatrix();

	CManager::GetInstance()->GetDebugProc()->Print("�G�̗̑� [ %d ]\n", m_nLife);
}

//===============================================
// �}�g���b�N�X�ݒ�
//===============================================
void CEnemy::SetMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
}

//===============================================
// �q�b�g����
//===============================================
void CEnemy::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{

}

//===============================================
// ��{��񃊃Z�b�g
//===============================================
void CEnemy::InfoReset(void)
{
	// �l�̃N���A
	m_Info.pos = { 0.0f, 0.0f, 0.0f };
	m_Info.rot = { 0.0f, 0.0f, 0.0f };
	m_Info.move = { 0.0f, 0.0f, 0.0f };
	m_Info.posOld = { 0.0f, 0.0f, 0.0f };
}