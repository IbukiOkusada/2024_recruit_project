//==========================================================
//
// �{�X�G [enemy_boss.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_boss.h"
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
#include "bullet.h"
#include "input.h"
#include "Xfile.h"
#include "bridge.h"

// �������O���
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\enemy_boss\\motion_body.txt";	// �t�@�C���̃p�X
	const char* LEGFILEPASS = "data\\TXT\\enemy_boss\\motion_leg.txt";	// �t�@�C���̃p�X
	const char* ARMFILEPASS[CEnemyBoss::PARTS_MAX] = {
		"data\\TXT\\enemy_boss\\motion_leftarm.txt",
		"data\\TXT\\enemy_boss\\motion_rightarm.txt",
	};
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// �����蔻��ő�
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// �����蔻��ŏ�
	const int DAMAGEINTERVAL = (60);	// �_���[�W�C���^�[�o��
	const float CHASE_MAXLENGTH = (1000.0f);	// �ǐՍŒ�����
	const float CHASE_NEARLENGTH = (700.0f);	// �ǐՋߋ���
	const float CHASE_MINLENGTH = (400.0f);		// �ǐ�0����
	const float SEARCH_HEIGHT = (180.0f);		// �T����������
	const float MOVE_INER = (0.3f);			// �ړ�����
	const int ATK_RANDRANGE = (6);			// �U���������_���͈�
	const int BACK_RANDRANGE = (2);
	const int FRONT_RANDRANGE = (3);
	const float ROTDIFF_INER = (0.05f);
}

// �ړ����x���O���
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// �������ړ�
	const float MOVE_NEAR = (-1.0f);	// �ߋ����ړ�
	const float MOVE_MIN = (0.15f);	// �ړ��ʈړ�
	const float GRAVITY = (-0.9f);	// �d��
	const float DAMAGE_MOVE = (2.0f);	// �ړ���
	const float BULLET = (-6.0f);	// �e��
}

// �C���^�[�o��
namespace INTERVAL
{
	const float DAMAGE = (20.0f);	// �_���[�W
	const float ATTACK = (500.0f);	// �U��
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CEnemyBoss::CEnemyBoss()
{
	// �l�̃N���A
	m_nAction = ACTION_NEUTRAL;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_Chase.pTarget = nullptr;
	m_pBridge = nullptr;
	m_Chase.fLength = 0.0f;
	m_fInterVal = 0;
	m_StateInfo.state = STATE_APPEAR;
	m_StateInfo.fCounter = 0.0f;
	m_fAtkCnter = 0.0f;
	m_nArmAction = 0;

	for (int i = 0; i < PARTS_MAX; i++) {
		m_apArm[i] = nullptr;
	}
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CEnemyBoss::~CEnemyBoss()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CEnemyBoss::Init(void)
{
	// ��ނ��{�X�ɐݒ�
	SetType(TYPE_BOSS);

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

	// �ǉ��r�̐ݒ�
	for (int i = 0; i < PARTS_MAX; i++) {
		m_apArm[i] = CCharacter::Create(ARMFILEPASS[i]);
		m_apArm[i]->SetParent(m_pWaist->GetMtxWorld());
		m_apArm[i]->SetShadow(true);
		m_apArm[i]->SetDraw();

		if (m_apArm[i]->GetMotion() != nullptr)
		{
			// �������[�V�����̐ݒ�
			m_apArm[i]->GetMotion()->InitSet(ARM_NEUTRAL);
		}
	}

	m_nArmAction = ARM_NEUTRAL;
	m_NowArm = PARTS_LEFTARM;

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
	m_fInterVal = 1;
	SetRotMulti(ROTDIFF_INER);

	// ���삷�鋴�𐶐�
	m_pBridge = CBridge::Create(D3DXVECTOR3(-2700.0f, 1010.0f, 3750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CEnemyBoss::Uninit(void)
{
	// �ǉ��r�̐ݒ�
	for (int i = 0; i < PARTS_MAX; i++) {
		if (m_apArm[i] != nullptr){
			m_apArm[i]->Uninit();
			delete m_apArm[i];
			m_apArm[i] = nullptr;
		}
	}

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

	if (m_pBridge != nullptr) {
		m_pBridge->SetClose(true);
		m_pBridge = nullptr;
	}

	// �e�N���X�̏I���Ăяo��
	CEnemy::Uninit();
}

//==========================================================
// �X�V����
//==========================================================
void CEnemyBoss::Update(void)
{
	// �O��̍��W���擾
	{
		SInfo* pInfo = GetInfo();
		pInfo->posOld = pInfo->pos;
	}
	m_fInterVal--;

	// ����
	MethodLine();

	//�}�g���b�N�X�ݒ�
	CEnemy::Update();
	BodySet();

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_H)) {
		ArmDamage();
	}
}

//===============================================
// �����̏���
//===============================================
void CEnemyBoss::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	if (m_StateInfo.state != STATE_DEATH || m_StateInfo.state != STATE_DAMAGE) {

		// �{��
		m_Chase.pTarget = Search(m_Chase.fLength);

		// �ǐ�
		LockOn();

		// �ړ�
		AddMove();

		// �U��
		Attack();
	}

	// �d��
	if (m_NowArm >= PARTS_MAX) {
		Gravity();
	}
	else {
		FootCheck();
	}

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
CEnemyBoss *CEnemyBoss::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyBoss *pEnemyBoss = nullptr;

	pEnemyBoss = new CEnemyBoss;

	if (pEnemyBoss != nullptr)
	{
		// ����������
		pEnemyBoss->Init();

		// �f�[�^���Z�b�g
		pEnemyBoss->InfoReset();

		// �f�[�^�ݒ�
		pEnemyBoss->SetPosition(pos);
		pEnemyBoss->SetRotation(rot);
		pEnemyBoss->SetRotDiff(rot.y);
		pEnemyBoss->SetIner(MOVE_INER);
	}

	return pEnemyBoss;
}

//===============================================
// �g�p�K�w�\���̐ݒ�
//===============================================
void CEnemyBoss::BodySet(void)
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

	// �r�X�V
	for (int i = 0; i < PARTS_MAX; i++) {
		if (BodyCheck(m_apArm[i]))
		{
			m_apArm[i]->Update();
		}
	}
}

//===============================================
// �̎g�p�m�F
//===============================================
bool CEnemyBoss::BodyCheck(CCharacter* pBody)
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
bool CEnemyBoss::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_fInterVal > 0) {
		return false;
	}

	if (m_NowArm < PARTS_MAX) {
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
void CEnemyBoss::Damage(const int nDamage)
{
	if (m_fInterVal > 0) {	// �C���^�[�o�����߂��Ă��Ă��Ȃ�
		return;
	}

	// ��Ԋm�F
	if (m_StateInfo.state == STATE_APPEAR || m_StateInfo.state == STATE_DEATH) {	// �_���[�W���󂯂Ȃ����
		return;
	}

	// �C���^�[�o����ύX
	m_fInterVal = DAMAGEINTERVAL;

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
CPlayer* CEnemyBoss::Search(float& fChaseLength)
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
void CEnemyBoss::LockOn(void)
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
		if (m_nArmAction != ARM_ATTACK && m_NowArm < ARM_MAX || m_NowArm >= ARM_MAX) {
			fDiff = atan2f(pos.x - MyPos.x, pos.z - MyPos.z) + D3DX_PI;
			if (fDiff < -D3DX_PI) {
				fDiff += D3DX_PI * 2;
			}
			else if (fDiff > D3DX_PI) {
				fDiff += -D3DX_PI * 2;
			}
		}
		else if (m_nArmAction == ARM_ATTACK && m_NowArm < ARM_MAX) {
			fDiff = 0.0f;
		}

		SetRotDiff(fDiff);
	}
}

//===============================================
// ��Ԑݒ�
//===============================================
void CEnemyBoss::SetState(void)
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
void CEnemyBoss::SetMotion(void)
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
void CEnemyBoss::Gravity(void)
{
	SInfo* pInfo = GetInfo();
	float fGravity = SPEED::GRAVITY;;

	// �d�͂ɃX���[�{�����|�����킹��
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	pInfo->move.y += fGravity;
	pInfo->pos.y += pInfo->move.y * CManager::GetInstance()->GetSlow()->Get();
}

//===============================================
// �U��
//===============================================
void CEnemyBoss::Attack(const int nRandRange)
{
	float CMinusCnter = static_cast<float>(rand() % nRandRange);
	m_fAtkCnter -= (CMinusCnter) * CManager::GetInstance()->GetSlow()->Get();

	if (m_fAtkCnter > 0.0f) {	// �U���ł��Ȃ�
		AttackCheck();
		return;
	}

	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}
	m_fAtkCnter = INTERVAL::ATTACK;
	m_apArm[m_NowArm]->GetMotion()->BlendSet(ARM_ATTACK);
	m_nArmAction = ARM_ATTACK;
}

//===============================================
// �U���m�F
//===============================================
void CEnemyBoss::AttackCheck(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetNowMotion() != ARM_ATTACK) {	// ���ݍU�����ł͂Ȃ�
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetEnd()) {	// ���[�V�����I��
		m_apArm[m_NowArm]->GetMotion()->BlendSet(ARM_NEUTRAL);
		m_nArmAction = ARM_NEUTRAL;
	}
}

//===============================================
// ����m�F
//===============================================
void CEnemyBoss::FootCheck(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}

	if (m_nArmAction != ARM_ATTACK) {
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetNowKey() != 3) {
		return;
	}

	int nNumParts = m_apArm[m_NowArm]->GetNumParts();

	for (int i = 0; i < nNumParts; i++) {
		CPlayer* pPlayer = CPlayerManager::GetInstance()->GetTop();
		D3DXVECTOR3 ArmPos = { m_apArm[m_NowArm]->GetParts(i)->GetMtx()->_41, GetPosition().y, m_apArm[m_NowArm]->GetParts(i)->GetMtx()->_43 };
		D3DXVECTOR3 VtxMax = CManager::GetInstance()->GetModelFile()->GetMax(m_apArm[m_NowArm]->GetParts(i)->GetId());
		D3DXVECTOR3 VtxMin = CManager::GetInstance()->GetModelFile()->GetMin(m_apArm[m_NowArm]->GetParts(i)->GetId());
		VtxMax.y *= 2;

		if (i == nNumParts - 1) {
			ArmPos.y += 60.0f;
		}

		// �v���C���[���J��Ԃ�
		while (pPlayer != nullptr) {
			CPlayer* pNext = pPlayer->GetNext();
			D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
			D3DXVECTOR3 PlayerPosOld = pPlayer->GetOldPosition();
			D3DXVECTOR3 PlayerMove = pPlayer->GetMove();
			D3DXVECTOR3 PlayerMax = pPlayer->GetColliMax();

			// Y���W�m�F
			if (PlayerPos.x >= ArmPos.x + VtxMin.x && PlayerPos.x <= ArmPos.x + VtxMax.x
				&& PlayerPos.z >= ArmPos.z + VtxMin.z && PlayerPos.z <= ArmPos.z + VtxMax.z) {

				if (PlayerPos.y < ArmPos.y + VtxMax.y && PlayerPosOld.y >= ArmPos.y + VtxMax.y) {
					PlayerPos.y = ArmPos.y + VtxMax.y;
					PlayerMove.y = 0.0f;
					pPlayer->SetJump(false);

					if (i == nNumParts - 1) {
						ArmDamage();
						return;
					}
				}

				if (PlayerPos.y + PlayerMax.y > ArmPos.y + VtxMin.y && PlayerPosOld.y + PlayerMax.y <= ArmPos.y + VtxMin.y) {
					PlayerPos.y = ArmPos.y + VtxMin.y - PlayerMax.y;
					PlayerMove.y = 0.0f;
				}
			}

			// XZ���W�m�F
			if (PlayerPos.y <= ArmPos.y + VtxMax.y && PlayerPos.y + PlayerMax.y >= ArmPos.y + VtxMin.y) {

				// X���W
				if (PlayerPos.z - PlayerMax.z <= ArmPos.z + VtxMax.z
					&& PlayerPos.z + PlayerMax.z >= ArmPos.z + VtxMin.z) {
					if (PlayerPos.x - PlayerMax.x < ArmPos.x + VtxMax.x
						&& PlayerPosOld.x - PlayerMax.x >= ArmPos.x + VtxMax.x) {
						PlayerPos.x = ArmPos.x + VtxMax.x + PlayerMax.x;
						PlayerMove.x = 0.0f;
					}
					if (PlayerPos.x + PlayerMax.x > ArmPos.x + VtxMin.x
						&& PlayerPosOld.x + PlayerMax.x <= ArmPos.x + VtxMin.x) {
						PlayerPos.x = ArmPos.x + VtxMin.x - PlayerMax.x;
						PlayerMove.x = 0.0f;
					}
				}

				// Z���W
				if (PlayerPos.x - PlayerMax.x <= ArmPos.x + VtxMax.x
					&& PlayerPos.x + PlayerMax.x >= ArmPos.x + VtxMin.x) {
					if (PlayerPos.z - PlayerMax.z < ArmPos.z + VtxMax.z
						&& PlayerPosOld.z - PlayerMax.z >= ArmPos.z + VtxMax.z) {
						PlayerPos.z = ArmPos.z + VtxMax.z + PlayerMax.z;
						PlayerMove.z = 0.0f;
					}
					if (PlayerPos.z + PlayerMax.z > ArmPos.z + VtxMin.z
						&& PlayerPosOld.z + PlayerMax.z <= ArmPos.z + VtxMin.z) {
						PlayerPos.z = ArmPos.z + VtxMin.z - PlayerMax.z;
						PlayerMove.z = 0.0f;
					}
				}
			}

			pPlayer->SetPosition(PlayerPos);
			pPlayer->SetMove(PlayerMove);

			pPlayer = pNext;
		}
	}
}

//===============================================
// �r�_���[�W
//===============================================
void CEnemyBoss::ArmDamage(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (m_apArm[m_NowArm] != nullptr) {
		m_apArm[m_NowArm]->Uninit();
		delete m_apArm[m_NowArm];
		m_apArm[m_NowArm] = nullptr;
	}

	m_NowArm++;

	if (m_NowArm >= PARTS_MAX) {
		m_nArmAction = ARM_NEUTRAL;
	}
}