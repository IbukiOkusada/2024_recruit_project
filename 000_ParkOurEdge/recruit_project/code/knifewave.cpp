//==========================================================
//
// �e [bullet.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "knifewave.h"
#include "enemy.h"
#include "player.h"
#include "objectX.h"
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
CKnifeWave::CKnifeWave()
{
	// �l�̃N���A
	m_pObject = nullptr;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = 0;
	m_nId = -1;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CKnifeWave::~CKnifeWave()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CKnifeWave::Init(void)
{
	// �I�u�W�F�N�g�̐���
	m_pObject = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\knifewave.x");
	//m_pObject->SetCurrent(&m_Info.mtxWorld);

	// �����̐ݒ�
	m_fLife = SETLIFE;

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CKnifeWave::Uninit(void)
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
void CKnifeWave::Update(void)
{
	m_Info.posOld = m_Info.pos;

	// ����֘A
	Controller();

	// �}�g���b�N�X���f
	SetMatrix();

	m_pObject->SetPosition(m_Info.pos);
	m_pObject->SetRotation(m_Info.rot);

	// �����m�F
	if (m_fLife <= 0) {	// �������Ȃ��Ȃ���
		Uninit();	// �I��
	}
	else
	{
		m_fLife-= CManager::GetInstance()->GetSlow()->Get();

		// �����蔻��
		Hit();
	}
}

//==========================================================
// ����
//==========================================================
CKnifeWave *CKnifeWave::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move, const TYPE type)
{
	CKnifeWave *pKnifeWave = nullptr;

	pKnifeWave = new CKnifeWave;

	if (pKnifeWave != nullptr)
	{
		// ����������
		pKnifeWave->Init();

		// ���W���f
		pKnifeWave->SetPosition(pos);

		// �������f
		pKnifeWave->SetRotation(rot);

		// �ړ��ʔ��f
		pKnifeWave->SetMove(move);

		// ��ޔ��f
		pKnifeWave->SetType(type);
	}

	return pKnifeWave;
}

//==========================================================
// �}�g���b�N�X�ݒ�
//==========================================================
void CKnifeWave::SetMatrix(void)
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
void CKnifeWave::Controller(void)
{
	// �ړ�
	m_Info.pos += m_Info.move * CManager::GetInstance()->GetSlow()->Get();
}

//==========================================================
// �q�b�g����
//==========================================================
void CKnifeWave::Hit(void)
{
	bool bHit = false;
	
	// �v���C���[�Ƃ̔���
	if (m_type == TYPE_ENEMY) {
		CPlayer* pPlay = CPlayerManager::GetInstance()->GetTop();

		while (pPlay != nullptr) {
			CPlayer* pPlayNext = pPlay->GetNext();

			if (m_nId != pPlay->GetId()) {	// �����̂ł͂Ȃ�
				if (pPlay->HitCheck(m_Info.pos, COLLRANGE, 200.0f, 1)) {	// �������Ă���
					bHit = true;
				}
			}

			pPlay = pPlayNext;
		}
	}

	// �I�u�W�F�N�g�Ƃ̔���
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(COLLRANGE * 0.5f, COLLRANGE * 0.15f, COLLRANGE * 0.5f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLRANGE * 0.5f, -COLLRANGE * 0.15f, -COLLRANGE * 0.5f);

	D3DXVECTOR3 moveOld = m_Info.move;
	D3DXVECTOR3 posOld = m_Info.pos;
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_X;
	CObjectX::Touch(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax);
	CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, axis);

	if (m_Info.pos.x != posOld.x || m_Info.pos.z != posOld.z) {	// �������Ĉړ��ʂ��Ȃ��Ȃ���
		bHit = true;
	}

	D3DXVECTOR3 moveTemp = m_Info.move;

	if (bHit) {	// ��������
		Uninit();
	}
}