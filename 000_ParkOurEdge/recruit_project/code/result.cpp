//===============================================
//
// ���U���g��ʂ̊Ǘ����� [result.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "result.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "objectX.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "result.h"
#include "fileload.h"
#include "meshdome.h"
#include "character.h"
#include "motion.h"
#include "camera.h"
#include "player.h"
#include "ranking.h"
#include "object_manager.h"
#include "particle.h"
#include "player_manager.h"
#include "time.h"

// �}�N����`
#define RANKING_FILE	"data\\FILE\\ranking.bin"	// �����L���O�t�@�C��
#define MOVE_TIMER	(660)

// �������O���
namespace {
	const float PLAYER_MOVESIZE = (100.0f);	// �v���C���[�̉��ړ��T�C�Y
	const float PLAYER_SPACE = (200.0f);	// �v���C���[���m�̊Ԃ̋���
	const float PLAYER_POSY = (2000.0f);	// �v���C���[�z�u��{Y���W
	const float PLAYER_RANK_POSY = (-350.0f);	// �v���C���[�̏��ʂ��Ƃɏグ��Y���W
	const float PLAYER_UPPOSY = (300.0f);	// �ő�l���ɂ���ĕς��v���C���[�̐ݒuY���W
	const float PLAYER_RANKMOVEPOS_Y = (900.0f);	// �����N�ƃX�R�A�����Ă���m�FY���W
	const float CAMERA_MOVESTARTPOSY = (1400.0f);	// �J���������n��Y���W
	const float PLAYER_GRAVITY = (-15.0f);	// �v���C���[�̗������x
	const float RANK_DOWNSPEED = (20.0f);	// �����N�ƃX�R�A�̗������x
	const D3DXVECTOR3 TOTALSCORE_POS = {SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.9f, 0.0f};	// ���v�X�R�A�̐ݒu���W
	const D3DXVECTOR3 SCORE_POS = {SCREEN_WIDTH * 0.43f, -SCREEN_HEIGHT * 1.4f, 0.0f};		// �l�X�R�A�̐ݒu���W
	const float SCORE_MOVESIZE = (130.0f);	// �X�R�A�̔z�u�ړ��T�C�Y
	const float SCORE_SPACE = (270.0f);		// �X�R�A�Ԃ̋��
	const float TOTALSCORE_SPACE = (1.0f);	// ���v�X�R�A�̐�����
	const float SCORE_NUMSPACE = (1.0f);
	const D3DXVECTOR3 RANK_POS = { SCREEN_WIDTH * 0.5f, -SCREEN_HEIGHT * 1.3f, 0.0f };		// ���v�X�R�A�̐ݒu���W
	const float RANK_MOVESIZE = (135.0f);	// �����N�̔z�u�ړ��T�C�Y
	const float RANK_SPACE = (270.0f);		// �����N�Ԃ̋��
	const D3DXVECTOR2 RANK_SIZE = { 50.0f, 30.0f };	// �����N�|���S���T�C�Y
	const D3DXVECTOR3 CLEAR_SETPOS = { -SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f};	// clear�̔z�u���W
	const D3DXVECTOR2 CLEAR_SIZE = {500.0f, 100.0f };	// �N���A�̃T�C�Y
	const D3DXVECTOR3 CLEAR_POS = { SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.9f, 0.0f };	// clear�̔z�u���W
	const float SCORE_MOVEX = (SCREEN_WIDTH * 0.2f);
	const float FAILED_ROTATE = (D3DX_PI * 0.125f);
	const float CLEAR_MOVE = (20.0f);
	const float CLEAR_ROTMOVEZ = (D3DX_PI * 0.015f);
	const float FAILED_UPY = (-2.5f);
	const float FAILED_NOBINOBISPEED = (0.025f);
	const float FAILED_NOBINOBISIZE = (10.0f);
	const float CLEAR_NOBINOBISPEED = (0.075f);	// �N���A�����ۂ̐L�яk�݂��鑬�x
	const float CLEAR_NOBINOBISIZE = (50.0f);	// �L�т�{��
	const int PLAYER_MAXMOTION = (13);			// �v���C���[�̍ő僂�[�V������
	const int SCORE_MINTORURURURU = (60);		// �Ƃ���Ƃ���̍����Œ�l
	const int SCORE_TORURURUSPEED = (13);		// �Ƃ���Ƃ���̍����␳���x
	const D3DXCOLOR CLEAR_COL = { 0.0f, 0.0f, 1.0f, 1.0f };
	const D3DXCOLOR FAILED_COL = { 1.0f, 0.0f, 0.0f, 1.0f };
}

// �ÓI�����o�ϐ�
int CResult::m_nScore = 0;

//===============================================
// �R���X�g���N�^
//===============================================
CResult::CResult()
{
	m_pMeshSky = NULL;
	m_pTime = NULL;
	m_pObjClear = nullptr;
}

//===============================================
// �f�X�g���N�^
//===============================================
CResult::~CResult()
{

}

//===============================================
// ����������
//===============================================
HRESULT CResult::Init(void)
{
	CMeshDome::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f, 10.0f, 3, 10, 10);

	if (m_pFileLoad == NULL)
	{// �g�p���Ă��Ȃ��ꍇ
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\result.txt");
		}
	}

	//�J����������
	{
		CManager::GetInstance()->GetCamera()->SetLength(300.0f);
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(1.0f, -D3DX_PI * 0.5f, 2.63f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(0.0f, 137.77f, -301.94f));
		D3DVIEWPORT9 viewport;

		//�v���C���[�Ǐ]�J�����̉�ʈʒu�ݒ�
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		CManager::GetInstance()->GetCamera()->SetViewPort(viewport);
	}

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CResult::Uninit(void)
{
	if (m_pTime != nullptr) {
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}
	if (m_pFileLoad != nullptr)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// �������̊J��
		m_pFileLoad = nullptr;
	}

	CRanking::SetScore(m_nScore);
	m_nScore = 0;
	CManager::GetInstance()->GetCamera()->SetActive(true);

	CManager::GetInstance()->GetSound()->Stop();
}

//===============================================
// �X�V����
//===============================================
void CResult::Update(void)
{

	if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_A, 0) ||
		CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_RANKING);
	}

	CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CResult::Draw(void)
{
	CScene::Draw();
}
