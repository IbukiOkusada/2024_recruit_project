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
#include "slow.h"

namespace {
	const float ROT_MULTI = (0.1f);	// �����␳�{��
	const float BUMP_SIZE = (25.0f);
}

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
	m_nLife = 8;
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fMoveIner = 0.0f;
	m_fRotMulti = ROT_MULTI;
	m_bLockOn = true;
	m_fBumpSize = BUMP_SIZE;

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
	// ���݂̌������l��
	m_fRotMove = m_Info.rot.y;
	Adjust();

	// �}�g���b�N�X�ݒ�
	SetMatrix();

	if (m_Info.pos.y <= -100.0f) {
		Uninit();
	}
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
bool CEnemy::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	return false;
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

//===============================================
// ����
//===============================================
void CEnemy::Adjust(void)
{
	while (1)
	{
		if (m_fRotDiff > D3DX_PI || m_fRotDiff < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_fRotDiff > D3DX_PI)
			{
				m_fRotDiff += (-D3DX_PI * 2);
			}
			else if (m_fRotDiff < -D3DX_PI)
			{
				m_fRotDiff += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	float fRotDest = m_fRotDiff - m_fRotMove;	//�ڕW�܂ł̈ړ������̍���

	while (1)
	{
		if (fRotDest > D3DX_PI || fRotDest < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (fRotDest > D3DX_PI)
			{
				fRotDest += (-D3DX_PI * 2);
			}
			else if (fRotDest < -D3DX_PI)
			{
				fRotDest += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_Info.rot.y += fRotDest * m_fRotMulti;

	while (1)
	{
		if (m_Info.rot.y > D3DX_PI || m_Info.rot.y < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_Info.rot.y > D3DX_PI)
			{
				m_Info.rot.y += (-D3DX_PI * 2);
			}
			else if (m_Info.rot.y < -D3DX_PI)
			{
				m_Info.rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}
}

//===============================================
// �ړ��ʉ��Z
//===============================================
void CEnemy::AddMove(void)
{
	m_Info.move.x += (0.0f - m_Info.move.x) * m_fMoveIner;	//x���W
	m_Info.move.z += (0.0f - m_Info.move.z) * m_fMoveIner;	//x���W

	m_Info.pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	m_Info.pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();
}
