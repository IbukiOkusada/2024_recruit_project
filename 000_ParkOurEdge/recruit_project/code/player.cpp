//===============================================
//
// �v���C���[�̏��� [player.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "Xfile.h"
#include "slow.h"
#include "texture.h"
#include "meshfield.h"
#include "Xfile.h"
#include "meshorbit.h"
#include "game.h"
#include "filter.h"
#include "object2D.h"
#include "meshwall.h"
#include "objectX.h"
#include "sound.h"
#include <assert.h>
#include "billboard.h"
#include "character.h"
#include "motion.h"
#include "sound.h"
#include "waist.h"
#include "model.h"
#include "score.h"
#include "particle.h"
#include "effect.h"
#include <math.h>
#include "meshwall.h"
#include "enemy_manager.h"
#include "player_manager.h"
#include "enemy.h"
#include "lockon.h"
#include "fade.h"
#include "life_ui.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE	(3.5f)		// �ړ���
#define GRAVITY	(-0.6f)		//�v���C���[�d��
#define ROT_MULTI	(0.1f)	// �����␳�{��
#define WIDTH	(20.0f)		// ��
#define HEIGHT	(80.0f)		// ����
#define INER	(0.3f)		// ����
#define JUMP	(16.0f)

namespace {
	const D3DXVECTOR3 PLAYERSTARTPOS = { 10.0f, 0.0f, -2300.0f };  // �v���C���[�̃X�^�[�g�ʒu
	const int HEADPARTS_IDX = (1);	// ���̃p�[�c�C���f�b�N�X
	const float DAMAGE_INTERVAL = (10.0f);	// �_���[�W�C���^�[�o��
	const float DAMAGE_APPEAR = (110.0f);	// ���G���ԃC���^�[�o��
	const float DEATH_INTERVAL = (120.0f);	// ���S�C���^�[�o��
	const float SPAWN_INTERVAL = (60.0f);	// �����C���^�[�o��
	const float SLIDING_INER = (0.015f);	// �X���C�f�B���O����
	const float SLIDING_MINMOVE = (1.0f);	// �X���C�f�B���O�\�Œ�ړ���
	const float SLIDING_STARTMOVE = (6.0f);	// �X���C�f�B���O�J�n�\�ړ���
	const float SLIDING_SPEED = (0.5f);
	const float WALLKICK_MOVE = (45.0f);	// �ǃL�b�N�ړ���
	const float WALLKICK_INER = (0.1f);		// �ǃL�b�N������
	const float WALLKICK_SPEED = (1.0f);	// �ǃL�b�N���ړ����x
	const float WALLSLIDE_GRAVITY = (-1.5f);	// �ǂ��蒆�������x
	const float WALLDUSH_GRAVITY = (-0.75f);
	const float SLIDEJUMP_INER = (0.02f);		// �X���C�f�B���O����
	const float SLIDEJUMP_GRAVITY = (-0.25f);	// �X���C�f�B���O�W�����v�d��
	const float SLIDEJUMP = (7.0f);				// �X���C�f�B���O�W�����v�W�����v��
	const float SLIDEJUMP_SPEED = (1.75f);		// �X���C�f�B���O�W�����v�ړ���
	const float WALLSLIDE_MOVE = (0.05f);		// �ǂ��肢�ǂ���傤
	const float WALLDUSH_MOVE = (5.0f);			// �Ǒ���ړ���
	const float CAMROT_INER = (0.2f);			// �J��������
	const float SLIDINNG_ROTZ = (D3DX_PI * 0.51f);	// �X���C�f�B���O�J�����p�x
	const float SLIDING_LENGTH = (200.0f);			//�X���C�f�B���O�J��������
	const float KICKUP_SPEED = (1.5f);			// �R�肠����ړ����x
	const float KICKUP_JUMP = (18.0f);			// �R�肠����W�����v��
	const float KICKUP_QUICKJUMP = (13.0f);		// ���C�_�[�L�b�N����̃W�����v��
	const float AXEKICK_ROTZ = (D3DX_PI * 0.21f);	// �����Ɨ��Ƃ��J����
	const float RIDERKICK_ROTZ = (D3DX_PI * 0.31f);	// ���C�_�[�L�b�N�J��������
	const float AXEKICK_CAMERALENGTH = (400.0f);	// �����Ɨ��Ƃ��J��������
	const float SLOW_KICKCHARGE = (15.0f);			// �X���[�܂ł̃`���[�W����
	const float KICK_LENGTH = (1000.0f);	// �U���͈�
	const float RIDERKICK_SPEED = (24.0f);	// ���C�_�[�L�b�N���x
	const float RIDERKICK_HIGHSPEED = (60.0f);	// ���C�_�[�L�b�N�ő�
	const float RIDERKICK_CAMERALENGTH = (600.0f);	// ���C�_�[�L�b�N�J��������
	const float WALLKICK_GRAVITY = (-0.4f);
	const float WALLKICK_JUMP = (11.0f);
	const float AXEKICK_GRAVITY = (-2.5f);
	const float AXEKICK_MOVE = (0.75f);
	const int LIFE = (10);
	const float KICK_STEPMOVE = (45.0f);
	const D3DXVECTOR3 LIFEUI_POS = { SCREEN_WIDTH * 0.175f, SCREEN_HEIGHT * 0.9f, 0.0f };
}

// �O���錾

//===============================================
// �R���X�g���N�^
//===============================================
//CPlayer::CPlayer()
//{
//	// �l���N���A����
//	m_nCounterAnim = 0;
//	m_nPatternAnim = 0;
//}

//===============================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//===============================================
CPlayer::CPlayer()
{
	// �l���N���A����
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_type = TYPE_NONE;
	m_pMyCamera = nullptr;
	m_bJump = false;
	m_nId = -1;
	m_bMove = false;
	m_nLife = LIFE;
	m_Info.fSlideMove = 0.0f;
	m_pLockOn = nullptr;
	m_pTarget = nullptr;
	m_pUI = nullptr;
	m_nWallType = 0;
	m_ColiNorOld = { 0.0f, 0.0f, 0.0f };

	CPlayerManager::GetInstance()->ListIn(this);
}

//===============================================
// �f�X�g���N�^
//===============================================
CPlayer::~CPlayer()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CPlayer::Init(void)
{
	// ���̐���
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
	}

	// ���̂̐ݒ�
	m_pBody = CCharacter::Create("data\\TXT\\motion_ninjabody.txt");
	m_pBody->SetParent(m_pWaist->GetMtxWorld());

	if (m_pBody->GetMotion() != nullptr)
	{
		// �������[�V�����̐ݒ�
		m_pBody->GetMotion()->InitSet(m_nAction);
	}

	// �����g�̐ݒ�
	m_pLeg = CCharacter::Create("data\\TXT\\motion_ninjaleg.txt");
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());

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

	m_Info.state = STATE_APPEAR;
	m_nAction = ACTION_NEUTRAL;
	m_type = TYPE_NONE;
	

	return S_OK;
}

//===============================================
// ����������(�I�[�o�[���[�h)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	SetMatrix();

	// ���̐���
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
		m_pWaist->SetMatrix();
	}

	// ���̂̐ݒ�
	if (pBodyName != nullptr)
	{// �t�@�C���������݂��Ă���
		m_pBody = CCharacter::Create(pBodyName);
		m_pBody->SetParent(m_pWaist->GetMtxWorld());
		m_pBody->SetShadow(true);
		m_pBody->SetDraw();

		if (m_pBody->GetMotion() != nullptr)
		{
			// �������[�V�����̐ݒ�
			m_pBody->GetMotion()->InitSet(m_nAction);
		}
	}

	// �����g�̐ݒ�
	if (pLegName != nullptr)
	{// �t�@�C���������݂��Ă���
		m_pLeg = CCharacter::Create(pLegName);
		m_pLeg->SetParent(m_pWaist->GetMtxWorld());
		m_pLeg->SetShadow(true);
		m_pLeg->SetDraw();

		if (m_pLeg->GetMotion() != nullptr)
		{
			// �������[�V�����̐ݒ�
			m_pLeg->GetMotion()->InitSet(m_nAction);
		}
	}

	// ���̍��������킹��
	if (m_pLeg != nullptr)
	{// �r���g�p����Ă���ꍇ
		CModel *pModel = m_pLeg->GetParts(0);	// ���p�[�c���擾

		if (pModel != nullptr)
		{// �p�[�c�����݂���ꍇ
			D3DXVECTOR3 pos = pModel->GetPosition();	// ���f���̑��Έʒu���擾

			// ������ݒ�
			m_pWaist->SetHeight(pos);

			// ���̃��f���̈ʒu��ύX
			pModel->SetPosition(pos);
		}
	}

	m_type = TYPE_NONE;
	SetAction(ACTION_NEUTRAL);

	// ���b�N�I���̐���
	m_pLockOn = CLockOn::Create(&m_Info.mtxWorld, CLockOn::TYPE_TARGET);

	// Ui�̍쐬
	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {	// �Q�[����ʂ̏ꍇ
		m_pUI = CLifeUI::Create(LIFEUI_POS, m_nLife);
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
{
	// ���̂̏I��
	if (m_pBody != nullptr) {
		m_pBody->Uninit();
		delete m_pBody;
		m_pBody = nullptr;
	}

	// �����g�̏I��
	if (m_pLeg != nullptr) {
		m_pLeg->Uninit();
		delete m_pLeg;
		m_pLeg = nullptr;
	}

	// ���̔p��
	if (m_pWaist != nullptr){
		delete m_pWaist;
		m_pWaist = nullptr;
	}

	if (m_pLockOn != nullptr) {
		m_pLockOn->Uninit();
		m_pLockOn = nullptr;
	}

	if (m_pUI != nullptr) {
		m_pUI->Uninit();
		m_pUI = nullptr;
	}

	CPlayerManager::GetInstance()->ListOut(this);

	// �p��
	Release();
}

//===============================================
// �X�V����
//===============================================
void CPlayer::Update(void)
{	
	// �O��̍��W���擾
	m_Info.posOld = GetPosition();

	StateSet();	

	if (m_type == TYPE_ACTIVE)
	{
		if (m_Info.state != STATE_SPAWN)
		{
			// �v���C���[����
			Controller();
		}

		if (m_pMyCamera != nullptr) {
			// �Ǐ]����
			m_pMyCamera->Update();
		}

		// �I�����C�����M
		CManager::GetInstance()->GetScene()->SendPosition(m_Info.pos);
		CManager::GetInstance()->GetScene()->SendRotation(m_Info.rot);
	}
	else {

	}

	// �J�����Ǐ]
	if (m_pMyCamera != nullptr) {
		// �Ǐ]����
		m_pMyCamera->CollisionObj();

		if (m_pMyCamera->GetMode() == CCamera::MODE_NORMAL)
		{
			// �p�x����
			float fRot = m_fCamRotZ;
			D3DXVECTOR3 CamRot = m_pMyCamera->GetRotation();
			if (m_nAction == ACTION_SLIDING || m_nAction == ACTION_CEILINGDUSH) {	// �X���C�f�B���O�̎�
				fRot = SLIDINNG_ROTZ;
			}
			else if (m_nAction == ACTION_AXEKICK) {
				fRot = AXEKICK_ROTZ;
			}
			else if (m_nAction == ACTION_RIDERKICK) {
				fRot = RIDERKICK_ROTZ;
			}
			CamRot.z = fRot;
			m_pMyCamera->InerRot(CamRot, CAMROT_INER);

			// �Ǐ]
			float fLength = m_fCamLength;
			if (m_nAction == ACTION_SLIDING || m_nAction == ACTION_CEILINGDUSH) {	// �X���C�f�B���O�̎�
				fLength = SLIDING_LENGTH;
			}
			else if (m_nAction == ACTION_AXEKICK) {
				fLength = AXEKICK_CAMERALENGTH;
			}
			else if (m_nAction == ACTION_RIDERKICK) {
				fLength = RIDERKICK_CAMERALENGTH;
			}
			m_pMyCamera->Pursue(GetPosition(), GetRotation(), fLength);
		}
	}
	
	SetMatrix();

	BodySet();

	if (CManager::GetInstance()->GetSlow()->Get() == 1.0f && CLockOn::GetNumAll() > 1) {

		if (!m_bJump) {
			CLockOn::MultiDeath();
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("�A�N�V����[ %d ], �O��̃A�N�V���� [ %d ]\n", m_nAction, m_nActionOld);
	CManager::GetInstance()->GetDebugProc()->Print("�̗� [ %d ], ���� [ %f ]\n", m_nLife, m_Info.rot.y);
	CManager::GetInstance()->GetDebugProc()->Print("�ړ� [ %f, %f, %f ]\n", m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

	{
		CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
		if (pInputKey->GetTrigger(DIK_BACKSPACE)) {
			m_Info.pos = { -1200.0f, 1000.0f, 2400.0f };
		}
	}
}

//===============================================
// ����
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName)
{
	CPlayer *pPlayer = nullptr;

	// �I�u�W�F�N�g2D�̐���
	pPlayer = new CPlayer();

	if (nullptr != pPlayer)
	{// �����ł����ꍇ
		// ����������
		pPlayer->Init(pBodyName, pLegName);

		// ���W�ݒ�
		pPlayer->SetPosition(pos);

		// �����ݒ�
		pPlayer->SetRotation(rot);

		pPlayer->m_fRotDest = rot.y;

		// �ړ��ʐݒ�
		pPlayer->SetMove(move);
	}
	else
	{// �����Ɏ��s�����ꍇ
		return nullptr;
	}

	return pPlayer;
}

//===============================================
// ���쏈��
//===============================================
void CPlayer::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// ���W���擾
	D3DXVECTOR3 rot = GetRotation();	// �������擾
	float fIner = INER;
	m_fRotMove = rot.y;	//���݂̌������擾

	// ���쏈��
	if (m_Info.state != STATE_DEATH) {	// ���S���Ă��Ȃ�

		if (m_nAction != ACTION_RIDERKICK) {	// ���C�_�[�L�b�N���ł͂Ȃ�
			Move();			// �ړ�
			Rotation();		// ��]
			WallDush();		// �Ǒ���
			WallSlide();	// �ǂ���m�F
			Jump();			// �W�����v
			CeilingDush();	// �V�䑖��
			Slide();		// �X���C�f�B���O
			Attack();		// �U��
			KickChance();	// �L�b�N�`�����X
		}
		else {
			RiderKick();
		}

		Hit();	// �U���`�F�b�N
	}

	MotionSet();	// ���[�V�����ݒ�

	// �d�͐ݒ�
	Gravity();
	pos = GetPosition();	// ���W���擾

	// ������ύX
	switch (m_nAction) {
	case ACTION_SLIDING:
		fIner = SLIDING_INER;
		break;

	case ACTION_WALLKICK:
		fIner = WALLKICK_INER;
		break;

	case ACTION_SLIDEJUMP:
		fIner = SLIDEJUMP_INER;
		break;

	case ACTION_RIDERKICK:
		fIner = 0.0f;
		break;

	default:

		break;
	}

	m_Info.move.x += (0.0f - m_Info.move.x) * fIner;	//x���W
	m_Info.move.z += (0.0f - m_Info.move.z) * fIner;	//x���W

	pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();

	// ����
	Adjust();

	m_Info.pos = pos;
	bool bOld = m_bJump;
	m_bJump = true;	// �W�����v��ԃ��Z�b�g

	// �N���Ƃ̓����蔻��
	// ���b�V���t�B�[���h�Ƃ̔���
	{
		float fHeight = CMeshField::GetHeight(m_Info.pos);
		if (m_Info.pos.y < fHeight && m_Info.posOld.y >= fHeight)
		{
			m_Info.pos.y = fHeight;
			m_Info.move.y = 0.0f;
			m_bJump = false;

			if (m_nAction == ACTION_WALLKICK) {	// �ǏR��̏ꍇ
				SetAction(ACTION_NEUTRAL);
				if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
					m_pBody->GetMotion()->Set(m_nAction);
					m_pLeg->GetMotion()->Set(m_nAction);
				}
			}
			else if (m_nAction == ACTION_SLIDEJUMP) {
				SetAction(ACTION_NEUTRAL);
			}

			if (m_fAtkChargeCnter > 0.0f) {
				m_fAtkChargeCnter = 0.0f;
				CManager::GetInstance()->GetSlow()->SetSlow(false);
				m_pLockOn->SetLock(false);
			}
		}
	}

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	D3DXVECTOR3 vtxMax = PLAYER::COLLIMAX;
	D3DXVECTOR3 vtxMin = PLAYER::COLLIMIN;
	D3DXVECTOR3 vtxMaxOld = vtxMax;
	D3DXVECTOR3 vtxMinOld = vtxMin;

	if (m_nAction == ACTION_SLIDING) {	// �X���C�f�B���O
		vtxMax.y *= 0.3f;
	}
	else if (m_nAction == ACTION_CEILINGDUSH) { // �V�䑖��
		vtxMin.y = vtxMax.y * 0.5f;
		vtxMinOld.y = vtxMin.y;
	}

	CObjectX::COLLISION_AXIS ColiAxis = CObjectX::TYPE_MAX;	// �������Ă�����������Z�b�g

	m_ColiNor = CObjectX::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, ColiAxis);

	if (ColiAxis == CObjectX::TYPE_Y && m_ColiNor.y > 0.0f) {	// �ォ�����Ă���
		m_bJump = false;

		if (m_nAction == ACTION_WALLKICK) {	// �ǏR��̏ꍇ
			SetAction(ACTION_NEUTRAL);
			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
				m_pBody->GetMotion()->Set(m_nAction);
				m_pLeg->GetMotion()->Set(m_nAction);
			}
		}
		else if (m_nAction == ACTION_SLIDEJUMP) {
			SetAction(ACTION_NEUTRAL);
		}

		if (m_fAtkChargeCnter > 0.0f) {
			m_fAtkChargeCnter = 0.0f;
			CManager::GetInstance()->GetSlow()->SetSlow(false);
			m_pLockOn->SetLock(false);
		}
	}

	m_nWallType = 0;
	D3DXVECTOR3 nor = CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, ColiAxis, m_nWallType);

	CManager::GetInstance()->GetDebugProc()->Print("���G��Ă���ǂ̎�� [ %d ]\n", m_nWallType);

	if (nor.x != 0.0f || nor.z != 0.0f) {
		m_ColiNor = nor;
	}

	if (bOld && !m_bJump) {
		CParticle::Create(m_Info.pos, CEffect::TYPE_LAND);
		
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_LAND);
	}

	// �M�~�b�N�Ƃ̔���
	bool bLand = false;

	if (bLand == true)
	{
		m_bJump = false;
	}

	if (m_Info.pos.y <= -100.0f && CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {
		m_Info.pos = PLAYERSTARTPOS;
	}
}

//===============================================
// �ړ�
//===============================================
void CPlayer::Move(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// ���͑��u�m�F
	if (nullptr == pInputKey){
		return;
	}

	if (m_nAction == ACTION_SLIDEJUMP) {
		return;
	}

	//�v���C���[�̍X�V
	MoveController();
}

//===============================================
// ��]
//===============================================
void CPlayer::Rotation(void)
{
	CCamera *pCamera = m_pMyCamera;		// �J�����̃|�C���^

	if (m_pMyCamera == nullptr) {
		pCamera = CManager::GetInstance()->GetCamera();
	}

	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// ���͑��u�m�F
	if (nullptr == pInputPad){
		return;
	}

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// �R���g���[���[���I�[�o�[
		return;
	}

	if (!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS) && 
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_MINUS) &&
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS) &&
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_MINUS))
	{// �R���g���[���[���͖���
		KeyBoardRotation();
		return;
	}
}

//===============================================
// ��]
//===============================================
void CPlayer::KeyBoardRotation(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	if (nullptr == pInputKey){
		return;
	}
}

//===============================================
// ����
//===============================================
void CPlayer::MoveController(void)
{
	m_bMove = false;

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// �R���g���[���[�����[�΁[
		return;
	}

	CCamera *pCamera = m_pMyCamera;		// �J�����̃|�C���^

	if (m_pMyCamera == nullptr) {
		pCamera = CManager::GetInstance()->GetCamera();
	}

	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// �ړ���
	D3DXVECTOR3 move = {0.0f, 0.0f, 0.0f};
	float fRotDestOld = m_fRotDest;

	// �ړ��ʂ�ύX
	switch (m_nAction) {
	case ACTION_SLIDING:
		fSpeed = SLIDING_SPEED;
		break;

	case ACTION_WALLKICK:
		fSpeed = WALLKICK_SPEED;
		break;

	case ACTION_WALLSTAND:
		fSpeed = WALLSLIDE_MOVE;
		break;

	case ACTION_WALLDUSH:
		fSpeed = WALLDUSH_MOVE;
		break;

	case ACTION_KICKUP:
		fSpeed = KICKUP_SPEED;
		break;

	case ACTION_NORMALATK:
		fSpeed = 0.0f;
		break;

	case ACTION_DAMAGE:
		fSpeed = 0.0f;
		break;

	case ACTION_AXEKICK:
		fSpeed = AXEKICK_MOVE;
		break;
	}

	// ���͊m�F
	if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_A))
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.25f);
		}
		else
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = -CamRot.y;
		}

		// �ړ�������Ԃɂ���
		m_bMove = true;
	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_D))
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;

			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 1.0f);
		}

		// �ړ�������Ԃɂ���
		m_bMove = true;
	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
	{
		move.x += -cosf(CamRot.y) * fSpeed;
		move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// �ړ�������Ԃɂ���
		m_bMove = true;

	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
	{
		move.x += cosf(CamRot.y) * fSpeed;
		move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// �ړ�������Ԃɂ���
		m_bMove = true;
	}

	// �ړ��ʂ����Z���邩�m�F
	if (m_nAction == ACTION_SLIDING || m_nAction == ACTION_WALLKICK) {
		
		float fRot = atan2f(-move.x, -move.z);
		float fRotDest = fRotDestOld - fRot;

		// �␳
		while (1) {
			if (fRotDest > D3DX_PI || fRotDest < -D3DX_PI) {//-3.14�`3.14�͈̔͊O�̏ꍇ
				if (fRotDest > D3DX_PI) {
					fRotDest += (-D3DX_PI * 2);
				}
				else if (fRotDest < -D3DX_PI) {
					fRotDest += (D3DX_PI * 2);
				}
			}
			else {
				break;
			}
		}

		switch (m_nAction) {
		case ACTION_SLIDING:
		{
			float fData = static_cast<float>(fabs(fRotDest));
			if (fData <= D3DX_PI * 0.25f || fData >= D3DX_PI * 0.75f) {
				m_fRotDest = fRotDestOld;
				return;
			}
		}
			break;

		case ACTION_WALLKICK:
			if (!BodyCheck(m_pBody)) {
				m_fRotDest = fRotDestOld;
				return;
			}

			if (m_pBody->GetMotion()->GetNowKey() <= 2) {
				if (static_cast<float>(fabs(fRotDest)) >= D3DX_PI * 0.25f) {
					m_fRotDest = fRotDestOld;
					return;
				}
			}
			break;
		}
	}
	else if (m_nAction == ACTION_WALLSTAND) {
		m_fRotDest = atan2f(m_ColiNor.x, m_ColiNor.z);
	}

	m_Info.move += move;
}

//===============================================
// �W�����v
//===============================================
void CPlayer::Jump(void)
{
	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// �R���g���[���[���I�[�o�[
		return;
	}

	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// ����
	if (pInputPad->GetTrigger(CInputPad::BUTTON_LEFTBUTTON, m_nId) || pInputKey->GetTrigger(DIK_SPACE))
	{
		if (m_bJump == false)
		{// �W�����v���Ă��Ȃ��ꍇ
			m_bJump = true;
		
			
			CParticle::Create(m_Info.pos, CEffect::TYPE_JUMP);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);

			if (m_nAction == ACTION_SLIDING) {
				m_Info.move.y = SLIDEJUMP;
				m_Info.move.x *= SLIDEJUMP_SPEED;
				m_Info.move.z *= SLIDEJUMP_SPEED;
				SetAction(ACTION_SLIDEJUMP);
			}
			else {
				m_Info.move.y = JUMP;
			}
		}
		else {	// �W�����v���Ă���
			if (m_nAction == ACTION_WALLSTAND || (
				m_nActionOld == ACTION_WALLSTAND && 
				m_pBody->GetMotion()->GetNowKey() == 0 && 
				m_pBody->GetMotion()->GetNowFrame() <= 10 && 
				!m_pBody->GetMotion()->GetEnd())) {	// �ǂ��蒆
				if (m_nAction == ACTION_WALLSTAND) {
					m_Info.move += m_ColiNor * WALLKICK_MOVE;
				}
				else {
					m_Info.move = m_ColiNorOld * WALLKICK_MOVE;
				}
				m_Info.move.y = WALLKICK_JUMP;
				m_fRotDest = atan2f(-m_Info.move.x, -m_Info.move.z);
				SetAction(ACTION_WALLKICK);
			}
		}

		return;
	}
}

//===============================================
// �X���C�f�B���O
//===============================================
void CPlayer::Slide(void)
{
	bool bSlide = false;

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// �R���g���[���[���I�[�o�[
		return;
	}

	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// ���݂̈ړ��ʂ��擾
	float fMove = static_cast<float>((fabs(m_Info.move.x) + fabs(m_Info.move.z)));

	// ���͊J�n���̈ړ��ʂ��擾
	if (pInputPad->GetTrigger(CInputPad::BUTTON_A, m_nId) || pInputKey->GetTrigger(DIK_RETURN)) {
		m_Info.fSlideMove = fMove;
	}

	// ����
	if (fMove > SLIDING_MINMOVE && m_Info.fSlideMove >= SLIDING_STARTMOVE) {	// �X���C�f�B���O�ł���ړ��ʂ��J�n���ɂ��ړ��ʂ�����
		if (pInputPad->GetPress(CInputPad::BUTTON_A, m_nId) || pInputKey->GetPress(DIK_RETURN))
		{
			if (m_bJump == false)
			{// �W�����v���Ă��Ȃ��ꍇ
				SetAction(ACTION_SLIDING);
				bSlide = true;
			}
		}
	}
	else {
		if (m_nAction == ACTION_SLIDEJUMP) {	// ���݃X���C�f�B���O�W�����v��
			SetAction(ACTION_NEUTRAL);
			m_Info.fSlideMove = 0.0f;
		}
	}

	// �I���m�F
	if(!bSlide){	// �X���C�f�B���O���Ă��Ȃ�
		if (m_nAction == ACTION_SLIDING) {	// ���Ă���
			SetAction(ACTION_NEUTRAL);
			m_Info.fSlideMove = 0.0f;
		}
	}
}

//===============================================
// �ǂ���
//===============================================
void CPlayer::WallSlide(void)
{
	if (!m_bJump) {	// �W�����v���Ă��Ȃ�

		if (m_nAction == ACTION_WALLSTAND) {	// �ǂ����Ԃ̏ꍇ
			SetAction(ACTION_NEUTRAL);
		}
		return;
	}	

	// �ǂ��蔻��
	if (m_ColiNor.x != 0.0f || m_ColiNor.z != 0.0f) {	// �I�u�W�F�N�g�ɐG��Ă���

		if (m_nAction == ACTION_WALLKICK) {	// �ǃL�b�N��
			if (m_nActionOld == ACTION_WALLDUSH) {	// �ǂ��R�����u�Ԃ�����
				return;
			}
			else {
				SetAction(ACTION_WALLSTAND);
				m_ColiNorOld = m_ColiNor;
			}
		}
		else if (m_nAction != ACTION_WALLDUSH) {	// ��dash�ȊO
			SetAction(ACTION_WALLSTAND);
			m_ColiNorOld = m_ColiNor;
		}
	}
	else { // �G��Ă��Ȃ�
		if (m_nAction == ACTION_WALLSTAND) { // �ǂ����Ԃ̏ꍇ
			SetAction(ACTION_JUMP);

			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
				m_pBody->GetMotion()->Set(m_nAction);
				m_pLeg->GetMotion()->Set(m_nAction);
			}
		}
	}
}

//===============================================
// �Ǒ���
//===============================================
void CPlayer::WallDush(void)
{
	if (m_nAction == ACTION_WALLSTAND) {	// �ǂ��蒆����Ȃ�
		if (!BodyCheck(m_pBody)) {
			return;
		}
		else if (m_pBody->GetMotion()->GetNowKey() != 0 || m_pBody->GetMotion()->GetNowFrame() != 0 || m_pBody->GetMotion()->GetEnd()) {
			return;
		}
	}

	// �ǂ��蔻��
	if (m_ColiNor.x == 0.0f && m_ColiNor.z == 0.0f) {	// �I�u�W�F�N�g�ɐG��Ă��Ȃ�
		if (m_nAction == ACTION_WALLDUSH) {
			SetAction(ACTION_NEUTRAL);
		}
		return;
	}

	if (!m_bJump) {
		return;
	}

	if (m_nWallType != CMeshWall::TYPE_DUSH) {
		return;
	}

	// ��dash�m�F
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	// ���͒�
	if (pInputPad->GetPress(CInputPad::BUTTON_X, m_nId) || pInputKey->GetPress(DIK_J)) {
		SetAction(ACTION_WALLDUSH);
	}
	if (pInputPad->GetTrigger(CInputPad::BUTTON_LEFTBUTTON, m_nId) || pInputKey->GetTrigger(DIK_SPACE)) {	// ������

		if (m_nAction == ACTION_WALLDUSH) {	// �ǂ��蒆
			m_Info.move += m_ColiNor * WALLKICK_MOVE * 1.0f;
			m_fRotDest = atan2f(-m_Info.move.x, -m_Info.move.z);
			m_Info.move.y = WALLKICK_JUMP;
			SetAction(ACTION_WALLKICK);
		}
	}
}

//===============================================
// �V�䑖��
//===============================================
void CPlayer::CeilingDush(void)
{
	// ���͊m�F
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	// �Ԃ��蔻��
	if (m_ColiNor.y >= 0.0f) {	// �V��ɐG��Ă��Ȃ�
		if (m_nAction == ACTION_CEILINGDUSH) {
			SetAction(ACTION_NEUTRAL);
			m_Info.move.y =0.0f;
		}
		return;
	}

	// ���͒�
	if (pInputPad->GetPress(CInputPad::BUTTON_LEFTBUTTON, m_nId) || pInputKey->GetPress(DIK_SPACE)) {
		SetAction(ACTION_CEILINGDUSH);
		m_Info.move.y = JUMP;
	}
	else {
		if (m_nAction == ACTION_CEILINGDUSH) {
			SetAction(ACTION_NEUTRAL);
			m_Info.move.y = 0.0f;
		}
	}
}

//===============================================
// �d�͐ݒ�
//===============================================
void CPlayer::Gravity(void)
{
	float fGravity = GRAVITY;
	// ������ύX
	switch (m_nAction) {
	case ACTION_WALLSTAND:
		if (m_Info.move.y <= 0.0f) {
			fGravity = 0.0f;
			m_Info.move.y = WALLSLIDE_GRAVITY;
		}
		break;

	case ACTION_SLIDEJUMP:
		fGravity = SLIDEJUMP_GRAVITY;
		break;

	case ACTION_WALLKICK:
		fGravity = WALLKICK_GRAVITY;
		break;

	case ACTION_WALLDUSH:
		if (m_Info.move.y <= 0.0f) {
			fGravity = 0.0f;
			m_Info.move.y = WALLDUSH_GRAVITY;
		}
		break;

	case ACTION_AXEKICK:	

		if (BodyCheck(m_pLeg)) {	// �r���g�p����Ă���
			if (m_pLeg->GetMotion()->GetNowKey() < 2 && m_Info.move.y <= 0.0f){
				fGravity = 0.0f;
				m_Info.move.y = 0.0f;
			}
			else if (m_pLeg->GetMotion()->GetNowKey() >= 2){
				fGravity = AXEKICK_GRAVITY;
			}
		}

		break;

	case ACTION_RIDERKICK:

		fGravity = 0.0f;

		break;

	default:

		break;
	}

	// �d�͂ɃX���[�{�����|�����킹��
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	m_Info.move.y += fGravity;
	m_Info.pos.y += m_Info.move.y * CManager::GetInstance()->GetSlow()->Get();
}

//===============================================
// ����
//===============================================
void CPlayer::Adjust(void)
{
	while (1)
	{
		if (m_fRotDest > D3DX_PI || m_fRotDest < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_fRotDest > D3DX_PI)
			{
				m_fRotDest += (-D3DX_PI * 2);
			}
			else if (m_fRotDest < -D3DX_PI)
			{
				m_fRotDest += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_fRotDiff = m_fRotDest - m_fRotMove;	//�ڕW�܂ł̈ړ������̍���

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

	m_Info.rot.y += m_fRotDiff * ROT_MULTI;

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
// ��ԊǗ�
//===============================================
void CPlayer::StateSet(void)
{
	float fSlawMul = CManager::GetInstance()->GetSlow()->Get();
	switch (m_Info.state)
	{
	case STATE_APPEAR:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = 0.0f;
			m_Info.state = STATE_NORMAL;
		}
	}
		break;

	case STATE_NORMAL:
	{

	}
		break;

	case STATE_DAMAGE:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_APPEAR;
		}
	}
		break;

	case STATE_DEATH:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			if (BodyCheck(m_pBody)) {
				if (!m_pBody->GetMotion()->GetEnd()) {
					return;
				}
			}
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_SPAWN;

			m_Info.pos = PLAYERSTARTPOS;

			CModel* pModel = m_pLeg->GetParts(0);  // ���̃p�[�c

			// ���̃p�[�e�B�N������
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_BLACKSMAKE);
		}
	}
		break;

	case STATE_SPAWN:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = SPAWN_INTERVAL;
			m_Info.state = STATE_APPEAR;
			m_nLife = LIFE;

			if (m_pUI != nullptr) {
				m_pUI->SetLife(m_nLife);
			}

			if (m_pBody != nullptr) {
				m_pBody->SetDraw();
			}

			if (m_pLeg != nullptr) {
				m_pLeg->SetDraw();
			}

			// ���̃p�[�e�B�N������
			CParticle::Create(m_Info.pos, CEffect::TYPE_SMAKE);
		}
	}
		break;

	}
}

//===============================================
// �_���[�W����
//===============================================
void CPlayer::Damage(int nDamage) 
{ 
	if (m_nAction == ACTION_RIDERKICK) {
		return;
	}

	if (m_Info.state != STATE_NORMAL)
	{// �_���[�W��H���Ȃ�
		return;
	}

	// �G�t�F�N�g�̐���
	if (m_pBody != nullptr) {
		if (m_pBody->GetParts(HEADPARTS_IDX) != nullptr) {
			D3DXVECTOR3 pos = D3DXVECTOR3(m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_41,
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_42,
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_43);
			CParticle::Create(pos, CEffect::TYPE_HIT);
		}
	}

	int nOldLife = m_nLife;
	m_nLife -= nDamage;

	if (m_nLife < 0)
	{
		m_nLife = 0;
	}

	if (m_pUI != nullptr) {
		m_pUI->SetLife(m_nLife);
	}
	
	if (m_nLife != nOldLife)
	{
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DAMAGE);
		m_Info.fStateCounter = DAMAGE_INTERVAL;
		m_Info.state = STATE_DAMAGE;
		SetAction(ACTION_DAMAGE);

		if (m_nLife <= 0)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEATH);
			m_nLife = 0;
			m_Info.state = STATE_DEATH;
			m_Info.fStateCounter = DAMAGE_APPEAR;
			SetAction(ACTION_DEATH);
		}
	}
}

//===============================================
// �̗͐ݒ�
//===============================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife < 0)
	{
		m_nLife = 0;
	}
}

//===============================================
// �̗͐ݒ�
//===============================================
void CPlayer::SetType(TYPE type)
{
	m_type = type;
}

//===============================================
// ���[�V�����ݒ�
//===============================================
void CPlayer::MotionSet(void)
{
	if (!BodyCheck(m_pBody)) {// ���̊m�F���s
		return;
	}

	if (!BodyCheck(m_pLeg)) {// �����g�m�F���s
		return;
	}

	if (m_nAction > ACTION_JUMP) {	// �h���A�N�V�����̏ꍇ
		// �h�����[�V�����ݒ�

		switch (m_nAction) {
		case ACTION_SLIDING:
		{
			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);
		}
		break;

		case ACTION_WALLSTAND:
		{
			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);
		}
		break;

		case ACTION_WALLKICK:
		{

			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);

			if (m_pBody->GetMotion()->GetEnd())
			{// ���[�V�����I��
				SetAction(ACTION_JUMP);
			}
		}
		break;

		case ACTION_NORMALATK:

			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd())
			{// ���[�V�����I��
				SetAction(ACTION_NEUTRAL);
			}
			break;

		case ACTION_WALLDUSH:
		{
			m_pBody->GetMotion()->BlendSet(ACTION_JUMP);
			m_pLeg->GetMotion()->BlendSet(ACTION_JUMP);
		}
		break;

		case ACTION_CEILINGDUSH:

		{
			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);
		}
		break;

		case ACTION_KICKUP:
		{
			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd())
			{// ���[�V�����I��
				SetAction(ACTION_JUMP);
			}
		}
		break;

		case ACTION_AXEKICK:
		{
			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd() && !m_bJump)
			{// ���[�V�����I��
				SetAction(ACTION_NEUTRAL);
			}
		}
		break;

		case ACTION_RIDERKICK:
		{
			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd() && !m_bJump)
			{// ���[�V�����I��
				SetAction(ACTION_NEUTRAL);
			}
		}
		break;

		case ACTION_DAMAGE:

			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd())
			{// ���[�V�����I��
				SetAction(ACTION_NEUTRAL);	// �ێ���ԂɕύX
			}
			else
			{
				return;
			}

			break;

		case ACTION_DEATH:

			m_pBody->GetMotion()->Set(m_nAction);
			m_pLeg->GetMotion()->Set(m_nAction);

			if (m_pBody->GetMotion()->GetEnd())
			{// ���[�V�����I��
				SetAction(ACTION_NEUTRAL);	// �ێ���ԂɕύX
				m_pBody->SetDraw(false);
				m_pLeg->SetDraw(false);
			}

			break;

		default:

			break;
		}

		return;
	}
	else if (!m_bJump && !m_bMove && 
		m_nAction >= ACTION_NEUTRAL && m_nAction <= ACTION_JUMP)
	{// �������Ă��Ȃ�
		SetAction(ACTION_NEUTRAL);
		m_pBody->GetMotion()->BlendSet(m_nAction);
	}
	else if(m_bJump && 
		m_nAction >= ACTION_NEUTRAL && m_nAction <= ACTION_JUMP)
	{// �W�����v����
		SetAction(ACTION_JUMP);
		m_pBody->GetMotion()->BlendSet(m_nAction);
	}
	else if (m_bMove &&
		m_nAction >= ACTION_NEUTRAL && m_nAction <= ACTION_JUMP)
	{// �ړ�����
		SetAction(ACTION_WALK);
		m_pBody->GetMotion()->BlendSet(m_nAction);
	}

	if (m_nAction == ACTION_SLIDING) {
		m_pLeg->GetMotion()->Set(m_nAction);
	}
	else if (m_bJump)
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_JUMP);
	}
	else if (m_bMove)
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_WALK);
		if (m_pLeg->GetMotion()->GetNowFrame() == 0 && (m_pLeg->GetMotion()->GetNowKey() == 0 || m_pLeg->GetMotion()->GetNowKey() == 2))
		{
			CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_STEP);
		}
	}
	else
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_NEUTRAL);
	}
}

//===============================================
// �U��
//===============================================
void CPlayer::Attack(void)
{
	if (m_nAction == ACTION_NORMALATK || m_nAction == ACTION_AXEKICK || m_nAction == ACTION_RIDERKICK) {
		return;
	}

	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();				// �p�b�h�̃|�C���^

	if (m_nAction == ACTION_WALLKICK || m_nActionOld == ACTION_WALLKICK) {	// ���O�A�������͌��ݕǃL�b�N���

		if (pInputKey->GetPress(DIK_I) || pInputPad->GetPress(CInputPad::BUTTON_RIGHTBUTTON, m_nId)) {
			m_fAtkChargeCnter += 1.0f;

			if (m_fAtkChargeCnter >= SLOW_KICKCHARGE) {	// �U���`���[�W���Ă���
				if (m_pLockOn == nullptr) {	// ���b�N�I�������݂��Ȃ�
					return;
				}

				m_pLockOn->SetLock(true);
				CManager::GetInstance()->GetSlow()->SetSlow(true);
			}

			return;
		}
		else {

			if (m_fAtkChargeCnter <= 0.0f) {	// �U����charge���Ă��Ȃ�
				return;
			}
			else if(m_fAtkChargeCnter >= SLOW_KICKCHARGE) {	// �`���[�W���Ă���

				if (m_pLockOn->GetNext() != nullptr){
					m_pTarget = m_pLockOn->GetNext()->GetEnemy();
				}

				if (m_pLockOn != nullptr) {	// ���b�N�I��������
					m_pLockOn->SetLock(false);
				}
				CManager::GetInstance()->GetSlow()->SetSlow(false);
			}
			else {	// ���o��
				NearLockOn();	// �U���Ώۂ����߂�
			}

			if (m_pTarget != nullptr) {	// �U���Ώۂ�����
				SetAction(ACTION_RIDERKICK);
			}

			m_fAtkChargeCnter = 0.0f;
			m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	if (pInputKey->GetTrigger(DIK_I) || pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, m_nId)) {
		// ���݂̏�Ԃɂ���čU�����@��ς���
		switch (m_nAction) {
		case ACTION_KICKUP:
			SetAction(ACTION_AXEKICK);
			break;
			
		case ACTION_JUMP:
			SetAction(ACTION_AXEKICK);
			break;
		default:
		{
			SetAction(ACTION_NORMALATK);
		}
		break;
		}
	}
}

//===============================================
// �}�g���b�N�X�ݒ�
//===============================================
void CPlayer::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);
}

//===============================================
// �U���̃q�b�g�m�F
//===============================================
bool CPlayer::HitCheck(D3DXVECTOR3 pos, float fRange, float fHeight, int nDamage)
{
	bool m_bValue = false;
	if (m_Info.state != STATE_NORMAL) {
		return m_bValue;
	}

	if (m_pBody == nullptr) {
		return m_bValue;
	}

	if (pos.x + fRange < m_Info.pos.x + PLAYER::COLLIMIN.x ||
		pos.x - fRange > m_Info.pos.x + PLAYER::COLLIMAX.x)
	{// X�͈͊O
		return m_bValue;
	}

	if (pos.z + fRange < m_Info.pos.z + PLAYER::COLLIMIN.z ||
		pos.z - fRange > m_Info.pos.z + PLAYER::COLLIMAX.z)
	{// Z�͈͊O
		return m_bValue;
	}

	if (pos.y + fHeight < m_Info.pos.y + PLAYER::COLLIMIN.y ||
		pos.y - fHeight > m_Info.pos.y + PLAYER::COLLIMAX.y)
	{// Y�͈͊O
		return m_bValue;
	}

	m_bValue = true;
	Damage(nDamage);

	return m_bValue;
}

//===============================================
// �w�胂�[�V�����ɐݒ�
//===============================================
void CPlayer::SetMotion(int nMotion) {
	
	if(!BodyCheck(m_pBody)) {
		return;
	}

	m_pBody->GetMotion()->InitSet(nMotion);

	if (!BodyCheck(m_pLeg)) {
		return;
	}

	m_pLeg->GetMotion()->InitSet(nMotion);
}

//===============================================
// ���[�V�������擾
//===============================================
int CPlayer::GetMotion(void) {

	if (m_pBody == nullptr) {
		return -1;
	}

	if (m_pBody->GetMotion() == nullptr) {
		return -1;
	}

	return m_pBody->GetMotion()->GetNowMotion();
}

//===============================================
// ���U���g���s���̃p�[�e�B�N��
//===============================================
void CPlayer::SetFailedParticle(void)
{
	if (!BodyCheck(m_pBody)) {	// ���̊m�F���s
		return;
	}

	if (m_pBody->GetMotion()->GetNowFrame() != 0)	// ����0�t���[���ł͂Ȃ�
	{
		return;
		CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_STEP);
	}

	CModel *pModel = nullptr;

	if (m_pBody->GetMotion()->GetNowKey() == 0)
	{
		pModel = m_pBody->GetParts(4);
	}
	else if (m_pBody->GetMotion()->GetNowKey() == 1)
	{
		pModel = m_pBody->GetParts(m_pBody->GetNumParts() - 1);
	}

	if (pModel == nullptr) {	// �g���Ă��Ȃ�
		return;
	}

	D3DXVECTOR3 pos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);
}

//===============================================
// �v���C���[�̕`��ݒ�
//===============================================
void CPlayer::SetDraw(bool bDraw)
{
	if (m_pBody != nullptr) {
		m_pBody->SetDraw(bDraw);
	}

	if (m_pLeg != nullptr) {
		m_pLeg->SetDraw(bDraw);
	}
}

//===============================================
// �g�p�K�w�\���̐ݒ�
//===============================================
void CPlayer::BodySet(void)
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
bool CPlayer::BodyCheck(CCharacter* pBody)
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
// �U���`�F�b�N
//===============================================
void CPlayer::Hit(void)
{
	if (!BodyCheck(m_pBody) || !BodyCheck(m_pLeg)) {
		return;
	}

	if (m_pLeg->GetMotion()->GetNowMotion() != m_nAction) {
		return;
	}

	// �U�����@
	switch (m_nAction) {
	case ACTION_NORMALATK:
		if (m_pLeg->GetMotion()->GetNowKey() < m_pLeg->GetMotion()->GetNowNumKey() - 2) {
			if (m_pLeg->GetMotion()->GetNowKey() == 1 && m_pLeg->GetMotion()->GetNowFrame() == 0) {
				m_Info.move += D3DXVECTOR3(-sinf(m_Info.rot.y) * KICK_STEPMOVE, 0.0f, -cosf(m_Info.rot.y) * KICK_STEPMOVE);
			}
			return;
		}
		break;

	case ACTION_AXEKICK:
		if (m_pLeg->GetMotion()->GetNowKey() < m_pLeg->GetMotion()->GetNowNumKey() - 1) {
			return;
		}
		break;

	case ACTION_RIDERKICK:
		if (m_pLeg->GetMotion()->GetNowKey() < m_pLeg->GetMotion()->GetNowNumKey() - 1) {
			return;
		}
		break;

	default:
		return;
		break;
	}

	// ��������ꏊ���擾
	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	
	if (CEnemyManager::GetInstance()->Hit(pos, fRange, nDamage, m_pTarget)) {	// ���������ꍇ
		if (m_nAction == ACTION_NORMALATK) {
			m_nAction = ACTION_KICKUP;
			m_Info.move = { 0.0f, 0.0f, 0.0f };	// �ړ��ʃ��Z�b�g
			m_Info.move.y = KICKUP_JUMP;
		}
		else if (m_nAction == ACTION_RIDERKICK) {
			m_nAction = ACTION_KICKUP;
			m_Info.move = { 0.0f, 0.0f, 0.0f };	// �ړ��ʃ��Z�b�g
			m_Info.move.y = KICKUP_JUMP;

			if (m_pLockOn->GetNext() == nullptr) {	// �������b�N�I���ł͂Ȃ�
				m_pTarget = nullptr;
				return;
			}

			m_Info.move.y = KICKUP_QUICKJUMP;
			m_pTarget = m_pLockOn->GetNext()->GetEnemy();
		}
	}
}

//===============================================
// �J�����ݒ�
//===============================================
void CPlayer::SetCamera(CCamera* pCamera) 
{ 
	m_pMyCamera = pCamera; 
	if (m_pMyCamera == nullptr) {
		return;
	}

	m_fCamRotZ = m_pMyCamera->GetRotation().z; 
	m_fCamLength = m_pMyCamera->GetLength();
}

//===============================================
// �g�p�A�N�V�����ݒ�
//===============================================
void CPlayer::SetAction(const ACTION action)
{
	if (m_nAction != action) {
		m_nActionOld = m_nAction;
		m_nAction = action;
	}
}

//===============================================
// ���b�N�I������
//===============================================
void CPlayer::NearLockOn(void)
{
	CEnemy* pEnemy = CEnemyManager::GetInstance()->GetTop();
	float fMinLength = KICK_LENGTH;
	m_pTarget = nullptr;
	D3DXVECTOR3 MyPos = GetPosition();

	// �����Ƃ��߂��G��������
	while (pEnemy != nullptr) {
		CEnemy* pEnemyNext = pEnemy->GetNext();
		D3DXVECTOR3 EnemPos = pEnemy->GetPosition();

		float fLength = sqrtf((EnemPos.x - MyPos.x) * (EnemPos.x - MyPos.x)	// �������擾
			+ (EnemPos.z - MyPos.z) * (EnemPos.z - MyPos.z));

		if (fLength > KICK_LENGTH) {	// �͈͊O
			pEnemy = pEnemyNext;
			continue;
		}

		if (!pEnemy->GetLockon()) {	// �ΏۊO
			pEnemy = pEnemyNext;
			continue;
		}

		if (m_pTarget == nullptr) {	// �܂��N�����b�N�I�����Ă��Ȃ�
			m_pTarget = pEnemy;
			fMinLength = fLength;
		}
		else if (fLength <= fMinLength) {	// ���݃��b�N�I�����Ă���G�����߂�
			m_pTarget = pEnemy;
			fMinLength = fLength;
		}

		pEnemy = pEnemyNext;
	}
}

//===============================================
// ���C�_�[�L�b�N������
//===============================================
void CPlayer::RiderKick(void)
{
	if (!BodyCheck(m_pLeg)) {
		return;
	}

	if (m_pTarget == nullptr) {
		return;
	}

	// �ڕW�����ݒ�
	D3DXVECTOR3 MyPos = GetPosition();	// �����̍��W
	D3DXVECTOR3 pos = m_pTarget->GetPosition();
	{
		float fDiff = m_fRotDest;
		fDiff = atan2f(pos.x - MyPos.x, pos.z - MyPos.z) + D3DX_PI;
		if (fDiff < -D3DX_PI) {
			fDiff += D3DX_PI * 2;
		}
		else if (fDiff > D3DX_PI) {
			fDiff += -D3DX_PI * 2;
		}
		m_fRotDest = fDiff;
	}

	if (!m_pLeg->GetMotion()->GetEnd()) {
		return;
	}

	// �ړ��ʂ�ݒ�
	{
		float fSpeed = RIDERKICK_SPEED;

		if (m_pTarget->GetNext() != nullptr) {
			fSpeed = RIDERKICK_HIGHSPEED;
		}

		D3DXVECTOR3 move = m_Info.move;
		D3DXVECTOR3 nor = pos - MyPos;
		D3DXVec3Normalize(&nor, &nor);
		move = nor * fSpeed;

		m_Info.move = move;
	}
}

//===============================================
// ���b�N�I���L�b�N�R���{�m�F
//===============================================
void CPlayer::KickChance(void)
{
	if (m_nAction != ACTION_KICKUP && m_nActionOld != ACTION_KICKUP) {
		return;
	}

	if (m_pTarget == nullptr) {
		return;
	}

	if (m_Info.move.y <= 1.0f) {
		SetAction(ACTION_RIDERKICK);
	}
	else {
		CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
		CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();				// �p�b�h�̃|�C���^

		// �U���𒆎~
		if (pInputKey->GetTrigger(DIK_I) || pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, m_nId)) {
			m_pTarget = nullptr;
			CLockOn::MultiDeath();
		}
	}
}

//===============================================
// ���b�N�I���L�b�N�R���{�m�F
//===============================================
D3DXVECTOR3 CPlayer::GetColliMax(void)
{
	return PLAYER::COLLIMAX;
}