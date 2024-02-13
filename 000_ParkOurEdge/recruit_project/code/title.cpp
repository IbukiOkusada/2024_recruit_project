//===============================================
//
// �^�C�g����ʂ̊Ǘ����� [title.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "fileload.h"
#include "sound.h"
#include "camera.h"
#include "objectX.h"
#include "meshdome.h"
#include "object3D.h"
#include "texture.h"
#include "player.h"

//===============================================
// �������O���
//===============================================
namespace {
	const D3DXVECTOR3 ENTERPOS = { 0.0f, 0.0f, 0.0f };	// ENTER ���W
	const D3DXVECTOR3 ENTERROT = { 0.0f, 0.0f, 0.0f };	// ENTER ����
	const int AUTOMOVE_RANKING = 640;	// �����L���O�����J�ڎ���
	const int MOVE_TUTORIAL = 120;		// �`���[�g���A���ɑJ�ڂ���܂ł̎���
	const int PLAYER_WALLKICKTIMER = 40;
	const int ENEMY_NUM = 3;			//���o�p�G�o����
	const char* FILEPASS = "data\\TXT\\player";	// �t�@�C���̃p�X
	const char* FILEEXT = ".txt";				// �t�@�C���̊g���q
	const int FILEPASS_SIZE = (200);			// �t�@�C���̃p�X�T�C�Y
}

//===============================================
// �R���X�g���N�^
//===============================================
CTitle::CTitle()
{
	m_nCounterRanking = 0;
	m_nCounterTutorial = 0;
	m_nCounterRanking = 0;
	m_bPush = false;
	m_pFileLoad = nullptr;
	m_pEnter = nullptr;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTitle::~CTitle()
{

}

//===============================================
// ����������
//===============================================
HRESULT CTitle::Init(void)
{
	//�h�[���ǉ�
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);

	if (m_pFileLoad == nullptr) {
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != nullptr)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\title_model.txt");			//���f����
		}
	}

	// �J�����̏����ʒu�ݒ�
	CManager::GetInstance()->GetCamera()->SetPositionV(D3DXVECTOR3(-874.3f, 1124.15f, 1717.2f));
	CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(80.0f, 95.0f, 220.0f));
	CManager::GetInstance()->GetCamera()->SetLength(350.0f);
	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -2.1f, 1.79f));

	// �v���C���[�̐���
	char aBodyPass[FILEPASS_SIZE] = "";		// ���̃p�X
	char aLegPass[FILEPASS_SIZE] = "";		// �����g�p�X
	sprintf(&aBodyPass[0], "%s\\motion_body%s", FILEPASS, FILEEXT);
	sprintf(&aLegPass[0], "%s\\motion_leg%s", FILEPASS, FILEEXT);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		&aBodyPass[0], &aLegPass[0]);
	m_pPlayer->SetDraw(true);
	m_pPlayer->SetMotion(CPlayer::ACTION_TITLENEUTRAL);

	// �J�ڃ^�C�}�[�ݒ�
	m_nCounterTutorial = MOVE_TUTORIAL;
	m_nCounterRanking = AUTOMOVE_RANKING;

	// �T�E���h�Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTitle::Uninit(void)
{
	if (m_pFileLoad != nullptr)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// �������̊J��
		m_pFileLoad = nullptr;
	}
}

//===============================================
// �X�V����
//===============================================
void CTitle::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0)) {
		if (!m_bPush) {
			if (m_pPlayer != nullptr) {
				m_pPlayer->SetMotion(CPlayer::ACTION_WALK);
			}
		}

		m_bPush = true;
	}

	if (m_bPush) {
		m_nCounterTutorial--;

		// �J�ڂ܂ł̃v���C���[�ړ�
		if (m_nCounterTutorial == PLAYER_WALLKICKTIMER) {	// �ǃL�b�N�^�C�~���O
			if (m_pPlayer != nullptr) {
				m_pPlayer->SetMotion(CPlayer::ACTION_WALLKICK);
				m_pPlayer->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				D3DXVECTOR3 move = { 0.0f, 16.0f, 0.0f };
				m_pPlayer->SetMove(move);
				m_pPlayer->SetType(CPlayer::TYPE_ACTIVE);
				m_pPlayer->SetRotDiff(0.0f);
			}
		}
		else if (m_nCounterTutorial <= PLAYER_WALLKICKTIMER) {	// �ǃL�b�N��
			if (m_pPlayer != nullptr) {
				D3DXVECTOR3 pos = m_pPlayer->GetPosition();
				pos.z += -6.0f;
				m_pPlayer->SetPosition(pos);
			}
		}
		else {	// ����܂�
			if (m_pPlayer != nullptr) {
				D3DXVECTOR3 pos = m_pPlayer->GetPosition();
				pos.z += 4.0f;
				m_pPlayer->SetPosition(pos);
			}
		}

		if (m_nCounterTutorial <= 0) {
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		}
	}
	else {
		m_nCounterRanking--;
		if (m_nCounterRanking <= 0) {
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RANKING);
		}
	}

	CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CTitle::Draw(void)
{
	CScene::Draw();
}
