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

//===============================================
// �}�N����`
//===============================================
#define MOVE	(4.5f)		// �ړ���
#define GRAVITY	(-1.0f)		//�v���C���[�d��
#define ROT_MULTI	(0.1f)	// �����␳�{��
#define WIDTH	(20.0f)		// ��
#define HEIGHT	(80.0f)	// ����
#define INER	(0.3f)		// ����
#define JUMP	(20.0f)

namespace {
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
	const float SLIDEJUMP_INER = (0.02f);
	const float SLIDEJUMP_GRAVITY = (-0.25f);
	const float SLIDEJUMP = (7.0f);
	const float SLIDEJUMP_SPEED = (1.75f);
	const float WALLSLIDE_MOVE = (0.05f);
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };
}

// �O���錾
CPlayer *CPlayer::m_pTop = nullptr;	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CPlayer *CPlayer::m_pCur = nullptr;	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CPlayer::m_nNumCount = 0;

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
	m_nLife = 0;
	m_Info.fSlideMove = 0.0f;
	
	// �������g�����X�g�ɒǉ�
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ
		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ
		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}

	m_nNumCount++;
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
		m_pBody->GetMotion()->InitSet(m_action);
	}

	// �����g�̐ݒ�
	m_pLeg = CCharacter::Create("data\\TXT\\motion_ninjaleg.txt");
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());

	if (m_pLeg->GetMotion() != nullptr)
	{
		// �������[�V�����̐ݒ�
		m_pLeg->GetMotion()->InitSet(m_action);
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
	m_action = ACTION_NEUTRAL;
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
			m_pBody->GetMotion()->InitSet(m_action);
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
			m_pLeg->GetMotion()->InitSet(m_action);
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

	/*if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(50.0f + (m_nNumCount - 1) * 500.0f, 50.0f, 0.0f), 30.0f, 30.0f);
	}*/

	m_type = TYPE_NONE;
	m_action = ACTION_NEUTRAL;

	//m_pScore->AddScore(500 * m_nItemCnt);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
{
	// ���X�g���玩�����g���폜����
	if (m_pTop == this) {	// ���g���擪
		if (m_pNext != nullptr)
		{// �������݂��Ă���
			m_pTop = m_pNext;	// ����擪�ɂ���
			m_pNext->m_pPrev = nullptr;	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else if (m_pCur == this) {	// ���g���Ō��
		if (m_pPrev != nullptr)
		{	// �������݂��Ă���
			m_pCur = m_pPrev;			// �O���Ō���ɂ���
			m_pPrev->m_pNext = nullptr;	// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{	// ���݂��Ă��Ȃ�
			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else { // ����ȊO
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// ���g�̎��ɑO�̃|�C���^���o��������
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// ���g�̑O�Ɏ��̃|�C���^���o��������
		}
	}

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

	// �l�������炷
	m_nNumCount--;

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
			m_pMyCamera->Pursue(GetPosition(), GetRotation());
		}
	}
	
	SetMatrix();

	BodySet();
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
			
			Move();		// �ړ�
			Rotation();	// ��]
			WallSlide();	// �ǂ���m�F
			Jump();		// �W�����v
			Slide();	// �X���C�f�B���O
		}

	MotionSet();	// ���[�V�����ݒ�
	CManager::GetInstance()->GetDebugProc()->Print("�A�N�V����[ %d ]\n", m_action);

	// �d�͐ݒ�
	Gravity();
	pos = GetPosition();	// ���W���擾

	// ������ύX
	switch (m_action) {
	case ACTION_SLIDING:
		fIner = SLIDING_INER;
		break;

	case ACTION_WALLKICK:
		fIner = WALLKICK_INER;
		break;

	case ACTION_SLIDEJUMP:
		fIner = SLIDEJUMP_INER;
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
		if (m_Info.pos.y < fHeight)
		{
			m_Info.pos.y = fHeight;
			m_Info.move.y = 0.0f;
			m_bJump = false;

			if (m_action == ACTION_WALLKICK) {	// �ǏR��̏ꍇ
				m_action = ACTION_NEUTRAL;
				if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
					m_pBody->GetMotion()->Set(m_action);
					m_pLeg->GetMotion()->Set(m_action);
				}
			}
			else if (m_action == ACTION_SLIDEJUMP) {
				m_action = ACTION_NEUTRAL;
			}
		}
	}

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	D3DXVECTOR3 vtxMax = COLLIMAX;
	D3DXVECTOR3 vtxMin = COLLIMIN;
	D3DXVECTOR3 vtxMaxOld = vtxMax;
	D3DXVECTOR3 vtxMinOld = vtxMin;
	CObjectX::COLLISION_AXIS ColiAxis = CObjectX::TYPE_MAX;	// �������Ă�����������Z�b�g

	m_ColiNor = CObjectX::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, ColiAxis);

	if (ColiAxis == CObjectX::TYPE_Y) {
		m_bJump = false;

		if (m_action == ACTION_WALLKICK) {	// �ǏR��̏ꍇ
			m_action = ACTION_NEUTRAL;
			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
				m_pBody->GetMotion()->Set(m_action);
				m_pLeg->GetMotion()->Set(m_action);
			}
		}
		else if (m_action == ACTION_SLIDEJUMP) {
			m_action = ACTION_NEUTRAL;
		}
	}

	D3DXVECTOR3 nor = CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, ColiAxis);

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

	// ���͑��u�m�F
	if (nullptr == pInputPad){
		return;
	}

	if (m_action == ACTION_SLIDEJUMP) {
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

	if (pInputKey->GetPress(DIK_W))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.25f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.25f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 0.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_S))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.75f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 1.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_A))
	{
		m_fRotDest = -D3DX_PI * 0.5f;
	}
	else if (pInputKey->GetPress(DIK_D))
	{
		m_fRotDest = D3DX_PI * 0.5f;
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
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// �ړ���
	D3DXVECTOR3 move = {0.0f, 0.0f, 0.0f};
	float fRotDestOld = m_fRotDest;

	// �ړ��ʂ�ύX
	switch (m_action) {
	case ACTION_SLIDING:
		fSpeed = SLIDING_SPEED;
		break;

	case ACTION_WALLKICK:
		fSpeed = WALLKICK_SPEED;
		break;

	case ACTION_WALLSTAND:
		fSpeed = WALLSLIDE_MOVE;
		break;
	}

	// ���͊m�F
	if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_MINUS) == true)
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
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
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_PLUS) == true)
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
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
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
	{
		move.x += -cosf(CamRot.y) * fSpeed;
		move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// �ړ�������Ԃɂ���
		m_bMove = true;

	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
	{
		move.x += cosf(CamRot.y) * fSpeed;
		move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// �ړ�������Ԃɂ���
		m_bMove = true;
	}

	// �ړ��ʂ����Z���邩�m�F
	if (m_action == ACTION_SLIDING || m_action == ACTION_WALLKICK) {
		
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

		switch (m_action) {
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
			if (static_cast<float>(fabs(fRotDest)) >= D3DX_PI * 0.25f) {
				m_fRotDest = fRotDestOld;
				return;
			}
			break;
		}
	}
	else if (m_action == ACTION_WALLSTAND) {
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

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// ����
	if (pInputPad->GetTrigger(CInputPad::BUTTON_B, m_nId))
	{
		if (m_bJump == false)
		{// �W�����v���Ă��Ȃ��ꍇ
			m_bJump = true;
		
			
			CParticle::Create(m_Info.pos, CEffect::TYPE_JUMP);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);

			if (m_action == ACTION_SLIDING) {
				m_Info.move.y = SLIDEJUMP;
				m_Info.move.x *= SLIDEJUMP_SPEED;
				m_Info.move.z *= SLIDEJUMP_SPEED;
				m_action = ACTION_SLIDEJUMP;
			}
			else {
				m_Info.move.y = JUMP;
			}
		}
		else {	// �W�����v���Ă���
			if (m_action == ACTION_WALLSTAND) {	// �ǂ��蒆
				m_Info.move += m_ColiNor * WALLKICK_MOVE;
				m_Info.move.y = JUMP;
				m_fRotDest = atan2f(-m_Info.move.x, -m_Info.move.z);
				m_action = ACTION_WALLKICK;
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

	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// ���݂̈ړ��ʂ��擾
	float fMove = static_cast<float>((fabs(m_Info.move.x) + fabs(m_Info.move.z)));

	// ���͊J�n���̈ړ��ʂ��擾
	if (pInputPad->GetTrigger(CInputPad::BUTTON_A, m_nId)) {
		m_Info.fSlideMove = fMove;
	}

	// ����
	if (fMove > SLIDING_MINMOVE && m_Info.fSlideMove >= SLIDING_STARTMOVE) {	// �X���C�f�B���O�ł���ړ��ʂ��J�n���ɂ��ړ��ʂ�����
		if (pInputPad->GetPress(CInputPad::BUTTON_A, m_nId))
		{
			if (m_bJump == false)
			{// �W�����v���Ă��Ȃ��ꍇ
				m_action = ACTION_SLIDING;
				bSlide = true;
			}
		}
	}

	// �I���m�F
	if(!bSlide){	// �X���C�f�B���O���Ă��Ȃ�
		if (m_action == ACTION_SLIDING) {	// ���Ă���
			m_action = ACTION_NEUTRAL;
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

		if (m_action == ACTION_WALLSTAND) {	// �ǂ����Ԃ̏ꍇ
			m_action = ACTION_NEUTRAL;
		}
		return;
	}

	// �ǂ��蔻��
	if (m_ColiNor.x != 0.0f || m_ColiNor.z != 0.0f) {	// �I�u�W�F�N�g�ɐG��Ă���
		m_action = ACTION_WALLSTAND;
	}
	else { // �G��Ă��Ȃ�
		if (m_action == ACTION_WALLSTAND) { // �ǂ����Ԃ̏ꍇ
			m_action = ACTION_NEUTRAL;

			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// �㉺�ǂ�����g�p��
				m_pBody->GetMotion()->Set(m_action);
				m_pLeg->GetMotion()->Set(m_action);
			}
		}
	}
}

//===============================================
// �d�͐ݒ�
//===============================================
void CPlayer::Gravity(void)
{
	float fGravity = GRAVITY;
	if (m_action == ACTION_WALLSTAND) {	// �ǂ��蒆
		if (m_Info.move.y <= 0.0f) {
			fGravity = 0.0f;
			m_Info.move.y = WALLSLIDE_GRAVITY;
		}
	}
	else if (m_action == ACTION_SLIDEJUMP) {
		fGravity = SLIDEJUMP_GRAVITY;
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
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_SPAWN;

			if (m_pBody != nullptr) {
				m_pBody->SetDraw();
			}

			if (m_pLeg != nullptr) {
				m_pLeg->SetDraw();
			}

			CModel *pModel = m_pLeg->GetParts(0);  // ���̃p�[�c

			// ���̃p�[�e�B�N������
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
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
	
	if (m_nLife != nOldLife)
	{
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DAMAGE);
		m_Info.fStateCounter = DAMAGE_INTERVAL;
		m_Info.state = STATE_DAMAGE;

		if (m_nLife <= 0)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEATH);
			m_nLife = 0;
			m_Info.state = STATE_DEATH;
			m_Info.fStateCounter = DAMAGE_APPEAR;

			CModel *pModel = m_pLeg->GetParts(0);  // ���̃p�[�c

			// ���̃p�[�e�B�N������
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_BLACKSMAKE);

			if (m_pBody != nullptr){
				m_pBody->SetDraw(false);
			}

			if (m_pLeg != nullptr){
				m_pLeg->SetDraw(false);
			}
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

	if (m_Info.state == STATE_DAMAGE)
	{// �_���[�W���
		m_pBody->GetMotion()->Set(m_action);
		m_pLeg->GetMotion()->Set(m_action);

		if (m_pBody->GetMotion()->GetEnd())
		{// ���[�V�����I��
			m_action = ACTION_NEUTRAL;	// �ێ���ԂɕύX
		}
		else
		{
			return;
		}
	}

	if (m_action > ACTION_JUMP) {	// �h���A�N�V�����̏ꍇ
		// �h�����[�V�����ݒ�
		
		switch (m_action) {
		case ACTION_SLIDING:
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_WALLSTAND:
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_WALLKICK:
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		default:

			break;
		}

		return;
	}
	else if (!m_bJump && !m_bMove && 
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// �������Ă��Ȃ�
		m_action = ACTION_NEUTRAL;
		m_pBody->GetMotion()->BlendSet(m_action);
	}
	else if(m_bJump && 
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// �W�����v����
		m_action = ACTION_JUMP;
		m_pBody->GetMotion()->BlendSet(m_action);
	}
	else if (m_bMove &&
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// �ړ�����
		m_action = ACTION_WALK;
		m_pBody->GetMotion()->BlendSet(m_action);
	}

	if (m_action == ACTION_SLIDING) {
		m_pLeg->GetMotion()->Set(m_action);
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
bool CPlayer::HitCheck(D3DXVECTOR3 pos, float fRange, int nDamage)
{
	bool m_bValue = false;
	if (m_Info.state != STATE_NORMAL) {
		return m_bValue;
	}

	if (m_pBody == nullptr) {
		return m_bValue;
	}

	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f,
		m_pBody->GetParts(1)->GetMtx()->_42 - ObjPos.y + pFile->GetMax(m_pBody->GetParts(1)->GetId()).y,
		0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, -10.0f, 0.0f);

	if (pos.y >= ObjPos.y + vtxMax.y || pos.y <= ObjPos.y - vtxMin.y) {	// �����͈͊O
		return m_bValue;
	}

	// �͈͓��`�F�b�N
	float fLength =
		sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	m_bValue = true;
	Damage(nDamage);

	return m_bValue;
}

//===============================================
// �w�胂�[�V�����ɐݒ�
//===============================================
void CPlayer::SetMotion(int nMotion) {
	if (m_pBody == nullptr) {
		return;
	}

	if (m_pBody->GetMotion() == nullptr) {
		return;
	}

	m_pBody->GetMotion()->InitSet(nMotion);

	if (m_pLeg == nullptr) {
		return;
	}

	if (m_pLeg->GetMotion() == nullptr) {
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
	CParticle::Create(pos, CEffect::TYPE_RESULTZITABATA);
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

		if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {
			m_pLeg->Update();
		}

		
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