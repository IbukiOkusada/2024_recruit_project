//==========================================================
//
// �e [bullet.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "billboard.h"
#include "particle.h"
#include "enemy_manager.h"
#include "player_manager.h"
#include "manager.h"
#include "slow.h"
#include "meshwall.h"

// �������O���
namespace {
	const char* FILENAME = "data\\MODEL\\bullet.x";	// �t�@�C����
	const int SETLIFE = (240);	// �e�̐ݒ����
	const int SETBOSSLIFE = (300);
	const float COLLRANGE = (25.0f);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CBullet::CBullet()
{
	// �l�̃N���A
	m_pObject = nullptr;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = 0;
	m_nId = -1;
	m_fInterVal = 0.0f;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CBullet::~CBullet()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CBullet::Init(void)
{
	// �I�u�W�F�N�g�̐���
	//m_pObject = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//m_pObject->SetCurrent(&m_Info.mtxWorld);

	// �����̐ݒ�
	m_fLife = SETLIFE;

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CBullet::Uninit(void)
{
	// �I�u�W�F�N�g�̏I��
	if (m_pObject != nullptr) {
		m_pObject->Uninit();
		m_pObject = nullptr;
	}

	// �J��
	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CBullet::Update(void)
{
	m_Info.posOld = m_Info.pos;

	// ����֘A
	Controller();

	// �}�g���b�N�X���f
	SetMatrix();

	// �����m�F
	if (m_fLife <= 0) {	// �������Ȃ��Ȃ���
		Uninit();	// �I��
	}
	else
	{
		m_fLife-= CManager::GetInstance()->GetSlow()->Get();

		// �����蔻��
		Hit();

		// �ړ����Ă���
		m_fInterVal += CManager::GetInstance()->GetSlow()->Get();

		if (m_fInterVal >= 2.0f) {
			if (m_type == TYPE_ENEMY) {
				CParticle::Create(m_Info.pos, CEffect::TYPE_KUNAI);
			}
			else {
				CEffect::Create(m_Info.pos, m_Info.move, D3DXCOLOR(0.0f, 0.5f, 0.5f, 0.5f), 20.0f, 10.0f, CEffect::TYPE_BOSSGUN);
			}
		}
	}
}

//==========================================================
// ����
//==========================================================
CBullet *CBullet::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move, const TYPE type)
{
	CBullet *pBullet = nullptr;

	pBullet = new CBullet;

	if (pBullet != nullptr)
	{
		// ����������
		pBullet->Init();

		// ���W���f
		pBullet->SetPosition(pos);

		// �������f
		pBullet->SetRotation(rot);

		// �ړ��ʔ��f
		pBullet->SetMove(move);

		// ��ޔ��f
		pBullet->SetType(type);

		if (type == TYPE_BOSS) {
			pBullet->m_fLife = SETBOSSLIFE;
		}
	}

	return pBullet;
}

//==========================================================
// �}�g���b�N�X�ݒ�
//==========================================================
void CBullet::SetMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
}

//==========================================================
// ����֘A
//==========================================================
void CBullet::Controller(void)
{
	// �ړ�
	m_Info.pos += m_Info.move * CManager::GetInstance()->GetSlow()->Get();
}

//==========================================================
// �q�b�g����
//==========================================================
void CBullet::Hit(void)
{
	bool bHit = false;
	
	// �v���C���[�Ƃ̔���
	if (m_type == TYPE_ENEMY) {
		CPlayer* pPlay = CPlayerManager::GetInstance()->GetTop();

		while (pPlay != nullptr) {
			CPlayer* pPlayNext = pPlay->GetNext();

			if (m_nId != pPlay->GetId()) {	// �����̂ł͂Ȃ�
				if (pPlay->HitCheck(m_Info.pos, COLLRANGE, COLLRANGE, 1)) {	// �������Ă���
					bHit = true;
				}
			}

			pPlay = pPlayNext;
		}
	}

	// �I�u�W�F�N�g�Ƃ̔���
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(COLLRANGE * 0.5f, COLLRANGE * 0.15f, COLLRANGE * 0.5f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLRANGE * 0.5f, -COLLRANGE * 0.15f, -COLLRANGE * 0.5f);

	int nType = 0;
	D3DXVECTOR3 moveOld = m_Info.move;
	D3DXVECTOR3 posOld = m_Info.pos;
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_X;
	CObjectX::Touch(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax);
	CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, axis, nType);

	if (m_Info.pos.x != posOld.x || m_Info.pos.z != posOld.z) {	// �������Ĉړ��ʂ��Ȃ��Ȃ���
		bHit = true;
	}

	D3DXVECTOR3 moveTemp = m_Info.move;

	if (bHit) {	// ��������
		Uninit();
	}
}