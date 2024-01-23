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
#include "player.h"
#include "debugproc.h"
#include "manager.h"
#include "slow.h"

// �������O���
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\player\\motion_body.txt";	// �t�@�C���̃p�X
	const char* LEGFILEPASS = "data\\TXT\\player\\motion_leg.txt";	// �t�@�C���̃p�X
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// �����蔻��ő�
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// �����蔻��ŏ�
	const int DAMAGEINTERVAL = (60);	// �_���[�W�C���^�[�o��
	const float CHASE_MAXLENGTH = (1000.0f);	// �ǐՍŒ�����
	const float CHASE_NEARLENGTH = (400.0f);
	const float CHASE_MINLENGTH = (200.0f);
	const float SEARCH_HEIGHT = (180.0f);	// �T����������
	const float MOVE_INER = (0.3f);			// �ړ�����
}

// �ړ����x���O���
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// �������ړ�
	const float MOVE_NEAR = (1.0f);	// �ߋ����ړ�
	const float MOVE_MIN = (0.25f);
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
	m_Chase.pTarget = nullptr;
	m_Chase.fLength = 0.0f;
	m_nInterVal = 0;
	m_StateInfo.state = STATE_APPEAR;
	m_StateInfo.fCounter = 0.0f;
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
	{
		SInfo* pInfo = GetInfo();
		pInfo->posOld = pInfo->pos;
	}
	m_nInterVal--;

	// ����
	MethodLine();

	//�}�g���b�N�X�ݒ�
	CEnemy::Update();
	BodySet();
}

//===============================================
// �����̏���
//===============================================
void CEnemyMelee::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	// �U���m�F
	AttackCheck();

	if (m_StateInfo.state != STATE_DEATH || m_StateInfo.state != STATE_DAMAGE) {

		// �{��
		m_Chase.pTarget = Search(m_Chase.fLength);

		// �ǐ�
		Chase();

		// �ړ�
		AddMove();
	}

	// �����蔻��m�F
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis);
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
		pEnemyMelee->SetRotDiff(rot.y);
		pEnemyMelee->SetIner(MOVE_INER);
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

	if (m_StateInfo.state != STATE_ATTACK) {
		return;
	}

	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	CPlayerManager::GetInstance()->Hit(pos, fRange, nDamage);
}

//===============================================
// �v���C���[��T��
//===============================================
CPlayer* CEnemyMelee::Search(float& fChaseLength)
{
	CPlayer* pPlayer = CPlayerManager::GetInstance()->GetTop();
	CPlayer* pChasePlayer = nullptr;
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// �����̍��W
	float fMinLength = CHASE_MAXLENGTH;	// �ǐՂ���v���C���[�Ƃ̋���
	fChaseLength = fMinLength;

	// �v���C���[�S�����J��Ԃ�
	while (pPlayer != nullptr) {
		CPlayer* pPlayerNext = pPlayer->GetNext();

		D3DXVECTOR3 pos = pPlayer->GetPosition();

		if (pos.y < MyPos.y - SEARCH_HEIGHT || pos.y > MyPos.y + SEARCH_HEIGHT) {	// �������m�͈͊O
			pPlayer = pPlayerNext;
			continue;
		}

		// �������擾
		float fLength = sqrtf((pos.x - MyPos.x) * (pos.x - MyPos.x)
			+ (pos.z - MyPos.z) * (pos.z - MyPos.z));

		// �ǐՊm�F
		if (fLength < fMinLength) {	// ���݌��Ă���v���C���[�̕����߂��ꍇ
			fMinLength = fLength;	// ��ԋ߂��������㏑��
			fChaseLength = fLength;
			pChasePlayer = pPlayer;	// �ǐՂ���v���C���[��ݒ�
		}

		pPlayer = pPlayerNext;
	}

	return pChasePlayer;
}

//===============================================
// �`�F�C�X������
//===============================================
void CEnemyMelee::Chase(void)
{
	if (m_Chase.pTarget == nullptr) {
		return;
	}

	CManager::GetInstance()->GetDebugProc()->Print("�v���C���[�Ƃ̋��� [%f]\n", m_Chase.fLength);

	// �ڕW�����ݒ�
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// �����̍��W
	D3DXVECTOR3 pos = m_Chase.pTarget->GetPosition();
	{
		float fDiff = GetRotDiff();
		fDiff = atan2f(pos.x - MyPos.x, pos.z - MyPos.z) + D3DX_PI;
		if (fDiff < -D3DX_PI) {
			fDiff += D3DX_PI * 2;
		}
		else if (fDiff > D3DX_PI) {
			fDiff += -D3DX_PI * 2;
		}
		SetRotDiff(fDiff);
	}

	// �ړ��ʂ�ݒ�
	{
		float fSpeed = SPEED::MOVE_FAR;
		if (m_Chase.fLength <= CHASE_NEARLENGTH && 
			m_Chase.fLength > CHASE_MINLENGTH) {	// �ߋ�������̋���
			fSpeed = SPEED::MOVE_NEAR + (rand() % 2) * SPEED::MOVE_MIN;
		}
		else if (m_Chase.fLength <= CHASE_MINLENGTH){	// �ł��߂���������
			fSpeed = SPEED::MOVE_MIN + (rand() % 3) * SPEED::MOVE_MIN;	// �ړ��ʂɏ��������_������������
		}
		D3DXVECTOR3 move = GetMove();
		D3DXVECTOR3 nor = pos - MyPos;
		nor.y = 0.0f;
		D3DXVec3Normalize(&nor, &nor);
		move += nor * fSpeed;

		SetMove(move);
	}
}

//===============================================
// ��Ԑݒ�
//===============================================
void CEnemyMelee::SetState(void)
{
	m_StateInfo.fCounter -= CManager::GetInstance()->GetSlow()->Get();

	switch (m_StateInfo.state) {
	case STATE_APPEAR:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// �J�E���^�[�I��
			m_StateInfo.state = STATE_NORMAL;
		}
	}
		break;

	case STATE_NORMAL:
	{

	}
		break;

	case STATE_CHASE:
	{

	}
		break;

	case STATE_ATTACK:
	{
		
	}
		break;

	case STATE_DAMAGE:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// �J�E���^�[�I��
			m_StateInfo.state = STATE_APPEAR;
		}
	}
		break;

	case STATE_DEATH:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// �J�E���^�[�I��
			Uninit();
		}
	}
		break;

	default:
	{

	}
		break;
	}
}