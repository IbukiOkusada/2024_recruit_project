//===============================================
//
// ランキング画面の管理処理 [ranking.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "ranking.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "light.h"
#include "sound.h"
#include "camera.h"
#include "number.h"
#include "score.h"
#include "objectX.h"
#include "item.h"
#include "object2D.h"
#include "meshdome.h"
#include "camera.h"
#include "fileload.h"
#include "sound.h"

//===============================================
// マクロ定義
//===============================================
#define RANKING_FILE_ONE	"data\\FILE\\rankingone.bin"	// ランキングファイル
#define RANKING_FILE_TEAM	"data\\FILE\\rankingteam.bin"	// ランキングファイル
#define AUTOMOVE_TITLE	(600)						// タイトル自動遷移タイマー

//===============================================
// 静的メンバ変数
//===============================================
CScore *CRanking::m_apScore[NUM_RANKING][NUM_RANK] = {};	// ランキングの
CScore *CRanking::m_apNowScore[NUM_NOWSCORE] = {};	// ランキングのポインタ
int CRanking::m_nScore = 0;					// スコア
int CRanking::m_nTotalScore = 0;			// スコア

//===============================================
// コンストラクタ
//===============================================
CRanking::CRanking()
{
	// 値をクリアする
	m_nTimer = 0;
	m_nRank = 0;
	m_nCounter = 0;
	m_bOne = false;
	m_bTotal = false;
	m_nOne = -1;
	m_nTotal = -1;

	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			m_apScore[nCntRanking][nCntRank] = nullptr;
		}
	}
}

//===============================================
// デストラクタ
//===============================================
CRanking::~CRanking()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CRanking::Init(void)
{
	int aScore[NUM_RANK] = {};	// スコア格納用
	int aTotalScore[NUM_RANK] = {};	// スコア格納用
	m_nRank = -1;	//ランクインしてない状態

	// 外部ファイル読み込みの生成
	if (nullptr == m_pFileLoad)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\ranking.txt");			//モデル類
		}
	}

	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{
			m_pObjectRank[nCntRanking][nCnt] = CObject2D::Create(7);
			m_pObjectRank[nCntRanking][nCnt]->SetPosition(D3DXVECTOR3(150.0f + nCntRanking * 600.0f, 400.0f + (nCnt* 100.0f), 0.0f));
			m_pObjectRank[nCntRanking][nCnt]->SetLength(150.0f, 75.0f);
			m_pObjectRank[nCntRanking][nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\rank00.png"));
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_pObject[nCnt] = CObject2D::Create(7);
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pObject[nCnt]->SetPosition(D3DXVECTOR3(450.0f + nCnt * 600.0f, 300.0f, 0.0f));
		m_pObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\new_record00.png"));
		m_pObject[nCnt]->SetLength(200.0f, 75.0f);
		m_pObject[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	m_pObject[2]->SetPosition(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
	m_pObject[2]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ranking_one00.png"));
	m_pObject[2]->SetLength(250.0f, 100.0f);

	m_pObject[3]->SetPosition(D3DXVECTOR3(1000.0f, 100.0f, 0.0f));
	m_pObject[3]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ranking_team00.png"));
	m_pObject[3]->SetLength(250.0f, 100.0f);

	//個人
	// データの読み込み
	Load(&aScore[0], RANKING_FILE_ONE);

	// データのソート
	Sort(&aScore[0]);

	// ランクイン確認
	RankIn(&aScore[0], m_nScore, RANKING_FILE_ONE, 0);

	if (m_bOne == true)
	{
		m_pObject[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//合計
	// データの読み込み
	Load(&aTotalScore[0], RANKING_FILE_TEAM);

	// データのソート
	Sort(&aTotalScore[0]);

	// ランクイン確認
	RankIn(&aTotalScore[0], m_nTotalScore, RANKING_FILE_TEAM, 1);
	
	if (m_bTotal == true)
	{
		m_pObject[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//今回のスコア
	for (int nCnt = 0; nCnt < NUM_NOWSCORE; nCnt++)
	{
		m_apNowScore[nCnt] = CScore::Create(D3DXVECTOR3(150.0f + nCnt * 700.0f, 200.0f, 0.0f), 6 + nCnt * 2, 1.0f, 23.0f, 35.0f);
	}

	m_apNowScore[0]->SetScore(m_nScore);
	m_apNowScore[1]->SetScore(m_nTotalScore);

	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			m_apScore[nCntRanking][nCntRank] = CScore::Create(D3DXVECTOR3(300.0f + nCntRanking * 600.0f, 400.0f + nCntRank * 100.0f, 0.0f), 6 + nCntRanking * 2, 1.0f, 23.0f, 35.0f);
		}
	}

	for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
	{
		m_apScore[0][nCntRank]->SetScore(aScore[nCntRank]);
		m_apScore[1][nCntRank]->SetScore(aTotalScore[nCntRank]);
	}

	if (m_nRank != -1)
	{
		if (m_nOne > -1) {
			m_apScore[0][m_nOne]->SetClo(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_nTotal > -1) {
			m_apScore[1][m_nTotal]->SetClo(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	//カメラ初期化
	{
		CManager::GetInstance()->GetCamera()->SetLength(2200.0f);
		CManager::GetInstance()->GetCamera()->SetPositionV(D3DXVECTOR3(387.0f, 793.0f, -2328.0f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(-400.0f, 441.0f, -304.0f));
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(1.0f, -1.20f, 1.41f));

		D3DVIEWPORT9 viewport;
		//プレイヤー追従カメラの画面位置設定
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		CManager::GetInstance()->GetCamera()->SetViewPort(viewport);
		CManager::GetInstance()->GetCamera()->SetActive(false);
	}

	//ドーム追加
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000.0f, 6000.0f, 3, 8, 8);

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RANKING);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRanking::Uninit(void)
{
	m_nScore = 0;
	m_nTotalScore = 0;

	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			if (m_apScore[nCntRanking][nCntRank] != nullptr) {
				m_apScore[nCntRanking][nCntRank]->Uninit();
				delete m_apScore[nCntRanking][nCntRank];
				m_apScore[nCntRanking][nCntRank] = nullptr;
			}
		}
	}

	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{
			if (m_pObjectRank[nCntRanking][nCnt] != nullptr) {
				m_pObjectRank[nCntRanking][nCnt]->Uninit();
			}
		}
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apNowScore[nCnt] != nullptr) {
			m_apNowScore[nCnt]->Uninit();
			delete m_apNowScore[nCnt];
			m_apNowScore[nCnt] = nullptr;
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_pObject[nCnt] != nullptr) {
			m_pObject[nCnt]->Uninit();
		}
	}

	if (m_pFileLoad != nullptr)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = nullptr;
	}

	CManager::GetInstance()->GetCamera()->SetActive(true);
}

//===============================================
// 更新処理
//===============================================
void CRanking::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	//サイズの設定
	for (int nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		for (int nCnt = 0; nCnt < NUM_RANK; nCnt++)
		{
			// 頂点情報の設定
			m_pObjectRank[nCntRanking][nCnt]->SetVtx();
			m_pObjectRank[nCntRanking][nCnt]->SetVtx(nCnt, NUM_RANK, 1);
		}
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		// 頂点情報の設定
		m_pObject[nCnt]->SetVtx();	

		if (nCnt >= 4)
		{
			m_pObject[nCnt]->SetVtx(0, NUM_RANK, 1);
		}
	}

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 pos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		pos.x = ((float)(rand() % 8000 - 4000) * 0.01f)* ((float)(rand() % 100)) * 0.6f;
		pos1.x = ((float)(rand() % 8000 - 4000) * 0.01f)* ((float)(rand() % 100)) * 0.6f;

		pos.z = ((float)(rand() % 2000 - 1000) * 0.01f)* ((float)(rand() % 100)) * 0.6f;
		pos1.z = ((float)(rand() % 2000 - 1000) * 0.01f)* ((float)(rand() % 100)) * 0.6f;

		m_nCounter++;

		CItem::Create(D3DXVECTOR3(pos.x , 1500.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_COIN, CItem::STATE_DOWN);
		CItem::Create(D3DXVECTOR3(pos1.x, 1500.0f, pos1.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_COIN, CItem::STATE_DOWN);

		if (m_nCounter % 55 == 0)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_COIN00);
		}
		if (m_nCounter % 30 == 0)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_COIN01);
		}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_TITLE);
	}

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
	{
		m_nTimer++;

		if (m_nTimer >= AUTOMOVE_TITLE)
		{// タイトル自動遷移規定値
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_TITLE);
		}
	}

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CRanking::Draw(void)
{
	CScene::Draw();
}

//===============================================
// ランキングデータ保存
//===============================================
void CRanking::Save(int *pScore, const char *pFileName)
{
	FILE *pFile;

	pFile = fopen(pFileName, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//データを読み込む
		fwrite(pScore, sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}

//===============================================
// ランキングデータ読み込み
//===============================================
void CRanking::Load(int *pScore, const char *pFileName)
{
	FILE *pFile;

	pFile = fopen(pFileName, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//データを読み込む
		fread(pScore, sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		//要素を入れておく
		for (int nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)
		{
			pScore[nCntRanking] = 5 - (nCntRanking * 5);
		}
	}
}

//===============================================
// ランキングデータソート
//===============================================
void CRanking::Sort(int *pScore)
{
	// 降順ソート
	for (int nCntFst = 0; nCntFst < NUM_RANK - 1; nCntFst++)
	{
		int nTempNum = nCntFst;	// 仮の一番大きい番号

		for (int nCntSec = nCntFst + 1; nCntSec < NUM_RANK; nCntSec++)
		{
			if (pScore[nCntSec] > pScore[nTempNum])
			{// 値が大きい場合
				nTempNum = nCntSec;	// 大きい番号を変更
			}
		}

		if (nTempNum != nCntFst)
		{// 変更する場合
			int nTemp = pScore[nCntFst];
			pScore[nCntFst] = pScore[nTempNum];
			pScore[nTempNum] = nTemp;
		}
	}
}

//===============================================
// ランキングイン確認
//===============================================
void CRanking::RankIn(int *pScore, int nResult, const char *pFileName, int nNew)
{
	if (nResult > pScore[NUM_RANK - 1])
	{
		pScore[NUM_RANK - 1] = nResult;

		// ソート処理
		Sort(pScore);

		// 保存処理
		Save(pScore, pFileName);

		//ランクインした順位を確認する
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			if (pScore[nCntRank] == nResult)
			{
				m_nRank = nCntRank;	// ランクインした順位を保存		

				if (nNew == 0)
				{
					if (m_nRank != -1)
					{
						m_nOne = m_nRank;
					}
					m_bOne = true;
				}
				else if (nNew == 1)
				{
					if (m_nRank != -1)
					{
						m_nTotal = m_nRank;
					}
					m_bTotal = true;
				}

				break;
			}
		}
	}
}