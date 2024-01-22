//==========================================================
//
// �ߐڍU���̓G [enemy_melee.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_melee.h"
#include "waist.h"
#include "character.h"
#include "mesh.h"
#include "model.h"
#include "motion.h"
#include "player_manager.h"
#include "meshwall.h"
#include "objectX.h"

// �������O���
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\player\\motion_body.txt";	// �t�@�C���̃p�X
	const char* LEGFILEPASS = "data\\TXT\\player\\motion_leg.txt";	// �t�@�C���̃p�X
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };
	const int DAMAGEINTERVAL = (60);
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CEnemyMelee::CEnemyMelee()
{
	// �l�̃N���A
	m_nAction = 7;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_nInterVal = 0;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CEnemyMelee::~CEnemyMelee()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CEnemyMelee::Init(void)
{
	// ��ނ��ߋ����ɐݒ�
	SetType(TYPE_MELEE);

	SInfo* pInfo = GetInfo();

	// ���̐���
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&pInfo->mtxWorld);
		m_pWaist->SetMatrix();
	}

	// ���̂̐ݒ�
	m_pBody = CCharacter::Create(BODYFILEPASS);
	m_pBody->SetParent(m_pWaist->GetMtxWorld());
	m_pBody->SetShadow(true);
	m_pBody->SetDraw();

	if (m_pBody->GetMotion() != nullptr)
	{
		// �������[�V�����̐ݒ�
		m_pBody->GetMotion()->InitSet(m_nAction);
	}

	// �����g�̐ݒ�
	m_pLeg = CCharacter::Create(LEGFILEPASS);
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());
	m_pLeg->SetShadow(true);
	m_pLeg->SetDraw();

	if (m_pLeg->GetMotion() != nullptr)
	{
		// �������[�V�����̐ݒ�
		m_pLeg->GetMotion()->InitSet(m_nAction);
	}

	// ���̍��������킹��
	if (m_pLeg != nullptr)
	{// �r���g�p����Ă���ꍇ
		CModel* pModel = m_pLeg->GetParts(0);	// ���p�[�c���擾

		if (pModel != nullptr)
		{// �p�[�c�����݂���ꍇ
			D3DXVECTOR3 pos = pModel->GetPosition();	// ���f���̑��Έʒu���擾

			// ������ݒ�
			m_pWaist->SetHeight(pos);

			// ���̃��f���̈ʒu��ύX
			pModel->SetPosition(pos);
		}
	}

	// ����q�b�g����΍�
	m_nInterVal = 1;

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CEnemyMelee::Uninit(void)
{
	if (m_pBody != nullptr) {
		m_pBody->Uninit();
		delete m_pBody;
		m_pBody = nullptr;
	}

	if (m_pLeg != nullptr) {
		m_pLeg->Uninit();
		delete m_pLeg;
		m_pLeg = nullptr;
	}

	if (m_pWaist != nullptr) {
		delete m_pWaist;
		m_pWaist = nullptr;
	}

	// �e�N���X�̏I���Ăяo��
	CEnemy::Uninit();
}

//==========================================================
// �X�V����
//==========================================================
void CEnemyMelee::Update(void)
{
	// �O��̍��W���擾
	SInfo* pInfo = GetInfo();
	pInfo->posOld = pInfo->pos;
	m_nInterVal--;

	// �U���m�F
	AttackCheck();

	// �����蔻��m�F
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis);

	//�}�g���b�N�X�ݒ�
	CEnemy::Update();
	BodySet();
}

//==========================================================
// ����
//==========================================================
CEnemyMelee *CEnemyMelee::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyMelee *pEnemyMelee = nullptr;

	pEnemyMelee = new CEnemyMelee;

	if (pEnemyMelee != nullptr)
	{
		// ����������
		pEnemyMelee->Init();

		// �f�[�^���Z�b�g
		pEnemyMelee->InfoReset();

		// �f�[�^�ݒ�
		pEnemyMelee->SetPosition(pos);
		pEnemyMelee->SetRotation(rot);
	}

	return pEnemyMelee;
}

//===============================================
// �g�p�K�w�\���̐ݒ�
//===============================================
void CEnemyMelee::BodySet(void)
{
	// �����g�X�V
	if (BodyCheck(m_pLeg))
	{// �g�p����Ă���ꍇ

		// ���̐ݒ�
		if (m_pWaist != nullptr)
		{
			CModel* pModel = m_pLeg->GetParts(0);

			// ���̍������U
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pModel->GetOldPosition());
			m_pWaist->SetMatrix();
		}

		m_pLeg->Update();

		CModel* pModel = m_pLeg->GetParts(0);
		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// ���̍X�V
	if (BodyCheck(m_pBody))
	{// �g�p����Ă���ꍇ
		m_pBody->Update();
	}
}

//===============================================
// �̎g�p�m�F
//===============================================
bool CEnemyMelee::BodyCheck(CCharacter* pBody)
{
	if (pBody == nullptr) {	// �g�p����Ă���
		return false;
	}

	if (pBody->GetMotion() == nullptr) {	// ���[�V�������g�p����Ă���
		return false;
	}

	return true;
}

//===============================================
// �����蔻��m�F
//===============================================
void CEnemyMelee::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_nInterVal > 0) {
		return;
	}

	if (pos.x + fRange < pInfo->pos.x + COLLIMIN.x ||
		pos.x - fRange > pInfo->pos.x + COLLIMAX.x)
	{// X�͈͊O
		return;
	}

	if (pos.z + fRange < pInfo->pos.z + COLLIMIN.z ||
		pos.z - fRange > pInfo->pos.z + COLLIMAX.z)
	{// Z�͈͊O
		return;
	}

	if (pos.y + fRange < pInfo->pos.y + COLLIMIN.y ||
		pos.y - fRange > pInfo->pos.y + COLLIMAX.y)
	{// Y�͈͊O
		return;
	}

	// �_���[�W����
	Damage(nDamage);
}

//===============================================
// �_���[�W����
//===============================================
void CEnemyMelee::Damage(const int nDamage)
{
	if (m_nInterVal > 0) {	// �C���^�[�o�����߂��Ă��Ă��Ȃ�
		return;
	}

	// �C���^�[�o����ύX
	m_nInterVal = DAMAGEINTERVAL;

	// �̗͂����炷
	int nLife = GetLife();
	nLife -= nDamage;

	if (nLife <= 0) {	// �̗͂��Ȃ��Ȃ���
		Uninit();
	}
	else {	// �܂�����
		SetLife(nLife);
	}
}

//===============================================
// �U������m�F
//===============================================
void CEnemyMelee::AttackCheck(void)
{
	if (!BodyCheck(m_pBody) || !BodyCheck(m_pLeg)) {
		return;
	}

	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	CPlayerManager::GetInstance()->Hit(pos, fRange, nDamage);
}