//===============================================
//
// �����L���O��ʂ̊Ǘ����� [ranking.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "ranking.h"
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
#include "time.h"
#include "fileload.h"
#include "meshdome.h"
#include "character.h"
#include "motion.h"
#include "camera.h"
#include "player.h"

// �}�N����`
#define RANKING_FILE	"data\\FILE\\ranking.bin"	// �����L���O�t�@�C��
#define MOVE_TIMER	(660)

namespace
{
	const char* FILEPASS = "data\\TXT\\player";	// �t�@�C���̃p�X
	const char* FILEEXT = ".txt";				// �t�@�C���̊g���q
	const int FILEPASS_SIZE = (200);			// �t�@�C���̃p�X�T�C�Y
}

int CRanking::m_nScore = 0;

//===============================================
// �R���X�g���N�^
//===============================================
CRanking::CRanking()
{
	m_pMeshSky = NULL;
	m_nTimer = 0;
	m_pTime = NULL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CRanking::~CRanking()
{

}

//===============================================
// ����������
//===============================================
HRESULT CRanking::Init(void)
{
	//CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
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

	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 1.0f, 1.61f));
	CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(1050.0f, 50.0f, 170.0f));
	CManager::GetInstance()->GetCamera()->SetActive(false);
	m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
	m_pTime->Set(m_nScore);
	m_pTime->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));

	int aScore[NUM_RANK] = {};	// �X�R�A�i�[�p
	m_nRank = -1;	//�����N�C�����ĂȂ����

	// �f�[�^�̓ǂݍ���
	Load(&aScore[0]);

	// �f�[�^�̃\�[�g
	Sort(&aScore[0]);

	// �����N�C���m�F
	RankIn(&aScore[0], m_nScore);

	// ���ʕ��X�R�A�̐���
	for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
	{
		if (m_apScore[nCntRank] == NULL)
		{
			CObject2D* p = CObject2D::Create(7);
			p->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ranking001.png"));
			p->SetPosition(D3DXVECTOR3(675.0f, 250.0f + nCntRank * 100.0f, 0.0f));
			p->SetSize(40, 50);
			p->SetVtx(nCntRank + 1, 10, 1);

			m_apScore[nCntRank] = CTime::Create(D3DXVECTOR3(835.0f, 250.0f + nCntRank * 100.0f, 0.0f));
			m_apScore[nCntRank]->Set(aScore[nCntRank]);

			if (m_nRank == nCntRank)
			{
				m_apScore[nCntRank]->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
			}
		}
	}

	char aBodyPass[FILEPASS_SIZE] = "";		// ���̃p�X
	char aLegPass[FILEPASS_SIZE] = "";		// �����g�p�X

	sprintf(&aBodyPass[0], "%s\\motion_body%s", FILEPASS, FILEEXT);
	sprintf(&aLegPass[0], "%s\\motion_leg%s", FILEPASS, FILEEXT);

	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(800.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), &aBodyPass[0], &aLegPass[0]);
	pPlayer->BindId(0);
	pPlayer->SetType(CPlayer::TYPE_NONE);
	pPlayer->SetDraw(true);
	pPlayer->SetMotion(16);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRanking::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
	{
		if (m_apScore[nCntRank] != NULL)
		{
			m_apScore[nCntRank]->Uninit();
			delete m_apScore[nCntRank];	// �������̊J��
			m_apScore[nCntRank] = NULL;	// �g�p���Ă��Ȃ���Ԃɂ���
		}
	}

	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = NULL;
	}

	if (m_pFileLoad != nullptr) {
		m_pFileLoad->Uninit();
		delete m_pFileLoad;
		m_pFileLoad = nullptr;
	}

	CManager::GetInstance()->GetCamera()->SetActive(true);
	m_nScore = 0;
}

//===============================================
// �X�V����
//===============================================
void CRanking::Update(void)
{
	m_nTimer++;

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) || m_nTimer > MOVE_TIMER
		|| CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_A, 0) || CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_TITLE);
	}

	CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CRanking::Draw(void)
{
	CScene::Draw();
}

//===============================================
// �����L���O�f�[�^�ۑ�
//===============================================
void CRanking::Save(int* pScore)
{
	FILE* pFile;

	pFile = fopen(RANKING_FILE, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

	 //�f�[�^��ǂݍ���
		fwrite(pScore, sizeof(int), NUM_RANK, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//===============================================
// �����L���O�f�[�^�ǂݍ���
//===============================================
void CRanking::Load(int* pScore)
{
	FILE* pFile;

	pFile = fopen(RANKING_FILE, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

	 //�f�[�^��ǂݍ���
		fread(pScore, sizeof(int), NUM_RANK, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
	 //�v�f�����Ă���
		for (int nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)
		{
			pScore[nCntRanking] = 6000 * 10 + (6000 * nCntRanking);
		}
	}
}

//===============================================
// �����L���O�f�[�^�\�[�g
//===============================================
void CRanking::Sort(int* pScore)
{
	// �~���\�[�g
	for (int nCntFst = 0; nCntFst < NUM_RANK - 1; nCntFst++)
	{
		int nTempNum = nCntFst;	// ���̈�ԑ傫���ԍ�

		for (int nCntSec = nCntFst + 1; nCntSec < NUM_RANK; nCntSec++)
		{
			if (pScore[nCntSec] < pScore[nTempNum])
			{// �l���傫���ꍇ
				nTempNum = nCntSec;	// �傫���ԍ���ύX
			}
		}

		if (nTempNum != nCntFst)
		{// �ύX����ꍇ
			int nTemp = pScore[nCntFst];
			pScore[nCntFst] = pScore[nTempNum];
			pScore[nTempNum] = nTemp;
		}
	}
}

//===============================================
// �����L���O�C���m�F
//===============================================
void CRanking::RankIn(int* pScore, int nRanking)
{
	if (nRanking < pScore[NUM_RANK - 1] && nRanking != 0)
	{
		pScore[NUM_RANK - 1] = nRanking;

		// �\�[�g����
		Sort(pScore);

		// �ۑ�����
		Save(pScore);

		//�����N�C���������ʂ��m�F����
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			if (pScore[nCntRank] == nRanking)
			{
				m_nRank = nCntRank;	// �����N�C���������ʂ�ۑ�			
				break;
			}
		}
	}
}