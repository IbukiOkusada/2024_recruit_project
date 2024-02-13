//===============================================
//
// ゲーム画面の管理処理 [game.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "game.h"
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
#include "filter.h"

//===============================================
// マクロ定義
//===============================================
#define START_TIME	(60 * 4)	// 制限時間
#define START_SCORE	(0)		// 開始スコア
#define MAP_SIZE	(100.0f)	// マップサイズ
#define STARTSET_NUMENEMY	(3)	// 開始時に配置する敵の数
#define MORNING_TIME	(60 * 6)	// 早朝終了時刻
#define AM_COMRUSH	(60 * 9)	// 通勤ラッシュ
#define RELAX_TIME	(60 * 11)	// リラックスタイム
#define NOON_TIME	(60 * 12.5)	// 昼休み
#define EVENT_RUSH	(60 * 15.5)	// イベント
#define PM_RELAX_TIME	(60 * 16)	// 帰宅開始時間
#define PM_GOHOME_RUSH	(60 * 19)	// 帰宅ラッシュ
#define MAX_TIME	(60 * 20 + 10)	// 最大時間
#define SPEED_UP	(30.0f)

//===============================================
// 静的メンバ変数
//===============================================
CScore *CGame::m_pScore = NULL;		// スコアのポインタ
CSlow *CGame::m_pSlow = NULL;		// スロー状態へのポインタ
CMeshField *CGame::m_pMeshField = NULL;	// メッシュフィールドのポインタ
CPause *CGame::m_pPause = NULL;		// ポーズのポインタ

//===============================================
// コンストラクタ
//===============================================
CGame::CGame()
{
	// 値のクリア
	m_pMapCamera = NULL;
	m_pMapThermo = NULL;
	m_nMaxEnemy = 0;
	m_pTime = NULL;
	m_pEnemyManager = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pEditor = NULL;
	m_pCarManager = NULL;
	m_pMeshDome = NULL;
	m_pStart = NULL;

	for (int nCnt = 0; nCnt < NUM_FILTER; nCnt++)
	{
		m_apFilter[nCnt] = NULL;
	}
}

//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CGame::Init(void)
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

	// エディットの生成
	if (m_pEditor == NULL)
	{// 使用していない場合
		m_pEditor = new CEditor;

		// 初期化
		if (m_pEditor != NULL)
		{
			m_pEditor->Init();
		}
	}

	// オブジェクト生成
	m_pMeshDome = CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20000.0f, 10.0f, 3, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17000.0f, 100.0f, 3, 10, 10);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-3900.0f, 0.0f, 3700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data\\TXT\\motion_body.txt", "data\\TXT\\motion_leg.txt", 1);

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(250.0f, 50.0f, 0.0f), 3);
	m_pScore->Set(START_SCORE);

	// タイムの生成
	m_pTime = CTime::Create(D3DXVECTOR3(550.0f, 100.0f, 0.0f));
	m_pTime->Set(START_TIME);
	m_pTime->SetMax(MAX_TIME);

	CObject2D *pUi = CObject2D::Create(7);
	pUi->SetPosition(D3DXVECTOR3(410.0f, 65.0f, 0.0f));
	pUi->SetSize(25, 30);
	pUi->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\target000.png"));

	pUi = CObject2D::Create(7);
	pUi->SetPosition(D3DXVECTOR3(MAP_SIZE, MAP_SIZE, 0.0f));
	pUi->SetSize(MAP_SIZE, MAP_SIZE);
	pUi->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\map000.png"));

	if (m_pStart == NULL)
	{
		m_pStart = CObject2D::Create(7);
		m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 0.4f, 0.0f));
		m_pStart->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.05f));
		m_pStart->SetLength(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f);
		m_pStart->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\start.png"));
	}

	// マップの生成
	if (m_pMapThermo == NULL)
	{
		m_pMapThermo = CThermo::Create();
		m_pMapThermo->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\thermo000.jpg"));
		m_pMapThermo->SetColor(D3DXCOLOR(0.7f, 0.05f, 0.05f, 0.0f));
		m_pMapThermo->SetpVtx(8000.0f, 8000.0f);
	}

	// ポーズの生成
	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}

	CManager::GetCamera()->SetMode(CCamera::MODE_NORMAL);

	m_nMaxEnemy = CObject::GetNumEnemAll();

	// ミニマップ用カメラを生成
	if (m_pMapCamera == NULL)
	{// 使用していない場合
		m_pMapCamera = new CMapCamera;

		// 初期化
		if (m_pMapCamera != NULL)
		{
			D3DVIEWPORT9 viewport;
			//プレイヤー追従カメラの画面位置設定
			viewport.X = 0;
			viewport.Y = 0;
			viewport.Width = 200;
			viewport.Height = 200;
			viewport.MinZ = 0.0f;
			viewport.MaxZ = 0.1f;
			m_pMapCamera->Init();
			m_pMapCamera->SetLength(11500.0f);
			m_pMapCamera->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.00001f));
			m_pMapCamera->SetViewPort(viewport);
		}
	}

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
		m_pEnemyManager->Spawn(STARTSET_NUMENEMY);
	}

	// カーマネージャーを生成
	if (m_pCarManager == NULL)
	{
		m_pCarManager = new CCarManager;
		m_pCarManager->Init();
	}

	// 敵の配置管理
	EnemySet();

	// 車の配置管理
	CarSet();

	// 空の色変更
	SkySet();

	// スポットライトをオン
	CManager::GetLight()->EnablePointLight(true);

	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	m_apFilter[0] = CHeatFilter::Create();
	m_apFilter[1] = CHeatFilter::Create();
	m_apFilter[1]->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\smake000.jpg"));

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
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
		CResult::SetSuv(m_pEnemyManager->GetSuv());
		CResult::SetDead(m_pEnemyManager->GetDead());
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

	// スロー
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;	// メモリの開放
		m_pPause = NULL;	// 使用していない状態にする
	}

	// エディター情報の廃棄
	if (m_pEditor != NULL)
	{// 使用している場合
		
		m_pEditor->Uninit();
		delete m_pEditor;	// メモリの開放
		m_pEditor = NULL;	// 使用していない状態にする
	}

	// ミニマップカメラ
	if (m_pMapCamera != NULL)
	{
		m_pMapCamera->Uninit();
		delete m_pMapCamera;
		m_pMapCamera = NULL;
	}

	if (m_pMapThermo != NULL)
	{
		m_pMapThermo = NULL;
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
void CGame::Update(void)
{
	if (m_pStart != NULL)
	{
		D3DXVECTOR3 pos = m_pStart->GetPosition();

		// 移動
		if (pos.x > SCREEN_WIDTH * 0.75f)
		{
			pos.x -= 30.0f;
		}
		else if (pos.x < SCREEN_WIDTH * 0.35f)
		{
			pos.x -= 40.0f;
		}
		else
		{
			pos.x -= 2.5f;
		}

		m_pStart->SetPosition(pos);
		m_pStart->SetVtx();
		
		if (pos.x < -SCREEN_WIDTH * 0.5f)
		{
			m_pStart->Uninit();
			m_pStart = NULL;
			CManager::GetSlow()->SetSlow(1.0f);
		}

		if (m_pScore != NULL)
		{
			m_pScore->Update();
		}

		if (m_pTime->GetNum() != m_pTime->GetStartNum())
		{
			CarSet();
		}

		CScene::Update();

		return;
	}

	// ポーズ
	if (m_pPause != NULL)
	{
		m_pPause->Update();

		if (m_pPause->GetEnable() == true)
		{
			if (m_pPause->GetSelect() == true)
			{
				CManager::GetFade()->Update();
			}
			return;
		}
	}

	// 陽炎フィルターの色を変更
	if (m_pEnemyManager->GetDead() > 0)
	{
		int nDead = m_pEnemyManager->GetDead();
		int nSuv = m_pEnemyManager->GetSuv();

		for (int nCnt = 0; nCnt < NUM_FILTER; nCnt++)
		{
			if (m_apFilter[nCnt] != NULL)
			{
				m_apFilter[nCnt]->SetFil((float)((float)nDead / (float)(nSuv + 20)));
			}
		}
	}

#if _DEBUG

	// エディターの更新処理
	if (m_pEditor != NULL)
	{
		m_pEditor->Update();
	}

	if (m_pEditor->GetActive() == true || m_pEditor->GetMeshActive() == true)
	{
		// 更新処理
		CScene::Update();
		return;
	}

#endif

	// タイム
	if (m_pTime != NULL)
	{
		m_pTime->Update();
		if (m_pTime->GetNum() >= MAX_TIME)
		{// 時間切れ
			//m_pTime->Set(START_TIME);

			CManager::GetFade()->Set(CScene::MODE_RESULT);
		}
		else
		{
			// 敵の配置管理
			EnemySet();

			// 車の配置管理
			CarSet();

			// 空の色変更
			SkySet();
		}
	}

	// 残り人数
	if (m_pScore != NULL)
	{
		m_pScore->Update();
	}

	if (m_pMapThermo != NULL)
	{
		int nDead = m_pEnemyManager->GetDead();
		D3DXCOLOR col = m_pMapThermo->GetCol();
		if (nDead > 0)
		{// 一体でも存在している
			col.a = (float)(nDead / 100);
		}
		else
		{
			col.a = 0.0f;
		}
		m_pMapThermo->SetColor(col);
	}

	if (CManager::GetSlow()->Get() == 1.0f && CManager::GetSlow()->GetOld() != 1.0f)
	{
		CLockOn::MultiDeath();
	}

	if (m_pPlayer != NULL)
	{
		if (m_pTime->GetDiff() <= 0.25f || m_pTime->GetDiff() >= 0.8f)
		{

			CManager::GetLight()->SetPositonPointLight(m_pPlayer->GetPosition());
		}
		else if(m_pTime->GetDiff() == 0.25f)
		{
			CManager::GetLight()->EnablePointLight(false);
		}
		else if (m_pTime->GetDiff() == 0.8f)
		{
			CManager::GetLight()->EnablePointLight(true);
		}
	}

	// 更新処理
	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CGame::Draw(void)
{
	CScene::Draw();

	// ポーズ
	if (m_pPause != NULL)
	{
		m_pPause->Draw();
	}
}

//===================================================
// スコアの取得
//===================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}

//===================================================
// スコアの取得
//===================================================
CTime *CGame::GetTime(void)
{
	return m_pTime;
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//===================================================
// スロー情報の取得
//===================================================
CSlow *CGame::GetSlow(void)
{
	return m_pSlow;
}

//===================================================
// メッシュフィールドの取得
//===================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CGame::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// ポーズの取得
//===================================================
CPause *CGame::GetPause(void)
{
	return m_pPause;
}

//===================================================
// エディター情報の取得
//===================================================
CEditor *CGame::GetEditor(void)
{
	return m_pEditor;
}

//===================================================
// 読み込みデータリセット
//===================================================
void CGame::DataReset(void)
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
void CGame::EnemySet(void)
{
	if (m_pEnemyManager == NULL)
	{
		return;
	}

	// 配置範囲、種類、数を設定
	if(m_pTime->GetNum() < MORNING_TIME)
	{// 早朝
		m_pEnemyManager->SetRouteRange(PARK_ROUTE, SHOP_ROUTE);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_MAX, 0);
		m_pEnemyManager->SetNum(1);
		m_pEnemyManager->SetTimer(15);
	}
	else if (m_pTime->GetNum() >= MORNING_TIME && m_pTime->GetNum() < AM_COMRUSH)
	{// 出勤ラッシュ
		m_pEnemyManager->SetRouteRange(SHOP_ROUTE, 0);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_KIDSBOY, 0);
		m_pEnemyManager->SetNum(3);
		m_pEnemyManager->SetTimer(8);

		if (m_pTime->GetNum() == MORNING_TIME && m_pTime->GetAnim() == 0.0f)
		{
			if (m_pStart == NULL)
			{
				m_pEnemyManager->Spawn(CObject::GetNumEnemAll() + 30);
				m_pStart = CObject2D::Create(7);
				m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 0.4f, 0.0f));
				m_pStart->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.05f));
				m_pStart->SetLength(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f);
				m_pStart->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\rush000.png"));
				m_pStart->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
				CManager::GetSlow()->SetSlow(SPEED_UP);
				CManager::GetSound()->Play(CSound::LABEL_SE_CLOCK);
			}
		}
	}
	else if (m_pTime->GetNum() >= RELAX_TIME && m_pTime->GetNum() < NOON_TIME)
	{// 昼休み
		m_pEnemyManager->SetRouteRange(PARK_ROUTE, SHOP_ROUTE);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_KIDSBOY, CEnemyManager::ENEMY_KIDSBOY);
		m_pEnemyManager->SetTimer(20);
		m_pEnemyManager->SetNum(2);
	}
	else if (m_pTime->GetNum() >= NOON_TIME && m_pTime->GetNum() < EVENT_RUSH)
	{// イベントラッシュ
		m_pEnemyManager->SetRouteRange(PARK_ROUTE, SHOP_ROUTE);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_KIDSBOY, CEnemyManager::ENEMY_KIDSBOY);
		m_pEnemyManager->SetTimer(5);
		m_pEnemyManager->SetNum(3);

		if (m_pTime->GetNum() == NOON_TIME && m_pTime->GetAnim() == 0.0f)
		{
			if (m_pStart == NULL)
			{
				m_pEnemyManager->Spawn(CObject::GetNumEnemAll() + 30);
				m_pStart = CObject2D::Create(7);
				m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 0.4f, 0.0f));
				m_pStart->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.05f));
				m_pStart->SetLength(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f);
				m_pStart->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\rush002.png"));
				CManager::GetSlow()->SetSlow(SPEED_UP);
				CManager::GetSound()->Play(CSound::LABEL_SE_CLOCK);
			}
		}
	}
	else if (m_pTime->GetNum() >= PM_RELAX_TIME && m_pTime->GetNum() < PM_GOHOME_RUSH)
	{// 帰宅ラッシュ
		m_pEnemyManager->SetRouteRange(NUM_ROUTE, 0);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_MAX, 0);
		m_pEnemyManager->SetNum(2);
		m_pEnemyManager->SetTimer(3);

		if (m_pTime->GetNum() == PM_RELAX_TIME && m_pTime->GetAnim() == 0.0f)
		{
			if (m_pStart == NULL)
			{
				m_pEnemyManager->Spawn(CObject::GetNumEnemAll() + 30);
				m_pStart = CObject2D::Create(7);
				m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 0.4f, 0.0f));
				m_pStart->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.05f));
				m_pStart->SetLength(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f);
				m_pStart->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\rush001.png"));
				CManager::GetSlow()->SetSlow(SPEED_UP);
				CManager::GetSound()->Play(CSound::LABEL_SE_CLOCK);
			}
		}
	}
	else if (m_pTime->GetNum() >= PM_GOHOME_RUSH)
	{// 帰宅ラッシュ終わりからゲーム終了まで
		m_pEnemyManager->SetRouteRange(SHOP_ROUTE + 5, 0);
		m_pEnemyManager->SetTypeRange(CEnemyManager::ENEMY_MAX, 0);
		m_pEnemyManager->SetNum(1);
		m_pEnemyManager->SetTimer(20);

		if (m_pTime->GetNum() == MAX_TIME - 10 && m_pTime->GetAnim() == 0.0f)
		{
			if (m_pStart == NULL)
			{
				m_pEnemyManager->Spawn(CObject::GetNumEnemAll() + 30);
				m_pStart = CObject2D::Create(7);
				m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 0.4f, 0.0f));
				m_pStart->SetRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.05f));
				m_pStart->SetLength(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f);
				m_pStart->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\rush003.png"));
				CManager::GetSlow()->SetSlow(SPEED_UP);
				CManager::GetSound()->Play(CSound::LABEL_SE_CLOCK);
			}
		}
	}
	else
	{// それ以外
		m_pEnemyManager->SetRouteRange(NUM_ROUTE, 0);
		m_pEnemyManager->SetTypeRange( CEnemyManager::ENEMY_MAX, 0);
		m_pEnemyManager->SetNum(1);
		m_pEnemyManager->SetTimer(40);
	}

	// 更新
	m_pEnemyManager->Update();
}

//===================================================
// 車の配置
//===================================================
void CGame::CarSet(void)
{
	if (m_pCarManager != NULL)
	{
		m_pCarManager->Update();
	}
}

//===================================================
// 空の色設定
//===================================================
void CGame::SkySet(void)
{
	if (m_pMeshDome != NULL)
	{
		if (CManager::GetSlow()->Get() >= 1.0f)
		{
			m_pMeshDome->SetColor(m_pTime->GetDiff());
			CManager::GetLight()->SetLight(m_pTime->GetDiff());
		}
	}
}