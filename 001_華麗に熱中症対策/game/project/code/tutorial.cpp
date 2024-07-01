//===============================================
//
// チュートリアル画面の管理処理 [tutorial.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "score.h"
#include "time.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "fileload.h"
#include "slow.h"
#include "enemy.h"
#include "player.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "result.h"
#include "lockon.h"
#include "meshballoon.h"
#include "editor.h"
#include "thermo.h"
#include "enemy_manager.h"
#include "car_manager.h"
#include "enemy_route.h"
#include "sound.h"

//===============================================
// マクロ定義
//===============================================
#define START_TIME	(60 * 3)	// 制限時間
#define START_SCORE	(0)		// 開始スコア
#define MAP_SIZE	(100.0f)	// マップサイズ
#define STARTSET_NUMENEMY	(30)	// 開始時に配置する敵の数
#define MAX_TIME	(60 * 20)	// 最大時間

//===============================================
// 静的メンバ変数
//===============================================
CScore *CTutorial::m_pScore = NULL;		// スコアのポインタ
CSlow *CTutorial::m_pSlow = NULL;		// スロー状態へのポインタ
CMeshField *CTutorial::m_pMeshField = NULL;	// メッシュフィールドのポインタ

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial()
{
	// 値のクリア
	m_pTime = NULL;
	m_pEnemyManager = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pCarManager = NULL;
	m_pMeshDome = NULL;
}

//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTutorial::Init(void)
{
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
	CManager::GetCamera()->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, CManager::GetCamera()->GetRotation().z));

	// オブジェクト生成
	m_pMeshDome = CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20000.0f, 10.0f, 3, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17000.0f, 100.0f, 3, 10, 10);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(2800.0f, 0.0f, -3800.0f), D3DXVECTOR3(0.0f, D3DX_PI - 0.01f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data\\TXT\\motion_body.txt", "data\\TXT\\motion_leg.txt", 1);
	m_pPlayer->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI - 0.01f, 0.0f));

	// タイムの生成
	m_pTime = CTime::Create(D3DXVECTOR3(600.0f, 100.0f, 0.0f));
	m_pTime->Set(START_TIME);
	m_pTime->SetMax(MAX_TIME);

	CManager::GetCamera()->SetMode(CCamera::MODE_NORMAL);

	// 敵順路管理を生成
	if (m_pEnemyRoute == NULL)
	{
		m_pEnemyRoute = new CEnemyRoute;
		m_pEnemyRoute->Init();
	}

	// エネミーマネージャーを生成
	if (m_pEnemyManager == NULL)
	{
		m_pEnemyManager = new CEnemyManager;
		m_pEnemyManager->Init();
		//m_pEnemyManager->Spawn(STARTSET_NUMENEMY);
	}

	// カーマネージャーを生成
	if (m_pCarManager == NULL)
	{
		m_pCarManager = new CCarManager;
		m_pCarManager->Init();
	}

	CObjectX::Create(D3DXVECTOR3(3000.0f, 0.0f, -3400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\board000.x");

	CObjectX::Create(D3DXVECTOR3(2800.0f, 0.0f, -2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\board001.x");

	CObjectX::Create(D3DXVECTOR3(3100.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\board002.x");

	CObjectX::Create(D3DXVECTOR3(2300.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\board003.x");

	CObjectX::Create(D3DXVECTOR3(1400.0f, 0.0f, 570.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\board004.x");

	CObjectX::Create(D3DXVECTOR3(650.0f, 0.0f, 70.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board005.x");

	CObjectX::Create(D3DXVECTOR3(-1250.0f, 0.0f, -1350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board006.x");

	CObjectX::Create(D3DXVECTOR3(-3500.0f, 0.0f, -1350.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board007.x");

	CObjectX::Create(D3DXVECTOR3(-3500.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board008.x");

	CObjectX::Create(D3DXVECTOR3(-3500.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board008.x");

	CObjectX::Create(D3DXVECTOR3(-3650.0f, 0.0f, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board009.x");

	CObjectX::Create(D3DXVECTOR3(1950.0f, 0.0f, -450.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(1200.0f, 0.0f, -450.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(850.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-1000.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-1000.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-600.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-1400.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-1700.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-2000.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-3850.0f, 0.0f, -900.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.75f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-3150.0f, 0.0f, 890.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-4000.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-4000.0f, 0.0f, 2400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board010.x");

	CObjectX::Create(D3DXVECTOR3(-4000.0f, 0.0f, 3400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), "data\\MODEL\\board010.x");

	// 空の色変更
	SkySet();

	StartEnemy();

	// ポイントライトを有効にする
	CManager::GetLight()->EnablePointLight(true);

	CManager::GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	// エネミーマネージャー
	if (m_pEnemyManager != NULL)
	{
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = NULL;
	}

	// カーマネージャー
	if (m_pCarManager != NULL)
	{
		m_pCarManager->Uninit();
		delete m_pCarManager;
		m_pCarManager = NULL;
	}

	// 敵順路管理
	if (m_pEnemyRoute != NULL)
	{
		m_pEnemyRoute->Uninit();
		delete m_pEnemyRoute;
		m_pEnemyRoute = NULL;
	}

	// スコア
	if (m_pScore != NULL)
	{
		m_pScore->Uninit();
		delete m_pScore;	// メモリの開放
		m_pScore = NULL;	// 使用していない状態にする
	}

	// タイム
	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;	// メモリの開放
		m_pTime = NULL;	// 使用していない状態にする
	}

	m_pScore = NULL;		// スコアのポインタ
	m_pTime = NULL;		// タイムのポインタ
	m_pPlayer = NULL;	// プレイヤーのポインタ
	m_pMeshField = NULL;
	m_pFileLoad = NULL;
	m_pMeshDome = NULL;
}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	if (CManager::GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetFade()->Set(CScene::MODE_GAME);
	}

	// タイム
	if (m_pTime != NULL)
	{
		if (m_pTime->GetNum() >= MAX_TIME)
		{// 時間切れ
			//m_pTime->Set(START_TIME);
		}
		else
		{
			if (m_pPlayer->GetPosition().x <= -3700.0f && m_pPlayer->GetPosition().x >= -4100.0f && m_pPlayer->GetPosition().z >= 3700.0f)
			{
				CManager::GetFade()->Set(CScene::MODE_GAME);
			}

			// 敵の配置管理
			EnemySet();

			// 車の配置管理
			CarSet();

			SkySet();
		}
	}

	// 残り人数
	if (m_pScore != NULL)
	{
		m_pScore->Update();
	}

	if (CManager::GetSlow()->Get() == 1.0f && CManager::GetSlow()->GetOld() != 1.0f)
	{
		CLockOn::MultiDeath();
	}

	if (m_pPlayer != NULL)
	{
		CManager::GetLight()->SetPositonPointLight(m_pPlayer->GetPosition());
	}

	// 更新処理
	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTutorial::Draw(void)
{
	CScene::Draw();
}

//===================================================
// スコアの取得
//===================================================
CScore *CTutorial::GetScore(void)
{
	return m_pScore;
}

//===================================================
// スコアの取得
//===================================================
CTime *CTutorial::GetTime(void)
{
	return m_pTime;
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//===================================================
// スロー情報の取得
//===================================================
CSlow *CTutorial::GetSlow(void)
{
	return m_pSlow;
}

//===================================================
// メッシュフィールドの取得
//===================================================
CMeshField *CTutorial::GetMeshField(void)
{
	return m_pMeshField;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CTutorial::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// 読み込みデータリセット
//===================================================
void CTutorial::DataReset(void)
{
	// 終了
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

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
}

//===================================================
// 敵の配置
//===================================================
void CTutorial::EnemySet(void)
{
	if (m_pEnemyManager != NULL)
	{
		m_pEnemyManager->UpdateTutorial();
	}
}

//===================================================
// 車の配置
//===================================================
void CTutorial::CarSet(void)
{
	if (m_pCarManager != NULL)
	{
		m_pCarManager->Update();
	}
}

//===================================================
// 空の色設定
//===================================================
void CTutorial::SkySet(void)
{
	if (m_pMeshDome != NULL)
	{
		m_pMeshDome->SetColor(0.1f);
		CManager::GetLight()->SetLight(0.05f);
	}
}

void CTutorial::StartEnemy(void)
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		CEnemy::Create(D3DXVECTOR3(2650.0f + nCnt * 100.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TXT\\motion_murabito.txt");
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		CEnemy::Create(D3DXVECTOR3(1650.0f + nCnt * 100.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TXT\\motion_murabito.txt");
	}

	CEnemy::Create(D3DXVECTOR3(1750.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TXT\\motion_murabito.txt");
	CEnemy::Create(D3DXVECTOR3(1750.0f, 0.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TXT\\motion_murabito.txt");

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < 5; nCnt2++)
		{
			CEnemy::Create(D3DXVECTOR3(-2300.0f + nCnt * -150.0f, 0.0f, -2750.0f + nCnt2 * 150.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TXT\\motion_murabito.txt");
		}
	}
}