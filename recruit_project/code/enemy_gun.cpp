//==========================================================
//
// �������U���̓G [enemy_gun.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_gun.h"
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
#include "meshfield.h"

// �������O���
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\enemy\\motion_body.txt";	// �t�@�C���̃p�X
	const char* LEGFILEPASS = "data\\TXT\\enemy\\motion_leg.txt";	// �t�@�C���̃p�X
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// �����蔻��ő�
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// �����蔻��ŏ�
	const int DAMAGEINTERVAL = (60);	// �_���[�W�C���^�[�o��
	const float CHASE_MAXLENGTH = (1000.0f);	// �ǐՍŒ�����
	const float CHASE_NEARLENGTH = (700.0f);	// �ǐՋߋ���
	const float CHASE_MINLENGTH = (400.0f);		// �ǐ�0����
	const float SEARCH_HEIGHT = (180.0f);		// �T����������
	const float MOVE_INER = (0.3f);			// �ړ�����
}

// �ړ����x���O���
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// �������ړ�
	const float MOVE_NEAR = (-1.0f);	// �ߋ����ړ�
	const float MOVE_MIN = (0.15f);	// �ړ��ʈړ�
	const float GRAVITY = (-0.9f);	// �d��
	const float DAMAGE_MOVE = (2.0f);	// �ړ���
}

// �C���^�[�o��
namespace INTERVAL
{
	const float DAMAGE = (30.0f);	// �_���[�W
	const float ATTACK = (30.0f);	// �U��
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CEnemyGun::CEnemyGun()
{
	// �l�̃N���A
	m_nAction = ACTION_NEUTRAL;
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
CEnemyGun::~CEnemyGun()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CEnemyGun::Init(void)
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
void CEnemyGun::Uninit(void)
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
void CEnemyGun::Update(void)
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
void CEnemyGun::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	if (m_StateInfo.state != STATE_DEATH || m_StateInfo.state != STATE_DAMAGE) {

		// �{��
		m_Chase.pTarget = Search(m_Chase.fLength);

		// �ǐ�
		LockOn();

		// �ړ�
		AddMove();
	}

	// �d��
	Gravity();

	// �����蔻��m�F
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis);
	float fHeight = CMeshField::GetHeight(pInfo->pos);
	if (pInfo->pos.y < fHeight && pInfo->posOld.y >= fHeight) {
		pInfo->pos.y = fHeight;
		pInfo->move.y = 0.0f;
	}

	// �A�N�V�����ݒ�
	SetMotion();

	// ��ԊǗ�
	SetState();
}

//==========================================================
// ����
//==========================================================
CEnemyGun *CEnemyGun::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyGun *pEnemyGun = nullptr;

	pEnemyGun = new CEnemyGun;

	if (pEnemyGun != nullptr)
	{
		// ����������
		pEnemyGun->Init();

		// �f�[�^���Z�b�g
		pEnemyGun->InfoReset();

		// �f�[�^�ݒ�
		pEnemyGun->SetPosition(pos);
		pEnemyGun->SetRotation(rot);
		pEnemyGun->SetRotDiff(rot.y);
		pEnemyGun->SetIner(MOVE_INER);
	}

	return pEnemyGun;
}

//===============================================
// �g�p�K�w�\���̐ݒ�
//===============================================
void CEnemyGun::BodySet(void)
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
bool CEnemyGun::BodyCheck(CCharacter* pBody)
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
bool CEnemyGun::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_nInterVal > 0) {
		return false;
	}

	if (pos.x + fRange < pInfo->pos.x + COLLIMIN.x ||
		pos.x - fRange > pInfo->pos.x + COLLIMAX.x)
	{// X�͈͊O
		return false;
	}

	if (pos.z + fRange < pInfo->pos.z + COLLIMIN.z ||
		pos.z - fRange > pInfo->pos.z + COLLIMAX.z)
	{// Z�͈͊O
		return false;
	}

	if (pos.y + fRange < pInfo->pos.y + COLLIMIN.y ||
		pos.y - fRange > pInfo->pos.y + COLLIMAX.y)
	{// Y�͈͊O
		return false;
	}

	// �_���[�W����
	Damage(nDamage);
	return true;
}

//===============================================
// �_���[�W����
//===============================================
void CEnemyGun::Damage(const int nDamage)
{
	if (m_nInterVal > 0) {	// �C���^�[�o�����߂��Ă��Ă��Ȃ�
		return;
	}

	// ��Ԋm�F
	if (m_StateInfo.state == STATE_APPEAR || m_StateInfo.state == STATE_DEATH) {	// �_���[�W���󂯂Ȃ����
		return;
	}

	// �C���^�[�o����ύX
	m_nInterVal = DAMAGEINTERVAL;

	// �̗͂����炷
	int nLife = GetLife();
	nLife -= nDamage;

	if (nLife <= 0) {	// �̗͂��Ȃ��Ȃ���
		m_StateInfo.state = STATE_DEATH;
		m_nAction = ACTION_DEATH;
	}
	else {	// �܂�����
		SetLife(nLife);
		m_StateInfo.state = STATE_DAMAGE;
		m_StateInfo.fCounter = INTERVAL::DAMAGE;
	}
}

//===============================================
// �v���C���[��T��
//===============================================
CPlayer* CEnemyGun::Search(float& fChaseLength)
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
void CEnemyGun::LockOn(void)
{
	if (m_Chase.pTarget == nullptr) {
		if (m_StateInfo.state == STATE_LOCKON) {
			m_StateInfo.state = STATE_NORMAL;
		}
		return;
	}

	if (m_StateInfo.state >= STATE_DAMAGE) {
		if (m_StateInfo.state == STATE_LOCKON) {
			m_StateInfo.state = STATE_NORMAL;
		}
		return;
	}

	m_StateInfo.state = STATE_LOCKON;

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
		float fSpeed = 0.0f;
		if (m_Chase.fLength >= CHASE_NEARLENGTH) {	// �K��������艓��
			fSpeed = SPEED::MOVE_FAR;
			m_nAction = ACTION_WALK;
		}
		else if (m_Chase.fLength <= CHASE_MINLENGTH) {	// �K���������߂�
			fSpeed = SPEED::MOVE_NEAR;
			m_nAction = ACTION_WALK;
		}
		else {
			m_nAction = ACTION_ATK;
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
void CEnemyGun::SetState(void)
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

	case STATE_DAMAGE:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// �J�E���^�[�I��
			m_StateInfo.fCounter = 0.0f;
		}
		else {
			// ���W�����ɉ�����
			D3DXVECTOR3 move = GetMove();
			move.x = sinf(GetInfo()->rot.y) * SPEED::DAMAGE_MOVE;
			move.z = cosf(GetInfo()->rot.y) * SPEED::DAMAGE_MOVE;

			SetMove(move);
		}
	}
		break;

	case STATE_DEATH:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// �J�E���^�[�I��
			if (!BodyCheck(m_pBody)) {// ���̊m�F���s
				// �����p�[�e�B�N���ݒu

				// �G���폜
				Uninit();
			}
			else {
				if (m_pBody->GetMotion()->GetEnd()) {	// ���S���[�V�����I��
					// �����p�[�e�B�N���ݒu

					// �G���폜
					Uninit();
				}
			}
		}
	}
		break;

	default:
	{

	}
		break;
	}
}

//===============================================
// �A�N�V�����ݒ�
//===============================================
void CEnemyGun::SetMotion(void)
{
	if (!BodyCheck(m_pBody)) {// ���̊m�F���s
		return;
	}

	if (!BodyCheck(m_pLeg)) {// �����g�m�F���s
		return;
	}

	// �_���[�W���
	if (m_StateInfo.state == STATE_DAMAGE) {

		m_nAction = ACTION_DAMAGE;
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// ���[�V�����I��
			m_nAction = ACTION_NEUTRAL;	// �ێ���ԂɕύX
			m_StateInfo.state = STATE_APPEAR;
		}
		else
		{
			return;
		}
	}

	switch (m_nAction) {
	case ACTION_NEUTRAL:
	{
		if (m_pBody->GetMotion()->GetEnd())
		{// ���[�V�����I��
			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);
		}
	}
		break;

	case ACTION_WALK:
	{
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);
	}
		break;

	case ACTION_ATK:
	{
		m_pBody->GetMotion()->Set(m_nAction);
		m_pLeg->GetMotion()->Set(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// ���[�V�����I��
			m_nAction = ACTION_NEUTRAL;
		}
	}
		break;

	case ACTION_DAMAGE:
	{
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// ���[�V�����I��	
			m_nAction = ACTION_NEUTRAL;
		}
	}
		break;

	case ACTION_DEATH:
	{
		m_pBody->GetMotion()->Set(m_nAction);
		m_pLeg->GetMotion()->Set(m_nAction);
	}
		break;
	}
}

//===============================================
// �d�͐ݒ�
//===============================================
void CEnemyGun::Gravity(void)
{
	SInfo* pInfo = GetInfo();
	float fGravity = SPEED::GRAVITY;;

	// �d�͂ɃX���[�{�����|�����킹��
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	pInfo->move.y += fGravity;
	pInfo->pos.y += pInfo->move.y * CManager::GetInstance()->GetSlow()->Get();
}