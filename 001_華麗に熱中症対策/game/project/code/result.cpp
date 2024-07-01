//===============================================
//
// リザルト画面の管理処理 [result.cpp]
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
#include "score.h"
#include "ranking.h"
#include "fileload.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "sound.h"
#include "slow.h"

//===============================================
// 静的メンバ変数
//===============================================
int CResult::m_nSuvNum = 0;
int CResult::m_nDeadNum = 0;

//===============================================
// コンストラクタ
//===============================================
CResult::CResult()
{
	m_pMeshSky = NULL;
	m_nTimer = 0;
}

//===============================================
// デストラクタ
//===============================================
CResult::~CResult()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CResult::Init(void)
{
	CObject2D *p = CObject2D::Create(7);
	p->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\result_logo.png"));
	p->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.15f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.1f);

	for (int nCnt = 0; nCnt < SCORE_MAX; nCnt++)
	{
		m_apScore[nCnt] = CScore::Create(D3DXVECTOR3(700.0f, 300.0f + nCnt * 150.0f, 0.0f), 8);

		switch (nCnt)
		{
		case SCORE_SUV:
			m_apScore[nCnt]->Set(m_nSuvNum * 1000);
			break;

		case SCORE_DEAD:
			m_apScore[nCnt]->Set(m_nDeadNum * 200);
			break;

		case SCORE_ALL:
			m_apScore[nCnt]->Set(m_nSuvNum * 1000 + m_nDeadNum * -200);
			break;
		}
	}

	for (int nCnt = 0; nCnt < SCORE_ALL; nCnt++)
	{
		m_apNum[nCnt] = CScore::Create(D3DXVECTOR3(370.0f, 300.0f + nCnt * 150.0f, 0.0f), 3);

		switch (nCnt)
		{
		case SCORE_SUV:
			m_apNum[nCnt]->Set(m_nSuvNum);
			break;

		case SCORE_DEAD:
			m_apNum[nCnt]->Set(m_nDeadNum);
			break;
		}
	}

	p = CObject2D::Create(7);
	p->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\resultminus.png"));
	p->SetPosition(D3DXVECTOR3(630.0f, 300.0f + 150.0f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.05f, SCREEN_HEIGHT * 0.05f);

	p = CObject2D::Create(7);
	p->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\resultdead.png"));
	p->SetPosition(D3DXVECTOR3(230.0f, 280.0f + 150.0f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);

	p = CObject2D::Create(7);
	p->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\resultsuv.png"));
	p->SetPosition(D3DXVECTOR3(230.0f, 280.0f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);

	p = CObject2D::Create(7);
	p->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\resultfont.png"));
	p->SetPosition(D3DXVECTOR3(400.0f, 600.0f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.1f);

	// オブジェクト生成
	m_pMeshSky = CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20000.0f, 10.0f, 3, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17000.0f, 100.0f, 3, 10, 10);

	// 外部ファイル読み込みの生成
	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\model.txt");
		}
	}

	CManager::GetSound()->Play(CSound::LABEL_BGM_RESULT);

	CManager::GetSlow()->SetSlow(false);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CResult::Uninit(void)
{
	m_nSuvNum = 0;
	m_nDeadNum = 0;

	// スコア
	for (int nCnt = 0; nCnt < SCORE_MAX; nCnt++)
	{
		if (m_apScore[nCnt] != NULL)
		{
			if (nCnt == SCORE_ALL)
			{
				CRanking::SetScore(m_apScore[nCnt]->GetNum());
			}

			m_apScore[nCnt]->Uninit();
			delete m_apScore[nCnt];	// メモリの開放
			m_apScore[nCnt] = NULL;	// 使用していない状態にする
		}
	}

	for (int nCnt = 0; nCnt < SCORE_ALL; nCnt++)
	{
		if (m_apNum[nCnt] != NULL)
		{
			m_apNum[nCnt]->Uninit();
			delete m_apNum[nCnt];	// メモリの開放
			m_apNum[nCnt] = NULL;	// 使用していない状態にする
		}
	}

	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	m_pMeshSky = NULL;
}

//===============================================
// 更新処理
//===============================================
void CResult::Update(void)
{
	CInputPad *pInputPad = CManager::GetInputPad();

	m_nTimer++;

	if (m_nTimer > 600)
	{
		m_nTimer = 0;
		CManager::GetFade()->Set(CScene::MODE_RANKING);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0))
	{
		CManager::GetFade()->Set(CScene::MODE_RANKING);
	}

	m_pMeshSky->SetColor(0.9f);

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CResult::Draw(void)
{
	CScene::Draw();
}
